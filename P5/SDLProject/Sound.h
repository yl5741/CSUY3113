#pragma once
#include <SDL_mixer.h>

class Sound {
	Mix_Chunk* jumpSound;
	Mix_Chunk* failureSound;
	Mix_Chunk* gameOverSound;
	Mix_Chunk* successSound;
public:
	void playJumpSound();
	void playFailureSound();
	void playGameOverSound();
	void playSuccessSound();

	Sound();
};