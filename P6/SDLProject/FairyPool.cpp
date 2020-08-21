#include "Fairy.h"
#include "FairyPool.h"
#include "Util.h"

FairyPool::FairyPool(int a, GLuint texID) : amountToPool(a) {
	for (int i = 0; i < amountToPool; i++) {
		bulToPool = new Fairy();
		bulToPool->modelMatrix = glm::mat4(1.0f);
		bulToPool->textureID = texID;
		bulToPool->isActive = false;
		bulToPool->hp = 100;
		bulToPool->position = glm::vec3(-2, 4, 1);
		bulToPool->velocity.y = -1;
		bulToPool->entityType = ENEMY;
		bulToPool->radius = 0.5f;
		bulToPool->speed = 0.1f;

		pooledBul.push_back(bulToPool);
	}
}

Fairy* FairyPool::getPooledBul() {
	for (int i = 0; i < pooledBul.size(); i++) {
		if (!pooledBul[i]->isActive) {
			pooledBul[i]->isActive = true;
			return pooledBul[i]; 
		}
	}
	return NULL;
}

void FairyPool::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
	if (enemies->hp % 100 == 0) {
			getPooledBul();
	}

	for (int i = 0; i < pooledBul.size(); i++) {
		pooledBul[i]->Update(deltaTime, player, playerHB, enemies, enemyCount, bullets, bulletCount);
	}
}