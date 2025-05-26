#include "Enemy.h"
#include <cmath>

Vector2Int Enemy::DecideMove(const Vector2Int& playerPos) {
    // 플레이어 방향으로 이동
    Vector2Int direction(0, 0);

    if (position.x < playerPos.x) direction.x = 1;
    else if (position.x > playerPos.x) direction.x = -1;

    if (position.y < playerPos.y) direction.y = 1;
    else if (position.y > playerPos.y) direction.y = -1;

    // x나 y 중 하나만 이동 (대각선 이동 방지)
    if (direction.x != 0 && direction.y != 0) {
        if (std::abs(position.x - playerPos.x) > std::abs(position.y - playerPos.y))
            direction.y = 0;
        else
            direction.x = 0;
    }

    return direction;
}
