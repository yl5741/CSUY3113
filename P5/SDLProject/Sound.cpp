#include "Sound.h"

Sound::Sound(){
    jumpSound = Mix_LoadWAV("bounce.wav");
    failureSound = Mix_LoadWAV("failure.wav");
    gameOverSound = Mix_LoadWAV("gameover.wav");
    successSound = Mix_LoadWAV("success.wav");
}

void Sound::playJumpSound() {
    Mix_PlayChannel(-1, jumpSound, 0);
}
void Sound::playFailureSound() {
    Mix_PlayChannel(-1, failureSound, 0);
}
void Sound::playGameOverSound() {
    Mix_PlayChannel(-1, gameOverSound, 0);
}
void Sound::playSuccessSound() {
    Mix_PlayChannel(-1, successSound, 0);
}

