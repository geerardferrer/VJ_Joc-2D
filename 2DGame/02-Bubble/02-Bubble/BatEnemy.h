#ifndef _BAT_ENEMY_INCLUDE
#define _BAT_ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class BatEnemy {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	void die();
	bool isEnemyDead();
	void startDeathAnimation();

private:
	bool isDead;
	glm::fvec2 posEnemy, posCollision;
	glm::ivec2 sizeCollision;

	float moveSpeed, maxDisplacement, currentDisplacement, deathTime;
	int moveDirection; // 1 cap a la dreta, -1 cap a l'esquerra

	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _BAT_ENEMY_INCLUDE
