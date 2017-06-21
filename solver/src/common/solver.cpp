#include "common/solver.h"

void Solver::setDataModel(DataModel *data_model) {
    this->data_model = data_model;
}

void Solver::setEvaluator(Evaluator *evaluator) {
    this->evaluator = evaluator;
}

void Solver::setLoader(Loader *loader) {
    this->loader = loader;
}

void Solver::setNodeChecker(NodeChecker *checker) {
    this->checker = checker;
}

void Solver::setNodeContainer(NodeContainer *container) {
    this->container = container;
}

void Solver::setSearcher(Searcher *searcher) {
    this->searcher = searcher;
}

void Solver::run() {
    
}