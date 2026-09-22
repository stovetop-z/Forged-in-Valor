#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include "components/health.h"
#include "components/movement.h"
#include "components/transform.h"
#include "components/collider.h"
#include "components/appearance.h"
#include "controllers/icontroller.h"

struct WorldContext;

class Entity
{
public:
    uint16_t id;
    HealthComponent health;
    MovementComponent movement;
    TransformComponent transform;
    ColliderComponent collider;
    Appearance appearance;
    std::unique_ptr<IController> controller;

    Entity(uint16_t id, std::unique_ptr<IController> controller) : id(id), controller(std::move(controller)) {}
    ~Entity() = default; 

    void update(const WorldContext& context, float dt)
    {
        Command command = controller->getCommand(context);

        
    }
};
#endif