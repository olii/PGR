#include "plane.h"
#include <SDL.h>

optional<float> Plane::Intersect(const Ray& ray)
{

    float denom = m_normal.Dot(ray.GetDirection());
    if (std::abs(denom) > 0.0001f)   // your favorite epsilon
    {
        float t = (m_p - ray.GetOrigin()).Dot(m_normal) / denom;
        if (t >= 0)
        {
            return t;   // you might want to allow an epsilon here too
        }
    }

/*
    float t = (m_normal.Dot(m_p - ray.GetOrigin())) / (m_normal.Dot(ray.GetDirection()));
    if (t > 0)
    {
        SDL_Log("intersect\n");
        return t;
    }
    */
}
