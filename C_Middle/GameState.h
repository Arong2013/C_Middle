#pragma once

class GameManager;

// 게임 상태의 기본 클래스
class GameState {
protected:
    GameManager* gameManager;

public:
    GameState(GameManager* manager) : gameManager(manager) {}
    virtual ~GameState() {}

    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
};

// 초기화 상태
class InitState : public GameState {
public:
    InitState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 플레이어 턴 상태
class PlayerTurnState : public GameState {
public:
    PlayerTurnState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 적 턴 상태
class EnemyTurnState : public GameState {
public:
    EnemyTurnState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};