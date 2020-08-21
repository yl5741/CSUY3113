#pragma once
#include "Entity.h"
#include "Player.h"
#include "PlayerHB.h"

class EnemyBul : public Entity {
public:
    float speed = 0;
    float rotation = 0;

    EnemyBul();

    void CheckCollisions(PlayerHB* objects, int objectCount);

    void Fire(glm::vec3 playerPos, glm::vec3 enemyPos, int spellNum, float bulSpeed, int i, int j);

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
};