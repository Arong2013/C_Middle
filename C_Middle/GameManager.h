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

    // ���� �ν��Ͻ���
    InitState initState;
    PlayerTurnState playerTurnState;
    EnemyTurnState enemyTurnState;

    // ���� ���
    std::shared_ptr<Player> player;
    std::map<Vector2Int, std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::shared_ptr<Gate> gate;

    // �� ũ��
    int width;
    int height;

    bool isRunning;
    int currentLevel;

public:
    GameManager();

    void Initialize();
    void Run();
    void Shutdown();

    // ���� ���� �޼ҵ�
    void ChangeState(GameState* newState);

    // ���� ���� �޼ҵ�
    void GenerateLevel();
    void MovePlayer(const Vector2Int& direction);
    void PlayerAttack(const Vector2Int& direction);
    void ProcessEnemyTurns();
    void CheckGameOver();
    void NextLevel();

    // ��ƿ��Ƽ �޼ҵ�
    bool IsPositionValid(const Vector2Int& position) const;
    std::shared_ptr<Entity> GetEntityAt(const Vector2Int& position);
    void RemoveEntity(const Vector2Int& position);
    void RenderGame() const;
    void ProcessInput();

    // ����
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    std::shared_ptr<Player> GetPlayer() const { return player; }
    bool IsRunning() const { return isRunning; }
    void QuitGame() { isRunning = false; }
};