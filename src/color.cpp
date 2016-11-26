#include <cstring>

#include "color.h"

Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
	: _red(red), _green(green), _blue(blue)
{
}

Color::Color(std::uint32_t rgba)
	: _red((rgba >> 24) & 0xFF), _green((rgba >> 16) & 0xFF), _blue((rgba >> 8) & 0xFF)
{
}

std::uint8_t Color::getRed() const
{
	return _red;
}

std::uint8_t Color::getGreen() const
{
	return _green;
}

std::uint8_t Color::getBlue() const
{
	return _blue;
}

Color operator "" _rgb(const char* str, std::size_t)
{
	if (std::strcmp(str, "white") == 0)
		return { 0xFF, 0xFF, 0xFF };
	else if (std::strcmp(str, "red") == 0)
		return { 0xFF, 0x00, 0x00 };
	else if (std::strcmp(str, "green") == 0)
		return { 0x00, 0xFF, 0x00 };
	else if (std::strcmp(str, "blue") == 0)
		return { 0x00, 0x00, 0xFF };

	return { 0x00, 0x00, 0x00 };
}
