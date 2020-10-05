#include "./Entity.h"

// Constructor
Entity::Entity(EntityManager& manager) : manager(manager) {  // <- Cpp style initialize
    this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name)
    : manager(manager), name(name) {  // <- Cpp style initialize
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

bool Entity::IsActive() const {
    return this->isActive;
}