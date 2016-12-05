#include "settings.h"

Settings& Settings::instance()
{
    static Settings settings;
    return settings;
}

void Settings::setSingleScatterSamplesCount(std::size_t count)
{
    _singleScatterSamples = count;
}

void Settings::setMultiScatterSamplesCount(std::size_t count)
{
    _multiScatterSamples = count;
}

void Settings::setAASamplesCount(std::size_t count)
{
    _aaSamples = count;
}
