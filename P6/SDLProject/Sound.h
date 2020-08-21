#pragma once
#include <SDL_mixer.h>

class Sound {
	Mix_Chunk* shootSound;
	Mix_Chunk* dieSound;
	Mix_Chunk* enemyShoot;
public:
	void playShootSound();
	void playDieSound();
	void playEnemyShootSound();

	Sound();
};
