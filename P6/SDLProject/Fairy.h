#pragma once
#include "Entity.h"
#include "PlayerHB.h"
#include "PlayerBul.h"

class Fairy;

class Fairy : public Entity {
public:
    bool hpAdded;

    Fairy();

    void CheckCollisions(Entity* objects, int objectCount);
    void CheckCollisions(PlayerHB* objects, int objectCount);
    void CheckCollisions(std::vector<PlayerBul*> objects, int objectCount);

    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
    void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, std::vector<PlayerBul*> pooledBul, int bulletCount);
};