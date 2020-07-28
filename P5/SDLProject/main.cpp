#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "MenuScreen.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "GameWin.h"
#include "GameLose.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[6];

Mix_Music* bgm;

void SwitchToScene(Scene* previousScene, Scene* nextScene) {
    currentScene = nextScene;
    currentScene->state.previousScene = previousScene;
    currentScene->state.lives = currentScene->state.previousScene->state.lives;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    bgm = Mix_LoadMUS("dooblydoo.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_PlayMusic(bgm, -1);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new MenuScreen();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new GameLose();
    sceneList[5] = new GameWin();

    SwitchToScene(sceneList[0], sceneList[0]);
}

void ProcessInput() {
    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                currentScene->state.player->animIndices = currentScene->state.player->animLeft;
                break;

            case SDLK_RIGHT:
                // Move the player right
                currentScene->state.player->animIndices = currentScene->state.player->animRight;
                break;

            case SDLK_UP:
                // Jump
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            case SDLK_RETURN:
                // start game
                currentScene->state.gameStart = true;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 0;
    }
    else if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1;
    }

    if (keys[SDL_SCANCODE_UP]) {
        if (currentScene->state.player->collidedBot) {
            currentScene->state.player->jump = true;
        }
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
}

#define FIXED_TIMESTEP 0.01666666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    viewMatrix = glm::mat4(1.0f);
    if (currentScene == sceneList[0] || currentScene == sceneList[4] || currentScene == sceneList[5]) {
        viewMatrix = glm::mat4(1.0f);
    }
    else if (currentScene == sceneList[3]) {
        if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 14.5 && currentScene->state.player->position.y > -7) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else if (currentScene->state.player->position.x >= 14.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x <= 11 && currentScene->state.player->position.y <= -7) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4.75, 0));
        }
        else if (currentScene->state.player->position.x <= 5 && currentScene->state.player->position.y > -7) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x <= 5 && currentScene->state.player->position.y < -7) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 4.75, 0));
        }
    }
    else if (currentScene->state.player->position.x < 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    else if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 8.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else if (currentScene->state.player->position.x >= 8.5 && currentScene) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-8.5, 3.75, 0));
    }

    if (currentScene->state.player->position.y < -10) {
        currentScene->state.lives--;
        currentScene->state.player->sound->playFailureSound();
        SwitchToScene(sceneList[currentScene->state.thisScene], sceneList[currentScene->state.thisScene]);
    }

    if (currentScene->state.lives == 0) {
        currentScene->state.nextScene = 4; //Game Lose
    }

    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);

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

        if (currentScene->state.nextScene > 0) SwitchToScene(currentScene, sceneList[currentScene->state.nextScene]);

        Render();
    }

    Shutdown();
    return 0;
}