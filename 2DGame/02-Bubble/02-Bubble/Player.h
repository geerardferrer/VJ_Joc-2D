#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	glm::vec2 getVelocity() const;

	bool isFallingAss() const;

	void takeDamage();
	bool canTakeDamage() const;

	void applyJump();
	void hasGrounded();

private:
	bool isGrounded;
	glm::fvec2 posPlayer, velPlayer, posCollision;
	glm::ivec2 sizeCollision;
	glm::ivec2 tileMapDispl;

	bool hasAppliedJump;

	int lives;
	float damageTakenTime;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _PLAYER_INCLUDE
