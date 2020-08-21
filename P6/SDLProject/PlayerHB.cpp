#include "PlayerHB.h"
#include "Sound.h"

PlayerHB::PlayerHB() : Entity::Entity() {
    sound = new Sound();
}

void PlayerHB::CheckCollisions(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            if (lastCollision == ENEMY) {
                wasHit = true;
                lastCollision = NA;
            }
        }
    }
}

void PlayerHB::gotHit(Player* player) {
    sound->playDieSound();
    lives--;
    player->position.y = -2.0f;
    player->position.x = 0.0f;
    invincible = true;
}

void PlayerHB::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* fairies, int fairiesCount) {
    if (invincible) {
        invincTimer += deltaTime;
        if (invincTimer >= 10.0f) {
            invincible = false;
            invincTimer = 0;
        }
    }
    else {
        CheckCollisions(enemies, enemyCount);
    }
    
    if (wasHit) {
        gotHit(player);
        wasHit = false;
    }

    position = player->position;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15, 0));
    
}

void PlayerHB::RenderHB(ShaderProgram* program) {
    if (!isHidden) {
        PlayerHB::Render(program);
    }
}