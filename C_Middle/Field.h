#pragma once
#include <vector>
#include "Vector2Int.h"
#include "Player.h"
#include "Enemy.h"
#include "Gate.h"

class Field {
private:
    int width;
    int height;
    std::vector<std::vector<char>> grid;
    std::vector<Enemy*> enemies;
    Gate* gate;

    // 필드의 초기 상태
    void InitializeGrid();

    // 적과 게이트 배치
    void PlaceEnemies(int enemyCount);
    void PlaceGate();

    // 빈 위치 찾기 (적이나 게이트 배치할 때 사용)
    Vector2Int FindEmptyPosition() const;

public:
    Field(int width = 20, int height = 10);
    ~Field();

    // 필드 초기화 및 생성
    void Generate(int level);

    // 필드 렌더링
    void Render() const;

    // 엔티티 배치 및 이동 관련
    bool IsValidPosition(const Vector2Int& position) const;
    bool IsEmpty(const Vector2Int& position) const;
    void PlacePlayer(Player* player, const Vector2Int& position);
    void MoveEntity(const Vector2Int& oldPos, const Vector2Int& newPos, char symbol);

    // 게임 상태 관련
    bool IsGatePosition(const Vector2Int& position) const;
    Enemy* GetEnemyAt(const Vector2Int& position);
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }
    void RemoveEnemy(Enemy* enemy);

    // 필드 상태 조회
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    char GetCell(const Vector2Int& position) const;
};