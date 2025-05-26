#include "GameState.h"
#include "GameManager.h"
#include <iostream>
#include <conio.h>

// InitState 구현
void InitState::Enter() {
    std::cout << "게임 초기화 중...\n";
}

void InitState::Update() {
    gameManager->GenerateLevel();
    gameManager->ChangeState(new PlayerTurnState(gameManager));
}

void InitState::Exit() {
    std::cout << "초기화 완료, 게임 시작!\n";
}

// PlayerTurnState 구현
void PlayerTurnState::Enter() {
    std::cout << "플레이어 턴\n";
    gameManager->RenderGame();
}

void PlayerTurnState::Update() {
    gameManager->ProcessInput();
    gameManager->CheckGameOver();

    if (gameManager->IsRunning()) {
        gameManager->ChangeState(new EnemyTurnState(gameManager));
    }
}

void PlayerTurnState::Exit() {
    std::cout << "플레이어 턴 종료\n";
}

// EnemyTurnState 구현
void EnemyTurnState::Enter() {
    std::cout << "적 턴\n";
}

void EnemyTurnState::Update() {
    gameManager->ProcessEnemyTurns();
    gameManager->CheckGameOver();

    if (gameManager->IsRunning()) {
        gameManager->ChangeState(new PlayerTurnState(gameManager));
    }
}

void EnemyTurnState::Exit() {
    std::cout << "적 턴 종료\n";
}

// GameOverState 구현
void GameOverState::Enter() {
    gameManager->ShowGameOverUI();
}

void GameOverState::Update() {
    std::cout << "아무 키나 누르면 종료합니다...\n";
    _getch();
    gameManager->QuitGame();
}

void GameOverState::Exit() {
    std::cout << "게임 종료\n";
}

// GameClearState 구현
void GameClearState::Enter() {
    gameManager->ShowGameClearUI();
}

void GameClearState::Update() {
    std::cout << "아무 키나 누르면 종료합니다...\n";
    _getch();
    gameManager->QuitGame();
}

void GameClearState::Exit() {
    std::cout << "게임 종료\n";
}
