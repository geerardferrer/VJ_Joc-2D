#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Object.h"
#include "Game.h"

void Object::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, ObjectType objType)
{
	spritesheet.loadFromFile("images/Objects.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(COIN, 8);
	sprite->addKeyframe(COIN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(HEART, 8);
	sprite->addKeyframe(HEART, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(ROCK, 8);
	sprite->addKeyframe(ROCK, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(CHEST, 8);
	sprite->addKeyframe(CHEST, glm::vec2(0.75f, 0.0f));

	sprite->setAnimationSpeed(MINERAL, 8);
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(OPEN_CHEST, 8);
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(0.75f, 0.5f));

	objectType = objType;

	switch (objectType)
	{
	case ROCK:
		sprite->changeAnimation(ROCK);
		break;
	case MINERAL:
		sprite->changeAnimation(MINERAL);
		break;
	case CHEST:
		sprite->changeAnimation(CHEST);
		break;
	case OPEN_CHEST:
		sprite->changeAnimation(OPEN_CHEST);
		break;
	case COIN:
		sprite->changeAnimation(COIN);
		break;
	case HEART:
		sprite->changeAnimation(HEART);
		break;
	default:
		break;
	}

	tileMapDispl = tileMapPos;

	posCollision = glm::vec2(0, 0);
	sizeCollision = glm::ivec2(32, 32);

	velObj = glm::fvec2(0.f, 0.f);

	isThrown = false;
	isPickedUp = false;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (isThrown) {
		posObj.x += velObj.x;

		if (velObj.x < 0 && map->collisionMoveLeft(posObj, posCollision, sizeCollision, PlayerType)) {
			switch (objectType) {
			case CHEST:
				objectType = COIN;
				sprite->changeAnimation(COIN);
				velObj.y = -8.f;
				velObj.x = 0.f;
				break;
			case ROCK:
				//break
				break;
			case MINERAL:
				velObj.x = -velObj.x / 2;
				break;
			}
		}
		if (velObj.x > 0 && map->collisionMoveRight(posObj, posCollision, sizeCollision, PlayerType)) {
			switch (objectType) {
			case CHEST:
				objectType = COIN;
				sprite->changeAnimation(COIN);
				velObj.y = -8.f;
				velObj.x = 0.f;
				break;
			case ROCK:
				//break
				break;
			case MINERAL:
				velObj.x = -velObj.x / 2;
				break;
			}
		}

		velObj.y += 0.4f;

		posObj.y += velObj.y;

		if (velObj.y > 0 && map->collisionMoveDown(posObj, posCollision, sizeCollision, PlayerType)) {
			isThrown = false;

			switch (objectType) {
			case CHEST:
				objectType = HEART;
				sprite->changeAnimation(HEART);
				velObj.y = -8.f;
				velObj.x = 0.f;
				break;

			case ROCK:
				//break
				break;
			case MINERAL:
				velObj.y = 0.f;
				velObj.x = 0.f;
				break;
			}
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

glm::vec2 Object::getVelocity() const
{
	return velObj;
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

