#include "core/solver.h"
#include "core/searcher.h"
#include "core/operator.h"
#include "core/evaluator.h"
#include "core/verifier.h"

using namespace core;

void Searcher::initialize() {
    operation    = parent()->getComponent<Operator>();
    evaluation   = parent()->getComponent<Evaluator>();
    verification = parent()->getComponent<Verifier>();
}