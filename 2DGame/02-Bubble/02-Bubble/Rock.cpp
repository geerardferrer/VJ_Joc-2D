#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Rock.h"
#include "Game.h"


#define FALL_STEP 4


void Rock::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Rock.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asegúrate de tener esta textura
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	spritesheet.setMagFilter(GL_NEAREST);

	tileMapDispl = tileMapPos;

	// Ajustamos la hitbox del enemigo
	posCollision = glm::vec2(0, 0); // Ajusta según la forma del sprite (igual que en el player)
	sizeCollision = glm::ivec2(32, 32); // Tamaño de la hitbox (ajustado a 30x96)

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Rock::update(int deltaTime)
{
	sprite->update(deltaTime);

	// Actualizamos la posición del sprite
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Rock::render()
{
	sprite->render();
}

void Rock::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Rock::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Rock::getPosCollision() const
{
	return posCollision;
}

glm::vec2 Rock::getSizeCollision() const
{
	return sizeCollision;
}

glm::vec2 Rock::getPosition() const
{
	return posEnemy;
}

