#include "MenuScreen.h"

void MenuScreen::Initialize() {
    state.nextScene = -1;
    state.thisScene = 0;
    state.player = new Entity();
    state.lives = 3;
}
void MenuScreen::Update(float deltaTime) {
    if (state.gameStart) {
        state.nextScene = 1;
    }
}
void MenuScreen::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press ENTER to Start", 1.0f, -0.55f, glm::vec3(-4, 0, 0));
}