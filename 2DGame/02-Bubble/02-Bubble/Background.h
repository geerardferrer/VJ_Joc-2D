#pragma once
#ifndef _BACKGROUND_INCLUDE
#define _BACKGROUND_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Background
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
	glm::ivec2 tileMapDispl, posBackground;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _BACKGROUND_INCLUDE
