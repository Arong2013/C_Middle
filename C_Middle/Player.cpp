#include "Player.h"

void Player::LevelUp() {
    level++;
    hp += 5;  // ü�� ����
    attack += 2;  // ���ݷ� ����
}
