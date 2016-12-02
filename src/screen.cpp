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
    pixels[y * _impl->w + x] = SDL_MapRGB(_impl->format, color.r * 255, color.g * 255, color.b * 255);
}

void Screen::export_image()
{
   std::string filename = "bssrdf_" + std::to_string(_i++)+".bmp";
   SDL_SaveBMP( _impl, filename.c_str());
   std::cout << "Image saved to: " << filename << std::endl;
}

