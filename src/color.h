#ifndef BSSRDF_COLOR_H
#define BSSRDF_COLOR_H

#include <cstdint>

class Color
{
public:
	Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
	Color(std::uint32_t rgba);

	std::uint8_t getRed() const;
	std::uint8_t getGreen() const;
	std::uint8_t getBlue() const;

private:
	std::uint32_t _red, _green, _blue;
};

Color operator "" _rgb(const char* str, std::size_t);

#endif
