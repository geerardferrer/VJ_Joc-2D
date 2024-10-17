#ifndef _ROCK_INCLUDE
#define _ROCK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents a enemy. As such it has
// all properties it needs to track its movement, and collisions.


class Rock
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

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

#endif // _ROCK_INCLUDE