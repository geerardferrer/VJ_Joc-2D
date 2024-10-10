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
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::ivec2 posCollision, sizeCollision;
	int jumpAngle, startY;
	float velocityX, accelerationX, maxSpeedX; // Nuevas variables
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _PLAYER_INCLUDE
