#include "Player.h"

Player::Player(): Entity::Entity(){
}

void Player::CheckCollisions(Entity* object, int objCount) {}

void Player::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
    if (velocity.x > 0) {
        animIndices = animRight;
    }
    else if (velocity.x < 0) {
        animIndices = animLeft;
    }
    else {
        animIndices = animStill;
    }

    if (animIndices != NULL) {
        animTime += deltaTime;
        if (animTime >= 0.5f)
        {
            animTime = 0.0f;
            animIndex++;
            if (animIndex >= animFrames)
            {
                animIndex = 0;
            }
        }
    }

    velocity += acceleration * deltaTime;

    position.y += velocity.y * speed * deltaTime;
    position.x += velocity.x * speed * deltaTime;

    if (position.x > 5.7f) {
        position.x = 5.7f;
    }
    if (position.x < -5.7f) {
        position.x = -5.7f;
    }

    if (position.y > 4.6f) {
        position.y = 4.6f;
    }
    if (position.y < -4.6f) {
        position.y = -4.6f;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}
