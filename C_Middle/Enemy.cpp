#include "Enemy.h"
#include <cmath>

Vector2Int Enemy::DecideMove(const Vector2Int& playerPos) {
    // �÷��̾� �������� �̵�
    Vector2Int direction(0, 0);

    if (position.x < playerPos.x) direction.x = 1;
    else if (position.x > playerPos.x) direction.x = -1;

    if (position.y < playerPos.y) direction.y = 1;
    else if (position.y > playerPos.y) direction.y = -1;

    // x�� y �� �ϳ��� �̵� (�밢�� �̵� ����)
    if (direction.x != 0 && direction.y != 0) {
        if (std::abs(position.x - playerPos.x) > std::abs(position.y - playerPos.y))
            direction.y = 0;
        else
            direction.x = 0;
    }

    return direction;
}
