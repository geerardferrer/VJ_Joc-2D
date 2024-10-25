#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Object.h"
#include "Game.h"

void Object::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, ObjectType objType)
{
	spritesheet.loadFromFile("images/Rock.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);

	tileMapDispl = tileMapPos;

	posCollision = glm::vec2(0, 0);
	sizeCollision = glm::ivec2(32, 32);

	velObj = glm::fvec2(0.f, 0.f);

	isThrown = false;
	isPickedUp = false;

	switch (objType)
	{
		case ROCK:
			break;
		case CHEST:
			break;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (isThrown) {
		posObj.x += velObj.x;

		if (velObj.x < 0 && map->collisionMoveLeft(posObj, posCollision, sizeCollision, PlayerType)) {
			velObj.x = -velObj.x/2;
		}
		if (velObj.x > 0 && map->collisionMoveRight(posObj, posCollision, sizeCollision, PlayerType)) {
			velObj.x = -velObj.x/2;
		}

		velObj.y += 0.4f;

		posObj.y += velObj.y;

		if (velObj.y > 0 && map->collisionMoveDown(posObj, posCollision, sizeCollision, PlayerType)) {
			velObj.y = 0;
			isThrown = false;
		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::render()
{
	sprite->render();
}

void Object::pickUpObject()
{
	isThrown = false;
	isPickedUp = true;
}

void Object::throwObject(const glm::fvec2 &velocity)
{
	isPickedUp = false;
	isThrown = true;

	velObj = velocity;
}

bool Object::isObjPickedUp() const
{
	return isPickedUp;
}

void Object::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Object::setPosition(const glm::vec2 &pos)
{
	posObj = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::setVelocity(const glm::vec2 &vel)
{
	velObj = vel;
}

glm::vec2 Object::getPosCollision() const
{
	return posCollision;
}

glm::vec2 Object::getSizeCollision() const
{
	return sizeCollision;
}

glm::vec2 Object::getPosition() const
{
	return posObj;
}

