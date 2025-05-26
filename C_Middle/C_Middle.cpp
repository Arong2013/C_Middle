#include <iostream>
#include "GameManager.h"

int main() {
    std::cout << "=== 머그 스타일 게임 ===\n";
    std::cout << "플레이어(@)를 조작하여 적(E)들을 물리치고 게이트(G)를 통해 다음 레벨로 진행하세요.\n\n";

    GameManager gameManager;
    gameManager.Initialize();
    gameManager.Run();
    gameManager.Shutdown();

    std::cout << "게임을 종료합니다. 아무 키나 누르세요...\n";

    return 0;
}