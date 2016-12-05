#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <unordered_map>

#include <json/json.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "materials/brdf_material.h"
#include "materials/bssrdf_material.h"
#include "ray.h"
#include "ray_tracer.h"
#include "scene.h"
#include "settings.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/cube.h"

#ifdef __MINGW32__
#include <windows.h>
#endif

using namespace std::literals;

namespace {

const std::size_t ScreenWidth = 800;
const std::size_t ScreenHeight = 600;

const std::size_t DefaultSingleScatterSamples = 25;
const std::size_t DefaultMultiScatterSamples = 25;

}

std::unique_ptr<Scene> parseJson(const std::string& filePath, SDL_Surface* surface)
{
    Json::Reader reader;
    Json::Value root;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open JSON file\n";
        return nullptr;
    }

    if (!reader.parse(file, root))
    {
        std::cerr << "Error in parsing JSON\n";
        std::cerr << reader.getFormattedErrorMessages() << "\n";
        return nullptr;
    }

    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    for (const auto& material : root["materials"])
    {
        if (!material.isMember("type") || !material.isMember("name"))
            continue;

        auto type = material["type"].asString();
        auto name = material["name"].asString();
        if (type == "brdf")
        {
            Color color;
            color.r = material["color"]["red"].asDouble();
            color.g = material["color"]["green"].asDouble();
            color.b = material["color"]["blue"].asDouble();
            double diffuse = material["diffuse"].asDouble();
            double specular = material["specular"].asDouble();

            materials.emplace(std::make_pair(name, std::make_shared<BrdfMaterial>(color, diffuse, specular)));
        }
        else if (type == "bssrdf")
        {
            Color scatter, absorb;
            scatter.r = material["scatter"]["red"].asDouble();
            scatter.g = material["scatter"]["green"].asDouble();
            scatter.b = material["scatter"]["blue"].asDouble();
            absorb.r = material["absorb"]["red"].asDouble();
            absorb.g = material["absorb"]["green"].asDouble();
            absorb.b = material["absorb"]["blue"].asDouble();
            double phase = material["phase"].asDouble();
            double eta = material["eta"].asDouble();

            materials.emplace(std::make_pair(name, std::make_shared<BssrdfMaterial>(absorb, scatter, phase, eta)));
        }
    }

    Color bgColor;
    if (root.isMember("background"))
    {
        bgColor.r = root["background"]["red"].asDouble();
        bgColor.g = root["background"]["green"].asDouble();
        bgColor.b = root["background"]["blue"].asDouble();
    }

    Vector cameraPos{0.0, 0.0, 1.0}, lookAt{0.0, 0.0, 0.0};
    auto cameraValue = root["camera"];
    if (cameraValue.isMember("position"))
    {
        cameraPos.x = cameraValue["position"]["x"].asDouble();
        cameraPos.y = cameraValue["position"]["y"].asDouble();
        cameraPos.z = cameraValue["position"]["z"].asDouble();
    }
    if (cameraValue.isMember("look_at"))
    {
        lookAt.x = cameraValue["look_at"]["x"].asDouble();
        lookAt.y = cameraValue["look_at"]["y"].asDouble();
        lookAt.z = cameraValue["look_at"]["z"].asDouble();
    }
    double fov = cameraValue.isMember("fov") ? cameraValue["fov"].asDouble() : 60.0;

    Screen screen(surface, bgColor);
    auto scene = std::make_unique<Scene>(Camera{screen, cameraPos, lookAt, fov});

    for (const auto& object : root["objects"])
    {
        if (!object.isMember("type") || !object.isMember("position") || !object.isMember("material"))
            continue;

        std::shared_ptr<Material> material;
        auto materialName = object["material"].asString();
        auto itr = materials.find(materialName);
        if (itr != materials.end())
            material = itr->second;

        Vector position;
        position.x = object["position"]["x"].asDouble();
        position.y = object["position"]["y"].asDouble();
        position.z = object["position"]["z"].asDouble();

        auto type = object["type"].asString();
        if (type == "sphere")
        {
            double radius = object.isMember("radius") ? object["radius"].asDouble() : 1.0;
            scene->addObject(std::make_unique<Sphere>(position, radius, material));
        }
        else if (type == "cube")
        {
            double edge = object.isMember("edge") ? object["edge"].asDouble() : 1.0;
            scene->addObject(std::make_unique<Cube>(position, edge, material));
        }
    }

    for (const auto& light : root["lights"])
    {
        if (!light.isMember("color") || !light.isMember("position"))
            continue;

        Vector position;
        position.x = light["position"]["x"].asDouble();
        position.y = light["position"]["y"].asDouble();
        position.z = light["position"]["z"].asDouble();

        Color color;
        color.r = light["color"]["red"].asDouble();
        color.g = light["color"]["green"].asDouble();
        color.b = light["color"]["blue"].asDouble();

        scene->addLight(std::make_unique<Light>(position, color));
    }

    return scene;
}

