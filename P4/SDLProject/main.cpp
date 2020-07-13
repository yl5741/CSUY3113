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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 10
#define ENEMY_COUNT 3

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* text;
    Entity* enemies;
    bool gameOver;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

float gravity = -8.25; 
int enemyKilled = 0;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Rise of the AI!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Game Objects
    state.platforms = new Entity[PLATFORM_COUNT];
    state.enemies = new Entity[ENEMY_COUNT];
    state.text = new Entity();

    GLuint platformTextureID = LoadTexture("platformTexture.png");
    GLuint enemyTextureID = LoadTexture("enemy.png");
    GLuint fontTextureID = LoadTexture("font1.png");
    
    //bottom tiles
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3((float)(i-4.5), -3.25, 0);
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].Update(0, NULL, NULL, NULL, 0, 0);
    }

    //text
    state.text->textureID = fontTextureID;
    state.text->position = glm::vec3(0);
    state.text->Update(0, NULL, NULL, NULL, 0, 0);
    

    //Initialize enemies
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].speed = 1.0f;
        state.enemies[i].velocity = glm::vec3(0);
        state.enemies[i].acceleration = glm::vec3(0, gravity, 0);
        state.enemies[i].height = 0.7f;
        state.enemies[i].width = 0.7f;
        state.enemies[i].jumpPower = 4.0f;
        state.enemies[i].animRight = new int[1]{ 0 };
        state.enemies[i].animLeft = new int[1]{ 1 };
        state.enemies[i].animIndices = state.enemies[i].animRight;
        state.enemies[i].animCols = 2;
        state.enemies[i].animRows = 1;
        state.enemies[i].animFrames = 1;
    }
    state.enemies[0].position = glm::vec3(4.0f, -1.5f, 0);
    state.enemies[0].aiType = JALKER;
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[1].position = glm::vec3(-4.0f, -1.5f, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[2].position = glm::vec3(-3.0f, -1.5f, 0);
    state.enemies[2].aiType = WALKER;

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, -1.5f, 0);
    state.player->movement = glm::vec3(0);
    state.player->velocity = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, gravity, 0);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("player.png");
    state.player->animRight = new int[4]{ 3,7,11,15 };
    state.player->animLeft = new int[4]{ 1,5,9,13 };
    state.player->animIndices = state.player->animRight;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->animFrames = 4;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->jumpPower = 5.5f;
    

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

}

void ProcessInput() {
    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            if (state.gameOver) return;
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                state.player->animIndices = state.player->animLeft;
                break;

            case SDLK_RIGHT:
                // Move the player right
                state.player->animIndices = state.player->animRight;
                break;

            case SDLK_UP:
                // Jump
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    if (state.gameOver) return;
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 0;
    }
    else if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1;
    }

    if (keys[SDL_SCANCODE_UP]) {
        if (state.player->collidedBot) {
            state.player->jump = true;
        }
    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
}

#define FIXED_TIMESTEP 0.01666666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void Update() {
    if (state.gameOver) return;

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.platforms, state.enemies, state.player, PLATFORM_COUNT, ENEMY_COUNT);
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, state.enemies, state.player, PLATFORM_COUNT, ENEMY_COUNT);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    if (state.player->lastCollision == ENEMY) {
        if (state.player->collidedLeft || state.player->collidedRight || state.player->collidedTop) {
            state.gameOver = true;
            glClear(GL_COLOR_BUFFER_BIT);
            for (int i = 0; i < PLATFORM_COUNT; i++) {
                state.platforms[i].Render(&program);
            }
            for (int i = 0; i < ENEMY_COUNT; i++) {
                state.enemies[i].Render(&program);
            }

            state.player->Render(&program);
            state.text->DrawText(&program, state.text->textureID, "You Lose", 1.0f, -0.5f, glm::vec3(-1.7, 0, 0));
            SDL_GL_SwapWindow(displayWindow);
            return;
        }
        else {
            state.player->jump = true;
            state.player->lastCollided->isActive = false;
        }
    }
    if (state.enemies[0].isActive == false && state.enemies[1].isActive == false && state.enemies[2].isActive == false) {
        state.gameOver = true;
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            state.platforms[i].Render(&program);
        }
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Render(&program);
        }

        state.player->Render(&program);
        state.text->DrawText(&program, state.text->textureID, "You Win", 1.0f, -0.5f, glm::vec3(-1.7, 0, 0));
        SDL_GL_SwapWindow(displayWindow);
    }
}

void Render() {
    if (state.gameOver) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);

    state.text->DrawText(&program, state.text->textureID, "Up Arrow to Jump,", 0.8f, -0.43f, glm::vec3(-2.5, 3, 0));
    state.text->DrawText(&program, state.text->textureID, "Land on enemy to kill", 0.8f, -0.43f, glm::vec3(-3.5, 2, 0));

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    state.player->Render(&program);
    

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}