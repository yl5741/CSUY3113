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

#include "Util.h"
#include "Entity.h"
#include "Map.h"

class Scene;

struct GameState {
	Map* map;
	Scene* previousScene;
	Entity* player;
	Entity* enemies;
	Entity* arrow;
	Entity* cake;
	bool gameOver = false; 
	bool gameStart = false;
	int nextScene;
	int thisScene;
	int lives;
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;


};