#ifndef SEARCHER__H
#define SEARCHER__H

#include "component.h"

namespace core {

class Operator;
class Evaluator;
class Verifier;

class Searcher : public Component {
public:
    void initialize() override;
    virtual void execute() = 0;

protected:
    Operator  *operation;
    Evaluator *evaluation;
    Verifier  *verification;
};

} // namespace core

#endif // SEARCHER__H