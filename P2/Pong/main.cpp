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
glm::mat4 viewMatrix, p1Matrix, p2Matrix, ballMatrix, startMatrix, projectionMatrix;

glm::vec3 p1Position = glm::vec3(-4.9f, 0.0f, 1.0f);
glm::vec3 p2Position = glm::vec3(4.9f, 0.0f, 1.0f);
glm::vec3 ballPosition = glm::vec3(0.0f, 0.0f, 1.0f);
bool gameStart = false;

void InitializeMatrices() {
    p1Matrix = glm::mat4(1.0f);
    p2Matrix = glm::mat4(1.0f);
    startMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    startMatrix = glm::translate(startMatrix, glm::vec3(0.0f, 1.0f, 1.0f));
}

GLuint gameStartTextureID;
GLuint ballTextureID;
GLuint playerTextureID;

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
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    gameStartTextureID = LoadTexture("press_space.png");
    ballTextureID = LoadTexture("ball.png");
    playerTextureID = LoadTexture("player.png");

    viewMatrix = glm::mat4(1.0f);
    InitializeMatrices();
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec3 p1Movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 p2Movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ballMovement = glm::vec3(1.0f, 1.0f, 0.0f);

void player1Up() {
    if (p1Position.y < 2.75f) {
        p1Movement = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}
void player1Down() {
    if (p1Position.y > -2.75f) {
        p1Movement = glm::vec3(0.0f, -1.0f, 0.0f);
    }
}
void player2Up() {
    if (p2Position.y < 2.75f) {
        p2Movement = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}
void player2Down() {
    if (p2Position.y > -2.75f) {
        p2Movement = glm::vec3(0.0f, -1.0f, 0.0f);
    }
}

void ProcessInput() {
    p1Movement = glm::vec3(0.0f, 0.0f, 0.0f);
    p2Movement = glm::vec3(0.0f, 0.0f, 0.0f);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false; break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        gameStart = true;
                    }
                    break;
            }
            break;
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (gameStart) {
        if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_S]) {
        }
        else if (keys[SDL_SCANCODE_W]) {
            player1Up();
        }
        else if (keys[SDL_SCANCODE_S]) {
            player1Down();
        }

        if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_DOWN]) {
        }
        else if (keys[SDL_SCANCODE_UP]) {
            player2Up();
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            player2Down();
        }
    }
}

float lastTicks = 0.0f;
float p1Speed = 4.0f;
float p2Speed = 4.0f;
float ballSpeed = 5.0f;

void collideP1() {
    ballMovement.x = 1.0f;
}
void collideP2() {
    ballMovement.x = -1.0f;
}
void collideTop() {
    ballMovement.y = -1.0f;
}
void collideBot() {
    ballMovement.y = 1.0f;
}
void collideLeft() {
    gameStart = false;
    InitializeMatrices();
    p1Position = glm::vec3(-4.9f, 0.0f, 1.0f);
    p2Position = glm::vec3(4.9f, 0.0f, 1.0f);
    ballPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    ballMovement.x *= -1.0f;
}
void collideRight() {
    gameStart = false;
    InitializeMatrices();
    p1Position = glm::vec3(-4.9f, 0.0f, 1.0f);
    p2Position = glm::vec3(4.9f, 0.0f, 1.0f);
    ballPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    ballMovement.x *= -1.0f;
}

void handleCollisions() {
    float p1_xdist = glm::abs(ballPosition.x - p1Position.x) - ((0.2f + 0.2f) / 2.0f);
    float p1_ydist = glm::abs(ballPosition.y - p1Position.y) - ((0.2f + 2.0f) / 2.0f);
    if (p1_xdist < 0 && p1_ydist < 0) {
        collideP1();
    }
    float p2_xdist = glm::abs(ballPosition.x - p2Position.x) - ((0.2f + 0.2f) / 2.0f);
    float p2_ydist = glm::abs(ballPosition.y - p2Position.y) - ((0.2f + 2.0f) / 2.0f);
    if (p2_xdist < 0 && p2_ydist < 0) {
        collideP2();
    }
    
    if (ballPosition.y >= 3.65f) {
        collideTop();
    }
    if (ballPosition.y <= -3.65f) {
        collideBot();
    }

    if (ballPosition.x >= 4.9f) {
        collideRight();
    }
    if (ballPosition.x <= -4.9f) {
        collideLeft();
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    InitializeMatrices();
    p1Position += p1Movement * p1Speed * deltaTime;
    p2Position += p2Movement * p2Speed * deltaTime;
    p1Matrix = glm::translate(p1Matrix, p1Position);
    p2Matrix = glm::translate(p2Matrix, p2Position);

    if (gameStart) {
        ballPosition += ballMovement * ballSpeed * deltaTime;
        ballMatrix = glm::translate(ballMatrix, ballPosition);
        handleCollisions();
    }
}

void drawStart() {
    program.SetModelMatrix(startMatrix);
    glBindTexture(GL_TEXTURE_2D, gameStartTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawBall() {
    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawP1() {
    program.SetModelMatrix(p1Matrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawP2() {
    program.SetModelMatrix(p2Matrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    float vertices[] = { -1.0f, -0.5f, 1.0f, -0.5f, 1.0f, 0.5f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 0.5f };
    float texCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    drawStart();

    float ballVertices[] = { -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f };
    float balltexCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, balltexCoords);
    
    drawBall();

    float playerVertices[] = { -0.1f, -1.0f, 0.1f, -1.0f, 0.1f, 1.0f, -0.1f, -1.0f, 0.1f, 1.0f, -0.1f, 1.0f};
    float playertexCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, playerVertices);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, playertexCoords);

    drawP1();
    drawP2();
        
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
