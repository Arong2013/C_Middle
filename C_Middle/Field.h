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

    // �ʵ��� �ʱ� ����
    void InitializeGrid();

    // ���� ����Ʈ ��ġ
    void PlaceEnemies(int enemyCount);
    void PlaceGate();

    // �� ��ġ ã�� (���̳� ����Ʈ ��ġ�� �� ���)
    Vector2Int FindEmptyPosition() const;

public:
    Field(int width = 20, int height = 10);
    ~Field();

    // �ʵ� �ʱ�ȭ �� ����
    void Generate(int level);

    // �ʵ� ������
    void Render() const;

    // ��ƼƼ ��ġ �� �̵� ����
    bool IsValidPosition(const Vector2Int& position) const;
    bool IsEmpty(const Vector2Int& position) const;
    void PlacePlayer(Player* player, const Vector2Int& position);
    void MoveEntity(const Vector2Int& oldPos, const Vector2Int& newPos, char symbol);

    // ���� ���� ����
    bool IsGatePosition(const Vector2Int& position) const;
    Enemy* GetEnemyAt(const Vector2Int& position);
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }
    void RemoveEnemy(Enemy* enemy);

    // �ʵ� ���� ��ȸ
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    char GetCell(const Vector2Int& position) const;
};