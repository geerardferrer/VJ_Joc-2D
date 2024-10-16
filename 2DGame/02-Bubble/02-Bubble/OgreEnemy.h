#ifndef _OGRE_ENEMY_INCLUDE
#define _OGRE_ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents a enemy. As such it has
// all properties it needs to track its movement, and collisions.


class OgreEnemy
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;

private:
	glm::fvec2 posEnemy, posCollision;
	float moveSpeed;
	int moveDirection; // 1 para derecha, -1 para izquierda
	glm::ivec2 sizeCollision;
	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _OGRE_ENEMY_INCLUDE