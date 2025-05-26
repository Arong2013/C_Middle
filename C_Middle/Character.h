#pragma once
#include "Entity.h"

class Character : public Entity {
protected:
    int hp;
    int attack;

public:
    Character(const std::string& name, EntityType type, char symbol, int hp, int attack)
        : Entity(name, type, symbol), hp(hp), attack(attack) {}

    // �⺻ �׼�
    virtual void TakeDamage(int damage);
    virtual int Attack() const { return attack; }
    virtual void Move(const Vector2Int& direction);

    // ���� Ȯ��
    bool IsAlive() const { return hp > 0; }

    // ����
    int GetHp() const { return hp; }
    int GetAttack() const { return attack; }
};