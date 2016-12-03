#include <cstring>

#include "color.h"

Color operator"" _rgb(const char* str, std::size_t)
{
    if (std::strcmp(str, "white") == 0)
        return {1.0f, 1.0f, 1.0f};
    else if (std::strcmp(str, "red") == 0)
        return {1.0f, 0.0f, 0.0f};
    else if (std::strcmp(str, "green") == 0)
        return {0.0f, 1.0f, 0.0f};
    else if (std::strcmp(str, "blue") == 0)
        return {0.0f, 0.0f, 1.0f};
    else if (std::strcmp(str, "purple") == 0)
        return {0.13f, 0.0f, 0.31f};

    return {0.0f, 0.0f, 0.0f};
}

Color operator*(const Color& color, double d)
{
    return { color.r * d, color.g * d, color.b * d };
}

Color operator*(double d, const Color& color)
{
    return color * d;
}

Color& operator*=(Color& color, double d)
{
    color.r *= d;
    color.g *= d;
    color.b *= d;
    return color;
}

Color sqrt(const Color& color)
{
    return { std::sqrt(color.r), std::sqrt(color.g), std::sqrt(color.b) };
}

Color exp(const Color& color)
{
    return { std::exp(color.r), std::exp(color.g), std::exp(color.b) };
}
