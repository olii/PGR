#ifndef BSSRDF_CAMERA_H
#define BSSRDF_CAMERA_H

#include "screen.h"
#include "vector.h"
#include <random>

class Ray;

class Camera
{
public:
    Camera(Screen& screen, const Vector& position, const Vector& lookAtPoint, double fov = 60.0);

    Screen& getScreen();
    const Screen& getScreen() const;
    const Vector& getPosition() const;
    Ray getRay(double x, double y) const;
    Ray getRayAA(double x, double y) const;

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

    Screen _screen;
    Vector _position, _lookAtPoint;
    double _fov;

    double _aspectRatio;
    double _halfFovTan;
    Vector _localForward, _localUp, _localRight;

    // precomputed values
    double _halfFovTan_aspectRatio;
    uint32_t _screenW;
    double _multiplierA;
    double _multiplierB;
    double _multiplierC;
    double _multiplierX;
    double _multiplierY;
};

#endif
