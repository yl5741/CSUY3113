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
#include "Background.h"

#include "Scene.h"
#include "Level1.h"
#include "Menu.h"
#include "GameWon.h"
#include "GameLost.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;
ShaderProgram bgShader;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[4];

void SwitchToScene(Scene* previousScene, Scene* nextScene) {
    currentScene = nextScene;
    currentScene->state.previousScene = previousScene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Dodge Those Bullets!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    bgShader.Load("shaders/vertex_textured.glsl", "shaders/bg_textured.glsl");
    bgShader.SetProjectionMatrix(projectionMatrix);
    bgShader.SetViewMatrix(viewMatrix);


    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new GameLost();
    sceneList[3] = new GameWon();

    SwitchToScene(sceneList[0], sceneList[0]);
}

void ProcessInput() {
    currentScene->state.player->velocity = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
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
        currentScene->state.player->velocity.x = 0;
    }
    else if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->velocity.x = -1;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->velocity.x = 1;
    }
    
    if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->velocity.y = 0;
    }
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->velocity.y = 1;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->velocity.y = -1;
    }

    if (keys[SDL_SCANCODE_LSHIFT]) {
        currentScene->state.player->speed = 0.2f;
        currentScene->state.player->fireMode = 0;
        currentScene->state.playerHB->isHidden = false;
    }
    else {
        currentScene->state.player->speed = 0.5f;
        currentScene->state.player->fireMode = 1;
        currentScene->state.playerHB->isHidden = true;
    }

    if (keys[SDL_SCANCODE_Z]) {
        currentScene->state.playerBulPooler->fire = true;
    }
    else {
        currentScene->state.playerBulPooler->fire = false;
    }

    if (glm::length(currentScene->state.player->velocity) > 1.0f) {
        currentScene->state.player->velocity = glm::normalize(currentScene->state.player->velocity);
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
    if (currentScene->state.player->position.x >= 1) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-1, 0, 0));
    }
    else if (currentScene->state.player->position.x <= -1) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(1, 0, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 0, 0));
    }
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    bgShader.SetViewMatrix(viewMatrix);

    currentScene->Render(&program, &bgShader);

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
