#include <glm/gtx/rotate_vector.hpp>

#include "camera.h"
#include "color.h"
#include "multithreading/thread_safe_prng.h"
#include "ray.h"
#include "shapes/sphere.h"

namespace {

const Vector Up = {0.0, 1.0, 0.0};
}

Camera::Camera(Screen& screen, const Vector& position, const Vector& lookAtPoint, double fov /* = 45.0*/)
  : _screen(screen), _position(position), _lookAtPoint(lookAtPoint), _fov(fov)
{
    _aspectRatio = static_cast<double>(screen.getWidth()) / static_cast<double>(screen.getHeight());
    _halfFovTan = tan(0.5 * fov * 180.0 / M_PI);

    _halfFovTan_aspectRatio = _halfFovTan * _aspectRatio;
    _screenW = _screen.getWidth();
    _multiplierB = _halfFovTan_aspectRatio / _screenW;
    _multiplierA = 2.0 * _multiplierB;
    _multiplierC = _multiplierA - _halfFovTan_aspectRatio;

    _multiplierX = _halfFovTan / _screen.getHeight();
    _multiplierY = _multiplierX * 2.0;

    _calculateLocalSpace();
}

Screen& Camera::getScreen()
{
    return _screen;
}

const Screen& Camera::getScreen() const
{
    return _screen;
}

const Vector& Camera::getPosition() const
{
    return _position;
}

Ray Camera::getRay(double x, double y) const
{
    auto xNdc = (x * _multiplierA) + _multiplierC;
    //auto xNdc = (2.0 * (x + 0.5) / _screen.getWidth() - 1.0) * _halfFovTan * _aspectRatio;
    auto yNdc = _halfFovTan - _multiplierY * y + _multiplierX;
    //auto yNdc = (1.0 - (2.0 * (y + 0.5) / _screen.getHeight())) * _halfFovTan;
    return Ray(_position, _localForward + xNdc * _localRight + yNdc * _localUp);
}

Ray Camera::getRayAA(double x, double y) const
{
    const double xx = x + getRandomUniform(-0.5, 0.5);
    const double yy = y + getRandomUniform(-0.5, 0.5);
    return getRay(xx, yy);
}

void Camera::moveForward(double step)
{
    _position += step * _localForward;
    _lookAtPoint += step * _localForward;

    _calculateLocalSpace();
}

void Camera::moveBackwards(double step)
{
    moveForward(-step);
}

void Camera::moveRight(double step)
{
    _position += step * _localRight;
    _lookAtPoint += step * _localRight;

    _calculateLocalSpace();
}

void Camera::moveLeft(double step)
{
    moveRight(-step);
}

void Camera::turnRight(double step)
{
    _localForward = glm::rotate(_localForward, step, _localUp);
    _lookAtPoint = _position + _localForward;

    _calculateLocalSpace();
}

void Camera::turnLeft(double step)
{
    turnRight(-step);
}

void Camera::turnUp(double step)
{
    _localForward = glm::rotate(_localForward, step, _localRight);
    _lookAtPoint = _position + _localForward;

    _calculateLocalSpace();
}

void Camera::turnDown(double step)
{
    turnUp(-step);
}

void Camera::_calculateLocalSpace()
{
    _localForward = glm::normalize(_lookAtPoint - _position);
    _localRight = glm::normalize(glm::cross(Up, _localForward));
    _localUp = glm::normalize(glm::cross(_localForward, _localRight));
}
