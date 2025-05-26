#pragma once
#include "Character.h"

class Player : public Character {
private:
    int level;
    int kills;

public:
    Player(const std::string& name)
        : Character(name, EntityType::PLAYER, '@', 20, 5), level(1), kills(0) {}

    void IncreaseKills() { kills++; }
    void LevelUp();

    int GetLevel() const { return level; }
    int GetKills() const { return kills; }
};