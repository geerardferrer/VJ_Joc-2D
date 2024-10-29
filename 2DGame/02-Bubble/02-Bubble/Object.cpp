#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Object.h"
#include "Game.h"
#include <cstdlib> 
#include <ctime>

void Object::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, ObjectType objType)
{
	spritesheet.loadFromFile("images/Objects.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.5f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(COIN, 8);
	sprite->addKeyframe(COIN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(HEART, 8);
	sprite->addKeyframe(HEART, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(ROCK, 8);
	sprite->addKeyframe(ROCK, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(CHEST, 8);
	sprite->addKeyframe(CHEST, glm::vec2(0.4f, 0.0f));

	sprite->setAnimationSpeed(MINERAL, 8);
	sprite->addKeyframe(MINERAL, glm::vec2(0.2f, 0.5f));

	sprite->setAnimationSpeed(OPEN_CHEST, 8);
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(0.4f, 0.5f));

	sprite->setAnimationSpeed(GEMSTONE, 8);
	sprite->addKeyframe(GEMSTONE, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(GEMSTONE, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(GEMSTONE, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(GEMSTONE, glm::vec2(0.8f, 0.5f));

	objectType = objType;

	sprite->changeAnimation(objType);

	tileMapDispl = tileMapPos;

	posCollision = glm::vec2(0, 0);
	sizeCollision = glm::ivec2(32, 32);

	velObj = glm::fvec2(0.f, 0.f);

	destroyed = false;
	isThrown = false;
	isPickedUp = false;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (isThrown || isDropped) {
		posObj.x += velObj.x;
		if (velObj.x < 0 && map->collisionMoveLeft(posObj, posCollision, sizeCollision, PlayerType)) {
			switch (objectType) {
				case CHEST:
					if (isThrown) {
						if (rand() % 2 == 0) {
							objectType = COIN;
							sprite->changeAnimation(COIN);
						}
						else {
							objectType = HEART;
							sprite->changeAnimation(HEART);
						}
					}
					else {
						velObj.x = 0.f; 
					}
					velObj.y = -8.f;
					velObj.x = 0.f;
					break;
				case ROCK:
					destroyed = true;
					break;
				case MINERAL:
					velObj.x = -velObj.x / 2;
					break;
			}
		}
		if (velObj.x > 0 && map->collisionMoveRight(posObj, posCollision, sizeCollision, PlayerType)) {
			switch (objectType) {
				case CHEST:
					if (isThrown) {
						objectType = (rand() % 2 == 0) ? COIN : HEART;
						sprite->changeAnimation(objectType);
						velObj.y = -8.f;
						velObj.x = 0.f;
					}
					else {
						velObj.x = 0.f;
					}
					break;
				case ROCK:
					destroyed = true;
					break;
				case MINERAL:
					velObj.x = -velObj.x / 2;
					break;
			}
		}
		velObj.y += 0.4f;
		posObj.y += velObj.y;
			

		if (velObj.y > 0 && map->collisionMoveDown(posObj, posCollision, sizeCollision, PlayerType)) {
			switch (objectType) {
				case CHEST:
					if (isThrown) {
						objectType = (rand() % 2 == 0) ? COIN : HEART;
						sprite->changeAnimation(objectType);
						velObj.y = -8.f;
						velObj.x = 0.f;
					}
					else {
						
						velObj.y = 0.f;
						velObj.x = 0.f;
					}
					break;
				case ROCK:
					destroyed = true;
					break;
				case MINERAL:
					velObj.y = 0.f;
					velObj.x = 0.f;
					break;
			}
			isThrown = false;
			isDropped = false;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void Object::render()
{
	sprite->render();
}

void Object::setObjectType(ObjectType newType) {
	objectType = newType;  
}

void Object::changeAnimation(int anim) {
	if (sprite) { 
		sprite->changeAnimation(anim);
	}
}

void Object::pickUpObject()
{
	isThrown = false;
	isPickedUp = true;
}

void Object::dropObject()
{
	isPickedUp = false;
	isThrown = false;  // Aseguramos que no se considera lanzado
	isDropped = true;  // Marcamos el objeto como dejado
	velObj = glm::fvec2(0.f, 0.f); // No aplicamos velocidad al objeto al dejarlo
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

ObjectType Object::getObjectType() const
{
	return objectType;
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


bool Object::canDelete() const
{
	return destroyed;
}

