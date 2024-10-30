#ifndef _PRACTICE_INCLUDE
#define _PRACTICE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "CollisionManager.h"
#include "TileMap.h"
#include "GameUI.h"
#include "Player.h"
#include "Object.h"
#include "OgreEnemy.h"
#include "BatEnemy.h"

class Practice
{

#define _NUM_MINERALS 1
#define _NUM_ROCKS 1
#define _NUM_CHESTS 1
#define _NUM_OBJECTS _NUM_MINERALS+_NUM_ROCKS+_NUM_CHESTS+_NUM_GEMSTONE
#define _NUM_OGRES 1
#define _NUM_BATS 1
#define _NUM_GEMSTONE 1

public:
	Practice();
	~Practice();

	void init();
	void update(int deltaTime);
	void render();
	void resetScene();

	Player* getPlayer();

private:
	void manageCollision();

	void initShaders();

private:
	TileMap *map;
	GameUI *UI;
	Player *player;
	vector<Object*> object;
	vector<OgreEnemy*> ogre;
	vector<BatEnemy*> bat;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool gameEnded;
	float endGameDelay;

};


#endif // _PRACTICE_INCLUDE

