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

double colorLuminance(const Color& color)
{
    return 0.212671 * color.r + 0.715160 * color.g + 0.072169 * color.b;
}
