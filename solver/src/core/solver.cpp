#include "solver.h"

using namespace core;

Solver::~Solver() {
    for (auto &it : components) delete it.second;
}

void Solver::initialize() {
    for (auto &it : components) it.second->initialize();
}