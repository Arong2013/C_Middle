#include <iostream>
#include "GameManager.h"
#include <windows.h>  // 여러 파일에 추가됨


int main() {
    // 콘솔 창 타이틀 설정
    SetConsoleTitle(TEXT("머그 스타일 게임"));

    // 콘솔 커서 숨기기
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    std::cout << "=== 머그 스타일 게임 ===\n";
    std::cout << "플레이어(@)를 조작하여 적(E)들을 물리치고 게이트(G)를 통해 다음 레벨로 진행하세요.\n\n";
    std::cout << "계속하려면 아무 키나 누르세요...\n";

    // 화면 지우기
    system("cls");

    GameManager gameManager;
    gameManager.Initialize();
    gameManager.Run();
    gameManager.Shutdown();

    // 커서 다시 보이게 하기
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    std::cout << "게임을 종료합니다. 아무 키나 누르세요...\n";

    return 0;
}
