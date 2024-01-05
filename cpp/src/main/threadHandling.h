#include <mutex>

#ifndef objective
#include "objective.cpp"
#define objective
#endif

std::mutex global_configuration_mutex;
int global_best_score;
gridmatrix global_best_configuration;