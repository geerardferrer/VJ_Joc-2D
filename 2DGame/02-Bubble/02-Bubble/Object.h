#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

enum ObjectType {
	ROCK, CHEST, OPEN_CHEST, COIN, HEART
};

class Object {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, ObjectType objType);
	void update(int deltaTime);

	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setVelocity(const glm::vec2 &vel);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;
	glm::vec2 getVelocity() const;

	void pickUpObject();
	void throwObject(const glm::fvec2 &velocity);

	bool isObjPickedUp() const;

private:
	bool isThrown, isPickedUp;
	glm::fvec2 posObj, velObj, posCollision;
	glm::ivec2 sizeCollision;

	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _OBJECT_INCLUDE