#include "GameManager.h"
#include <random>
#include <conio.h>  // Windows���� _getch() ����� ���� ���

GameManager::GameManager()
    : initState(this), playerTurnState(this), enemyTurnState(this),
    width(20), height(10), isRunning(false), currentLevel(1) {
}

void GameManager::Initialize() {
    // �÷��̾� ����
    player = std::make_shared<Player>("���");

    // ���� �ʱ�ȭ ���·� ��ȯ
    currentState.reset(new InitState(this));
    currentState->Enter();
    isRunning = true;
}

void GameManager::Run() {
    while (isRunning) {
        currentState->Update();
    }
}

void GameManager::Shutdown() {
    std::cout << "���� ����\n";
    if (currentState) {
        currentState->Exit();
        currentState.reset();
    }
}

void GameManager::ChangeState(GameState* newState) {
    if (currentState) {
        currentState->Exit();
    }

    currentState.reset(newState);

    if (currentState) {
        currentState->Enter();
    }
}

void GameManager::GenerateLevel() {
    // ���� ��ƼƼ ����
    entities.clear();
    enemies.clear();

    // �÷��̾� ��ġ ���� (�� �߾�)
    player->SetPosition(Vector2Int(width / 2, height / 2));
    entities[player->GetPosition()] = player;

    // ���� ������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);

    // �� ���� (������ ���� ���� ����)
    int enemyCount = 2 + currentLevel;
    for (int i = 0; i < enemyCount; ++i) {
        Vector2Int pos;
        do {
            pos = Vector2Int(disX(gen), disY(gen));
        } while (entities.find(pos) != entities.end());

        int enemyHp = 5 + currentLevel;
        int enemyAttack = 2 + currentLevel / 2;
        auto enemy = std::make_shared<Enemy>("�� " + std::to_string(i + 1), enemyHp, enemyAttack);
        enemy->SetPosition(pos);

        entities[pos] = enemy;
        enemies.push_back(enemy);
    }

    // ����Ʈ ����
    Vector2Int gatePos;
    do {
        gatePos = Vector2Int(disX(gen), disY(gen));
    } while (entities.find(gatePos) != entities.end());

    gate = std::make_shared<Gate>();
    gate->SetPosition(gatePos);
    entities[gatePos] = gate;

    std::cout << "���� " << currentLevel << " ���� �Ϸ�! (��: " << enemyCount << "����)\n";
}

void GameManager::MovePlayer(const Vector2Int& direction) {
    Vector2Int newPos = player->GetPosition() + direction;

    // �� ���� Ȯ��
    if (!IsPositionValid(newPos)) {
        std::cout << "�̵��� �� �����ϴ�.\n";
        return;
    }

    // �浹 Ȯ��
    auto entityAtNewPos = GetEntityAt(newPos);
    if (entityAtNewPos) {
        if (entityAtNewPos->GetType() == EntityType::ENEMY) {
            // ���� �浹 �� �ڵ� ����
            PlayerAttack(direction);
            return;
        }
        else if (entityAtNewPos->GetType() == EntityType::GATE) {
            // ����Ʈ�� �浹 �� ���� ������
            NextLevel();
            return;
        }
    }

    // �̵� ó��
    RemoveEntity(player->GetPosition());
    player->Move(direction);
    entities[player->GetPosition()] = player;
}

void GameManager::PlayerAttack(const Vector2Int& direction) {
    Vector2Int targetPos = player->GetPosition() + direction;

    // �� ���� Ȯ��
    if (!IsPositionValid(targetPos)) {
        std::cout << "���� ����� �����ϴ�.\n";
        return;
    }

    // ��� Ȯ��
    auto targetEntity = GetEntityAt(targetPos);
    if (!targetEntity || targetEntity->GetType() != EntityType::ENEMY) {
        std::cout << "���� ����� �����ϴ�.\n";
        return;
    }

    // ���� ó��
    auto enemy = std::static_pointer_cast<Enemy>(targetEntity);
    int damage = player->Attack();
    enemy->TakeDamage(damage);

    std::cout << player->GetName() << "��(��) " << enemy->GetName()
        << "���� " << damage << "�� �������� �������ϴ�.\n";

    // �� ��� ó��
    if (!enemy->IsAlive()) {
        std::cout << enemy->GetName() << "��(��) �����ƽ��ϴ�!\n";
        RemoveEntity(enemy->GetPosition());

        // ų ī��Ʈ ���� �� ������ üũ
        player->IncreaseKills();
        if (player->GetKills() % 3 == 0) {  // 3ų���� ������
            player->LevelUp();
            std::cout << "���� ��! ���� ����: " << player->GetLevel() << "\n";
            std::cout << "ü�°� ���ݷ��� �����߽��ϴ�. (HP: " << player->GetHp()
                << ", ���ݷ�: " << player->GetAttack() << ")\n";
        }

        // �� ��Ͽ��� ����
        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (*it == enemy) {
                enemies.erase(it);
                break;
            }
        }
    }
}

