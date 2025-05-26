#pragma once
#include <string>
#include "Vector2Int.h"

// ��ƼƼ Ÿ�� ������
enum class EntityType {
    PLAYER,
    ENEMY,
    GATE
};

class Entity {
protected:
    std::string name;
    EntityType type;
    Vector2Int position;
    char symbol; // �ֿܼ��� ǥ���� �ɺ�

public:
    Entity(const std::string& name, EntityType type, char symbol)
        : name(name), type(type), symbol(symbol) {}

    virtual ~Entity() {}

    // ����/����
    const std::string& GetName() const { return name; }
    EntityType GetType() const { return type; }
    const Vector2Int& GetPosition() const { return position; }
    void SetPosition(const Vector2Int& pos) { position = pos; }
    char GetSymbol() const { return symbol; }
};