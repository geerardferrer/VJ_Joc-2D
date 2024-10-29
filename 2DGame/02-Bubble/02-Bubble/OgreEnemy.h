#ifndef _OGRE_ENEMY_INCLUDE
#define _OGRE_ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class OgreEnemy {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPlayer(Player* p);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	FaceDir getMoveDir() const;
	void setMoveDir(FaceDir dir);

	bool hasGroundInFront();

	void die();
	bool isEnemyDead();
	void startDeathAnimation();
	float getDeathTime() const { return deathTime; }

	bool canDelete() const;

private:
	bool isDead;
	glm::fvec2 posEnemy, posCollision;
	glm::ivec2 sizeCollision;

	float moveSpeed, distanceToPlayer, deathTime;
	FaceDir moveDir;

	glm::ivec2 tileMapDispl;

	Player* player;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _OGRE_ENEMY_INCLUDE