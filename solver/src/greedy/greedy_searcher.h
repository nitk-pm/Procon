#ifndef GREEDY_SERCHER__H
#define GREEDY_SERCHER__H

#include "core/searcher.h"

namespace greedy {

struct Node {
    short id;
};

class GreedySearcher : public core::Searcher {
public:
    void execute() override;
};

} // namespace greedy

#endif // GREEDY_SERCHER__H