#include "Menu.h"

void Menu::Initialize() {
    state.nextScene = -1;
    state.thisScene = 0;
    state.player = new Player();
    state.playerHB = new PlayerHB();
    state.playerBulPooler = new PlayerBulPool(0, 0);
}
void Menu::Update(float deltaTime) {
    if (state.gameStart) {
        state.nextScene = 1;
    }
}
void Menu::Render(ShaderProgram* program, ShaderProgram* bgShader) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press ENTER to Start", 1.0f, -0.55f, glm::vec3(-4, 2, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "ARROW KEYS to move, Z to Shoot", 0.7f, -0.4f, glm::vec3(-4, 0, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "LSHIFT to focus and show HitBox", 0.7f, -0.45f, glm::vec3(-4, -1, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Kill Fairies to get extra life", 0.7f, -0.45f, glm::vec3(-4, -2, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Kill enemy and you Win!", 0.7f, -0.45f, glm::vec3(-4, -3, 0));
}