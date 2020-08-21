#include "GameLost.h"

void GameLost::Initialize() {
    state.nextScene = -1;
    state.thisScene = 3;
    state.gameStart = false;
    state.player = new Player();
    state.playerHB = new PlayerHB();
    state.playerBulPooler = new PlayerBulPool(0, 0);
}
void GameLost::Update(float deltaTime) {
    if (state.gameStart) {
        state.nextScene = 1;
    }
}
void GameLost::Render(ShaderProgram* program, ShaderProgram* bgShader) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "You Lost!", 1.0f, -0.55f, glm::vec3(-1.7, 0.5f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press ENTER to Restart", 0.8f, -0.45f, glm::vec3(-3.55, -1, 0));
}