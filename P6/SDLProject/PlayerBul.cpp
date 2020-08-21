#include "PlayerBul.h"

PlayerBul::PlayerBul() : Entity::Entity() {
    entityType = PLAYERBUL;
    radius = 0.01f;
}

void PlayerBul::CheckCollisions(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            if (lastCollision == ENEMY) {
                lastCollided->hp -= 1;
                isActive = false;
                lastCollision = NA;
            }
        }
    }
}

void PlayerBul::Fire(glm::vec3 playerPos, int fireMode, int i) {
    if (!isActive) {
        return;
    }
    if (fireMode == 0) {
        position = playerPos;
        position.x = playerPos.x - 0.6f + (0.15f * i);
        position.y += 0.7f;

        rotation = 0;
    }
    else if (fireMode == 1) {
        position = playerPos;
        position.x = playerPos.x - 0.6f + (0.15f * i);
        position.y += 0.7f;

        rotation = -20.0f + 5.0f * i;
    }
    
}

void PlayerBul::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* fairies, int fairiesCount) {
    if (!isActive) {
        return;
    }

    if (position.y > -5.0f && position.y < 5.0f && position.x < 6.0f && position.x > -6.0f) {
        CheckCollisions(fairies, fairiesCount);
        CheckCollisions(enemies, enemyCount);
        position.y += speed * cos(glm::radians(rotation)) * deltaTime;
        position.x += speed * sin(glm::radians(rotation)) * deltaTime;
    }
    else {
        isActive = false;
    }

    modelMatrix = glm::mat4(1.0f);
    
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-rotation), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0));
}
