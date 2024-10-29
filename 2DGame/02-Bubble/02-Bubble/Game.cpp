#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

#include <iostream>

void Game::init()
{
	bPlay = true;
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	mainMenu.init();

	audio = createIrrKlangDevice();

	state = MAIN_MENU;
}

bool Game::update(int deltaTime)
{
	switch (state) {
		case GAME_SCENE:
			scene.update(deltaTime);
			if (scene.getPlayer()->isDead()) {
				scene.init();
			}
			break;
		case PRACTICE:
			practiceScene.update(deltaTime);
			if (practiceScene.getPlayer()->isDead()) {
				practiceScene.init();
			}
			break;
		case MAIN_MENU:
			mainMenu.update(deltaTime);
			break;
		case CONTROLS_MENU:
			break;
		case CREDITS_MENU:
			break;
		default:
			bPlay = false;
			break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (state) {
	case GAME_SCENE:
		scene.render();
		break;
	case PRACTICE:
		practiceScene.render();
		break;
	case MAIN_MENU:
		mainMenu.render();
		break;
	case CONTROLS_MENU:
		break;
	case CREDITS_MENU:
		break;
	default:
		bPlay = false;
		break;
	}
}

void Game::changeScene(GameScene gameScene)
{
	state = gameScene;

	switch (state) {
	case GAME_SCENE:
		scene.init();
		break;
	case PRACTICE:
		practiceScene.init();
		break;
	case MAIN_MENU:
		mainMenu.init();
		break;
	case CONTROLS_MENU:
		break;
	case CREDITS_MENU:
		break;
	default:
		bPlay = false;
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) 
		bPlay = false;
	keys[key] = true;

	if (key == GLFW_KEY_Q) {
		if (state != MAIN_MENU) 
			changeScene(MAIN_MENU);  
		return;
	}

	if (key == GLFW_KEY_R) {
		if (state == GAME_SCENE) {
			scene.init();  
		}
		else if (state == PRACTICE) {
			practiceScene.init();  
		}
		return;
	}

	if (key == GLFW_KEY_G) {
		if (state == GAME_SCENE) {
			scene.getPlayer()->toggleGodMode();  
		}
		else if (state == PRACTICE) {
			practiceScene.getPlayer()->toggleGodMode(); 
		}
	}
	else if (key == GLFW_KEY_H) {
		if (state == GAME_SCENE) {
			scene.getPlayer()->heal(); 
		}
		else if (state == PRACTICE) {
			practiceScene.getPlayer()->heal();  
		}
	}
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

ISound* Game::playSound(const char* audioPath, bool loop)
{
	ISound *sound = audio->play2D(audioPath, loop, false, true);
	return sound;
}

void Game::stopSound(ISound *sound)
{
	if (sound == NULL) return;

	sound->stop();
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::leftKeyPressed() const
{
	return (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]);
}

bool Game::rightKeyPressed() const
{
	return (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]);
}

bool Game::downKeyPressed() const
{
	return (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]);
}

bool Game::upKeyPressed() const
{
	return (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]);
}

bool Game::interactKeyPressed() const
{
	return (keys[GLFW_KEY_SPACE] || keys[GLFW_KEY_ENTER]);
}

bool Game::restartKeyPressed() const
{
	return (keys[GLFW_KEY_R]);
}

bool Game::menuKeyPressed() const
{
	return (keys[GLFW_KEY_M]);
}

void Game::renderGameOverScreen()
{
	GLuint textureID = spritesheet.getTextureID();
	if (textureID == 0) {
		std::cerr << "Error: ID de textura es 0, la textura no se cargó correctamente." << std::endl;
	}
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);

	// Dibuja un cuadrado de pantalla completa para la imagen
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}