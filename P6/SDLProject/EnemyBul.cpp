#include "EnemyBul.h"
#include "PlayerHB.h"

EnemyBul::EnemyBul() {
    radius = 0.08f;
    entityType = BULLET;
}

void EnemyBul::CheckCollisions(PlayerHB* playerHB, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        if (CheckCollision(playerHB))
        {
            playerHB->wasHit = true;
            isActive = false;
            playerHB->invincible = true;
        }
    }
}

void EnemyBul::Fire(glm::vec3 playerPos, glm::vec3 enemyPos, int spellNum, float bulSpeed, int i, int j) {
    if (!isActive) {
        return;
    }
    speed = bulSpeed;

    if (spellNum == 0) {
        position = enemyPos;
        rotation = 0;
        float dx = playerPos.x - position.x;
        float dy = playerPos.y - position.y;

        //rotation = std::atan(dy / dx);
    }
    else if (spellNum == 1) {
        //insert spell1 pattern
        position = enemyPos;
        speed = bulSpeed;
        rotation = 5.625f * i;
    }
    else if (spellNum == 2) {
        //insert spell2 pattern
        position = enemyPos;
        speed = bulSpeed;
        rotation = 5.625f * i;
    }
    else if (spellNum == 3) {
        //insert spell3 pattern
        position = enemyPos;
        speed = bulSpeed;
        rotation = 5.625f * i;
    }
    else if (spellNum == 4) {
        //insert spell4 pattern
        position = enemyPos;
        speed = bulSpeed;
        rotation = 5.625f * i;
    }
    else if (spellNum == 5) {
        //insert spell5 pattern
        position = enemyPos;
        speed = bulSpeed;
        rotation = 0.05f * i * i;
    }
}

void EnemyBul::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
    if (!isActive) {
        return;
    }

    if (playerHB->invincible) {
    }
    else {
        CheckCollisions(playerHB, 1);
    }

    if (position.y > -5.0f && position.y < 5.0f && position.x < 6.0f && position.x > -6.0f) {
        position.y -= speed * cos(glm::radians(rotation)) * deltaTime;
        position.x -= speed * sin(glm::radians(rotation)) * deltaTime;
    }
    else {
        isActive = false;
    }

    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-rotation), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0));
}
