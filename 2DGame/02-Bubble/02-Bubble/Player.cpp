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

	sprite->setAnimationSpeed(STAND_OBJ_LEFT, 8);
	sprite->addKeyframe(STAND_OBJ_LEFT, glm::vec2(0.125f, 0.25f));

	sprite->setAnimationSpeed(STAND_OBJ_RIGHT, 8);
	sprite->addKeyframe(STAND_OBJ_RIGHT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(MOVE_OBJ_LEFT, 8);
	sprite->addKeyframe(MOVE_OBJ_LEFT, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(MOVE_OBJ_LEFT, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(MOVE_OBJ_LEFT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(MOVE_OBJ_RIGHT, 8);
	sprite->addKeyframe(MOVE_OBJ_RIGHT, glm::vec2(0.375f, 0.25f));
	sprite->addKeyframe(MOVE_OBJ_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVE_OBJ_RIGHT, glm::vec2(0.625f, 0.25f));

	sprite->setAnimationSpeed(JUMP_OBJ_LEFT, 8);
	sprite->addKeyframe(JUMP_OBJ_LEFT, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(JUMP_OBJ_RIGHT, 8);
	sprite->addKeyframe(JUMP_OBJ_RIGHT, glm::vec2(0.375f, 0.25f));

	sprite->setAnimationSpeed(VICTORY_STAND, 8);
	sprite->addKeyframe(VICTORY_STAND, glm::vec2(0.5f, 0.75f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	posCollision = glm::vec2(33, 0);
	sizeCollision = glm::vec2(30, 96);

	isGrounded = false;
	lives = 3;
	score = 0;
	damageTakenTime = 0.f;
	holdingObjectTime = 0.f;


}

void Player::update(int deltaTime)
{
	if (lives <= 0)
	{
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
			if (isHoldingObj())
			{
				if (sprite->animation() != MOVE_OBJ_LEFT) {
					sprite->changeAnimation(MOVE_OBJ_LEFT);
				}
			}
			else
			{
				if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			}
		}
		else
		{
			if (isHoldingObj())
			{
				if (sprite->animation() != JUMP_OBJ_LEFT) sprite->changeAnimation(JUMP_OBJ_LEFT);
			}
			else
			{
				if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
			}
		}
	}
	else if (Game::instance().rightKeyPressed() && !Game::instance().downKeyPressed())
	{
		if (velPlayer.x < 0) velPlayer.x += acceleration * 2;
		else velPlayer.x += acceleration;

		if (velPlayer.x > maxVelPlayer) velPlayer.x = maxVelPlayer;

		if (isGrounded)
		{
			if (isHoldingObj())
			{
				if (sprite->animation() != MOVE_OBJ_RIGHT) sprite->changeAnimation(MOVE_OBJ_RIGHT);
			}
			else
			{
				if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else
		{
			if (isHoldingObj())
			{
				if (sprite->animation() != JUMP_OBJ_RIGHT) sprite->changeAnimation(JUMP_OBJ_RIGHT);
			}
			else
			{
				if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			}
		}
	}
	else
	{
		velPlayer.x *= frictionForce;

		if (fabs(velPlayer.x) < 0.1f) velPlayer.x = 0.f;

		if (isGrounded)
		{
			if (Game::instance().downKeyPressed() && !isHoldingObj())
			{
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(CROUCH_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);
			}
			else
			{
				if (isHoldingObj())
				{
					if (sprite->animation() == STAND_OBJ_LEFT || sprite->animation() == MOVE_OBJ_LEFT || sprite->animation() == CROUCH_LEFT) sprite->changeAnimation(STAND_OBJ_LEFT);
					else if (sprite->animation() == STAND_OBJ_RIGHT || sprite->animation() == MOVE_OBJ_RIGHT || sprite->animation() == CROUCH_RIGHT) sprite->changeAnimation(STAND_OBJ_RIGHT);
				}
				else
				{
					if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT) sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				}
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
		if (Game::instance().upKeyPressed()) {
			velPlayer.y = -jumpForce;
			isGrounded = false;
			Game::instance().playSound("sounds/jump.wav", false);

			if (isHoldingObj())
			{
				FaceDir dir = getFacingDir();
				if (dir == LEFT) sprite->changeAnimation(JUMP_OBJ_LEFT);
				else sprite->changeAnimation(JUMP_OBJ_RIGHT);
			}
			else
			{
				// Animación de salto normal sin objeto
				FaceDir dir = getFacingDir();
				if (dir == LEFT) sprite->changeAnimation(JUMP_LEFT);
				else sprite->changeAnimation(JUMP_RIGHT);
			}
		}
	}
	else
	{
		// Si estamos en el aire y se presiona la tecla DOWN, cambia a la animación de caer de culo
		if (Game::instance().downKeyPressed() && !isHoldingObj() && !hasAppliedJump)
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

			if (isHoldingObj())
			{
				if (sprite->animation() == STAND_OBJ_LEFT || sprite->animation() == MOVE_OBJ_LEFT || sprite->animation() == CROUCH_LEFT) sprite->changeAnimation(JUMP_OBJ_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT) sprite->changeAnimation(JUMP_OBJ_RIGHT);
			}
			else
			{
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT) sprite->changeAnimation(JUMP_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			}
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

		if (isHoldingObj())
		{
			FaceDir dir = getFacingDir();
			if (dir == LEFT) sprite->changeAnimation(STAND_OBJ_LEFT);
			else sprite->changeAnimation(STAND_OBJ_RIGHT);
		}
		else
		{
			if (sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_ASS_LEFT)
			{
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_ASS_RIGHT)
			{
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else if (isGrounded && !map->collisionMoveDown(posPlayer, posCollision, sizeCollision, PlayerType)) {
		isGrounded = false;
	}

	if (damageTakenTime > 0.f) {
		damageTakenTime -= deltaTime / 1000.f;

		if (int(damageTakenTime * 10) % 2 == 0) isVisible = false;
		else isVisible = true;
	}
	else {
		isVisible = true; 			  
		if (map->getTileAt(posPlayer + glm::fvec2(48.f, 85.f)) == 22 || map->getTileAt(posPlayer + glm::fvec2(48.f, 85.f)) == 38) {
			takeDamage(); 
		}
		if (map->getTileAt(posPlayer + glm::fvec2(48.f, 85.f)) == 119) {
			lives = 0;
		}
	}


	if (holdingObjectTime > 0.f) {
		holdingObjectTime -= deltaTime / 1000.0f;
	}
	else {
		if (Game::instance().interactKeyPressed() && isHoldingObj()) {
			throwObject();
		}
	}
	if (isHoldingObj()) {
		FaceDir facingDir = getFacingDir();

		if (facingDir == LEFT) holdingObj->setPosition(posPlayer + glm::vec2(8, 48));
		else if (facingDir == RIGHT) holdingObj->setPosition(posPlayer + glm::vec2(56, 48));
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::pickUpObject(Object *obj)
{
	holdingObj = obj;
	holdingObj->pickUpObject();
	holdingObjectTime = 0.2f;

	FaceDir dir = getFacingDir();
	if (dir == LEFT)
		sprite->changeAnimation(STAND_OBJ_LEFT);
	else
		sprite->changeAnimation(STAND_OBJ_RIGHT);
}

void Player::throwObject()
{
	if (!isHoldingObj()) return;

	FaceDir dir = getFacingDir();
	if (sprite->animation() == STAND_OBJ_LEFT || sprite->animation() == STAND_OBJ_RIGHT) holdingObj->dropObject();
	else if (dir == LEFT) holdingObj->throwObject(glm::fvec2(-12.f, -4.f));
	else if (dir == RIGHT) holdingObj->throwObject(glm::fvec2(12.f, -4.f));
	holdingObj = NULL;
	holdingObjectTime = 0.2f;

	if (isGrounded)
	{
		if (velPlayer.x == 0)
		{
			if (dir == LEFT) sprite->changeAnimation(STAND_LEFT);
			else sprite->changeAnimation(STAND_RIGHT);
		}
		else
		{
			if (velPlayer.x < 0) sprite->changeAnimation(MOVE_LEFT);
			else sprite->changeAnimation(MOVE_RIGHT);
		}
	}
	else
	{
		if (dir == LEFT) sprite->changeAnimation(JUMP_LEFT);
		else sprite->changeAnimation(JUMP_RIGHT);
	}
}


Object* Player::holdingObject() const
{
	return holdingObj;
}

bool Player::isHoldingObj() const
{
	return (holdingObj != NULL);
}

bool Player::isFallingAss() const
{
	return (sprite->animation() == FALL_ASS_LEFT || sprite->animation() == FALL_ASS_RIGHT);
}

bool Player::canPickUpObject() const
{
	return holdingObjectTime <= 0.f;
}


void Player::takeDamage()
{
	if (godMode) {
		cout << "No damage taken (God Mode)" << endl;
		return;
	}
	--lives;
	damageTakenTime = 2.f;
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

	if (isHoldingObj()) {
		FaceDir dir = getFacingDir();
		if (dir == LEFT) sprite->changeAnimation(STAND_OBJ_LEFT);
		else sprite->changeAnimation(STAND_OBJ_RIGHT);
	}
	else {
		if (sprite->animation() == FALL_ASS_LEFT || sprite->animation() == JUMP_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == FALL_ASS_RIGHT || sprite->animation() == JUMP_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
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

int Player::getLives() const {
	return lives;
}

int Player::getTries() const {
	return tries;
}

int Player::getScore() const {
	return score;
}

void Player::setTries(int currentTries) {
	tries = currentTries;
}

void Player::toggleGodMode() {
	godMode = !godMode;
	cout << (godMode ? "God Mode activated!" : "God Mode deactivated!") << endl;
}

void Player::heal() {
	lives = 3;
	cout << "Health restored to maximum!" << endl;
}

bool Player::isGodMode() const {
	return godMode;
}

bool Player::isDead() const {
	return lives <= 0;
}

void Player::addPoints(int points) {
	score += points;
	std::cout << "Punts actuals: " << score << std::endl;
}

void Player::addLife() {
	if (lives < 3)
		lives += 1;
	std::cout << "Vides actuals: " << lives << std::endl;
}

void Player::setVictoryStandAnimation() {
	sprite->changeAnimation(VICTORY_STAND);
}