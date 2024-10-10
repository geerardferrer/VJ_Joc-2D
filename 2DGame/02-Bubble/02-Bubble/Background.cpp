#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Background.h"
#include "Game.h"


void Background::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(496, 272), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite->setNumberAnimations(0);

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBackground.x), float(tileMapDispl.y + posBackground.y)));
}

void Background::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBackground.x), float(tileMapDispl.y + posBackground.y)));
}

void Background::render()
{
	sprite->render();
}

glm::vec2 Background::getPosition() const {
	return posBackground;
}

void Background::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Background::setPosition(const glm::vec2 &pos)
{
	posBackground = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBackground.x), float(tileMapDispl.y + posBackground.y)));
}




