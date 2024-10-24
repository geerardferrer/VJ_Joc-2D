#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
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

