#ifndef COMPONENT__H
#define COMPONENT__H

#include <memory>

namespace core {

class Entity;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    Component(Entity *parent) {
        setParent(parent);
    }

    void setParent(Entity *parent) {
        _parent = parent;
    }

    Entity* parent() const {
        return _parent;
    }

private:
    Entity *_parent;
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

TypeId ComponentTypeID::next_type_id = 0;

} // namespace core

#endif