#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Object.h"


class Scene;

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

	void applyKillJump();
	void hasGrounded();

	void pickUpObject(Object *obj);
	void throwObject();
	bool canPickUpObject() const;

	bool isHoldingObj() const;
	Object* holdingObject() const;
	
	int getTries() const;
	void setTries(int currentTries);
	void addPoints(int points);
	void addLife();

	void toggleGodMode();  
	void heal();           
	bool isGodMode() const, isDead() const;

	void setVictoryStandAnimation();

private:
	const float acceleration = 0.3f;
	const float frictionForce = 0.9f;
	const float maxVelPlayer = 6.f;
	const float gravity = 0.4f;
	const float jumpForce = 11.f;
	const float maxFallPlayer = 30.f;

	bool isGrounded, isVisible;
	glm::fvec2 posPlayer, velPlayer, posCollision, initialPosition;
	glm::ivec2 sizeCollision;

	glm::ivec2 tileMapDispl;

	bool hasAppliedJump;

	int lives, tries = 3, score;
	bool godMode = false;
	float damageTakenTime;

	Object *holdingObj;
	float holdingObjectTime;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _PLAYER_INCLUDE
