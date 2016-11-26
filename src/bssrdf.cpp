#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

#include "camera.h"
#include "ray_tracer.h"
#include "scene.h"
#include "shapes/sphere.h"

namespace {

const std::size_t ScreenWidth = 800;
const std::size_t ScreenHeight = 600;

}

int main()
{
	auto window = SDL_CreateWindow("PGR - BSSRDF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, 0);
	auto surface = SDL_GetWindowSurface(window);
	if (surface == nullptr)
	{
		std::cerr << "Cannot retrieve surface out of a window!\n";
		return 1;
	}

	Screen screen(surface);
	Camera camera(screen, {0.0, 1.5, 0.75}, {0.0, 0.0, 0.0});

	Material redMaterial("red"_rgb);
	Material greenMaterial("green"_rgb);

	Scene scene;
	scene.addObject(std::make_unique<Sphere>(glm::dvec3{0.0, 0.0, 0.0}, 1.0, redMaterial));
	scene.addObject(std::make_unique<Sphere>(glm::dvec3{0.0, 0.0, -2.0}, 1.0, greenMaterial));

	RayTracer raytracer;

	bool running = true;
	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}

		raytracer.raytrace(camera, scene);

		SDL_UpdateWindowSurface(window);
	}

	return 0;
}
