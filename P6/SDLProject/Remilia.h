#pragma once
#include "Entity.h"
#include "EnemyBulPool.h"
class Remilia;

class Remilia : public Entity {
public:
    float accumulator;
    float updateRate;
    float pauseTimer;
    EnemyBulPool* bulletPool;

    int maxBullets;

    int spellNum;
    int shotsFired;
    int i;

    Sound* sound;

    Remilia();

    void CheckCollisions(Entity* objects, int objectCount) override;

    void Movement(float deltaTime);

    void Spell0(glm::vec3 playerPos, float deltaTime);
    void Spell1(glm::vec3 playerPos, float deltaTime);
    void Spell2(glm::vec3 playerPos, float deltaTime);
    void Spell3(glm::vec3 playerPos, float deltaTime);
    void Spell4(glm::vec3 playerPos, float deltaTime);
    void Spell5(glm::vec3 playerPos, float deltaTime);

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) override;

    void Render(ShaderProgram* program) override;
};