#ifndef BSSRDF_COLOR_H
#define BSSRDF_COLOR_H

#include <glm/glm.hpp>

#include <cstdint>

using Color = glm::dvec3;

Color operator"" _rgb(const char* str, std::size_t);

Color sqrt(const Color& color);
Color exp(const Color& color);

#endif
