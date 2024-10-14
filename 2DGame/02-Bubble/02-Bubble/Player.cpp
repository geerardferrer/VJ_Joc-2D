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
	CROUCH_LEFT, CROUCH_RIGHT, //FET 
	JUMP_LEFT, JUMP_RIGHT, //FET (POTSER FALTA ARREGLAR ALGUN BUG)
	FALL_ASS_LEFT, FALL_ASS_RIGHT, //NO FUNCIONA AL CAURE D'UNA PLATAFORMA (SI SALTA SI, SI CAU NO)
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
	bFalling = false;

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

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.f)); 

	sprite->setAnimationSpeed(FALL_ASS_LEFT, 8);
	sprite->addKeyframe(FALL_ASS_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(FALL_ASS_RIGHT, 8);
	sprite->addKeyframe(FALL_ASS_RIGHT, glm::vec2(0.625f, 0.5f));




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
		if (bJumping)
		{
			if (sprite->animation() != JUMP_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
		}
		else
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		posPlayer.x -= 4;
		if (map->collisionMoveLeft(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x += 4;
			if (!bJumping)
				sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (bJumping) {
			if (sprite->animation() != JUMP_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}
		posPlayer.x += 4;
		if (map->collisionMoveRight(posPlayer + posCollision, sizeCollision))
		{
			posPlayer.x -= 4;
			if (!bJumping)
				sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN))
	{
		// Si está en el aire, aplicar la animación de caída en picado
		if (bJumping || bFalling) {
			if (sprite->animation() == JUMP_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
				sprite->changeAnimation(FALL_ASS_LEFT);
			}
			else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
				sprite->changeAnimation(FALL_ASS_RIGHT);
			}

			// Aumentar la velocidad de caída para simular la caída en picado
			posPlayer.y += FALL_STEP * 2;  // Incrementa la velocidad de caída
		}
		else // En el suelo, animación de agacharse normal
		{
			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
				sprite->changeAnimation(CROUCH_LEFT);
			}
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
				sprite->changeAnimation(CROUCH_RIGHT);
			}
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
			jumpAngle = 180; // Asegúrate de que el ángulo esté en 180

			if (!bJumping)
			{
				if (sprite->animation() == JUMP_LEFT) {
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->animation() == JUMP_RIGHT) {
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
		else {
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f)); // Calcula la nueva posición Y
			 // Forzar animación de salto si no se presionan teclas
            if (sprite->animation() != JUMP_LEFT && sprite->animation() != JUMP_RIGHT) {
                if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
                    sprite->changeAnimation(JUMP_LEFT);
                } else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
                    sprite->changeAnimation(JUMP_RIGHT);
                }
            }
			if (jumpAngle < 90) {  // Només ens interessa la col·lisió durant el moviment ascendent
				if (map->collisionMoveUp(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					// Si hi ha col·lisió, para el salt i ajusta la posició
					bJumping = false;
					jumpAngle = 180;  // Força el final del salt
					posPlayer.y += 1; // Ajusta lleugerament la posició perquè no quedi "enganxat"

					if (!bJumping)
					{
						if (sprite->animation() == JUMP_LEFT) {
							sprite->changeAnimation(STAND_LEFT);
						}
						else if (sprite->animation() == JUMP_RIGHT) {
							sprite->changeAnimation(STAND_RIGHT);
						}
					}
				}
			}

			// Comprobar colisiones en la caída
			if (jumpAngle > 90) {
				bFalling = true;
				if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
					bJumping = false; // Si hay colisión, termina el salto
					jumpAngle = 180; // Asegúrate de que el ángulo esté en 180
				}
				if (!bJumping)
				{
					if (sprite->animation() == JUMP_LEFT) {
						sprite->changeAnimation(STAND_LEFT);
					}
					else if (sprite->animation() == JUMP_RIGHT) {
						sprite->changeAnimation(STAND_RIGHT);
					}
				}
				
			}
		}
	}
	else {
		posPlayer.y += FALL_STEP; // Si no está saltando, cae

		if (bFalling)
		{
			std::cout << "Cau" << endl;
			if (sprite->animation() == MOVE_LEFT) {
				std::cout << "Cau left" << endl;
				sprite->changeAnimation(JUMP_LEFT);
			}

			else if (sprite->animation() == MOVE_RIGHT) {
				std::cout << "Cau right" << endl;
				sprite->changeAnimation(JUMP_RIGHT);
			}
		}
		if (map->collisionMoveDown(posPlayer + posCollision, sizeCollision, &posPlayer.y)) {
			// Se detectó colisión al caer
			bFalling = false;
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




