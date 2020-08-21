#pragma once
#include "Player.h"
#include <vector>
#include "EnemyBul.h"

class EnemyBulPool {
public:
	std::vector<EnemyBul*> pooledBul;
	EnemyBul* bulToPool;
	int amountToPool;

	EnemyBulPool(int a, GLuint texID);

	EnemyBul* getPooledBul();

	void clearBulPool();

	void Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount);
};