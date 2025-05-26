#pragma once
#include "Entity.h"

class Gate : public Entity {
public:
    Gate() : Entity("Gate", EntityType::GATE, 'G') {}
};