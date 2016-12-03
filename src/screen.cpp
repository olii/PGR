#include "color.h"
#include "ray.h"
#include "screen.h"

#include <string>
#include <iostream>

Screen::Screen(SDL_Surface* surfaceImpl, const Color& backgroundColor) : _impl(surfaceImpl), _backgroundColor(backgroundColor), _i(0)
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

const Color& Screen::getBackgroundColor() const
{
    return _backgroundColor;
}

void Screen::putPixel(std::uint32_t x, std::uint32_t y, const Color& color)
{
    if (x >= static_cast<std::uint32_t>(_impl->w) || y >= static_cast<std::uint32_t>(_impl->h))
        return;

    auto pixels = static_cast<std::uint32_t*>(_impl->pixels);
    auto clampedColor = Color{std::min(color.r, 1.0), std::min(color.g, 1.0), std::min(color.b, 1.0)};
    pixels[y * _impl->w + x] = SDL_MapRGB(_impl->format, clampedColor.r * 255, clampedColor.g * 255, clampedColor.b * 255);
}

void Screen::exportImage()
{
    std::string filename = "bssrdf_" + std::to_string(_i++) + ".bmp";
    SDL_SaveBMP(_impl, filename.c_str());
    std::cout << "Image saved to: " << filename << std::endl;
}

