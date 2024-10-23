#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtx/norm.hpp>
#include "OgreEnemy.h"
#include "Game.h"

#define FALL_STEP 4
#define ACTIVATION_RANGE 250.0f

enum OgreEnemyAnims {
	IDLE_LEFT, IDLE_RIGHT,
	MOVE_LEFT, MOVE_RIGHT,
	DIE_LEFT, DIE_RIGHT,
};

void OgreEnemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Enemy_Ogre_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asegúrate de tener esta textura
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(IDLE_LEFT, 8);
	sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(IDLE_RIGHT, 8);
	sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.875f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.25f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.5f));

	sprite->setAnimationSpeed(DIE_LEFT, 4);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.875f, 0.25f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.875f, 0.5f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.75f));

	sprite->setAnimationSpeed(DIE_RIGHT, 4);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.0f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.125f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.375f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.75f));

	sprite->changeAnimation(IDLE_LEFT);

	tileMapDispl = tileMapPos;

	isDead = false;
	deathTime = 0.0f;

	posCollision = glm::vec2(18, 0);
	sizeCollision = glm::ivec2(28, 64);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void OgreEnemy::update(int deltaTime)
{

	sprite->update(deltaTime);

	if (isDead) {
		deathTime += deltaTime / 1000.0f; 

		if (deathTime >= 2.0f) moveSpeed = 0;

		return; 
	}
	

	if (moveSpeed == 0) {
		distanceToPlayer = glm::distance(posEnemy, playerPos);

		if (distanceToPlayer <= ACTIVATION_RANGE) {
			moveSpeed = 2;

			if (playerPos.x < posEnemy.x) {
				moveDirection = -1;  // Moure's cap a l'esquerra
				sprite->changeAnimation(MOVE_LEFT); 
			}
			else {
				moveDirection = 1;   // Moure's cap a la dreta
				sprite->changeAnimation(MOVE_RIGHT); 
			}
		}
	}
	else {
		posEnemy.x += moveDirection * moveSpeed;
		
		// Moviment a l'esquerra
		if (moveDirection == -1) {
			if (map->collisionMoveLeft(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision, EnemyType)) {
				posEnemy.x += moveSpeed;
				moveDirection = 1;
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		// Moviment a la dreta
		else {
			if (map->collisionMoveRight(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision, EnemyType)) {
				posEnemy.x -= moveSpeed;
				moveDirection = -1;
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
	}

	posEnemy.y += FALL_STEP;

	if (map->collisionMoveDown(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision, EnemyType)) {}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void OgreEnemy::die()
{
	isDead = true;
	startDeathAnimation();
}

bool OgreEnemy::isEnemyDead()
{
	return isDead;
}


void OgreEnemy::render()
{
	if (!isDead || deathTime < 1.75f)
	{
		sprite->render(); // Només renderitzem si no ha acabat l'animació de mort
	}
}

void OgreEnemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void OgreEnemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 OgreEnemy::getPosition() const
{
	return posEnemy;
}

void OgreEnemy::setPlayerPosition(const glm::vec2 &playerPos)
{
	this->playerPos = playerPos;
}

glm::vec2 OgreEnemy::getPosCollision() const
{
	return posCollision;
}

glm::vec2 OgreEnemy::getSizeCollision() const {
	return sizeCollision;
}

void OgreEnemy::startDeathAnimation()
{
	if (sprite->animation() == MOVE_LEFT)
	{
		sprite->changeAnimation(DIE_LEFT); // Reprodueix l'animació de mort cap a l'esquerra
	}
	else if (sprite->animation() == MOVE_RIGHT)
	{
		sprite->changeAnimation(DIE_RIGHT); // Reprodueix l'animació de mort cap a la dreta
	}

	deathTime = 0.0f; // Inicialitza el temporitzador per eliminar l'enemic després de la mort
}
