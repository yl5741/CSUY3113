#pragma once
#include "Entity.h"
class Player;

class Player: public Entity {
public:
    float fireRate = 0;
    float bulTimer = 0;
    int fireMode = 0;

    Player();

    void CheckCollisions(Entity* objects, int objectCount) override;

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) override;
};