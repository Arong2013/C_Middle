#include "Character.h"
#include <iostream>

void Character::TakeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Character::Move(const Vector2Int& direction) {
    position = position + direction;
}
