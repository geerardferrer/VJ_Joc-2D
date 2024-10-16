#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "OgreEnemy.h"
#include "Game.h"


#define FALL_STEP 4

enum OgreEnemyAnims
{
	IDLE_LEFT, IDLE_RIGHT,
	MOVE_LEFT, MOVE_RIGHT
};



void OgreEnemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	moveSpeed = 2;        // Velocidad de movimiento del enemigo
	moveDirection = -1;

	spritesheet.loadFromFile("images/Enemy_Ogre_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asegúrate de tener esta textura
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(4);

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


	sprite->changeAnimation(MOVE_LEFT);  // Iniciamos con la animación de movimiento a la izquierda

	tileMapDispl = tileMapPos;

	// Ajustamos la hitbox del enemigo
	posCollision = glm::vec2(18, 0); // Ajusta según la forma del sprite (igual que en el player)
	sizeCollision = glm::ivec2(28, 64); // Tamaño de la hitbox (ajustado a 30x96)

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void OgreEnemy::update(int deltaTime)
{
	sprite->update(deltaTime);

	// Movimiento automático
	posEnemy.x += moveDirection * moveSpeed;

	// Detectar colisiones y cambiar de dirección si es necesario
	if (moveDirection == -1) // Moviéndose hacia la izquierda
	{
		if (map->collisionMoveLeft(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision))
		{
			posEnemy.x += moveSpeed; // Revertir movimiento
			moveDirection = 1;       // Cambiar dirección a derecha
			sprite->changeAnimation(MOVE_RIGHT); // Actualizar animación si es necesario
		}
	}
	else // Moviéndose hacia la derecha
	{
		if (map->collisionMoveRight(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision))
		{
			posEnemy.x -= moveSpeed; // Revertir movimiento
			moveDirection = -1;      // Cambiar dirección a izquierda
			sprite->changeAnimation(MOVE_LEFT); // Actualizar animación si es necesario
		}
	}

	// Aplicar gravedad para que el enemigo caiga si no está sobre una plataforma
	posEnemy.y += FALL_STEP;
	if (map->collisionMoveDown(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision));
	{
		// Si está en el suelo, no hacemos nada
	}

	// Actualizamos la posición del sprite
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void OgreEnemy::render()
{
	sprite->render();
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

glm::vec2 OgreEnemy::getPosition() const {
	return posEnemy;
}

