attribute vec3 aVertexPosition;
attribute vec3 aNormalPosition;
attribute vec2 aTextureCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uLightMVP;

attribute mat3 aPrecomputeLT;

uniform mat3 uPrecomputeLR;
uniform mat3 uPrecomputeLG;
uniform mat3 uPrecomputeLB;

varying highp vec3 vColor;

void main(void) {
  float r = 0.0, g = 0.0, b = 0.0;
  for (int i = 0; i < 3; ++i)
  {
    r += dot(uPrecomputeLR[i], aPrecomputeLT[i]);
    g += dot(uPrecomputeLG[i], aPrecomputeLT[i]);
    b += dot(uPrecomputeLB[i], aPrecomputeLT[i]);
  }
  r = pow(r, 1.0 / 2.2);
  g = pow(g, 1.0 / 2.2);
  b = pow(b, 1.0 / 2.2);

  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix *
                vec4(aVertexPosition, 1.0);

  vColor = vec3(r, g, b);
}