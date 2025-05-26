#include "GameState.h"
#include "GameManager.h"
#include <iostream>
#include <conio.h>

// InitState ����
void InitState::Enter() {
    std::cout << "���� �ʱ�ȭ ��...\n";
}

void InitState::Update() {
    gameManager->GenerateLevel();
    gameManager->ChangeState(new PlayerTurnState(gameManager));
}

void InitState::Exit() {
    std::cout << "�ʱ�ȭ �Ϸ�, ���� ����!\n";
}

// PlayerTurnState ����
void PlayerTurnState::Enter() {
    std::cout << "�÷��̾� ��\n";
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
    std::cout << "�÷��̾� �� ����\n";
}

// EnemyTurnState ����
void EnemyTurnState::Enter() {
    std::cout << "�� ��\n";
}

void EnemyTurnState::Update() {
    gameManager->ProcessEnemyTurns();
    gameManager->CheckGameOver();

    if (gameManager->IsRunning()) {
        gameManager->ChangeState(new PlayerTurnState(gameManager));
    }
}

void EnemyTurnState::Exit() {
    std::cout << "�� �� ����\n";
}

// GameOverState ����
void GameOverState::Enter() {
    gameManager->ShowGameOverUI();
}

void GameOverState::Update() {
    std::cout << "�ƹ� Ű�� ������ �����մϴ�...\n";
    _getch();
    gameManager->QuitGame();
}

void GameOverState::Exit() {
    std::cout << "���� ����\n";
}

// GameClearState ����
void GameClearState::Enter() {
    gameManager->ShowGameClearUI();
}

void GameClearState::Update() {
    std::cout << "�ƹ� Ű�� ������ �����մϴ�...\n";
    _getch();
    gameManager->QuitGame();
}

void GameClearState::Exit() {
    std::cout << "���� ����\n";
}
