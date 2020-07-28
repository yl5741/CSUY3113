#include "Entity.h"
#include <vector>

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (this->entityType == other->entityType) return false;
    if (!isActive || !(other->isActive)) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) {
        lastCollision = other->entityType;
        lastCollided = other;
        return true;
    }
    return false;
}
void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y >= 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBot = true;
            }
        }
    }
}
void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (movement.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (movement.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}


void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        lastCollision = TILE;
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        lastCollision = TILE;
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        lastCollision = TILE;
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        lastCollision = TILE;
        position.y += penetration_y;
        velocity.y = 0;
        collidedBot = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        lastCollision = TILE;
        position.y += penetration_y;
        velocity.y = 0;
        collidedBot = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        lastCollision = TILE;
        position.y += penetration_y;
        velocity.y = 0;
        collidedBot = true;
    }   
}


void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && movement.x < 0) {
        lastCollision = TILE;
        position.x += penetration_x;
        movement.x = 0;
        collidedLeft = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && movement.x > 0) {
        lastCollision = TILE;
        position.x -= penetration_x;
        movement.x = 0;
        collidedRight = true;
    }
}


void Entity::AIWalker(float deltaTime, Map* map) {
    switch (aiState){
    case IDLE:
        aiState = WALKING;
        movement.x = -1.0f;
        animIndices = animLeft;
        break;
    case WALKING:
        if (collidedLeft) {
            movement.x = 1.0f;
            animIndices = animRight;
        }
        if (collidedRight) {
            movement.x = -1.0f;
            animIndices = animLeft;
        }
        break;
    case JUMPING:
        break;
    case JALKING:
        break;
    }
}

void Entity::AIJumper(float deltaTime, Map* map) {
    switch (aiState) {
    case IDLE:
        aiState = JUMPING;
        break;
    case WALKING:
        break;
    case JUMPING:
        jump = true;
        if (jump && collidedBot) {
            jump = false;
            collidedBot = false;
            velocity.y = jumpPower;
        }
        break;
    case JALKING:
        break;
    }
}

void Entity::AIJalker(float deltaTime, Map* map) {
    switch (aiState) {
    case IDLE:
        aiState = JALKING;
        movement.x = -1.0f;
        break;
    case WALKING:
        break;
    case JUMPING:
        break;
    case JALKING:
        jump = true;
        if (jump && collidedBot) {
            jump = false;
            collidedBot = false;
            velocity.y = jumpPower;
        }
        if (collidedLeft) {
            movement.x = 1.0f;
            animIndices = animRight;
        }
        else if (collidedLeft) {
            movement.x = -1.0f;
            animIndices = animLeft;
        }
        break;
    }
}

void Entity::AI(float deltaTime, Map* map) {
    switch (aiType){
    case WALKER:
        AIWalker(deltaTime, map);
        break;
    case JUMPER:
        AIJumper(deltaTime, map);
        break;
    case JALKER:
        AIJalker(deltaTime, map);
        break;
    }
}

void Entity::Update(float deltaTime, Entity* enemies, Entity* player, Map* map, int enemyCount)
{
    if (!isActive) return;
    if (entityType == ENEMY) {
        AI(deltaTime, map);

        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        collidedTop = false;
        collidedBot = false;
        CheckCollisionsY(map);
        position.x += movement.x * speed * deltaTime;
        collidedRight = false;
        collidedLeft = false;
        CheckCollisionsX(map);

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
    else if (entityType == PLAYER) {
        if (animIndices != NULL) {
            if (glm::length(movement) != 0) {
                animTime += deltaTime;

                if (animTime >= 0.25f)
                {
                    animTime = 0.0f;
                    animIndex++;
                    if (animIndex >= animFrames)
                    {
                        animIndex = 0;
                    }
                }
            }
            else {
                animIndex = 0;
            }
        }
        if (!collidedBot) {
            jump = false;
        }
        if (jump) {
            jump = false;
            collidedBot = false;
            velocity.y = jumpPower;
            sound->playJumpSound();
        }
        
        
        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        collidedTop = false;
        collidedBot = false;
        CheckCollisionsY(map);
        CheckCollisionsY(enemies, enemyCount);

        position.x += movement.x * speed * deltaTime;
        collidedRight = false;
        collidedLeft = false;
        CheckCollisionsX(map);
        CheckCollisionsX(enemies, enemyCount);

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
    else {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
    if (!isActive) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}