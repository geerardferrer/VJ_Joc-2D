#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Object.h"
#include "OgreEnemy.h"
#include "BatEnemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_OBJECTS 12
#define NUM_OGRES 8
#define NUM_BATS 4

enum CollisionDir
{
	NO_COLLISION, LEFT_COLLISION, RIGHT_COLLISION, TOP_COLLISION, BOTTOM_COLLISION
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	CollisionDir checkCollision(const glm::fvec2 &entityPos1, const glm::fvec2 &entityCollision1, const glm::ivec2 &entitySize1, const glm::fvec2 &entityPos2, const glm::fvec2 &entityCollision2, const glm::ivec2 &entitySize2);
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

