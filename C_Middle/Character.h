#pragma once
#include "Entity.h"

class Character : public Entity {
protected:
    int hp;
    int attack;

public:
    Character(const std::string& name, EntityType type, char symbol, int hp, int attack)
        : Entity(name, type, symbol), hp(hp), attack(attack) {}

    // 기본 액션
    virtual void TakeDamage(int damage);
    virtual int Attack() const { return attack; }
    virtual void Move(const Vector2Int& direction);

    // 상태 확인
    bool IsAlive() const { return hp > 0; }

    // 게터
    int GetHp() const { return hp; }
    int GetAttack() const { return attack; }
};