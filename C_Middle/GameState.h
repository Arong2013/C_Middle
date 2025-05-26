#pragma once

class GameManager;

// ���� ������ �⺻ Ŭ����
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

// �ʱ�ȭ ����
class InitState : public GameState {
public:
    InitState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// �÷��̾� �� ����
class PlayerTurnState : public GameState {
public:
    PlayerTurnState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// �� �� ����
class EnemyTurnState : public GameState {
public:
    EnemyTurnState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// ���� ���� ����
class GameOverState : public GameState {
public:
    GameOverState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// ���� Ŭ���� ����
class GameClearState : public GameState {
public:
    GameClearState(GameManager* manager) : GameState(manager) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};
