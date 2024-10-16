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

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	bool checkCollision(const glm::fvec2 &playerPos, const glm::fvec2 &playerCollision, const glm::ivec2 &playerSize,
		const glm::fvec2 &enemyPos, const glm::fvec2 &enemyCollision, const glm::ivec2 &enemySize);
	void manageCollision(int enemyIndex);

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

