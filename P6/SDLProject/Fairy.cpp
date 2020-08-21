#include "Fairy.h"
#include "PlayerHB.h"
#include "Util.h"

Fairy::Fairy() : Entity::Entity() {
    //modelMatrix = glm::mat4(1.0f);
    //entityType = ENEMY;
    radius = 0.5f;
}


void Fairy::CheckCollisions(Entity* objects, int objectCount) {}

void Fairy::CheckCollisions(PlayerHB* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        { 
            if (lastCollision == PLAYERHB) {
                objects->wasHit = true;
                lastCollision = NA;
            }
        }
    }
}

void Fairy::CheckCollisions(std::vector<PlayerBul*> objects, int objectCount)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (CheckCollision(objects[i]))
        {
            if (lastCollision == PLAYERBUL) {
                objects[i]->isActive = false;
                hp--;
                lastCollision = NA;
            }
        }
    }
}


void Fairy::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
}

void Fairy::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, std::vector<PlayerBul*> pooledBul, int bulletCount)
{
    if (hp < 1) {
        isActive = false;
        if (hpAdded == false) {
            playerHB->lives++;
            hpAdded = true;
        }
    }
    if (!isActive) {
        return;
    }

    if (position.y > -6.0f && position.y < 6.0f && position.x < 6.0f && position.x > -6.0f) {
        position.y += velocity.y * speed * deltaTime;
        CheckCollisions(playerHB, 1);
        CheckCollisions(pooledBul, 1);
    }
    else {
        isActive = false;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}
