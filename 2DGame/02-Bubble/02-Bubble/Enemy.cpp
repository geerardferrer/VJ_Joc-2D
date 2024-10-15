#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"
#include "Game.h"


#define FALL_STEP 4

enum EnemyAnims
{
	IDLE
};



void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	moveSpeed = 0.5;        // Velocidad de movimiento del enemigo
	moveDirection = -1;

	spritesheet.loadFromFile("images/Enemy_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asegúrate de tener esta textura
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.375f, 0.5f));
	


	sprite->changeAnimation(IDLE);  // Iniciamos con la animación de movimiento a la izquierda

	tileMapDispl = tileMapPos;

	// Ajustamos la hitbox del enemigo
	posCollision = glm::vec2(22, 0); // Ajusta según la forma del sprite (igual que en el player)
	sizeCollision = glm::ivec2(22, 64); // Tamaño de la hitbox (ajustado a 30x96)

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime)
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
			sprite->changeAnimation(IDLE); // Actualizar animación si es necesario
		}
	}
	else // Moviéndose hacia la derecha
	{
		if (map->collisionMoveRight(posEnemy, posCollision, glm::ivec2(64, 64), sizeCollision))
		{
			posEnemy.x -= moveSpeed; // Revertir movimiento
			moveDirection = -1;      // Cambiar dirección a izquierda
			sprite->changeAnimation(IDLE); // Actualizar animación si es necesario
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

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Enemy::getPosition() const {
	return posEnemy;
}

