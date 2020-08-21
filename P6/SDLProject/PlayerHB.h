#pragma once
#include "Entity.h"
#include "Player.h"
#include "EnemyBul.h"
#include "Sound.h"

class PlayerHB : public Entity {
public:
    int lives;

    bool isHidden;
    bool wasHit;

    bool invincible;
    float invincTimer;

    Sound* sound;

    PlayerHB();

    void CheckCollisions(Entity* objects, int objectCount) override;

    void gotHit(Player* player);

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) override;

    void RenderHB(ShaderProgram* program);
};