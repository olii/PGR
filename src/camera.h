#ifndef BSSRDF_CAMERA_H
#define BSSRDF_CAMERA_H

#include "screen.h"
#include "vector.h"

class Ray;

class Camera
{
public:
    Camera(Screen& screen, const Vector& position, const Vector& lookAtPoint, double fov = 60.0);

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
    Vector _position, _lookAtPoint;
    double _fov;

    double _aspectRatio;
    double _halfFovTan;
    Vector _localForward, _localUp, _localRight;
};

#endif
