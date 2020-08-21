#pragma once
#include "Player.h"
#include <vector>
#include "Fairy.h"

class FairyPool {
public:
	std::vector<Fairy*> pooledBul;
	Fairy* bulToPool;
	int amountToPool;

	bool fire = false;

	FairyPool(int a, GLuint texID);

	Fairy* getPooledBul();

	void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
};