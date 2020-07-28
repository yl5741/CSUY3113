#include "GameWin.h"

void GameWin::Initialize() {
    state.nextScene = -1;
    state.thisScene = 5;
    state.lives = 3;
    state.gameStart = false;
    state.player = new Entity();
}
void GameWin::Update(float deltaTime) {
    if (state.gameStart) {
        state.nextScene = 1;
    }
}
void GameWin::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "You Win!", 1.0f, -0.55f, glm::vec3(-1.7, 0.5f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press ENTER to Restart", 0.8f, -0.45f, glm::vec3(-3.55, -1, 0));
}