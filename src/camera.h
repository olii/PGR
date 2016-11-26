#ifndef BSSRDF_CAMERA_H
#define BSSRDF_CAMERA_H

#include <glm/glm.hpp>

#include "screen.h"

class Ray;

class Camera
{
public:
	Camera(Screen& screen, const glm::dvec3& position, const glm::dvec3& lookAtPoint, double distance = 50.0);

	Screen& getScreen() const;
	Ray getRay(double x, double y) const;

private:
	Screen& _screen;
	glm::dvec3 _position, _lookAtPoint;
	double _distance;

	glm::dvec3 _u, _v, _w;
};

#endif
