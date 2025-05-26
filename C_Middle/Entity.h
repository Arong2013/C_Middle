#pragma once
#include <string>
#include "Vector2Int.h"

// 엔티티 타입 열거형
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
    char symbol; // 콘솔에서 표시할 심볼

public:
    Entity(const std::string& name, EntityType type, char symbol)
        : name(name), type(type), symbol(symbol) {}

    virtual ~Entity() {}

    // 게터/세터
    const std::string& GetName() const { return name; }
    EntityType GetType() const { return type; }
    const Vector2Int& GetPosition() const { return position; }
    void SetPosition(const Vector2Int& pos) { position = pos; }
    char GetSymbol() const { return symbol; }
};