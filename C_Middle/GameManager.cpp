#include "GameManager.h"
#include <random>
#include <conio.h>  // Windows에서 _getch() 사용을 위한 헤더

GameManager::GameManager()
    : initState(this), playerTurnState(this), enemyTurnState(this),
    width(20), height(10), isRunning(false), currentLevel(1) {
}

void GameManager::Initialize() {
    // 플레이어 생성
    player = std::make_shared<Player>("용사");

    // 게임 초기화 상태로 전환
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
    std::cout << "게임 종료\n";
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
    // 기존 엔티티 정리
    entities.clear();
    enemies.clear();

    // 플레이어 위치 설정 (맵 중앙)
    player->SetPosition(Vector2Int(width / 2, height / 2));
    entities[player->GetPosition()] = player;

    // 랜덤 생성기
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);

    // 적 생성 (레벨에 따라 수가 증가)
    int enemyCount = 2 + currentLevel;
    for (int i = 0; i < enemyCount; ++i) {
        Vector2Int pos;
        do {
            pos = Vector2Int(disX(gen), disY(gen));
        } while (entities.find(pos) != entities.end());

        int enemyHp = 5 + currentLevel;
        int enemyAttack = 2 + currentLevel / 2;
        auto enemy = std::make_shared<Enemy>("적 " + std::to_string(i + 1), enemyHp, enemyAttack);
        enemy->SetPosition(pos);

        entities[pos] = enemy;
        enemies.push_back(enemy);
    }

    // 게이트 생성
    Vector2Int gatePos;
    do {
        gatePos = Vector2Int(disX(gen), disY(gen));
    } while (entities.find(gatePos) != entities.end());

    gate = std::make_shared<Gate>();
    gate->SetPosition(gatePos);
    entities[gatePos] = gate;

    std::cout << "레벨 " << currentLevel << " 생성 완료! (적: " << enemyCount << "마리)\n";
}

void GameManager::MovePlayer(const Vector2Int& direction) {
    Vector2Int newPos = player->GetPosition() + direction;

    // 맵 범위 확인
    if (!IsPositionValid(newPos)) {
        std::cout << "이동할 수 없습니다.\n";
        return;
    }

    // 충돌 확인
    auto entityAtNewPos = GetEntityAt(newPos);
    if (entityAtNewPos) {
        if (entityAtNewPos->GetType() == EntityType::ENEMY) {
            // 적과 충돌 시 자동 공격
            PlayerAttack(direction);
            return;
        }
        else if (entityAtNewPos->GetType() == EntityType::GATE) {
            // 게이트와 충돌 시 다음 레벨로
            NextLevel();
            return;
        }
    }

    // 이동 처리
    RemoveEntity(player->GetPosition());
    player->Move(direction);
    entities[player->GetPosition()] = player;
}

