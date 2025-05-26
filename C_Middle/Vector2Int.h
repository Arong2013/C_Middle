#pragma once

class Vector2Int {
public:
    int x;
    int y;

    Vector2Int() : x(0), y(0) {}
    Vector2Int(int x, int y) : x(x), y(y) {}

    // 연산자 오버로딩
    bool operator<(const Vector2Int& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Vector2Int& other) const {
        return x == other.x && y == other.y;
    }

    Vector2Int operator+(const Vector2Int& other) const {
        return Vector2Int(x + other.x, y + other.y);
    }
};