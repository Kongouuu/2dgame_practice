#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include "./Component.h"
#include "./Constants.h"

class EntityManager;
class Component;

class Entity {
    private:
        EntityManager& manager;
        bool isActive;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> componentTypeMap;
    public:
        std::string name;
        LayerType layer;

        // Constructor
        Entity(EntityManager &manager);
        Entity(EntityManager &manager, std::string name, LayerType layer);
        void Update(float deltaTime);
        void Render();
        void Destroy();
        void DestroyProjectile();
        void ListComponent() const;
        bool IsActive() const;
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args){
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->owner = this;
            components.emplace_back(newComponent);
            // &typeid identifies the type
            componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->Initialize();
            return *newComponent;
        }

        template <typename T>
        T* GetComponent() {
            // make more safe?
            return static_cast<T*> (componentTypeMap[&typeid(T)]);
        }

        template <typename T>
        bool HasComponent() const {
            return componentTypeMap.count(&typeid(T));
        }

};

#endif
