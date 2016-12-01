#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

#include "camera.h"
#include "light_models/lambertian_light_model.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"
#include "shapes/sphere.h"
#include <chrono>
#include <thread>

namespace {

const std::size_t ScreenWidth = 800;
const std::size_t ScreenHeight = 600;
}

int main(int, char* [])
{
    auto window = SDL_CreateWindow("PGR - BSSRDF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, 0);
    auto surface = SDL_GetWindowSurface(window);
    if (surface == nullptr)
    {
        std::cerr << "Cannot retrieve surface out of a window!\n";
        return 1;
    }

    Material redMaterial("red"_rgb);
    Material greenMaterial("green"_rgb);
    Material blueMaterial("blue"_rgb);

    Screen screen(surface, Colors::DarkBackground);
    Scene scene({screen, {0.0, 2.0, 2.0}, {0.0, 0.0, 0.0}});
    scene.addLight(std::make_unique<Light>(Vector{0.0, 2.0, 2.0}, "white"_rgb));
    scene.addLight(std::make_unique<Light>(Vector{0.0, 1.0, -3.5}, "white"_rgb));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, 0.0, 0.0}, 1.0, redMaterial));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, -0.25, -2.0}, 1.0, greenMaterial));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, 0.0, -5.0}, 1.0, blueMaterial));

    auto lightModel = std::make_unique<LambertianLightModel>();

    RayTracer raytracer;

    bool running = true;
    SDL_Event event;

    bool redraw = true;

    while (running)
    {
        while(SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_w:
                            scene.getCamera().moveForward(0.25);
                            redraw = true;
                            break;
                        case SDLK_s:
                            scene.getCamera().moveBackwards(0.25);
                            redraw = true;
                            break;
                        case SDLK_a:
                            scene.getCamera().moveLeft(0.25);
                            redraw = true;
                            break;
                        case SDLK_d:
                            scene.getCamera().moveRight(0.25);
                            redraw = true;
                            break;
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    if (event.motion.state & SDL_BUTTON_RMASK)
                    {
                        scene.getCamera().turnRight(0.2 * event.motion.xrel * M_PI / 180.0);
                        scene.getCamera().turnDown(0.2 * event.motion.yrel * M_PI / 180.0);
                        redraw = true;
                    }
                    break;
                }
            }
        }
        if (redraw)
        {
            raytracer.raytrace(scene, lightModel.get());
            SDL_UpdateWindowSurface(window);
            redraw = false;
        }
	else 
	{
	    std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
    }

    return 0;
}
