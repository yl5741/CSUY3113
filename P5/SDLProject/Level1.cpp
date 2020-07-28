#include "Level1.h"
#include <SDL_mixer.h>
#include <string>
#include "Sound.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define ENEMY_COUNT 3
unsigned int level1_data[] =
{
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3,
3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3,
3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3
};

void Level1::Initialize() {
    state.nextScene = -1;
    state.thisScene = 1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    state.arrow = new Entity();
    state.arrow->textureID = Util::LoadTexture("arrow.png");
    state.arrow->position = glm::vec3(13, -1, 0);

    // Initialize Enemies
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    state.enemies = new Entity[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].speed = 1.0f;
        state.enemies[i].velocity = glm::vec3(0);
        state.enemies[i].acceleration = glm::vec3(0, -8.25f, 0);
        state.enemies[i].height = 0.7f;
        state.enemies[i].width = 0.7f;
        state.enemies[i].jumpPower = 4.0f;
        state.enemies[i].animRight = new int[1]{ 0 };
        state.enemies[i].animLeft = new int[1]{ 1 };
        state.enemies[i].animIndices = state.enemies[i].animRight;
        state.enemies[i].animCols = 2;
        state.enemies[i].animRows = 1;
        state.enemies[i].animFrames = 1;
        //state.enemies[i].isActive = false;
    }
    state.enemies[0].position = glm::vec3(13, -2, 0);
    state.enemies[0].aiType = JALKER;
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[1].position = glm::vec3(2, -4, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[2].position = glm::vec3(8, -5, 0);
    state.enemies[2].aiType = WALKER;

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->velocity = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -8.25f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->animRight = new int[4]{ 3,7,11,15 };
    state.player->animLeft = new int[4]{ 1,5,9,13 };
    state.player->animIndices = state.player->animRight;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->animFrames = 4;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->jumpPower = 5.5f;
    state.player->sound = new Sound();
}
void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.enemies, state.player, state.map, ENEMY_COUNT);
    state.arrow->Update(deltaTime, state.enemies, state.player, state.map, ENEMY_COUNT);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, state.map, ENEMY_COUNT);
    }

    if (state.player->lastCollision == ENEMY) {
        if (state.player->collidedLeft || state.player->collidedRight || state.player->collidedTop) {
            state.lives--;
            state.player->sound->playFailureSound();
            state.nextScene = 1; // Restart Level
        }
        else {
            state.player->jump = true;
            state.player->lastCollided->isActive = false;
        }
    }

    if (state.player->position.x >= 13) {
        state.nextScene = 2;
    }
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    state.arrow->Render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    std::string lives = std::to_string(state.lives);

    if (state.player->position.x <= 5) {
        Util::DrawText(program, Util::LoadTexture("font1.png"), "Lives: " + lives, 0.6f, -0.4f, glm::vec3(5-0.5f, -1, 0));
    }
    else if (state.player->position.x >= 8.5) {
        Util::DrawText(program, Util::LoadTexture("font1.png"), "Lives: " + lives, 0.6f, -0.4f, glm::vec3(8.5f - 0.5f, -1, 0));
    }
    else {
        Util::DrawText(program, Util::LoadTexture("font1.png"), "Lives: " + lives, 0.6f, -0.4f, glm::vec3(state.player->position.x-0.5f, -1, 0));
    }
}