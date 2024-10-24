#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT,
	MOVE_LEFT, MOVE_RIGHT,
	CROUCH_LEFT, CROUCH_RIGHT,
	JUMP_LEFT, JUMP_RIGHT,
	FALL_ASS_LEFT, FALL_ASS_RIGHT,
	STAND_OBJ_LEFT, STAND_OBJ_RIGHT,
	MOVE_OBJ_LEFT, MOVE_OBJ_RIGHT,
	JUMP_OBJ_LEFT, JUMP_OBJ_RIGHT,
	CLIMB,
	VICTORY_STAND
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
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

	sprite->setAnimationSpeed(CLIMB, 8);
	sprite->addKeyframe(CLIMB, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.375f, 0.5f));

	sprite->setAnimationSpeed(VICTORY_STAND, 8);
	sprite->addKeyframe(VICTORY_STAND, glm::vec2(0.5f, 0.75f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	posCollision = glm::vec2(33, 0);
	sizeCollision = glm::vec2(30, 96);

	isGrounded = false;
	lives = 3;
	damageTakenTime = 0.f;
}

void Player::update(int deltaTime)
{
	if (lives <= 0)
	{
		sprite->changeAnimation(VICTORY_STAND);
		return;
	}

	sprite->update(deltaTime);

	// MOVIMENT HORITZONTAL
	if (Game::instance().leftKeyPressed() && !Game::instance().downKeyPressed())
	{
		if (velPlayer.x > 0) velPlayer.x -= acceleration * 2;
		else velPlayer.x -= acceleration;

		if (velPlayer.x < -maxVelPlayer) velPlayer.x = -maxVelPlayer;

		if (isGrounded)
		{
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		}
		else
		{
			if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
		}
	}
	else if (Game::instance().rightKeyPressed() && !Game::instance().downKeyPressed())
	{
		if (velPlayer.x < 0) velPlayer.x += acceleration * 2;
		else velPlayer.x += acceleration;

		if (velPlayer.x > maxVelPlayer) velPlayer.x = maxVelPlayer;

		if (isGrounded)
		{
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		}
		else
		{
			if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
		}
	}
	else
	{
		velPlayer.x *= frictionForce;

		if (fabs(velPlayer.x) < 0.1f) velPlayer.x = 0.f;

		if (isGrounded)
		{
			if (Game::instance().downKeyPressed())
			{
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(CROUCH_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);
			}
			else
			{
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT) sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT) sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}

	posPlayer.x += velPlayer.x;

	// COLISIONS HORITZONTALS
	if (velPlayer.x < 0 && map->collisionMoveLeft(posPlayer, posCollision, sizeCollision, PlayerType))
	{
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
	}
	else if (velPlayer.x > 0 && map->collisionMoveRight(posPlayer, posCollision, sizeCollision, PlayerType))
	{
		if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}

	// MOVIMENT VERTICAL
	if (isGrounded)
	{
		if (Game::instance().upKeyPressed())
		{
			velPlayer.y = -jumpForce;
			isGrounded = false;
		}
	}
	else
	{
		// Si estamos en el aire y se presiona la tecla DOWN, cambia a la animación de caer de culo
		if (Game::instance().downKeyPressed() && !hasAppliedJump)
		{
			velPlayer.y += gravity * 5; // Caída más rápida si se presiona DOWN

			if (velPlayer.y > maxFallPlayer) velPlayer.y = maxFallPlayer;

			if (sprite->animation() == JUMP_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(FALL_ASS_LEFT);
			else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(FALL_ASS_RIGHT);
		}
		else
		{
			velPlayer.y += gravity; // Caída normal

			if (velPlayer.y > maxFallPlayer) velPlayer.y = maxFallPlayer;

			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == FALL_ASS_LEFT) sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT || sprite->animation() == FALL_ASS_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
		}

	}

	posPlayer.y += velPlayer.y;

	// COLISIONS VERTICALS
	if (velPlayer.y < 0 && map->collisionMoveUp(posPlayer, posCollision, sizeCollision, PlayerType)) // Es mou cap a dalt
	{
		velPlayer.y = 0;
	}
	else if (velPlayer.y > 0 && map->collisionMoveDown(posPlayer, posCollision, sizeCollision, PlayerType)) // Es mou cap a baix
	{
		velPlayer.y = 0;
		isGrounded = true;
		if (hasAppliedJump) hasAppliedJump = false;

		if (sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_ASS_LEFT)
		{
			sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_ASS_RIGHT)
		{
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (isGrounded && !map->collisionMoveDown(posPlayer, posCollision, sizeCollision, PlayerType))
	{
		isGrounded = false;
	}

	if (damageTakenTime > 0.f)
	{
		damageTakenTime -= deltaTime / 1000.f;

		// Alternar visibilidad basado en damageTakenTime para hacer que "parpadee"
		if (int(damageTakenTime * 10) % 2 == 0)
		{
			isVisible = false;  // Ocultar temporalmente al jugador
		}
		else
		{
			isVisible = true;   // Mostrar al jugador
		}
	}
	else
	{
		isVisible = true;  // Siempre visible si no está en estado de daño

						   // Verifica si el jugador está en un tile que causa daño
		if (map->getTileAt(posPlayer + glm::fvec2(48.f, 95.f)) == 22 || map->getTileAt(posPlayer + glm::fvec2(48.f, 95.f)) == 38)
		{
			takeDamage();  // Solo toma daño cuando damageTakenTime es 0 (invulnerabilidad ha terminado)
		}
	}

	// Verifica si el jugador ha llegado a un tile específico (ejemplo de final de juego)
	if (map->getTileAt(posPlayer + glm::fvec2(48.f, 95.f)) == 119)
	{
		lives = 0;
	}

	if (isHoldingObj()) {
		FaceDir facingDir = getFacingDir();

		if (facingDir == LEFT) holdingObj->setPosition(posPlayer + glm::vec2(8, 48));
		else if (facingDir == RIGHT) holdingObj->setPosition(posPlayer + glm::vec2(56, 48));
	}

	if (Game::instance().interactKeyPressed() && isHoldingObj()) {
		throwObject();
	}

	//cout << map->getTileAt(posPlayer + glm::fvec2(48.f, 95.f)) << endl;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::pichUpObject(Object *obj)
{
	holdingObj = obj;
	holdingObj->pickUpObject();
}

void Player::throwObject()
{
	if (!isHoldingObj()) return;

	FaceDir dir = getFacingDir();
	if (dir == LEFT) holdingObj->throwObject(glm::fvec2(-8.f, -10.f));
	else if (dir == RIGHT) holdingObj->throwObject(glm::fvec2(8.f, -10.f));
	holdingObj = NULL;
}

bool Player::isHoldingObj() const
{
	return (holdingObj != NULL);
}

bool Player::isFallingAss() const
{
	return (sprite->animation() == FALL_ASS_LEFT || sprite->animation() == FALL_ASS_RIGHT);
}


void Player::takeDamage()
{
	--lives;
	damageTakenTime = 2.f;
	cout << lives << endl;
}

bool Player::canTakeDamage() const
{
	return damageTakenTime <= 0.f;
}

void Player::applyKillJump()
{
	velPlayer.y = -10.f;
	hasAppliedJump = true;
}

void Player::hasGrounded()
{
	velPlayer.y = 0;
	isGrounded = true;
	if (hasAppliedJump) hasAppliedJump = false;
}

FaceDir Player::getFacingDir() const {
	if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_ASS_LEFT || sprite->animation() == STAND_OBJ_LEFT || sprite->animation() == MOVE_OBJ_LEFT || sprite->animation() == JUMP_OBJ_LEFT)
		return LEFT;
	else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_ASS_RIGHT || sprite->animation() == STAND_OBJ_RIGHT || sprite->animation() == MOVE_OBJ_RIGHT || sprite->animation() == JUMP_OBJ_RIGHT)
		return RIGHT;
}

void Player::render()
{
	if (isVisible)
	{
		sprite->render();
	}
}

glm::vec2 Player::getPosition() const
{
	return posPlayer;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	velPlayer = glm::fvec2(0.f, 0.f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosCollision() const
{
	return posCollision;
}

glm::vec2 Player::getSizeCollision() const
{
	return sizeCollision;
}

glm::vec2 Player::getVelocity() const
{
	return velPlayer;
}