void printHelp()
{
    std::cout << "Project PGR 2016/2017 - Marek Milkovic, Oliver Nemcek, Vladimir Cillo\n"
              << "Usage:\n"
              << "\tbssrdf JSON_FILE [SS_SAMPLES MS_SAMPLES]\n"
              << "\n"
              << "JSON_FILE                                             JSON file representing scene (example scenes are in scenes/ folder)\n"
              << "SS_SAMPLES                                            Number of samples for single-scattering (Default: " << DefaultSingleScatterSamples << ")\n"
              << "MS_SAMPLES                                            Number of samples for multi-scattering (Default: " << DefaultMultiScatterSamples << ")"
              << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 4)
    {
        printHelp();
        return 0;
    }

    auto window = SDL_CreateWindow("PGR - BSSRDF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, 0);
    auto surface = SDL_GetWindowSurface(window);
    if (surface == nullptr)
    {
        std::cerr << "Cannot retrieve surface out of a window!\n";
        return 1;
    }

#ifdef __MINGW32__
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    std::string filePath = argv[1];
    auto scene = parseJson(filePath, surface);
    if (scene == nullptr)
        return 1;

    std::size_t singleScatterSamples = DefaultSingleScatterSamples;
    std::size_t multiScatterSamples = DefaultMultiScatterSamples;
    if (argc == 4)
    {
        std::stringstream ss(std::string(argv[2]) + " " + argv[3]);
        ss >> singleScatterSamples;
        ss >> multiScatterSamples;
    }

    Settings::instance().setSingleScatterSamplesCount(singleScatterSamples);
    Settings::instance().setMultiScatterSamplesCount(multiScatterSamples);

    RayTracer raytracer(std::thread::hardware_concurrency());

    bool running = true;
    SDL_Event event;

    bool redraw = true;
    while (running)
    {
        while (SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_w:
                            scene->getCamera().moveForward(0.25);
                            redraw = true;
                            break;
                        case SDLK_s:
                            scene->getCamera().moveBackwards(0.25);
                            redraw = true;
                            break;
                        case SDLK_a:
                            scene->getCamera().moveLeft(0.25);
                            redraw = true;
                            break;
                        case SDLK_d:
                            scene->getCamera().moveRight(0.25);
                            redraw = true;
                            break;
                        case SDLK_F12:
                            scene->getCamera().getScreen().exportImage();
                            break;
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    if (event.motion.state & SDL_BUTTON_RMASK)
                    {
                        scene->getCamera().turnRight(0.2 * event.motion.xrel * M_PI / 180.0);
                        scene->getCamera().turnDown(0.2 * event.motion.yrel * M_PI / 180.0);
                        redraw = true;
                    }
                    break;
                }
            }
        }
        if (redraw)
        {
            raytracer.raytrace(scene.get());
            SDL_UpdateWindowSurface(window);
            redraw = false;
        }
        else
        {
            std::this_thread::sleep_for(50ms);
        }
    }

    raytracer.stop();
    return 0;
}
