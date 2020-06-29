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

#define PLATFORM_COUNT 31

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* text;
    Entity* goal;
    bool gameOver;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

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
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    state.goal = new Entity[2];
    state.text = new Entity();

    GLuint platformTextureID = LoadTexture("platformTexture.png");
    GLuint goalTextureID = LoadTexture("goal.png");
    GLuint fontTextureID = LoadTexture("font1.png");
    
    int c = 0;
    //bottom tiles
    for (int i = 0; i < 9; i++) {
        state.platforms[c].textureID = platformTextureID;
        state.platforms[c++].position = glm::vec3((float)(i-4.5), -3.25, 0);
    }

    //left wall
    for (int i = 0; i < 8; i++) {
        state.platforms[c].textureID = platformTextureID;
        state.platforms[c++].position = glm::vec3(-4.5f, (float)(-3.25 + i), 0);
    }
    
    //right wall
    for (int i = 0; i < 8; i++) {
        state.platforms[c].textureID = platformTextureID;
        state.platforms[c++].position = glm::vec3(4.5f, (float)(-3.25 + i), 0);
    }

    //obstables
    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(-3.5f, 2.25f, 0);
    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(-2.5f, 2.25f, 0);
    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(-1.5f, 2.25f, 0);

    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(3.5f, 0, 0);
    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(2.5f, 0, 0);
    state.platforms[c].textureID = platformTextureID;
    state.platforms[c++].position = glm::vec3(1.5f, 0, 0);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = WALL;
        state.platforms[i].Update(0, NULL, 0);
    }

    state.platforms[6].entityType = GOAL;
    state.platforms[7].entityType = GOAL;

    //Goal
    state.goal[0].textureID = goalTextureID;
    state.goal[0].position = glm::vec3(1.5f, -3.25f, 0);
    state.goal[1].textureID = goalTextureID;
    state.goal[1].position = glm::vec3(2.5f, -3.25f, 0);

    for (int i = 0; i < 2; i++) {
        state.goal[i].isActive = true;
        state.goal[i].entityType = GOAL;
        state.goal[i].Update(0, NULL, 0);
    }

    //text
    state.text->textureID = fontTextureID;
    state.text->position = glm::vec3(0);
    state.text->Update(0, NULL, 0);


    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0, 4.25f, 0);
    state.player->movement = glm::vec3(0);
    state.player->velocity = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.08f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("playerShip3_green.png");

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
                break;

            case SDLK_RIGHT:
                // Move the player right
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

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->acceleration.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->acceleration.x = 1.0f;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    if (state.player->lastCollision == WALL) {
        state.text->DrawText(&program, state.text->textureID, "Mission Failed", 1.5f, -0.9f, glm::vec3(-3.8f, 0.5f, 0));
        SDL_GL_SwapWindow(displayWindow);
        state.gameOver = true;
        return;
    }
    else if (state.player->lastCollision == GOAL) {
        state.text->DrawText(&program, state.text->textureID, "Mission Successful", 1.5f, -0.98f, glm::vec3(-4.35f, 0.5f, 0));
        SDL_GL_SwapWindow(displayWindow);
        state.gameOver = true;
        return;
    }
}

void Render() {
    if (state.gameOver) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    for (int i = 0; i < 2; i++) {
        state.goal[i].Render(&program);
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