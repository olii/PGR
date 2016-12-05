#ifndef BSSRDF_MULTITHREADING_THREAD_SAFE_PRNG_H
#define BSSRDF_MULTITHREADING_THREAD_SAFE_PRNG_H

#include <random>

std::mt19937::result_type genRandom();

double getRandomExponential(double falloff);
double getRandomUniform(double min = 0.0, double max = 1.0);
//double getRandomGaussian(double, double);

#endif
