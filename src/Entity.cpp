#include "./Entity.h"
#include <iostream>
// Constructor
Entity::Entity(EntityManager& manager) : manager(manager) {  // <- Cpp style initialize
    this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer)
    : manager(manager), name(name), layer(layer) {  // <- Cpp style initialize
    this->isActive = true;
}

void Entity::Update(float deltaTime) {
    // Loop through components
    for (auto& component : components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component : components) {
        component->Render();
    }
}

void Entity::Destroy() {
    this->isActive = false;
}

void Entity::ListComponent() const {
    for (auto& component : components) {
        component->List();
    }
}

bool Entity::IsActive() const {
    return this->isActive;
}