#ifndef BSSRDF_SETTINGS_H
#define BSSRDF_SETTINGS_H

#include <cstdint>

class Settings
{
public:
    static Settings& instance();

    void setSingleScatterSamplesCount(std::size_t count);
    void setMultiScatterSamplesCount(std::size_t count);
    void setAASamplesCount(std::size_t count);

    std::size_t getSingleScatterSamplesCount() const { return _singleScatterSamples; }
    std::size_t getMultiScatterSamplesCount() const { return _multiScatterSamples; }
    std::size_t getAASamplesCount() const { return _aaSamples; }
    bool isAAEnabled() const { return _aaSamples != 0; }

private:
    Settings() = default;

    std::size_t _singleScatterSamples;
    std::size_t _multiScatterSamples;
    std::size_t _aaSamples;
};

#endif
