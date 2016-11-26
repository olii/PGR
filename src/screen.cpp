#include "color.h"
#include "ray.h"
#include "screen.h"

Screen::Screen(SDL_Surface* surfaceImpl) : _impl(surfaceImpl)
{
}

std::uint32_t Screen::getWidth() const
{
	return _impl->w;
}

std::uint32_t Screen::getHeight() const
{
	return _impl->h;
}

void Screen::putPixel(std::uint32_t x, std::uint32_t y, const Color& color)
{
	if (x >= static_cast<std::uint32_t>(_impl->w) || y >= static_cast<std::uint32_t>(_impl->h))
		return;

	auto pixels = static_cast<std::uint32_t*>(_impl->pixels);
	pixels[y * _impl->w + x] = SDL_MapRGB(_impl->format, color.getRed(), color.getGreen(), color.getBlue());
}
