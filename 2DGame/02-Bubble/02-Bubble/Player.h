#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

enum FaceDir {
	LEFT, RIGHT
};

class Player {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	glm::vec2 getVelocity() const;

	FaceDir getFacingDir() const;

	bool isFallingAss() const;

	void takeDamage();
	bool canTakeDamage() const;

	void applyJump();
	void hasGrounded();

	bool leftKeyPressed() const;
	bool rightKeyPressed() const;
	bool downKeyPressed() const;
	bool upKeyPressed() const;
	bool interactKeyPressed() const;

private:
	const float acceleration = 0.3f;
	const float frictionForce = 0.9f;
	const float maxVelPlayer = 6.f;
	const float gravity = 0.4f;
	const float jumpForce = 11.f;
	const float maxFallPlayer = 30.f;

	bool isGrounded, isVisible;
	glm::fvec2 posPlayer, velPlayer, posCollision;
	glm::ivec2 sizeCollision;

	glm::ivec2 tileMapDispl;

	bool isHoldingObj;

	bool hasAppliedJump;

	int lives;
	float damageTakenTime;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _PLAYER_INCLUDE
