/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, math, and strings
#include <main.h>
#include <SDL.h>
#include <string>
#include <iostream>
#include <experimental/optional>
#include "camera.h"
#include "plane.h"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

inline void put_pixel32(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    if (x >= surface->w)
        return;
    if (y >= surface->h)
        return;
    //Convert the pixels to 32 bit
    Uint32* pixels = static_cast<Uint32*>(surface->pixels);

    //Set the pixel
    pixels[(y * surface->w) + x] = pixel;
}

bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5 * b / a;
    else
    {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1)
        std::swap(x0, x1);

    return true;
}

bool intersect(const Ray& ray, float& t)
{
    /*
    float t0, t1;   // solutions for t if the ray intersects
#if 0
        // geometric solution
        Vector3 L = ray.GetOrigin() - Point(0,0,10);
        float tca = L.Dot(ray.GetDirection());
        // if (tca < 0) return false;
        float d2 = L.Dot(L) - tca * tca;
        if (d2 > 5.0) return false;
        float thc = sqrt(5.0 - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else
    // analytic solution
    Vector3 L = ray.GetOrigin() - Point(0, 0, 100);
    float a = ray.GetDirection().Dot(ray.GetDirection());
    float b = 2 * ray.GetDirection().Dot(L);
    float c = L.Dot(L) - 5.0;
    if (!solveQuadratic(a, b, c, t0, t1))
        return false;
#endif
    if (t0 > t1)
        std::swap(t0, t1);

    if (t0 < 0)
    {
        t0 = t1;   // if t0 is negative, let's use t1 instead
        if (t0 < 0)
            return false;   // both t0 and t1 are negative
    }

    t = t0;

    return true;
    */

    const Point& a = ray.GetOrigin();
    const Point c = Point(0, 0, 100);
    float radius = 5.0;
    const Vector3& p = ray.GetDirection();
    double aa = p.GetLengthSquared();
    double bb = (p.Dot(a - c)) * 2;
    double cc = (a - c).GetLengthSquared() - radius * radius;
    double D = bb * bb - 4 * aa * cc;
    if (D > 0)
    {
        double sD = sqrt(D);
        double t1 = (-bb - sD) / (2 * aa);
        if (t1 > 0)
        {
            t = t1;
            return true;
        }
        double t2 = (-bb + sD) / (2 * aa);
        if (t2 > 0)
        {
            t = t2;
            return true;
        }
    }
    return false;
}

int main(int argc, char* args[])
{
    SDL_Surface* screen;   // even with SDL2, we can still bring ancient code back
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);   // init video

#ifdef __MINGW32__
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // create the window like normal
    window = SDL_CreateWindow(
        "SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    // but instead of creating a renderer, we can draw directly to the screen
    screen = SDL_GetWindowSurface(window);
    //SDL_GetClipRect(screen, &screenRect);

    Camera camera(Point(0, 0, 0), Point(0, 0, 1), 60, screen);
    //Plane plane(Point(0, -100, -10), Point(1,1,0).GetNormalized());

    while (1)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }
        else if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_BUTTON_LMASK)
        {
            //camera.RotateY(event.motion.xrel / 200.0);
            //camera.RotateX(-event.motion.yrel / 200.0);
            camera.LookAt( event.motion.xrel / 200.0, event.motion.yrel / 200.0);
            //TODO: ROTATE BY POINTod intersection
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_h)
        {
            camera.ModifyFov(1);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n)
        {
            camera.ModifyFov(-1.0);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d)
        {
            camera.Strafe(1.0);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a)
        {
            camera.Strafe(-1.0);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w)
        {
            camera.moveForward(1.0);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
        {
            camera.moveForward(-1.0);
        }
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 135, 206, 250));
        //std::cout << camera;

        //SDL_FillRect(screen, &resultRect, SDL_MapRGB(screen->format, 255, 0, 0));

        for (int i = 0; i < screen->w; i++)
            for (int j = 0; j < screen->h; j++)
            {
                float t = 0.0;
                Ray ray = camera(i, j);
                bool f = intersect(ray, t);
                if (f)
                {
                    Point intersect = ray.GetOrigin() + ray.GetDirection() * t;
                    put_pixel32(screen,
                        i,
                        j,
                        SDL_MapRGB(screen->format,
                                    intersect.GetX() > 0 ? 50 : 230,
                                    intersect.GetY() > 0 ? 50 : 230,
                                    intersect.GetZ() > 100 ? 50 : 230));
                }
            }

        //camera.Rotate(0.1);

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
