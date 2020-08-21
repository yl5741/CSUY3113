#pragma once
#include "PlayerBul.h"
#include "Player.h"
#include <vector>
#include "Sound.h"

class PlayerBulPool {
public:
	std::vector<PlayerBul*> pooledBul;
	PlayerBul* bulToPool;
	int amountToPool;

	bool fire = false;

	Sound* sound;

	PlayerBulPool(int a, GLuint texID);
	PlayerBul* getPooledBul();
	void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
};