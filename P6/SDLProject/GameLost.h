#pragma once
#include "Scene.h"
class GameLost : public Scene {
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program, ShaderProgram* bgShader) override;
};