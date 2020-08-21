#include "EnemyBulPool.h"
#include "Util.h"

EnemyBulPool::EnemyBulPool(int a, GLuint texID) : amountToPool(a) {
	for (int i = 0; i < amountToPool; i++) {
		bulToPool = new EnemyBul();
		bulToPool->modelMatrix = glm::mat4(1.0f);
		bulToPool->textureID = texID;
		bulToPool->isActive = false;
		pooledBul.push_back(bulToPool);
	}
}

EnemyBul* EnemyBulPool::getPooledBul() {
	for (int i = 0; i < pooledBul.size(); i++) {
		if (!pooledBul[i]->isActive) {
			pooledBul[i]->isActive = true;
			return pooledBul[i];
		}
	}
	return NULL;
}

void EnemyBulPool::clearBulPool() {
	for (int i = 0; i < amountToPool; i++) {
		pooledBul[i]->isActive = false;
	}
}

void EnemyBulPool::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
	for (int i = 0; i < pooledBul.size(); i++) {
		pooledBul[i]->Update(deltaTime, player, playerHB, enemies, enemyCount, bullets, bulletCount);
	}
}