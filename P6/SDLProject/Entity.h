#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

enum EntityType { PLAYER, PLAYERHB, ENEMY, BULLET, PLAYERBUL, NA};
class Player;
class PlayerHB;

class Entity {
public:
    EntityType entityType;
    EntityType lastCollision;
    Entity* lastCollided = nullptr;

    glm::vec3 position;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float radius;
    float speed;

    int hp;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animStill = NULL;
    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;

    Entity();

    bool CheckCollision(Entity* other);
    virtual void CheckCollisions(Entity* objects, int objectCount);

    virtual void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
    virtual void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};