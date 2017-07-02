#ifndef SOLVER__H
#define SOLVER__H

#include <type_traits>
#include <assert.h>
#include <unordered_map>

#include "component.h"

namespace core {

/************************************************

Solverクラス
・Componentを登録して、Solverを構成する
・Componentの追加時、追加しようとしているComponentが
　既に追加したComponentの場合は、警告を出してプログラ
　ムの実行が中断される
・Componentの取得、削除も同様に、対象のComponentが取
　得出来なかった時に警告を出してプログラムが中断される

*************************************************/

class Solver {
public:
    Solver() = default;
    ~Solver();

    void initialize();

    template <class T, class Id = T, class... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to Solver");
        const TypeId id = ComponentTypeID::get<Id>();
        assert(hasComponent(id));
        auto component = new T{std::forward<Args>(args)...};
        component->setParent(this);
        components.insert(std::make_pair(id, component));
        return component;
    }

    template <class T>
    T* getComponent() {
        static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot get T to Solver");
        const TypeId id = ComponentTypeID::get<T>();
        assert(!hasComponent(id));
        return static_cast<T*>(components[id]);
    }

    template <class T>
    void removeComponent() {
        static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T to Solver");
        const TypeId id = ComponentTypeID::get<T>();
        assert(!hasComponent(id));
        delete components[id];
        components.erase(id);
    }

    template <class T>
    bool hasComponent() {
        static_assert(std::is_base_of<Component, T>(), "T is not a component");
        const TypeId id = ComponentTypeID::get<T>();
        return hasComponent(id);
    }

private:
    bool hasComponent(TypeId id) {
        auto iter = components.find(id);
        return iter != components.end();
    }

private:
    std::unordered_map<TypeId, Component*> components;
};

} // namespace core

#endif // SOLVER__H