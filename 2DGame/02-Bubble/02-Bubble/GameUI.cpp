#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "GameUI.h"
#include "Game.h"

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

GameUI *GameUI::createGameUI()
{
	GameUI *UI = new GameUI();

	return UI;
}

void GameUI::init(ShaderProgram &shaderProgram)
{
	bg_sheet.loadFromFile("images/Game_UI.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bg_sheet.setMagFilter(GL_NEAREST);

	lives_sheet.loadFromFile("images/Lives.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lives_sheet.setMagFilter(GL_NEAREST);

	numbers_sheet.loadFromFile("images/Numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbers_sheet.setMagFilter(GL_NEAREST);

	bg = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bg_sheet, &shaderProgram);

	lives = Sprite::createSprite(glm::ivec2(208, 52), glm::vec2(0.5f, 0.5f), &lives_sheet, &shaderProgram);
	lives->setNumberAnimations(4);

	lives->setAnimationSpeed(0, 1);
	lives->addKeyframe(0, glm::vec2(0.0f, 0.f));

	lives->setAnimationSpeed(1, 1);
	lives->addKeyframe(0, glm::vec2(0.5f, 0.f));

	lives->setAnimationSpeed(2, 1);
	lives->addKeyframe(0, glm::vec2(0.5f, 0.f));

	lives->setAnimationSpeed(3, 1);
	lives->addKeyframe(0, glm::vec2(0.5f, 0.5f));

	for (int i = 0; i < N_CHARS_TRIES; ++i) {
		tries.push_back(Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 1.f), &numbers_sheet, &shaderProgram));
		tries[i]->setNumberAnimations(10);

		tries[i]->setAnimationSpeed(0, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.9f, 0.f));

		tries[i]->setAnimationSpeed(1, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.0f, 0.f));

		tries[i]->setAnimationSpeed(2, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.1f, 0.f));

		tries[i]->setAnimationSpeed(3, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.2f, 0.f));

		tries[i]->setAnimationSpeed(4, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.3f, 0.f));

		tries[i]->setAnimationSpeed(5, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.4f, 0.f));

		tries[i]->setAnimationSpeed(6, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.5f, 0.f));

		tries[i]->setAnimationSpeed(7, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.6f, 0.f));

		tries[i]->setAnimationSpeed(8, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.7f, 0.f));

		tries[i]->setAnimationSpeed(9, 1);
		tries[i]->addKeyframe(0, glm::vec2(0.8f, 0.f));
	}

	for (int i = 0; i < N_CHARS_SCORE; ++i) {
		score.push_back(Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 1.f), &numbers_sheet, &shaderProgram));
		score[i]->setNumberAnimations(10);

		score[i]->setAnimationSpeed(0, 1);
		score[i]->addKeyframe(0, glm::vec2(0.9f, 0.f));

		score[i]->setAnimationSpeed(1, 1);
		score[i]->addKeyframe(0, glm::vec2(0.0f, 0.f));

		score[i]->setAnimationSpeed(2, 1);
		score[i]->addKeyframe(0, glm::vec2(0.1f, 0.f));

		score[i]->setAnimationSpeed(3, 1);
		score[i]->addKeyframe(0, glm::vec2(0.2f, 0.f));

		score[i]->setAnimationSpeed(4, 1);
		score[i]->addKeyframe(0, glm::vec2(0.3f, 0.f));

		score[i]->setAnimationSpeed(5, 1);
		score[i]->addKeyframe(0, glm::vec2(0.4f, 0.f));

		score[i]->setAnimationSpeed(6, 1);
		score[i]->addKeyframe(0, glm::vec2(0.5f, 0.f));

		score[i]->setAnimationSpeed(7, 1);
		score[i]->addKeyframe(0, glm::vec2(0.6f, 0.f));

		score[i]->setAnimationSpeed(8, 1);
		score[i]->addKeyframe(0, glm::vec2(0.7f, 0.f));

		score[i]->setAnimationSpeed(9, 1);
		score[i]->addKeyframe(0, glm::vec2(0.8f, 0.f));
	}

	for (int i = 0; i < N_CHARS_TIME; ++i) {
		time.push_back(Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 1.f), &numbers_sheet, &shaderProgram));
		time[i]->setNumberAnimations(10);

		time[i]->setAnimationSpeed(0, 1);
		time[i]->addKeyframe(0, glm::vec2(0.9f, 0.f));

		time[i]->setAnimationSpeed(1, 1);
		time[i]->addKeyframe(0, glm::vec2(0.0f, 0.f));

		time[i]->setAnimationSpeed(2, 1);
		time[i]->addKeyframe(0, glm::vec2(0.1f, 0.f));

		time[i]->setAnimationSpeed(3, 1);
		time[i]->addKeyframe(0, glm::vec2(0.2f, 0.f));

		time[i]->setAnimationSpeed(4, 1);
		time[i]->addKeyframe(0, glm::vec2(0.3f, 0.f));

		time[i]->setAnimationSpeed(5, 1);
		time[i]->addKeyframe(0, glm::vec2(0.4f, 0.f));

		time[i]->setAnimationSpeed(6, 1);
		time[i]->addKeyframe(0, glm::vec2(0.5f, 0.f));

		time[i]->setAnimationSpeed(7, 1);
		time[i]->addKeyframe(0, glm::vec2(0.6f, 0.f));

		time[i]->setAnimationSpeed(8, 1);
		time[i]->addKeyframe(0, glm::vec2(0.7f, 0.f));

		time[i]->setAnimationSpeed(9, 1);
		time[i]->addKeyframe(0, glm::vec2(0.8f, 0.f));
	}
}

void GameUI::update(int deltaTime)
{
	bg->update(deltaTime);

	lives->update(deltaTime);

	for (Sprite *s : tries) s->update(deltaTime);

	for (Sprite *s : score) s->update(deltaTime);

	for (Sprite *s : time) s->update(deltaTime);

	cout << player->getLives() << endl;
	cout << player->getTries() << endl;
	cout << player->getScore() << endl << endl;
}

void GameUI::render()
{
	bg->render();

	lives->render();

	for (Sprite *s : tries) s->render();

	for (Sprite *s : score) s->render();

	for (Sprite *s : time) s->render();
}

void GameUI::setPlayer(Player *p)
{
	player = p;
}