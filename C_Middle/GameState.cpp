#include "GameState.h"
#include "GameManager.h"
#include <iostream>

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
