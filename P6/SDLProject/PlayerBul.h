#pragma once
#include "Entity.h"
#include "Player.h"

class PlayerBul : public Entity {
public:
    float speed = 1.3f;

    float rotation = 0;

    PlayerBul();

    void CheckCollisions(Entity* objects, int objectCount) override;

    void Fire(glm::vec3 playerPos, int fireMode, int i);

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity*, int bulletCount) override;
};