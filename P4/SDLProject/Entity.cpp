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

void Entity::AIWalker(float deltaTime, Entity* platforms, int platformCount) {
    switch (aiState){
    case IDLE:
        aiState = WALKING;
        movement.x = -1.0f;
        animIndices = animLeft;
        break;
    case WALKING:
        if (position.x < -4.75f) {
            movement.x = 1.0f;
            animIndices = animRight;
        }
        else if (position.x > 4.75f) {
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

void Entity::AIJumper(float deltaTime, Entity* platforms, int platformCount) {
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

void Entity::AIJalker(float deltaTime, Entity* platforms, int platformCount) {
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
        if (position.x < -4.75f) {
            movement.x = 1.0f;
            animIndices = animRight;
        }
        else if (position.x > 4.75f) {
            movement.x = -1.0f;
            animIndices = animLeft;
        }
        break;
    }
}

void Entity::AI(float deltaTime, Entity* platforms, int platformCount) {
    switch (aiType){
    case WALKER:
        AIWalker(deltaTime, platforms, platformCount);
        break;
    case JUMPER:
        AIJumper(deltaTime, platforms, platformCount);
        break;
    case JALKER:
        AIJalker(deltaTime, platforms, platformCount);
        break;
    }
}

void Entity::Update(float deltaTime, Entity *platforms, Entity* enemies, Entity* player, int platformCount, int enemyCount)
{
    if (!isActive) return;

    if (entityType == ENEMY) {
        AI(deltaTime, platforms, platformCount);

        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        CheckCollisionsY(platforms, platformCount);
        position.x += movement.x * speed * deltaTime;
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        return;
    }
    else if (entityType == PLATFORM) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        return;
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
        }
        if (position.x < -5) {
            position.x = -4.99f;
        }
        if (position.x > 5) {
            position.x = 4.99f;
        }
            
        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        CheckCollisionsY(platforms, platformCount);
        CheckCollisionsY(enemies, enemyCount);

        position.x += movement.x * speed * deltaTime;
        CheckCollisionsX(platforms, platformCount);
        CheckCollisionsX(enemies, enemyCount);

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

void Entity::DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    }
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

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