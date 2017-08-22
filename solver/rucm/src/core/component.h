#ifndef COMPONENT__H
#define COMPONENT__H

#include <memory>

namespace core {

class Solver;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    Component(Solver *parent) {
        setParent(parent);
    }

    virtual void initialize() {}

    void setParent(Solver *parent) {
        _parent = parent;
    }

    Solver* parent() const {
        return _parent;
    }

private:
    Solver *_parent;
};

typedef std::size_t TypeId;

class ComponentTypeID {
public:
    template <class T>
    static const TypeId get() {
        static const TypeId id = next_type_id++;
        return id;
    }

private:
    static TypeId next_type_id;
};

} // namespace core

#endif // COMPONENT__H