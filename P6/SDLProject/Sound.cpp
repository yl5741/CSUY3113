#include "Sound.h"

Sound::Sound() {
    shootSound = Mix_LoadWAV("shoot.wav");
    dieSound = Mix_LoadWAV("death.wav");
    enemyShoot = Mix_LoadWAV("enemyShoot.wav");
}

void Sound::playShootSound() {
    Mix_VolumeChunk(shootSound, MIX_MAX_VOLUME / 10);
    Mix_PlayChannel(-1, shootSound, 0);
}
void Sound::playDieSound() {
    Mix_VolumeChunk(dieSound, MIX_MAX_VOLUME / 10);
    Mix_PlayChannel(-1, dieSound, 0);
}
void Sound::playEnemyShootSound() {
    Mix_VolumeChunk(enemyShoot, MIX_MAX_VOLUME / 10);
    Mix_PlayChannel(-1, enemyShoot, 0);
}