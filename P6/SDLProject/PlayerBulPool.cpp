#include "PlayerBulPool.h"
#include "Util.h"

PlayerBulPool::PlayerBulPool(int a, GLuint texID): amountToPool(a){
	sound = new Sound();
	for (int i = 0; i < amountToPool; i++) {
		bulToPool = new PlayerBul();
		bulToPool->modelMatrix = glm::mat4(1.0f);
		bulToPool->textureID = texID;
		bulToPool->isActive = false;
		pooledBul.push_back(bulToPool);
	}
}

PlayerBul* PlayerBulPool::getPooledBul() {
	for (int i = 0; i < pooledBul.size(); i++) {
		if (!pooledBul[i]->isActive) {
			pooledBul[i]->isActive = true;
			return pooledBul[i];
		}
	}
	return NULL;
}

void PlayerBulPool::Update(float deltaTime, Player* player, PlayerHB* playerHB, Entity* enemies, int enemyCount, Entity* bullets, int bulletCount) {
	if (fire) {
		player->bulTimer += deltaTime;
		if (player->bulTimer >= player->fireRate)
		{
			player->bulTimer = 0.0f;
			for (int i = 0; i < 9; i++) {
				getPooledBul()->Fire(player->position, player->fireMode, i);
			}
		}
		sound->playShootSound();
	}
	
	for (int i = 0; i < pooledBul.size(); i++) {
		pooledBul[i]->Update(deltaTime, player, playerHB, enemies, enemyCount, bullets, bulletCount);
	}
}