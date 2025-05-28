#include "Field.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <windows.h>

Field::Field(int width, int height) : width(width), height(height), gate(nullptr) {
    InitializeGrid();
}

Field::~Field() {
    // �� ��ü�� �޸� ����
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // ����Ʈ �޸� ����
    if (gate) {
        delete gate;
        gate = nullptr;
    }
}

void Field::InitializeGrid() {
    // 2D �׸��� �ʱ�ȭ
    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width, ' ');

        // �׵θ� ����
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                grid[y][x] = '#';
            }
        }
    }
}

void Field::Generate(int level) {
    // ���� ��ƼƼ�� ����
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    if (gate) {
        delete gate;
        gate = nullptr;
    }

    // �׸��� �ʱ�ȭ
    InitializeGrid();

    // ������ ���� ���� �� ����
    int enemyCount = 3 + level;

    // �� ��ġ
    PlaceEnemies(enemyCount);

    // ����Ʈ ��ġ
    PlaceGate();
}

Vector2Int Field::FindEmptyPosition() const {
    Vector2Int position;
    do {
        position.x = rand() % (width - 2) + 1;  // �׵θ� ����
        position.y = rand() % (height - 2) + 1; // �׵θ� ����
    } while (!IsEmpty(position));

    return position;
}

void Field::PlaceEnemies(int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        Vector2Int position = FindEmptyPosition();
        Enemy* enemy = new Enemy("Enemy_" + std::to_string(i));
        enemy->SetPosition(position);
        enemies.push_back(enemy);
        grid[position.y][position.x] = enemy->GetSymbol();
    }
}

void Field::PlaceGate() {
    Vector2Int position = FindEmptyPosition();
    gate = new Gate();
    gate->SetPosition(position);
    grid[position.y][position.x] = gate->GetSymbol();
}

void Field::PlacePlayer(Player* player, const Vector2Int& position) {
    if (IsValidPosition(position) && IsEmpty(position)) {
        player->SetPosition(position);
        grid[position.y][position.x] = player->GetSymbol();
    }
}

void Field::Render() const {
    // ȭ�� Ŀ�� ��ġ �ʱ�ȭ
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(console, cursorPos);

    // �ʵ� ���
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::cout << grid[y][x];
        }
        std::cout << std::endl;
    }
}

bool Field::IsValidPosition(const Vector2Int& position) const {
    return position.x > 0 && position.x < width - 1 &&
        position.y > 0 && position.y < height - 1;
}

bool Field::IsEmpty(const Vector2Int& position) const {
    if (!IsValidPosition(position)) {
        return false;
    }
    return grid[position.y][position.x] == ' ';
}

void Field::MoveEntity(const Vector2Int& oldPos, const Vector2Int& newPos, char symbol) {
    if (IsValidPosition(oldPos) && IsValidPosition(newPos)) {
        grid[oldPos.y][oldPos.x] = ' ';
        grid[newPos.y][newPos.x] = symbol;
    }
}

bool Field::IsGatePosition(const Vector2Int& position) const {
    if (gate && gate->GetPosition() == position) {
        return true;
    }
    return false;
}

Enemy* Field::GetEnemyAt(const Vector2Int& position) {
    for (auto enemy : enemies) {
        if (enemy->GetPosition() == position) {
            return enemy;
        }
    }
    return nullptr;
}

void Field::RemoveEnemy(Enemy* enemy) {
    if (!enemy) return;

    Vector2Int position = enemy->GetPosition();
    grid[position.y][position.x] = ' ';

    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        enemies.erase(it);
        delete enemy;
    }
}

char Field::GetCell(const Vector2Int& position) const {
    if (IsValidPosition(position)) {
        return grid[position.y][position.x];
    }
    return '#'; // ��ȿ���� ���� ��ġ�� ������ ó��
}