void GameManager::PlayerAttack(const Vector2Int& direction) {
    Vector2Int targetPos = player->GetPosition() + direction;

    // 맵 범위 확인
    if (!IsPositionValid(targetPos)) {
        std::cout << "공격 대상이 없습니다.\n";
        return;
    }

    // 대상 확인
    auto targetEntity = GetEntityAt(targetPos);
    if (!targetEntity || targetEntity->GetType() != EntityType::ENEMY) {
        std::cout << "공격 대상이 없습니다.\n";
        return;
    }

    // 공격 처리
    auto enemy = std::static_pointer_cast<Enemy>(targetEntity);
    int damage = player->Attack();
    enemy->TakeDamage(damage);

    std::cout << player->GetName() << "이(가) " << enemy->GetName()
        << "에게 " << damage << "의 데미지를 입혔습니다.\n";

    // 적 사망 처리
    if (!enemy->IsAlive()) {
        std::cout << enemy->GetName() << "을(를) 물리쳤습니다!\n";
        RemoveEntity(enemy->GetPosition());

        // 킬 카운트 증가 및 레벨업 체크
        player->IncreaseKills();
        if (player->GetKills() % 3 == 0) {  // 3킬마다 레벨업
            player->LevelUp();
            std::cout << "레벨 업! 현재 레벨: " << player->GetLevel() << "\n";
            std::cout << "체력과 공격력이 증가했습니다. (HP: " << player->GetHp()
                << ", 공격력: " << player->GetAttack() << ")\n";
        }

        // 적 목록에서 제거
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

        // 플레이어와의 거리 확인
        Vector2Int enemyPos = enemy->GetPosition();
        Vector2Int playerPos = player->GetPosition();

        // 맨해튼 거리
        int distance = std::abs(enemyPos.x - playerPos.x) + std::abs(enemyPos.y - playerPos.y);

        if (distance <= 1) {
            // 공격 범위 내면 공격
            int damage = enemy->Attack();
            player->TakeDamage(damage);

            std::cout << enemy->GetName() << "이(가) " << player->GetName()
                << "에게 " << damage << "의 데미지를 입혔습니다.\n";
        }
        else {
            // 그렇지 않으면 이동
            Vector2Int moveDir = enemy->DecideMove(playerPos);
            Vector2Int newPos = enemyPos + moveDir;

            // 이동 가능한지 확인
            if (IsPositionValid(newPos) && !GetEntityAt(newPos)) {
                RemoveEntity(enemyPos);
                enemy->Move(moveDir);
                entities[enemy->GetPosition()] = enemy;
            }
        }
    }

    // 턴 종료 후 상태 출력
    std::cout << "플레이어 HP: " << player->GetHp() << "\n";
    RenderGame();
}

void GameManager::CheckGameOver() {
    // 플레이어 사망 확인
    if (!player->IsAlive()) {
        std::cout << "게임 오버! 플레이어가 쓰러졌습니다.\n";
        std::cout << "최종 레벨: " << currentLevel << ", 처치한 적: " << player->GetKills() << "\n";
        QuitGame();
        return;
    }

    // 모든 적 처치 확인
    if (enemies.empty()) {
        std::cout << "모든 적을 물리쳤습니다! 게이트를 찾아 다음 레벨로 진행하세요.\n";
    }
}

void GameManager::NextLevel() {
    std::cout << "게이트를 통과했습니다!\n";
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
    // 맵 상단 경계
    std::cout << "+";
    for (int x = 0; x < width; ++x) {
        std::cout << "-";
    }
    std::cout << "+\n";

    // 맵 내용
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

    // 맵 하단 경계
    std::cout << "+";
    for (int x = 0; x < width; ++x) {
        std::cout << "-";
    }
    std::cout << "+\n";

    // 상태 정보
    std::cout << "레벨: " << currentLevel << " | ";
    std::cout << "플레이어 HP: " << player->GetHp() << " | ";
    std::cout << "공격력: " << player->GetAttack() << " | ";
    std::cout << "킬: " << player->GetKills() << "\n";

    // 조작법
    std::cout << "조작: 방향키(WASD) 이동/공격, Q 종료\n";
}

void GameManager::ProcessInput() {
    std::cout << "행동을 선택하세요 (WASD: 이동/공격, Q: 종료): ";
    char input = _getch();  // 키 입력 받기
    std::cout << input << std::endl;

    Vector2Int direction(0, 0);

    switch (input) {
    case 'w': case 'W':  // 위
        direction = Vector2Int(0, -1);
        break;
    case 's': case 'S':  // 아래
        direction = Vector2Int(0, 1);
        break;
    case 'a': case 'A':  // 왼쪽
        direction = Vector2Int(-1, 0);
        break;
    case 'd': case 'D':  // 오른쪽
        direction = Vector2Int(1, 0);
        break;
    case 'q': case 'Q':  // 종료
        QuitGame();
        return;
    default:
        std::cout << "잘못된 입력입니다.\n";
        ProcessInput();  // 다시 입력 받기
        return;
    }

    MovePlayer(direction);
}