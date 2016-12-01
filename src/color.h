#ifndef BSSRDF_COLOR_H
#define BSSRDF_COLOR_H

#include <glm/glm.hpp>

#include <cstdint>

using Color = glm::vec3;

Color operator"" _rgb(const char* str, std::size_t);
Color operator*(const Color& color, double d);
Color operator*(double d, const Color& color);
Color& operator*=(Color& color, double d);

namespace Colors {
static Color DarkBackground(0.13f, 0.0f, 0.31f);
}

#endif
