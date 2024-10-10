#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
#define JUMP_HEIGHT 150
#define FALL_STEP 9


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/Luigi_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	posCollision = glm::ivec2(16, 0);
	sizeCollision = glm::ivec2(32, 64);
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 6;
		if (map->collisionMoveLeft(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x += 6;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 6;
		if (map->collisionMoveRight(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x -= 6;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false; // Termina el salto
			jumpAngle = 180; // Asegúrate de que el ángulo esté en 180
		}
		else {
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f)); // Calcula la nueva posición Y

			if (jumpAngle < 90) {  // Només ens interessa la col·lisió durant el moviment ascendent
				if (map->collisionMoveUp(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					// Si hi ha col·lisió, para el salt i ajusta la posició
					bJumping = false;
					jumpAngle = 180;  // Força el final del salt
					posPlayer.y += 1; // Ajusta lleugerament la posició perquè no quedi "enganxat"
				}
			}

			// Comprobar colisiones en la caída
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					bJumping = false; // Si hay colisión, termina el salto
					jumpAngle = 180; // Asegúrate de que el ángulo esté en 180
				}
			}
		}
	}
	else {
		posPlayer.y += FALL_STEP; // Si no está saltando, cae

		if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
			// Se detectó colisión al caer
			if (Game::instance().getKey(GLFW_KEY_UP)) {
				bJumping = true; // Inicia el salto si se presiona la tecla
				jumpAngle = 0;
				startY = posPlayer.y; // Guarda la posición inicial
			}
		}
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

glm::vec2 Player::getPosition() const {
	return posPlayer;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




