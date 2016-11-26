#include "camera.h"
#include "color.h"
#include "ray.h"
#include "shapes/sphere.h"

namespace {

const glm::dvec3 Up = { 0.0, 1.0, 0.0 };

}

Camera::Camera(Screen& screen, const glm::dvec3& position, const  glm::dvec3& lookAtPoint, double distance/* = 50.0*/)
	: _screen(screen), _position(position), _lookAtPoint(lookAtPoint), _distance(distance)
{
	_w = glm::normalize(_position - _lookAtPoint);
	_u = glm::normalize(glm::cross(Up, -_w));
	_v = glm::normalize(glm::cross(_w, _u));
}

Screen& Camera::getScreen() const
{
	return _screen;
}

Ray Camera::getRay(double x, double y) const
{
	auto xView = x - 0.5 * (_screen.getWidth() - 1.0);
	auto yView = y - 0.5 * (_screen.getHeight() - 1.0);
	auto dir = glm::normalize(xView * _u + yView * _v - _distance * _w);
	return Ray(_position, dir);
}
