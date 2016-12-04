#include <chrono>
#include <thread>

#include "multithreading/thread_safe_prng.h"

std::mt19937::result_type genRandom()
{
    static thread_local std::mt19937 prng(std::chrono::system_clock::now().time_since_epoch().count() ^ std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return prng();
}
