#ifdef GL_ES
precision mediump float;
#endif

// Phong related variables
uniform sampler2D uSampler;
uniform vec3 uKd;
uniform vec3 uKs;
uniform vec3 uLightPos;
uniform vec3 uLightPos2;
uniform vec3 uCameraPos;
uniform vec3 uLightIntensity;
uniform vec3 uLightIntensity2;

varying highp vec2 vTextureCoord;
varying highp vec3 vFragPos;
varying highp vec3 vNormal;

// Shadow map related variables
#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

#define BIAS 0.008

#define LIGHT_WIDTH 0.006

uniform sampler2D uShadowMap;
uniform sampler2D uShadowMap2;

varying vec4 vPositionFromLight;
varying vec4 vPositionFromLight2;

highp float rand_1to1(highp float x ) { 
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv ) { 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

void uniformDiskSamples( const in vec2 randomSeed ) {

  float randNum = rand_2to1(randomSeed);
  float sampleX = rand_1to1( randNum ) ;
  float sampleY = rand_1to1( sampleX ) ;

  float angle = sampleX * PI2;
  float radius = sqrt(sampleY);

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

    sampleX = rand_1to1( sampleY ) ;
    sampleY = rand_1to1( sampleX ) ;

    angle = sampleX * PI2;
    radius = sqrt(sampleY);
  }
}

float findBlocker( sampler2D shadowMap, vec2 uv, float zReceiver ) {
  poissonDiskSamples(uv);
  // uniformDiskSamples(uv);

  float sum = 0.0;
  int count = 0;
  for(int i = 0; i < NUM_SAMPLES; i++) {
    float receiver = unpack(texture2D(shadowMap, uv + poissonDisk[i].xy * 0.015));
    if (receiver + BIAS < zReceiver) {
      sum += receiver;
      count ++;
    }
  }
	return sum / float(count);
}

float PCF(sampler2D shadowMap, vec4 coords, float filter) {
  poissonDiskSamples(coords.xy);
  // uniformDiskSamples(coords.xy);

  float sum = 0.0;
  for(int i = 0; i < NUM_SAMPLES; i++) {
    float depth = unpack(texture2D(shadowMap, coords.xy + poissonDisk[i].xy * filter));
    sum += (depth + BIAS < coords.z) ? 0.0 : 1.0;
  }
  return sum / float(NUM_SAMPLES);
}

float PCSS(sampler2D shadowMap, vec4 coords){

  // STEP 1: avgblocker depth
  float avgblocker = findBlocker(shadowMap, coords.xy, coords.z);
  if (avgblocker == coords.z) return 1.0;

  // STEP 2: penumbra size
  float filter = (coords.z - avgblocker) * LIGHT_WIDTH / avgblocker;

  // STEP 3: filtering
  return PCF(shadowMap, coords, filter);

}


float useShadowMap(sampler2D shadowMap, vec4 shadowCoord){
  float depth = unpack(texture2D(shadowMap, shadowCoord.xy));
  return (depth + BIAS < shadowCoord.z) ? 0.3 : 1.0;
}

vec3 blinnPhongLight(vec3 lightpos, vec3 intensity) {
  vec3 color = texture2D(uSampler, vTextureCoord).rgb;
  color = pow(color, vec3(2.2));

  vec3 ambient = 0.05 * color;

  vec3 lightDir = normalize(lightpos);
  vec3 normal = normalize(vNormal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 light_atten_coff =
      intensity / pow(length(lightpos - vFragPos), 2.0);
  vec3 diffuse = diff * light_atten_coff * color;

  vec3 viewDir = normalize(uCameraPos - vFragPos);
  vec3 halfDir = normalize((lightDir + viewDir));
  float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
  vec3 specular = uKs * light_atten_coff * spec;

  return (diffuse + specular);
}

vec3 blinnPhong(float visibility1, float visibility2) {
  vec3 phong1 = blinnPhongLight(uLightPos, uLightIntensity);
  vec3 phong2 = blinnPhongLight(uLightPos2, uLightIntensity2);

  vec3 color = texture2D(uSampler, vTextureCoord).rgb;
  color = pow(color, vec3(2.2));

  vec3 ambient = 0.05 * color;

  vec3 radiance = ambient + (phong1 * visibility1 + phong2 * visibility2);
  vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
  return phongColor;
}

void main(void) {

  float visibility, visibility2;
  vec3 shadowCoord = (vPositionFromLight.xyz + 1.0) * 0.5;
  // visibility = useShadowMap(uShadowMap, vec4(shadowCoord, 1.0));
  // visibility = PCF(uShadowMap, vec4(shadowCoord, 1.0), 0.003);
  visibility = PCSS(uShadowMap, vec4(shadowCoord, 1.0));

  vec3 shadowCoord2 = (vPositionFromLight2.xyz + 1.0) * 0.5;
  // visibility2 = PCF(uShadowMap2, vec4(shadowCoord2, 1.0), 0.003);
  visibility2 = PCSS(uShadowMap2, vec4(shadowCoord2, 1.0));

  vec3 phongColor = blinnPhong(visibility, visibility2);

  // gl_FragColor = vec4(phongColor * visibility, 1.0);
  gl_FragColor = vec4(phongColor, 1.0);
  // gl_FragColor = vec4(phongColor, 1.0);
  // gl_FragColor = vec4(visibility, visibility, visibility, 1.0);
  // gl_FragColor = vec4(visibility2, visibility2, visibility2, 1.0);
}