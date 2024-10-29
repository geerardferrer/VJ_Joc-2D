#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "CollisionManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Object.h"
#include "OgreEnemy.h"
#include "BatEnemy.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

#define NUM_MINERALS 6
#define NUM_ROCKS 2
#define NUM_CHESTS 7
#define NUM_OBJECTS NUM_MINERALS+NUM_ROCKS+NUM_CHESTS
#define NUM_OGRES 8
#define NUM_BATS 3

public:
	Scene();
	~Scene();

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
	Player *player;
	vector<Object*> object;
	vector<OgreEnemy*> ogre;
	vector<BatEnemy*> bat;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

