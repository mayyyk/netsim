// Helper objects and global functions

#pragma once

#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern NetSim::ProbabilityGenerator probability_generator;

