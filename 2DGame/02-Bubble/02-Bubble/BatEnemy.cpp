#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtx/norm.hpp>
#include "BatEnemy.h"
#include "Game.h"


enum BatEnemyAnims
{
	FLY, 
	DIE
};

void BatEnemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	moveSpeed = 0.0f; // Velocitat de moviment
	//flySpeed = 2.5f;  // Velocitat de vol
	moveDirection = -1;

	spritesheet.loadFromFile("images/Enemy_Bat_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA); // Textura del bat
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(2);

	// Configurar animacions
	sprite->setAnimationSpeed(FLY, 8);
	sprite->addKeyframe(FLY, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(0.625f, 0.f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.625f, 0.5f));
	
	sprite->changeAnimation(FLY); // Animació inicial

	tileMapDispl = tileMapPos;

	isDead = false;
	deathTime = 0.0f;

	posCollision = glm::vec2(10, 10);  
	sizeCollision = glm::ivec2(44, 44); 

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void BatEnemy::update(int deltaTime)
{

	sprite->update(deltaTime);
	
	if (isDead)
	{
		deathTime += deltaTime / 1000.0f;
		if (deathTime >= 2.0f)
		{
			moveSpeed = 0;
		}
		return;
	}

	posEnemy.y += moveDirection * moveSpeed;

	// Detectar col·lisions i canviar de direcció si és necessari
	if (moveDirection == -1) // Moviment a dalt
	{
		if (map->collisionMoveUp(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision, EnemyType))
		{
			posEnemy.y += moveSpeed;  // Revertir el moviment
			moveDirection = 1;        // Canviar direcció cap a la dreta
		
		}
	}
	else // Moviment a baix
	{
		if (map->collisionMoveDown(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision, EnemyType))
		{
			posEnemy.y -= moveSpeed;  // Revertir el moviment
			moveDirection = -1;       // Canviar direcció cap a l'esquerra
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void BatEnemy::die()
{
	isDead = true;
	startDeathAnimation();
}

void BatEnemy::render()
{
	if (!isDead || deathTime < 1.75f)
	{
		sprite->render();
	}
}

void BatEnemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void BatEnemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 BatEnemy::getPosition() const
{
	return posEnemy;
}

void BatEnemy::setPlayerPosition(const glm::vec2 &playerPos)
{
	this->playerPos = playerPos;
}

glm::vec2 BatEnemy::getPosCollision() const
{
	return posCollision;
}

glm::vec2 BatEnemy::getSizeCollision() const
{
	return sizeCollision;
}

void BatEnemy::startDeathAnimation()
{
	sprite->changeAnimation(DIE);

	deathTime = 0.0f;
}
