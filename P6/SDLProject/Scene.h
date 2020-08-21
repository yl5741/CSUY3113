#pragma once
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
#include "Player.h"
#include "PlayerHB.h"
#include "PlayerBulPool.h"
#include "Remilia.h"
#include "Fairy.h"
#include "FairyPool.h"

#include "Util.h"
#include "Background.h"

class Scene;

struct GameState {
	Background* bg;
	Player* player;
	Scene* previousScene;
	PlayerHB* playerHB;
	PlayerBulPool* playerBulPooler;
	std::vector<Remilia*> enemies;
	FairyPool* fairyPooler;

	Fairy* fairy;
	Fairy* fairy2;

	Player* testObj;

	GLuint textTexID;

	int thisScene;
	int nextScene;

	bool gameStart = false;
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program, ShaderProgram* bgShader) = 0;
};