#ifndef BSSRDF_SCREEN_H
#define BSSRDF_SCREEN_H

#include <cstdint>

#include <SDL2/SDL.h>

#include "color.h"

class Color;
class Ray;

class Screen
{
public:
	Screen(SDL_Surface* sceneImpl, const Color& backgroundColor);

	std::uint32_t getWidth() const;
	std::uint32_t getHeight() const;
	const Color& getBackgroundColor() const;

	void putPixel(std::uint32_t x, std::uint32_t y, const Color& color);

private:
	SDL_Surface* _impl;
	Color _backgroundColor;
};

#endif
