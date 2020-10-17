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
    DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities(){
    for(int i=0; i<entities.size(); i++){
        if(!entities[i]->IsActive()){
            entities.erase(entities.begin()+i);
        }
    }
    
}

void EntityManager::Render(){
    for(unsigned int layer = 0; layer<NUM_LAYERS; layer++){
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

CollisionType EntityManager::CheckCollision() const {
    for (auto& entity : entities)
        if(entity->HasComponent<ColliderComponent>()){
            ColliderComponent* curCollider = entity->GetComponent<ColliderComponent>();
            for(auto& otherEntity : entities)
                if(otherEntity->HasComponent<ColliderComponent>() && otherEntity->name != entity->name){
                    ColliderComponent* otherCollider = otherEntity->GetComponent<ColliderComponent>();
                    if(Collision::CheckCollision(curCollider->collider,otherCollider->collider)){
                        if(curCollider->tag=="PLAYER" && otherCollider->tag=="ENEMY")
                            return PLAYER_ENEMY;
                        if(curCollider->tag=="PLAYER" && otherCollider->tag=="ENEMY_PROJECTILE")
                            return PLAYER_PROJECTILE;
                        if(curCollider->tag=="ENEMY" && otherCollider->tag=="PROJECTILE"){
                            return ENEMY_PROJECTILE;
                        }
                        if(curCollider->tag=="PLAYER" && otherCollider->tag=="LEVEL_COMPLETE")
                            return PLAYER_LEVEL_COMPLETE;
                    }
                }
        }
    return NONE;
}