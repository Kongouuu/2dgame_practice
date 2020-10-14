#include "./EntityManager.h"
#include "./Game.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData(){
    for (auto& entity : entities){
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.size()==0;
}

void EntityManager::Update(float deltaTime){
    for (auto& entity : entities){
        entity->Update(deltaTime);
    }
}

void EntityManager::Render(){
    for(int layer = 0; layer<NUM_LAYERS; layer++){
        for(const auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layer))){
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName,LayerType layer) {
    Entity* entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
};

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> entitiesByLayer;
    for(auto const &entity : entities){
        if(entity->layer==layer)
            entitiesByLayer.push_back(entity);
    }
    return entitiesByLayer;
}


unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

void EntityManager::ListEntity() const{
    for (auto& entity :entities){
        if(entity->name != "Tile")
            std::cout<<"Entity Name: "<<entity->name<<std::endl;
        entity->ListComponent();
    }
}

std::string EntityManager::CheckCollision(Entity &subjectEntity){
    ColliderComponent* subjectCollider = subjectEntity.GetComponent<ColliderComponent>();
    for (auto& entity : entities){
        // Even though Tile should not have collision component
        if(entity->name == "Tile" || entity->name == "Player")
            continue;
        // In this game so far, only "enemy&projectile" will collide
        if(entity->HasComponent<ColliderComponent>()){
            ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
            if(Collision::CheckCollision(subjectCollider->collider, otherCollider->collider))
                return otherCollider->tag;
        }
    }
    return "";
}