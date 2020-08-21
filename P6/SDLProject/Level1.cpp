#include "Level1.h"
#include <SDL_mixer.h>
#include <string>
#include "Player.h"
#include "PlayerHB.h"
#include "PlayerBul.h"
#include "PlayerBulPool.h"
#include "FairyPool.h"

#define MAX_PLAYER_BUL 150

void Level1::Initialize() {
    state.nextScene = -1; 
    state.thisScene = 1;

    state.bg = new Background("bgs.png");
    
    Mix_Music* bgm;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    bgm = Mix_LoadMUS("bgm.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
    Mix_PlayMusic(bgm, -1);

    state.textTexID = Util::LoadTexture("font1.png");

    state.player = new Player();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, -2.0f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 0.5f;
    state.player->textureID = Util::LoadTexture("sanae.png");
    state.player->animStill = new int[4]{ 0,1,2,3 };
    state.player->animLeft = new int[4]{ 4,5,6,7 };
    state.player->animRight = new int[4]{ 8,9,10,11 };
    state.player->animIndices = state.player->animStill;
    state.player->animCols = 4;
    state.player->animRows = 3;
    state.player->animFrames = 4;
    state.player->fireRate = 1.3f;
    state.player->bulTimer = 0;
    state.player->isActive = true;

    state.playerHB = new PlayerHB();
    state.playerHB->entityType = PLAYERHB;
    state.playerHB->lives = 10;
    state.playerHB->radius = 0.075f;
    state.playerHB->position = glm::vec3(0);
    state.playerHB->invincible = false;
    state.playerHB->invincTimer = 0;
    state.playerHB->textureID = Util::LoadTexture("playerHB.png");
    state.playerHB->modelMatrix = glm::scale(state.playerHB->modelMatrix, glm::vec3(2 * state.playerHB->radius, 2 * state.playerHB->radius, 0));
    state.playerHB->wasHit = false;
    state.playerHB->isHidden = true;
    state.player->isActive = true;

    GLuint playerBulTexID = Util::LoadTexture("playerBul.png");
    state.playerBulPooler = new PlayerBulPool(MAX_PLAYER_BUL, playerBulTexID);

    state.enemies.push_back(new Remilia());
    state.enemies[0]->hp = 2500;
    state.enemies[0]->position = glm::vec3(0, 4, 1);
    state.enemies[0]->velocity.x = 1;
    state.enemies[0]->entityType = ENEMY;
    state.enemies[0]->radius = 0.5f;
    state.enemies[0]->speed = 0.1f;
    state.enemies[0]->textureID = Util::LoadTexture("remilia.png");
    state.enemies[0]->isActive = true;

    GLuint fairyTexID = Util::LoadTexture("fairy.png");
    //state.fairyPooler = new FairyPool(25, fairyTexID);

    state.fairy = new Fairy[11];

    /*
    state.fairy2 = new Fairy();
    state.fairy2->hp = 400;
    state.fairy2->velocity.y = -1;
    state.fairy2->entityType = ENEMY;
    state.fairy2->radius = 0.5f;
    state.fairy2->speed = 0.008f;
    state.fairy2->hpAdded = false;
    state.fairy2->textureID = fairyTexID;
    state.fairy2->position = glm::vec3(-5 + i, 4, 1);
    */

    for (int i = 0; i < 11; i++) {
        state.fairy[i].hp = 400;
        state.fairy[i].velocity.y = -1;
        state.fairy[i].entityType = ENEMY;
        state.fairy[i].radius = 0.5f;
        state.fairy[i].speed = 0.008f;
        state.fairy[i].hpAdded = false;
        state.fairy[i].textureID = fairyTexID;
    }

    for (int i = 0; i < 11; i++) {
        state.fairy[i].position = glm::vec3(-5+i, 5-abs(-2.0f + i/2.5f), 1);
    }
    
    /*
    state.testObj = new Player();
    state.testObj->textureID = Util::LoadTexture("circle.png");
    state.testObj->position = glm::vec3(0);
    state.testObj->entityType = BULLET;
    state.testObj->isActive = true;
    state.testObj->radius = 0.5f;
    */
    //state.fairyPooler->getPooledBul()->Fire(0.1f);
}


void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.playerHB, NULL, 0, NULL, 0);
    state.playerHB->Update(deltaTime, state.player, state.playerHB, state.enemies[0], state.enemies.size(), state.fairy, 11);
    state.playerBulPooler->Update(deltaTime, state.player, state.playerHB, state.enemies[0], state.enemies.size(), state.fairy, 11);
    state.enemies[0]->Update(deltaTime, state.player, state.playerHB, NULL, 0, NULL, 0);
    //state.fairyPooler->Update(deltaTime, state.player, state.playerHB, state.enemies[0], state.enemies.size(), NULL, 0);
    
    for (int i = 0; i < 11; i++) {
        state.fairy[i].Update(deltaTime, state.player, state.playerHB, state.enemies[0], state.enemies.size(), state.playerBulPooler->pooledBul, 150);
    }

    if (state.playerHB->lives <= 0) {
        state.nextScene = 2;
    }
    if (state.enemies[0]->hp <= 0) {
        state.nextScene = 3;
    }
    //state.fairy2->Update(deltaTime, state.player, state.playerHB, state.enemies[0], state.enemies.size(), NULL, 0);

    //state.testObj->Update(deltaTime, state.player, state.playerHB, state.enemies[0], 1, NULL, 0);
}
void Level1::Render(ShaderProgram* program, ShaderProgram* bgShader) {
    state.bg->Render(bgShader);
    state.player->Render(program);
    state.playerHB->RenderHB(program);
    for (int i = 0; i < state.playerBulPooler->pooledBul.size(); i++) {
        state.playerBulPooler->pooledBul[i]->Render(program);
    }

    state.enemies[0]->Render(program);

    for (int i = 0; i < 11; i++) {
        state.fairy[i].Render(program);
    }

    /*
    for (int i = 0; i < 25; i++) {
        state.fairyPooler->pooledBul[i]->Render(program);
    }
    */


    std::string lives = std::to_string(state.playerHB->lives);
    if (state.player->position.x <= -1) {
        Util::DrawText(program, state.textTexID, "Lives: " + lives, 0.6f, -0.4f, glm::vec3(-1 - 4.5f, 4.5f, 0));
    }
    else if (state.player->position.x >= 1) {
        Util::DrawText(program, state.textTexID, "Lives: " + lives, 0.6f, -0.4f, glm::vec3(1 - 4.5f, 4.5f, 0));
    }
    else {
        Util::DrawText(program, state.textTexID, "Lives: " + lives, 0.6f, -0.4f, glm::vec3(state.player->position.x - 4.5f, 4.5f, 0));
    }

    if (state.player->position.x <= -1) {
        Util::DrawText(program, state.textTexID, "EnemyHP: " + std::to_string(state.enemies[0]->hp), 0.6f, -0.35f, glm::vec3(-1 + 1.5f, 4.5f, 0));
    }
    else if (state.player->position.x >= 1) {
        Util::DrawText(program, state.textTexID, "EnemyHP: " + std::to_string(state.enemies[0]->hp), 0.6f, -0.35f, glm::vec3(1 + 1.5f, 4.5f, 0));
    }
    else {
        Util::DrawText(program, state.textTexID, "EnemyHP: " + std::to_string(state.enemies[0]->hp), 0.6f, -0.35f, glm::vec3(state.player->position.x + 1.5f, 4.5f, 0));
    }

    
    //Util::DrawText(program, state.textTexID, "canSpawn: " + std::to_string(state.fairyPooler->fire), 0.6f, -0.4f, glm::vec3(state.player->position.x - 4.5f, 2.5f, 0));

    //state.testObj->Render(program);
}