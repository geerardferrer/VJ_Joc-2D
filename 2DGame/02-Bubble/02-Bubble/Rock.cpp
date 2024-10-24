#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Rock.h"
#include "Game.h"


#define FALL_STEP 4


void Rock::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Rock.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asegúrate de tener esta textura
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);

	tileMapDispl = tileMapPos;

	posCollision = glm::vec2(0, 0);
	sizeCollision = glm::ivec2(32, 32);

	velRock = glm::fvec2(0.f, 0.f);

	isThrown = false;
	isPickedUp = true;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
}

void Rock::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (isThrown) {
		velRock.x += 0.4f;

		posRock.x += velRock.x;

		if (velRock.x < 0 && map->collisionMoveLeft(posRock, posCollision, sizeCollision, PlayerType)) {
			//break
		}
		if (velRock.x > 0 && map->collisionMoveRight(posRock, posCollision, sizeCollision, PlayerType)) {
			//break
		}

		velRock.y += 0.4f;

		posRock.y += velRock.y;

		if (velRock.y > 0 && map->collisionMoveDown(posRock, posCollision, sizeCollision, PlayerType)) {
			velRock.y = 0;
			isThrown = false;
			//break
		}

	}
	else if (isPickedUp) {
		FaceDir playerDir = player->getFacingDir();

		if (playerDir == LEFT) posRock = player->getPosition() + glm::vec2(8, 48);
		else if (playerDir == RIGHT) posRock = player->getPosition() + glm::vec2(56, 48);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
}

void Rock::render()
{
	sprite->render();
}

void Rock::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Rock::setPlayer(Player *p)
{
	player = p;
}

void Rock::setPosition(const glm::vec2 &pos)
{
	posRock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
}

glm::vec2 Rock::getPosCollision() const
{
	return posCollision;
}

glm::vec2 Rock::getSizeCollision() const
{
	return sizeCollision;
}

glm::vec2 Rock::getPosition() const
{
	return posRock;
}

