#ifndef _ROCK_INCLUDE
#define _ROCK_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Rock {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPlayer(Player *p);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const;
	glm::vec2 getPosCollision() const;
	glm::vec2 getSizeCollision() const;

	void pickUp();
	void throwRock();

private:
	bool isThrown, isPickedUp;
	glm::fvec2 posRock, velRock, posCollision;
	glm::ivec2 sizeCollision;

	glm::ivec2 tileMapDispl;

	Player *player;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _ROCK_INCLUDE