#include "ComponentStorage.hpp"
#include "Components.hpp"

template<typename T>
void ComponentStorage<T>::remove(Entity id) {
    if (!has(id)) {
        return;
    }

    std::swap(components_[sparse_[id]], components_.back());
    std::swap(entities_[sparse_[id]], entities_.back());
    sparse_[entities_[sparse_[id]]] = sparse_[id];
    sparse_[id] = EMPTY;
    entities_.pop_back();
    components_.pop_back();
}

template<typename T>
void ComponentStorage<T>::add(Entity id, T component) {
    assert(!has(id) && "Component already exists");

    if (id >= sparse_.size()) {
        sparse_.resize(id + 1, EMPTY);
    }
    entities_.push_back(id);
    components_.push_back(component);
    sparse_[id] = entities_.size() - 1;
}

template<typename T>
T& ComponentStorage<T>::get(Entity id) {
    return components_[sparse_[id]];
}

template<typename T>
Entity ComponentStorage<T>::getEntityAt(std::uint32_t index) {
    return entities_[index];
}

template<typename T>
bool ComponentStorage<T>::has(Entity id) const {
    return id < sparse_.size() && sparse_[id] != EMPTY;
}

template<typename T>
std::uint32_t ComponentStorage<T>::getSize() const {
    return static_cast<std::uint32_t>(components_.size());
}

template class ComponentStorage<Position>;
template class ComponentStorage<Velocity>;
template class ComponentStorage<Collider>;
template class ComponentStorage<SpriteComponent>;
template class ComponentStorage<Player>;
template class ComponentStorage<Enemy>;
template class ComponentStorage<ChaseComponent>;
template class ComponentStorage<PatrolComponent>;
template class ComponentStorage<HealthComponent>;
template class ComponentStorage<Invulnerability>;
