#ifndef SOLVER__H
#define SOLVER__H

class DataModel;
class Evaluator;
class Loader;
class NodeChecker;
class NodeContainer;
class Searcher;

class Solver {
public:
    void setDataModel(DataModel *data_model);
    void setEvaluator(Evaluator *evaluator);
    void setLoader(Loader *loader);
    void setNodeChecker(NodeChecker *checker);
    void setNodeContainer(NodeContainer *container);
    void setSearcher(Searcher *searcher);

    void run();

private:
    DataModel     *data_model;
    Evaluator     *evaluator;
    Loader        *loader;
    NodeChecker   *checker;
    NodeContainer *container;
    Searcher      *searcher;
};

#endif