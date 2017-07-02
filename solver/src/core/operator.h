#ifndef OPERATOR__H
#define OPERATOR__H

#include "component.h"

namespace core {

class Datastore;

class Operator : public Component {
public:
    void initialize() override;
    bool canPut(short id, short type, short pos);
    void put(short id);

private:
    Datastore *datastore;
};

} // namespace core

#endif // OPERATOR__H