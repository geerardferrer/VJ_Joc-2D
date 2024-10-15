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

	posCollision = glm::vec2(33, 0);
	sizeCollision = glm::vec2(30, 96);

	isGrounded = false;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	cout << isGrounded << endl;

	// MOVIMENT HORITZONTAL
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (velPlayer.x > 0) velPlayer.x -= acceleration*2;
		else velPlayer.x -= acceleration;
		
		if (velPlayer.x < -maxVelPlayer) velPlayer.x = -maxVelPlayer;

		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (velPlayer.x < 0) velPlayer.x += acceleration * 2;
		else velPlayer.x += acceleration;
		
		if (velPlayer.x > maxVelPlayer) velPlayer.x = maxVelPlayer;

		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	}
	else
	{
		velPlayer.x *= frictionForce;
		if (fabs(velPlayer.x) < 0.1f) velPlayer.x = 0.f;

		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}

	posPlayer.x += velPlayer.x;

	// COLISIONS HORITZONTALS
	if (velPlayer.x < 0 && map->collisionMoveLeft(posPlayer, posCollision, glm::ivec2(96, 96), sizeCollision))
	{
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
	}
	else if (velPlayer.x > 0 && map->collisionMoveRight(posPlayer, posCollision, glm::ivec2(96, 96), sizeCollision))
	{
		if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}

	// MOVIMENT VERTICAL
	if (isGrounded)
	{
		if (Game::instance().getKey(GLFW_KEY_UP))
		{
			velPlayer.y = -jumpForce;
			isGrounded = false;

			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
		}
	}
	else if (!isGrounded)
	{
		if (Game::instance().getKey(GLFW_KEY_DOWN))
		{
			velPlayer.y += gravity*5;
		}
		else
		{
			velPlayer.y += gravity;
		}
	}

	posPlayer.y += velPlayer.y;

	// COLISIONS VERTICALS
	if (velPlayer.y < 0 && map->collisionMoveUp(posPlayer, posCollision, glm::ivec2(96, 96), sizeCollision)) // Es mou cap a dalt
	{
		velPlayer.y = 0;
	}
	else if (velPlayer.y > 0 && map->collisionMoveDown(posPlayer, posCollision, glm::ivec2(96, 96), sizeCollision)) // Es mou cap a baix
	{
		velPlayer.y = 0;
		isGrounded = true;
	}
	else if (isGrounded && !map->collisionMoveDown(posPlayer, posCollision, glm::ivec2(96, 96), sizeCollision))
	{
		isGrounded = false;
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
	velPlayer = glm::fvec2(0.f, 0.f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




