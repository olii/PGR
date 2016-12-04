#ifndef BSSRDF_MULTITHREADING_THREAD_SAFE_PRNG_H
#define BSSRDF_MULTITHREADING_THREAD_SAFE_PRNG_H

#include <random>

std::mt19937::result_type genRandom();

double getRandomExponential(double falloff);
double getRandomUniform();
//double getRandomGaussian(double, double);

#endif