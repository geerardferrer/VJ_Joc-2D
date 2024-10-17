#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Background.h"
#include "Player.h"
#include "Rock.h"
#include "OgreEnemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_ROCKS 12
#define NUM_OGRES 8

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
	vector<Rock*> rock;
	vector<OgreEnemy*> ogre;
	Background *background;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

