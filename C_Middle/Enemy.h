#pragma once
#include "Character.h"

class Enemy : public Character {
public:
    Enemy(const std::string& name, int hp, int attack)
        : Character(name, EntityType::ENEMY, 'E', hp, attack) {}

    // °£´ÜÇÑ AI
    Vector2Int DecideMove(const Vector2Int& playerPos);
};