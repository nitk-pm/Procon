#include "solver.h"
#include "searcher.h"
#include "operator.h"
#include "evaluator.h"
#include "verifier.h"

using namespace core;

void Searcher::initialize() {
    operation    = parent()->getComponent<Operator>();
    evaluation   = parent()->getComponent<Evaluator>();
    verification = parent()->getComponent<Verifier>();
}