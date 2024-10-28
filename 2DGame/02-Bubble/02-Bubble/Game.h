#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Texture.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameState {
	PLAYING,
	GAME_OVER, 
	MAIN_MENU,
	CONTROLS_MENU,
	CREDITS
};


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void gameOver();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

	bool leftKeyPressed() const;
	bool rightKeyPressed() const;
	bool downKeyPressed() const;
	bool upKeyPressed() const;
	bool interactKeyPressed() const;
	bool restartKeyPressed() const;
	bool menuKeyPressed() const;


	void renderGameOverScreen();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene scene;
	Texture spritesheet;
	GameState state;

};


#endif // _GAME_INCLUDE


