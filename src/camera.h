#ifndef BSSRDF_CAMERA_H
#define BSSRDF_CAMERA_H

#include <glm/glm.hpp>

#include "screen.h"

class Ray;

class Camera
{
public:
    Camera(Screen& screen, const glm::dvec3& position, const glm::dvec3& lookAtPoint, double fov = 60.0);

    Screen& getScreen() const;
    Ray getRay(double x, double y) const;

    void moveForward(double step);
    void moveBackwards(double step);
    void moveRight(double step);
    void moveLeft(double step);

    void turnRight(double step);
    void turnLeft(double step);
    void turnUp(double step);
    void turnDown(double step);

private:
    void _calculateLocalSpace();

    Screen& _screen;
    glm::dvec3 _position, _lookAtPoint;
    double _fov;

    double _aspectRatio;
    double _halfFovTan;
    glm::dvec3 _localForward, _localUp, _localRight;
};

#endif
