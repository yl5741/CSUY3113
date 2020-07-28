#include "GameLose.h"

void GameLose::Initialize() {
    state.nextScene = -1;
    state.thisScene = 4;
    state.lives = 3;
    state.gameStart = false;
    state.player = new Entity();
}
void GameLose::Update(float deltaTime) {
    if (state.gameStart) {
        state.nextScene = 1;
    }
}
void GameLose::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "You Lose!", 1.0f, -0.55f, glm::vec3(-1.8, 0.5f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press ENTER to Restart", 0.8f, -0.45f, glm::vec3(-3.55, -1, 0));
}