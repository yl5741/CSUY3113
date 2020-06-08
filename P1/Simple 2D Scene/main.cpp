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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, playerMatrix, preyMatrix1, preyMatrix2, preyMatrix3, projectionMatrix;

GLuint playerTextureID;
GLuint preyTextureID;
GLuint preyTextureID2;
GLuint preyTextureID3;

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

void InitializeMatrices() {
    viewMatrix = glm::mat4(1.0f);
    playerMatrix = glm::mat4(1.0f);
    preyMatrix1 = glm::mat4(1.0f);
    preyMatrix2 = glm::mat4(1.0f);
    preyMatrix3 = glm::mat4(1.0f);
    playerMatrix = glm::scale(playerMatrix, glm::vec3(2.4f, 2.4f, 0.0f));
    preyMatrix1 = glm::scale(preyMatrix1, glm::vec3(1.5f, 1.5f, 0.0f));
    preyMatrix2 = glm::scale(preyMatrix2, glm::vec3(1.5f, 1.5f, 0.0f));
    preyMatrix3 = glm::scale(preyMatrix3, glm::vec3(1.5f, 1.5f, 0.0f));
    playerMatrix = glm::translate(playerMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
    preyMatrix1 = glm::translate(preyMatrix1, glm::vec3(1.0f, 0.9f, 0.0f));
    preyMatrix2 = glm::translate(preyMatrix2, glm::vec3(2.0f, -0.1f, 0.0f));
    preyMatrix3 = glm::translate(preyMatrix3, glm::vec3(1.0f, -0.8f, 0.0f));
    
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Simple 2D Scene!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    playerTextureID = LoadTexture("bear.png");
    preyTextureID = LoadTexture("pekora.png");
    preyTextureID2 = LoadTexture("pekora2.png");
    preyTextureID3 = LoadTexture("pekora3.png");

    InitializeMatrices();

    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.4f, 0.8f, 0.4f, 1.0f);
}


void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;
float player_y = 0.0f;
float prey1_rotation = 0.0f;
float prey2_x = 0.0f;
float prey2_y = 0.0f;
float prey2_rotation = 0.0f;
float prey3_x = 0.0f;
float prey3_y = 0.0f;
float prey3_rotation = 0.0f;


bool playerUp = true;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (player_y > 1.3f) {
        playerUp = false;
    }
    else if (player_y < -1.3f) {
        playerUp = true;
    }

    if (playerUp) {
        player_y += 1.0f * deltaTime;
    }
    else if (!playerUp) {
        player_y -= 1.0f * deltaTime;
    }

    prey1_rotation += -720.0f * deltaTime;

    prey2_rotation += 360.0f / 2 / 3.1415926 * deltaTime;
    prey2_x += glm::cos(ticks) * deltaTime;
    prey2_y += glm::sin(ticks) * deltaTime;

    prey3_rotation += -360.0f * 2 / 3.1415926 * deltaTime;
    prey3_x += glm::cos(4*ticks) * deltaTime;
    prey3_y += -glm::sin(4*ticks) * deltaTime;

    InitializeMatrices();
    playerMatrix = glm::translate(playerMatrix, glm::vec3(0.0f, player_y, 0.0f));

    preyMatrix1 = glm::rotate(preyMatrix1, glm::radians(prey1_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    preyMatrix2 = glm::translate(preyMatrix2, glm::vec3(prey2_x, prey2_y, 0.0f));
    preyMatrix2 = glm::rotate(preyMatrix2, glm::radians(prey2_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    preyMatrix3 = glm::translate(preyMatrix3, glm::vec3(prey3_x, prey3_y, 0.0f));
    preyMatrix3 = glm::rotate(preyMatrix3, glm::radians(prey3_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
}

void drawPlayer() {
    program.SetModelMatrix(playerMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawPrey1() {
    program.SetModelMatrix(preyMatrix1);
    glBindTexture(GL_TEXTURE_2D, preyTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawPrey2() {
    program.SetModelMatrix(preyMatrix2);
    glBindTexture(GL_TEXTURE_2D, preyTextureID2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawPrey3() {
    program.SetModelMatrix(preyMatrix3);
    glBindTexture(GL_TEXTURE_2D, preyTextureID2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    drawPlayer();
    drawPrey1();
    drawPrey2();
    drawPrey3();

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    
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
