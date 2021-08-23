//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include <thread>
#include "Scene.hpp"
#include "Renderer.hpp"


inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);

    // change the spp value to change sample ammount
    int spp = 16;
    std::cout << "SPP: " << spp << "\n";
//#pragma omp parallel for
    auto func = [&](int left, int right, int bottom, int top)
    {
        for (uint32_t j = left; j < right; ++j) {
            for (uint32_t i = bottom; i < top; ++i) {
                // generate primary ray direction
                float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                          imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

                Vector3f dir = normalize(Vector3f(-x, y, 1));
                int index = j * scene.width + i;
                for (int k = 0; k < spp; k++){
                    framebuffer[index] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
                }
            }
            //UpdateProgress(j / (float)scene.height);
            UpdateProgress( ( j - left ) / (float)( right - left ) );
        }
    };

    int halfw = scene.width / 2, halfh = scene.height / 2;
    std::thread t1( func, 0, halfw, 0, halfh );
    std::thread t2( func, 0, halfw, halfh, scene.height );
    std::thread t3( func, halfw, scene.width, 0, halfh );
    std::thread t4( func, halfw, scene.width, halfh, scene.height );
    t1.join( );
    t2.join( );
    t3.join( );
    t4.join( );
    UpdateProgress(1.f);

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
