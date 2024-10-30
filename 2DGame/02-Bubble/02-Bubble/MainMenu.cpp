#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenu.h"
#include "Game.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}


void MainMenu::init()
{
	initShaders();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;

	menu.loadFromFile("images/Main_Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.5f, 0.25f), &menu, &texProgram);
	menu.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(PLAY, 1);
	sprite->addKeyframe(PLAY, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(PRACTICE, 1);
	sprite->addKeyframe(PRACTICE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(CONTROLS, 1);
	sprite->addKeyframe(CONTROLS, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(CREDITS, 1);
	sprite->addKeyframe(CREDITS, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(CREDITS_INFO, 1);
	sprite->addKeyframe(CREDITS_INFO, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(CONTROLS_INFO, 1);
	sprite->addKeyframe(CONTROLS_INFO, glm::vec2(0.0f, 0.5f));

	state = PLAY;

	delayTime = 0.f;
}

void MainMenu::update(int deltaTime)
{
	currentTime += deltaTime;

	if (delayTime <= 0) {
		if (Game::instance().downKeyPressed() && state != CREDITS) {
			state = static_cast<MenuState>(static_cast<int>(state) + 1);
			delayTime = 0.1f;
		}
		else if (Game::instance().upKeyPressed() && state != PLAY) {
			state = static_cast<MenuState>(static_cast<int>(state) - 1);
			delayTime = 0.1f;
		}

		sprite->changeAnimation(state);
	}
	else delayTime -= deltaTime / 1000.f;

	if (Game::instance().interactKeyPressed()) {
		switch (state)
		{
		case PLAY:
			Game::instance().changeScene(GAME_SCENE);
			break;
		case PRACTICE:
			Game::instance().changeScene(PRACTICE_SCENE);
			break;
		case CONTROLS:
			sprite->changeAnimation(CONTROLS_INFO);
			state = CONTROLS_INFO;
			break;
		case CREDITS:
			sprite->changeAnimation(CREDITS_INFO);
			state = CREDITS_INFO;
			break;
		}
	}

	if (Game::instance().quitKeyPressed()) {
		switch (state)
		{
		case CONTROLS_INFO:
			sprite->changeAnimation(CONTROLS);
			state = CONTROLS;
			break;
		case CREDITS_INFO:
			sprite->changeAnimation(CREDITS);
			state = CREDITS;
			break;
		}
	}

	sprite->update(deltaTime);
}

void MainMenu::render()
{
	glm::mat4 modelview;

	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	sprite->render();

}

void MainMenu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}