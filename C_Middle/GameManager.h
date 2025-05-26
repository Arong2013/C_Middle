#pragma once
#include "GameState.h"
#include "Player.h"
#include "Enemy.h"
#include "Gate.h"
#include <memory>
#include <vector>
#include <map>
#include <iostream>

class GameManager {
private:
    std::unique_ptr<GameState> currentState;

    // 상태 인스턴스들
    InitState initState;
    PlayerTurnState playerTurnState;
    EnemyTurnState enemyTurnState;

    // 게임 요소
    std::shared_ptr<Player> player;
    std::map<Vector2Int, std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::shared_ptr<Gate> gate;

    // 맵 크기
    int width;
    int height;

    bool isRunning;
    int currentLevel;

public:
    GameManager();

    void Initialize();
    void Run();
    void Shutdown();

    // 상태 변경 메소드
    void ChangeState(GameState* newState);

    // 게임 진행 메소드
    void GenerateLevel();
    void MovePlayer(const Vector2Int& direction);
    void PlayerAttack(const Vector2Int& direction);
    void ProcessEnemyTurns();
    void CheckGameOver();
    void NextLevel();

    // 유틸리티 메소드
    bool IsPositionValid(const Vector2Int& position) const;
    std::shared_ptr<Entity> GetEntityAt(const Vector2Int& position);
    void RemoveEntity(const Vector2Int& position);
    void RenderGame() const;
    void ProcessInput();

    // 게터
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    std::shared_ptr<Player> GetPlayer() const { return player; }
    bool IsRunning() const { return isRunning; }
    void QuitGame() { isRunning = false; }
};