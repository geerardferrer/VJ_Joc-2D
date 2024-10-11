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
	STAND_LEFT, STAND_RIGHT, //FET
	MOVE_LEFT, MOVE_RIGHT,  //FET
	CROUCH_LEFT, CROUCH_RIGHT, //FET (FALTA MIRAR QUE NOM�S S'HA D'AJUPIR QUAN EST� AL TERRA)
	JUMP_LEFT, JUMP_RIGHT, 
	FALL_ASS_LEFT, FALL_ASS_RIGHT, 
	DRIFT_LEFT, DRIFT_RIGHT, 
	STAND_OBJ_LEFT, STAND_OBJ_RIGHT, 
	MOVE_OBJ_LEFT, MOVE_OBJ_RIGHT, 
	JUMP_OBJ_LEFT, JUMP_OBJ_RIGHT, 
	CLIMB, 
	VICTORY_STAND
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/Luigi_Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(96, 96), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(20);

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

	sprite->setAnimationSpeed(CROUCH_LEFT, 8);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.f, 0.75f));

	sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.125f, 0.75f));



	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	posCollision = glm::ivec2(33, 0);
	sizeCollision = glm::ivec2(30, 96);
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 4;
		if (map->collisionMoveLeft(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x += 4;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 4;
		if (map->collisionMoveRight(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x -= 4;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN))
	{
		if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
			std::cout << "Ha entrat al LEFT" << endl;
			sprite->changeAnimation(CROUCH_LEFT);
		}
		else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
			std::cout << "Ha entrat al RIGHT" << endl;
			sprite->changeAnimation(CROUCH_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false; // Termina el salto
			jumpAngle = 180; // Aseg�rate de que el �ngulo est� en 180
		}
		else {
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f)); // Calcula la nueva posici�n Y

			if (jumpAngle < 90) {  // Nom�s ens interessa la col�lisi� durant el moviment ascendent
				if (map->collisionMoveUp(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					// Si hi ha col�lisi�, para el salt i ajusta la posici�
					bJumping = false;
					jumpAngle = 180;  // For�a el final del salt
					posPlayer.y += 1; // Ajusta lleugerament la posici� perqu� no quedi "enganxat"
				}
			}

			// Comprobar colisiones en la ca�da
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					bJumping = false; // Si hay colisi�n, termina el salto
					jumpAngle = 180; // Aseg�rate de que el �ngulo est� en 180
				}
			}
		}
	}
	else {
		posPlayer.y += FALL_STEP; // Si no est� saltando, cae

		if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
			// Se detect� colisi�n al caer
			if (Game::instance().getKey(GLFW_KEY_UP)) {
				bJumping = true; // Inicia el salto si se presiona la tecla
				jumpAngle = 0;
				startY = posPlayer.y; // Guarda la posici�n inicial
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