void GameManager::ProcessEnemyTurns() {
    for (const auto& enemy : enemies) {
        if (!enemy->IsAlive()) continue;

        // �÷��̾���� �Ÿ� Ȯ��
        Vector2Int enemyPos = enemy->GetPosition();
        Vector2Int playerPos = player->GetPosition();

        // ����ư �Ÿ�
        int distance = std::abs(enemyPos.x - playerPos.x) + std::abs(enemyPos.y - playerPos.y);

        if (distance <= 1) {
            // ���� ���� ���� ����
            int damage = enemy->Attack();
            player->TakeDamage(damage);

            std::cout << enemy->GetName() << "��(��) " << player->GetName()
                << "���� " << damage << "�� �������� �������ϴ�.\n";
        }
        else {
            // �׷��� ������ �̵�
            Vector2Int moveDir = enemy->DecideMove(playerPos);
            Vector2Int newPos = enemyPos + moveDir;

            // �̵� �������� Ȯ��
            if (IsPositionValid(newPos) && !GetEntityAt(newPos)) {
                RemoveEntity(enemyPos);
                enemy->Move(moveDir);
                entities[enemy->GetPosition()] = enemy;
            }
        }
    }

    // �� ���� �� ���� ���
    std::cout << "�÷��̾� HP: " << player->GetHp() << "\n";
    RenderGame();
}

void GameManager::CheckGameOver() {
    // �÷��̾� ��� Ȯ��
    if (!player->IsAlive()) {
        std::cout << "���� ����! �÷��̾ ���������ϴ�.\n";
        std::cout << "���� ����: " << currentLevel << ", óġ�� ��: " << player->GetKills() << "\n";
        QuitGame();
        return;
    }

    // ��� �� óġ Ȯ��
    if (enemies.empty()) {
        std::cout << "��� ���� �����ƽ��ϴ�! ����Ʈ�� ã�� ���� ������ �����ϼ���.\n";
    }
}

void GameManager::NextLevel() {
    std::cout << "����Ʈ�� ����߽��ϴ�!\n";
    currentLevel++;
    GenerateLevel();
}

bool GameManager::IsPositionValid(const Vector2Int& position) const {
    return position.x >= 0 && position.x < width &&
        position.y >= 0 && position.y < height;
}

std::shared_ptr<Entity> GameManager::GetEntityAt(const Vector2Int& position) {
    auto it = entities.find(position);
    if (it != entities.end()) {
        return it->second;
    }
    return nullptr;
}

void GameManager::RemoveEntity(const Vector2Int& position) {
    entities.erase(position);
}

void GameManager::RenderGame() const {
    // �� ��� ���
    std::cout << "+";
    for (int x = 0; x < width; ++x) {
        std::cout << "-";
    }
    std::cout << "+\n";

    // �� ����
    for (int y = 0; y < height; ++y) {
        std::cout << "|";
        for (int x = 0; x < width; ++x) {
            Vector2Int pos(x, y);
            auto it = entities.find(pos);

            if (it != entities.end()) {
                std::cout << it->second->GetSymbol();
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "|\n";
    }

    // �� �ϴ� ���
    std::cout << "+";
    for (int x = 0; x < width; ++x) {
        std::cout << "-";
    }
    std::cout << "+\n";

    // ���� ����
    std::cout << "����: " << currentLevel << " | ";
    std::cout << "�÷��̾� HP: " << player->GetHp() << " | ";
    std::cout << "���ݷ�: " << player->GetAttack() << " | ";
    std::cout << "ų: " << player->GetKills() << "\n";

    // ���۹�
    std::cout << "����: ����Ű(WASD) �̵�/����, Q ����\n";
}

void GameManager::ProcessInput() {
    std::cout << "�ൿ�� �����ϼ��� (WASD: �̵�/����, Q: ����): ";
    char input = _getch();  // Ű �Է� �ޱ�
    std::cout << input << std::endl;

    Vector2Int direction(0, 0);

    switch (input) {
    case 'w': case 'W':  // ��
        direction = Vector2Int(0, -1);
        break;
    case 's': case 'S':  // �Ʒ�
        direction = Vector2Int(0, 1);
        break;
    case 'a': case 'A':  // ����
        direction = Vector2Int(-1, 0);
        break;
    case 'd': case 'D':  // ������
        direction = Vector2Int(1, 0);
        break;
    case 'q': case 'Q':  // ����
        QuitGame();
        return;
    default:
        std::cout << "�߸��� �Է��Դϴ�.\n";
        ProcessInput();  // �ٽ� �Է� �ޱ�
        return;
    }

    MovePlayer(direction);
}