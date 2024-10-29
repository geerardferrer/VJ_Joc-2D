#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

#include <iostream>

void Game::init()
{
	bPlay = true;
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	scene.init();

	if (!spritesheet.loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA)) cout << "No se pudo cargar la textura" << endl;

	audio = createIrrKlangDevice();

	state = PLAYING;
}

bool Game::update(int deltaTime)
{
	if (state == GAME_OVER) {
		
		if (restartKeyPressed()) {
			init();           
			state = PLAYING;  
		}
		else if (menuKeyPressed()) {
			bPlay = false;
			audio->drop();
		}

		return bPlay;  
	}
	scene.update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (state == GAME_OVER) {
		renderGameOverScreen();
	}
	else {
		scene.render();
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;

	if (key == GLFW_KEY_G) {
		scene.getPlayer()->toggleGodMode();  // Activa/desactiva el God Mode
	}
	else if (key == GLFW_KEY_H) {
		scene.getPlayer()->heal();  // Restaura la salut
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
	return (keys[GLFW_KEY_SPACE]);
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

void Game::gameOver()
{
	state = GAME_OVER;
}