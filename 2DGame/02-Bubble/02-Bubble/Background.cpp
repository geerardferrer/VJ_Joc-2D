#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Background.h"
#include "Game.h"


Background *Background::createBackground(const string &filename, const glm::vec2 &size, ShaderProgram &shaderProgram) {
	Background *bg = new Background();

	bg->spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA); 
	bg->sprite = Sprite::createSprite(size, glm::vec2(1.f, 1.f), &bg->spritesheet, &shaderProgram);
	bg->spritesheet.setMagFilter(GL_NEAREST);

	return bg;
}

void Background::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(posBackground.x), float(posBackground.y)));
}

void Background::render()
{
	sprite->render();
}

glm::vec2 Background::getPosition() const {
	return posBackground;
}

void Background::setPosition(const glm::vec2 &pos)
{
	posBackground = pos;
	sprite->setPosition(glm::vec2(float(posBackground.x), float(posBackground.y)));
}




