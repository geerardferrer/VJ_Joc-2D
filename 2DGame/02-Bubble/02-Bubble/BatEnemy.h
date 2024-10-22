#ifndef _BAT_ENEMY_INCLUDE
#define _BAT_ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

// BatEnemy és un Sprite que representa un enemic. Té propietats per gestionar el seu moviment, vol i col·lisions.

class BatEnemy
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setPlayerPosition(const glm::vec2 &playerPos);
	void startDeathAnimation();

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	void die();

private:
	glm::fvec2 posEnemy, posCollision;
	glm::vec2 playerPos; // Emmagatzema la posició del jugador per calcular la distància
	bool isDead;
	float moveSpeed, flySpeed, distanceToPlayer, deathTime;
	int moveDirection; // 1 cap a la dreta, -1 cap a l'esquerra
	glm::ivec2 sizeCollision;
	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _BAT_ENEMY_INCLUDE
