#ifndef _MAIN_MENU_INCLUDE
#define _MAIN_MENU_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "Sprite.h"

enum MenuState
{
	PLAY, PRACTICE, CONTROLS, CREDITS
};

// Scene contains all the entities of our game.
// It is responsible for updating and render them.
class MainMenu
{

public:
	MainMenu();
	~MainMenu();

	void init();
	void update(int deltaTime);
	void render();

private:

	void initShaders();

private:
	float delayTime;

	Texture menu;
	Sprite *sprite;

	MenuState state;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _MAIN_MENU_INCLUDE
