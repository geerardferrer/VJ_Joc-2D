#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 70



Scene::Scene()
{
	map = NULL;
	player = NULL;
	background = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (background != NULL)
		delete background;
}


void Scene::init()
{
	initShaders();

	// TILEMAP
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	// BACKGROUND
	background = Background::createBackground("images/Background.png", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), texProgram);
	background->setPosition(glm::vec2(4 * map->getTileSize(), 70 * map->getTileSize()));

	// PLAYER
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// ROCKS
	for (int i = 0; i < NUM_ROCKS; ++i) rock.push_back(new Rock());

	for (int i = 0; i < rock.size(); ++i)
	{
		rock[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		rock[i]->setTileMap(map);
	}
	
	rock[0]->setPosition(glm::vec2(12 * map->getTileSize(), (75 * map->getTileSize())));
	rock[1]->setPosition(glm::vec2(28 * map->getTileSize(), (73 * map->getTileSize())));
	rock[2]->setPosition(glm::vec2(81 * map->getTileSize(), (89 * map->getTileSize())));
	rock[3]->setPosition(glm::vec2(115 * map->getTileSize(), (91 * map->getTileSize())));
	rock[4]->setPosition(glm::vec2(157 * map->getTileSize(), (119 * map->getTileSize())));
	rock[5]->setPosition(glm::vec2(105 * map->getTileSize(), (131 * map->getTileSize())));
	rock[6]->setPosition(glm::vec2(69 * map->getTileSize(), (157 * map->getTileSize())));
	rock[7]->setPosition(glm::vec2(48 * map->getTileSize(), (162 * map->getTileSize())));
	rock[8]->setPosition(glm::vec2(48 * map->getTileSize(), (161 * map->getTileSize())));
	rock[9]->setPosition(glm::vec2(48 * map->getTileSize(), (160 * map->getTileSize())));
	rock[10]->setPosition(glm::vec2(22 * map->getTileSize(), (150 * map->getTileSize())));
	rock[11]->setPosition(glm::vec2(61 * map->getTileSize(), (176 * map->getTileSize())));

	// OGRES
	for (int i = 0; i < NUM_OGRES; ++i) ogre.push_back(new OgreEnemy());

	for (int i = 0; i < ogre.size(); ++i)
	{
		ogre[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ogre[i]->setTileMap(map);
	}

	ogre[0]->setPosition(glm::vec2(37 * map->getTileSize(), (72 * map->getTileSize())));
	ogre[1]->setPosition(glm::vec2(56 * map->getTileSize(), (76 * map->getTileSize())));
	ogre[2]->setPosition(glm::vec2(77 * map->getTileSize(), (75 * map->getTileSize())));
	ogre[3]->setPosition(glm::vec2(71 * map->getTileSize(), (94 * map->getTileSize())));
	ogre[4]->setPosition(glm::vec2(118 * map->getTileSize(), (70 * map->getTileSize())));
	ogre[5]->setPosition(glm::vec2(150 * map->getTileSize(), (118 * map->getTileSize())));
	ogre[6]->setPosition(glm::vec2(142 * map->getTileSize(), (118 * map->getTileSize())));
	ogre[7]->setPosition(glm::vec2(44 * map->getTileSize(), (161 * map->getTileSize())));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	manageCollision();

	player->update(deltaTime);

	for (int i = 0; i < rock.size(); ++i) rock[i]->update(deltaTime);

	for (int i = 0; i < ogre.size(); ++i) {
		ogre[i]->setPlayerPosition(player->getPosition());
		ogre[i]->update(deltaTime);
	}


	float halfWidth = SCREEN_WIDTH / 2.0f;
	float halfHeight = SCREEN_HEIGHT / 2.0f;

	// Calcular el ancho del mapa
	float mapWidth = map->getMapSize().x * map->getTileSize(); // Método para obtener el tamaño del mapa


	glm::vec2 playerPos = player->getPosition();

	float cameraX = playerPos.x;

	
	if (cameraX < halfWidth) {
		cameraX = halfWidth; 
	}
	else if (cameraX > mapWidth - halfWidth) {
		cameraX = mapWidth - halfWidth; 
	}

	projection = glm::ortho(cameraX - halfWidth, cameraX + halfWidth, playerPos.y + halfHeight, playerPos.y - halfHeight);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();

	//background->render();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();

	for (int i = 0; i < rock.size(); ++i) rock[i]->render();
	player->render();
	for (int i = 0; i < ogre.size(); ++i) ogre[i]->render();
	ogre[0]->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

CollisionDir Scene::checkCollision(const glm::fvec2 &entityPos1, const glm::fvec2 &entityCollision1, const glm::ivec2 &entitySize1, const glm::fvec2 &entityPos2, const glm::fvec2 &entityCollision2, const glm::ivec2 &entitySize2)
{
	float left1 = entityPos1.x + entityCollision1.x;
	float right1 = left1 + float(entitySize1.x);
	float top1 = entityPos1.y + entityCollision1.y;
	float bottom1 = top1 + float(entitySize1.y);

	float left2 = entityPos2.x + entityCollision2.x;
	float right2 = left2 + float(entitySize2.x);
	float top2 = entityPos2.y + entityCollision2.y;
	float bottom2 = top2 + float(entitySize2.y);

	bool collisionX = (right1 >= left2) && (left1 <= right2);
	bool collisionY = (bottom1 >= top2) && (top1 <= bottom2);

	if (collisionX && collisionY)
	{
		float overlapX = std::min(right1 - left2, right2 - left1);
		float overlapY = std::min(bottom1 - top2, bottom2 - top1) * 0.5f; // factor para priorizar caidas por encima/debajo

		if (overlapX < overlapY)
		{
			return (right1 - left2 < right2 - left1) ? RIGHT_COLLISION : LEFT_COLLISION;
		}
		else
		{
			return (bottom1 - top2 < bottom2 - top1) ? BOTTOM_COLLISION : TOP_COLLISION;
		}
	}
	return NO_COLLISION;
}


void Scene::manageCollision()
{
	// PLAYER x ROCK
	for (int i = 0; i < rock.size(); ++i) {
		CollisionDir dir;

		if (dir = checkCollision(player->getPosition(), player->getPosCollision(), player->getSizeCollision(), rock[i]->getPosition(), rock[i]->getPosCollision(), rock[i]->getSizeCollision()))
		{
			switch (dir)
			{
			case LEFT_COLLISION:
				cout << "LEFT_COLLISION" << endl;
				break;
			case RIGHT_COLLISION:
				cout << "RIGHT_COLLISION" << endl;
				break;
			case TOP_COLLISION:
				cout << "TOP_COLLISION" << endl;
				break;
			case BOTTOM_COLLISION:
				cout << "BOTTOM_COLLISION" << endl;
				break;
			default:
				break;
			}

			if (dir == BOTTOM_COLLISION)
			{
				// ogre[i] mor
				// aplica salt al player
			}
			else
			{
				// player perd una vida
			}
		}
	}

	// PLAYER x OGRE
	for (int i = 0; i < ogre.size(); ++i) {
		CollisionDir dir;

		if (dir = checkCollision(player->getPosition(), player->getPosCollision(), player->getSizeCollision(), ogre[i]->getPosition(), ogre[i]->getPosCollision(), ogre[i]->getSizeCollision()))
		{
			switch (dir)
			{
			case LEFT_COLLISION:
				cout << "LEFT_COLLISION" << endl;
				break;
			case RIGHT_COLLISION:
				cout << "RIGHT_COLLISION" << endl;
				break;
			case TOP_COLLISION:
				cout << "TOP_COLLISION" << endl;
				break;
			case BOTTOM_COLLISION:
				cout << "BOTTOM_COLLISION" << endl;
				break;
			default:
				break;
			}

			if (dir == BOTTOM_COLLISION && player->isFallingAss())
			{
				ogre[i]->die();
				// aplica salt al player
			}
			else
			{
				if (player->canTakeDamage() && !ogre[i]->isEnemyDead()) player->takeDamage();
			}
		}
	}
}




