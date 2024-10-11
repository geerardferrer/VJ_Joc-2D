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
	static Background *createBackground(const string &filename, const glm::vec2 &size, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;

private:
	glm::ivec2 posBackground;
	Texture spritesheet;
	Sprite *sprite;
};

#endif // _BACKGROUND_INCLUDE
