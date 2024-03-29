#include <algorithm>

#include "cube.h"
#include "intersection.h"
#include "ray.h"

Cube::Cube(const Vector& pos, double edge, const std::shared_ptr<Material>& material) : Shape(pos, material), _edge(edge)
{
    _init();
}

Cube::Cube(const Vector& pos, double edge, std::shared_ptr<Material>&& material) : Shape(pos, std::move(material)), _edge(edge)
{
    _init();
}

void Cube::_init()
{
    const double edge2 = _edge / 2.0;

    _bounds[0] = {_position.x - edge2, _position.y - edge2, _position.z - edge2};   // lower front left
    _bounds[1] = {_position.x + edge2, _position.y + edge2, _position.z + edge2};   // upper back right
}

/*
 * Compute intersection with cube
 * Inspired by https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection 
 */
Intersection Cube::intersects(const Ray& ray) const
{
    const auto& D = ray.getDirection();
    const auto& O = ray.getOrigin();

    double tmin = (_bounds[0].x - O.x) / D.x;
    double tmax = (_bounds[1].x - O.x) / D.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    double tymin = (_bounds[0].y - O.y) / D.y;
    double tymax = (_bounds[1].y - O.y) / D.y;

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return {};   // no hit

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    double tzmin = (_bounds[0].z - O.z) / D.z;
    double tzmax = (_bounds[1].z - O.z) / D.z;

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return {};   // no hit

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    if (tmin <= 0.0 && tmax <= 0.0)
        return {};

    // return first hit (tmin)
    if (tmin >= 0.0)
        return {tmin, ray.getPoint(tmin), ray.getDirection(), this};
    else
        return {tmax, ray.getPoint(tmax), ray.getDirection(), this};
}

Vector Cube::getNormal(const Vector& position) const
{
    double d0x = std::abs(position.x - _bounds[0].x);
    double d1x = std::abs(position.x - _bounds[1].x);

    double d0y = std::abs(position.y - _bounds[0].y);
    double d1y = std::abs(position.y - _bounds[1].y);

    double d0z = std::abs(position.z - _bounds[0].z);
    double d1z = std::abs(position.z - _bounds[1].z);

    const double tresh = 1e-12;
    const double s = 1.0;   // size of returned normal vector

    if (d0x < tresh)   // left
        return {-s, 0, 0};
    else if (d0y < tresh)   // bottom
        return {0, -s, 0};
    else if (d0z < tresh)   // front
        return {0, 0, -s};
    else if (d1x < tresh)   // right
        return {s, 0, 0};
    else if (d1z < tresh)   // back
        return {0, 0, s};
    else if (d1y < tresh)   // top
        return {0, s, 0};

    return {};
}
