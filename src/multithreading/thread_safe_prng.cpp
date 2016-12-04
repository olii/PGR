#include <chrono>
#include <thread>

#include "multithreading/thread_safe_prng.h"

std::mt19937::result_type genRandom()
{
    static thread_local std::mt19937 prng(
        std::chrono::system_clock::now().time_since_epoch().count() ^ std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return prng();
}

double getRandomExponential(double falloff)
{
    static thread_local std::mt19937 prng(
        std::chrono::system_clock::now().time_since_epoch().count() ^ std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return std::exponential_distribution<double>(falloff)(prng);
}

double getRandomUniform()
{
    static thread_local std::mt19937 prng(
        std::chrono::system_clock::now().time_since_epoch().count() ^ std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return std::uniform_real_distribution<double>(0.0, 1.0)(prng);
}
