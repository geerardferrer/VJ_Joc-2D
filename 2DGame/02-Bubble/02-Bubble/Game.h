#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include <irrKlang.h>
#include "MainMenu.h"
#include "Scene.h"
#include "Practice.h"
#include "Texture.h"

using namespace irrklang;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameScene {
	GAME_SCENE,
	PRACTICE_SCENE, 
	MAIN_MENU,
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

	void changeScene(GameScene gameScene);
	
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
	bool quitKeyPressed() const;

	ISound* playSound(const char* audioPath, bool loop);
	void stopSound(ISound *sound);

	void renderGameOverScreen();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time

	MainMenu mainMenu;
	Scene scene;
	Practice practiceScene;

	ISoundEngine* audio;
	Texture spritesheet;
	GameScene state;

};


#endif // _GAME_INCLUDE


