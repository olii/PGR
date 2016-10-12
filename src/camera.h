#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "vector3.h"
#include "ray.h"
#include <SDL.h>
#include <ostream>

double deg2rad(double degrees)
{
    return degrees * 4.0 * atan(1.0) / 180.0;
}

class Camera
{
public:
    Camera(const Point& point, Point center, float fov, SDL_Surface* surface)
      : m_origin(point), m_target(center), m_fovDegree(fov), m_image(surface)
    {
        m_up = Vector3(0,1,0);
        m_aspect = surface->w / float(surface->h);
        recalculate();
    }

    Ray operator()(int x, int y)
    {

        float xx = 2.0*x/m_image->w - 1;
        float yy = 2.0*y/m_image->h - 1;
        return Ray(m_origin, m_dir + m_right*xx*m_tanf + m_up*yy*m_tanf);
    }

    const Point& GetOrigin() { return m_origin; }

    friend std::ostream& operator<<(std::ostream& os, const Camera& cam);

    void LookAt( const Point & a)
    {
        m_target = a;
        recalculate();
    }

    void LookAt(float x, float y)
    {
        //SDL_Log("[%f %f]\n", x,y);
        SDL_Log("---\n");
        m_target = (m_origin + m_dir) + m_right*x*m_tanf + m_up*y*m_tanf;
        SDL_Log("[%f %f %f] -> [%f %f %f]\n", m_origin.GetX(),m_origin.GetY(), m_origin.GetZ(),
                m_target.GetX(),m_target.GetY(), m_target.GetZ());
        recalculate();
        SDL_Log(">>---\n");

    }

    void ModifyFov(float fov)
    {
        m_fovDegree += fov;
        recalculate();
    }

    void recalculate()
    {
        m_dir = (m_target - m_origin).GetNormalized();
        m_right = m_up.Cross(m_dir).GetNormalized() * (m_aspect);
        m_up = m_dir.Cross(m_right).GetNormalized();

        m_fov = deg2rad(m_fovDegree) / 2.0;
        m_tanf = tan(m_fov);
    }
    void moveForward(float t)
    {
        //SDL_Log("move event to %d\n", t);
        m_origin += m_dir*t;
        m_target += m_dir*t;
        recalculate();
    }

    void RotateY( float dx)
    {
        float x = cos(dx)*m_dir.GetX() + sin(dx)*m_dir.GetZ();
        float y = m_dir.GetY();
        float z = -sin(dx)*m_dir.GetX() + cos(dx)*m_dir.GetZ();

        m_target = m_origin + Vector3(x,y,z);
        recalculate();
    }
    void RotateX( float dx)
    {
        float x = m_dir.GetX();
        float y = cos(dx)*m_dir.GetY() - sin(dx)*m_dir.GetZ();
        float z = sin(dx)*m_dir.GetY() + cos(dx)*m_dir.GetZ();

        m_target = m_origin + Vector3(x,y,z);
        recalculate();
    }

    void Strafe( float dx)
    {
        m_origin += m_right*dx;
        m_target += m_right*dx;
        recalculate();
    }

private:
    Point m_origin;
    Point m_target;
    Vector3 m_up;
    Vector3 m_right;
    Vector3 m_dir;
    float m_fovDegree;
    float m_fov;
    float m_tanf;
    float m_aspect;
    SDL_Surface* m_image;
};

std::ostream& operator<<(std::ostream& os, const Camera& cam)
{
    os << "DUMP Camera: "
       << "[" << cam.m_origin.GetX() << ", " << cam.m_origin.GetY() << ", " << cam.m_origin.GetZ()
       << "], vector [" << cam.m_dir.GetX() << ", " << cam.m_dir.GetY() << ", " << cam.m_dir.GetZ()
       << "]"  << ", fov = " << cam.m_fovDegree<< std::endl;
    return os;
}
#endif   // CAMERA_H
