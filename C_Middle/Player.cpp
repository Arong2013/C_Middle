#include "Player.h"

void Player::LevelUp() {
    level++;
    hp += 5;  // 체력 증가
    attack += 2;  // 공격력 증가
}
