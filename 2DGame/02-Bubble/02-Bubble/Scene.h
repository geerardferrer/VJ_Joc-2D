#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Background.h"
#include "Player.h"
#include "OgreEnemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_OGRES 8

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	vector<OgreEnemy*> ogre;
	Background *background;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

