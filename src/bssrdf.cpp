#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

#include "camera.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
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

    Screen screen(surface, Colors::DarkBackground);
    Camera camera(screen, {0.0, 2.0, 2.0}, {0.0, 0.0, 0.0});

    Material redMaterial("red"_rgb);
    Material greenMaterial("green"_rgb);
    Material blueMaterial("blue"_rgb);

    Scene scene;
    scene.addLight(std::make_unique<Light>(Vector{0.0, 2.0, 2.0}, "white"_rgb));
    scene.addLight(std::make_unique<Light>(Vector{0.0, 1.0, -3.5}, "white"_rgb));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, 0.0, 0.0}, 1.0, redMaterial));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, -0.25, -2.0}, 1.0, greenMaterial));
    scene.addObject(std::make_unique<Sphere>(Vector{0.0, 0.0, -5.0}, 1.0, blueMaterial));
    scene.addObject(std::make_unique<Plane>(Vector{0.0, 125.0, 0.0}, Vector{0.0, 1.0, 0.0} , greenMaterial));

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
                            camera.moveForward(0.25);
                            redraw = true;
                            break;
                        case SDLK_s:
                            camera.moveBackwards(0.25);
                            redraw = true;
                            break;
                        case SDLK_a:
                            camera.moveLeft(0.25);
                            redraw = true;
                            break;
                        case SDLK_d:
                            camera.moveRight(0.25);
                            redraw = true;
                            break;
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    if (event.motion.state & SDL_BUTTON_RMASK)
                    {
                        camera.turnRight(0.2 * event.motion.xrel * M_PI / 180.0);
                        camera.turnDown(0.2 * event.motion.yrel * M_PI / 180.0);
                        redraw = true;
                    }
                    break;
                }
            }
        }
        if (redraw)
        {
            raytracer.raytrace(camera, scene);
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
