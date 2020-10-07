#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"

class Component {
   public:
    Entity* owner;
    // Avoid deletion of base class
    // Virtual also work good for later templating
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
};

#endif
