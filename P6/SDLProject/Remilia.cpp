#include "Remilia.h"
#include "Util.h"
#include "Sound.h"

Remilia::Remilia() {
    accumulator = 0.0f;
    updateRate = 0.4f;
    maxBullets = 1000;
    spellNum = 0;
    pauseTimer = 0;
    i = 0;
    shotsFired = 0;

    bulletPool = new EnemyBulPool(maxBullets, Util::LoadTexture("enemyBul.png"));

    sound = new Sound();
}

void Remilia::CheckCollisions(Entity* objects, int objectCount) {
}

void Remilia::Movement(float deltaTime) {
    if (position.x > 3) {
        velocity.x = -1;
    }
    else if(position.x < -3){
        velocity.x = 1;
    }

    position.x += velocity.x * speed * deltaTime;
}

void Remilia::Spell0(glm::vec3 playerPos, float deltaTime) {
    pauseTimer += deltaTime;
    if (pauseTimer >= 3.0f || shotsFired != 0) {
        accumulator += deltaTime;
        if (accumulator >= updateRate) {
            accumulator = 0;
            bulletPool->getPooledBul()->Fire(playerPos, position, 0, 1.0f, shotsFired, 0);
            
            shotsFired++;

            if (shotsFired >= 9) {
                shotsFired = 0;
                pauseTimer = 0;
            }
        }
    }
}
void Remilia::Spell1(glm::vec3 playerPos, float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= updateRate) {
        accumulator = 0;
        bulletPool->getPooledBul()->Fire(playerPos, position, 1, 0.5f, shotsFired, 0);

        shotsFired++;

        if (shotsFired >= 64) {
            shotsFired = 0;
        }
    }
}
void Remilia::Spell2(glm::vec3 playerPos, float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= updateRate) {
        accumulator = 0;
        bulletPool->getPooledBul()->Fire(playerPos, position, 2, 0.4f, shotsFired, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 2, 0.4f, -shotsFired, 0);

        shotsFired++;

        if (shotsFired >= 16) {
            shotsFired = 0;
        }
    }
}
void Remilia::Spell3(glm::vec3 playerPos, float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= updateRate) {
        accumulator = 0;
        bulletPool->getPooledBul()->Fire(playerPos, position, 3, 0.4f, shotsFired, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 3, 0.4f, -shotsFired, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 3, 0.6f, 2*i, 0);

        shotsFired++;
        i++;

        if (shotsFired >= 10) {
            shotsFired = 0;
        }
        if (i >= 32) {
            i = 0;
        }
    }
}

void Remilia::Spell4(glm::vec3 playerPos, float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= updateRate) {
        accumulator = 0;
        bulletPool->getPooledBul()->Fire(playerPos, position, 4, 0.4f, shotsFired, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 4, 0.4f, -shotsFired, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 4, 0.6f, -2 * i, 0);
        bulletPool->getPooledBul()->Fire(playerPos, position, 4, 0.6f, 2 * i, 0);

        shotsFired++;
        i++;

        if (shotsFired >= 16) {
            shotsFired = 0;
        }
        if (i >= 32) {
            i = 0;
        }
    }
}

void Remilia::Spell5(glm::vec3 playerPos, float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= updateRate/8) {
        accumulator = 0;
        bulletPool->getPooledBul()->Fire(playerPos, position, 5, 0.7f, shotsFired, 0);

        shotsFired++;

        if (shotsFired >= 360 * 360) {
            shotsFired = 0;
        }
    }
}

void Remilia::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
    if (hp < 1) {
        isActive = false;
    }

    if (!isActive) {
        bulletPool->clearBulPool();
        return;
    }

    Movement(deltaTime);

    if (hp >= 2000) {
        spellNum = 0;
    }
    else if (hp < 2000 && hp >= 1600) {
        spellNum = 1;
    }
    else if (hp < 1600 && hp >= 1250) {
        spellNum = 2;
    }
    else if (hp < 1250 && hp >= 900) {
        spellNum = 3;
    }
    else if (hp < 900 && hp >= 600) {
        spellNum = 4;
    }
    else if (hp < 600 && hp >= 0) {
        spellNum = 5;
    }

    switch (spellNum)
    {
    case 0:
        Spell0(player->position, deltaTime);
        break;
    case 1:
        Spell1(player->position, deltaTime);
        break;
    case 2:
        Spell2(player->position, deltaTime);
        break;
    case 3:
        Spell3(player->position, deltaTime);
        break;
    case 4:
        Spell4(player->position, deltaTime);
        break;
    case 5:
        Spell5(player->position, deltaTime);
        break;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);

    bulletPool->Update(deltaTime, player, playerHB, enemies, enemyCount, bullets, bulletCount);
}

void Remilia::Render(ShaderProgram* program) {
    Entity::Render(program);
    for (int i = 0; i < bulletPool->pooledBul.size(); i++) {
        bulletPool->pooledBul[i]->Entity::Render(program);
    }
}