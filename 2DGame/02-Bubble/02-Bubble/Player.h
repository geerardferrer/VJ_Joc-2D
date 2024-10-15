#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;

private:
	const float acceleration = 0.3f;
	const float frictionForce = 0.9f;
	const float maxVelPlayer = 6.f;
	const float gravity = 0.4f;
	const float jumpForce = 11.f;
	const float maxFallPlayer = 10.f;

	bool isGrounded;
	glm::fvec2 posPlayer, velPlayer, posCollision;
	glm::ivec2 sizeCollision;
	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _PLAYER_INCLUDE
