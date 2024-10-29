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
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	for (Object* obj : object) {
		delete obj;
	}
	object.clear();

	for (OgreEnemy* ogreEnemy : ogre) {
		delete ogreEnemy;
	}
	ogre.clear();

	for (BatEnemy* batEnemy : bat) {
		delete batEnemy;
	}
	bat.clear();

	initShaders();

	// TILEMAP
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	// PLAYER
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// OBJECT
	for (int i = 0; i < NUM_OBJECTS; ++i) object.push_back(new Object());


	for (int i = 0; i < object.size(); ++i)
	{
		if (i < NUM_ROCKS) object[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, ROCK);
		else if (i < NUM_ROCKS + NUM_MINERALS) object[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, MINERAL);
		else if (i < NUM_ROCKS + NUM_MINERALS + NUM_CHESTS) object[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, CHEST);
		else  object[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, GEMSTONE);

		object[i]->setTileMap(map);
	}
	
	// rocks
	object[0]->setPosition(glm::vec2(28 * map->getTileSize(), (73 * map->getTileSize())));
	object[1]->setPosition(glm::vec2(81 * map->getTileSize(), (89 * map->getTileSize())));

	// minerals
	object[2]->setPosition(glm::vec2(115 * map->getTileSize(), (91 * map->getTileSize())));
	object[3]->setPosition(glm::vec2(157 * map->getTileSize(), (119 * map->getTileSize())));
	object[4]->setPosition(glm::vec2(105 * map->getTileSize(), (131 * map->getTileSize())));
	object[5]->setPosition(glm::vec2(69 * map->getTileSize(), (157 * map->getTileSize())));
	object[6]->setPosition(glm::vec2(20 * map->getTileSize(), (149 * map->getTileSize())));
	object[7]->setPosition(glm::vec2(61 * map->getTileSize(), (176 * map->getTileSize())));

	//chests
	object[8]->setPosition(glm::vec2(22 * map->getTileSize(), (56 * map->getTileSize())));
	object[9]->setPosition(glm::vec2(60 * map->getTileSize(), (88 * map->getTileSize())));
	object[10]->setPosition(glm::vec2(62 * map->getTileSize(), (176 * map->getTileSize())));
	object[11]->setPosition(glm::vec2(50 * map->getTileSize(), (79 * map->getTileSize())));
	object[12]->setPosition(glm::vec2(117 * map->getTileSize(), (55 * map->getTileSize())));
	object[13]->setPosition(glm::vec2(113 * map->getTileSize(), (61 * map->getTileSize())));
	object[14]->setPosition(glm::vec2(168 * map->getTileSize(), (41 * map->getTileSize())));

	//gemstone
	object[15]->setPosition(glm::vec2(41 * map->getTileSize(), (153 * map->getTileSize())));


	// OGRES
	for (int i = 0; i < NUM_OGRES; ++i) ogre.push_back(new OgreEnemy());

	for (int i = 0; i < ogre.size(); ++i)
	{
		ogre[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ogre[i]->setTileMap(map);
		ogre[i]->setPlayer(player);
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

	//BATS
	for (int i = 0; i < NUM_BATS; ++i) bat.push_back(new BatEnemy());

	for (int i = 0; i < bat.size(); ++i)
	{
		bat[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		bat[i]->setTileMap(map);
	}

	bat[0]->setPosition(glm::vec2(134 * map->getTileSize(), (59 * map->getTileSize())));
	bat[1]->setPosition(glm::vec2(96 * map->getTileSize(), (59 * map->getTileSize())));
	bat[2]->setPosition(glm::vec2(135 * map->getTileSize(), (45 * map->getTileSize())));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	endGameDelay = -1.f;
}

void Scene::update(int deltaTime)
{
	if (gameEnded) return;
	currentTime += deltaTime;

	player->update(deltaTime);

	if (player->isDead()) {
		init();
		return;
	}

	for (int i = 0; i < object.size(); ++i) {
		object[i]->update(deltaTime);
		if (object[i]->canDelete()) {
			delete object[i];
			object.erase(object.begin() + i);
			--i;
		}
	}

	for (int i = 0; i < ogre.size(); ++i) {
		ogre[i]->update(deltaTime);

		if (ogre[i]->canDelete()) {
			delete ogre[i];
			ogre.erase(ogre.begin() + i);
			--i;
		}
	}

	for (int i = 0; i < bat.size(); ++i) {
		bat[i]->update(deltaTime);

		if (bat[i]->canDelete()) {
			delete bat[i];
			bat.erase(bat.begin() + i);
			--i;
		}
	}

	manageCollision();


	float halfWidth = SCREEN_WIDTH / 2.0f;
	float halfHeight = SCREEN_HEIGHT / 2.0f;

	// Calcular el ancho del mapa
	float mapWidth = map->getMapSize().x * map->getTileSize();


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

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();

	player->render();
	for (int i = 0; i < ogre.size(); ++i) ogre[i]->render();
	for (int i = 0; i < bat.size(); ++i) bat[i]->render();
	for (int i = 0; i < object.size(); ++i) object[i]->render();
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


void Scene::manageCollision()
{
	// PLAYER x OBJECT
	
	for (int i = 0; i < object.size(); ++i) {
		if (object[i]->isObjPickedUp()) continue;

		CollisionDir dir;

		if (dir = CollisionManager::instance().checkCollision(player->getPosition(), player->getPosCollision(), player->getSizeCollision(), object[i]->getPosition(), object[i]->getPosCollision(), object[i]->getSizeCollision()))
		{
			switch (object[i]->getObjectType()) {
				case ROCK:
				case MINERAL:
					float x, y;
					float playerSide, rockSide;

					switch (dir) {
					case RIGHT_COLLISION:
						if (player->getVelocity().y == 0 && player->getVelocity().x > 0) {
							// Right player side
							playerSide = player->getPosition().x + player->getPosCollision().x + player->getSizeCollision().x;
							// Left rock side
							rockSide = object[i]->getPosition().x + object[i]->getPosCollision().x;

							x = player->getPosition().x - (playerSide - rockSide);
							y = player->getPosition().y;

							player->setPosition(glm::vec2(x, y));
						}

						if (!player->isHoldingObj() && player->canPickUpObject() && Game::instance().interactKeyPressed()) {
							player->pickUpObject(object[i]);
						}
						break;
					case LEFT_COLLISION:
						if (player->getVelocity().y == 0 && player->getVelocity().x < 0) {
							// Left player side
							playerSide = player->getPosition().x + player->getPosCollision().x;
							// Right rock side
							rockSide = object[i]->getPosition().x + object[i]->getPosCollision().x + object[i]->getSizeCollision().x;

							x = player->getPosition().x - (playerSide - rockSide);
							y = player->getPosition().y;

							player->setPosition(glm::vec2(x, y));
						}

						if (!player->isHoldingObj() && player->canPickUpObject() && Game::instance().interactKeyPressed()) {
							player->pickUpObject(object[i]);
						}
						break;
					case BOTTOM_COLLISION:
						if (player->getVelocity().y > 0) {
							// Bottom player side
							playerSide = player->getPosition().y + player->getPosCollision().y + player->getSizeCollision().y;
							// Top rock side
							rockSide = object[i]->getPosition().y + object[i]->getPosCollision().y;

							x = player->getPosition().x;
							y = player->getPosition().y - (playerSide - rockSide);

							player->setPosition(glm::vec2(x, y));
						}
						player->hasGrounded();


						break;
					case TOP_COLLISION:
						if (player->getVelocity().y < 0) {
							// Top player side
							playerSide = player->getPosition().y + player->getPosCollision().y;
							// Bottom rock side
							rockSide = object[i]->getPosition().y + object[i]->getPosCollision().y + object[i]->getSizeCollision().y;

							x = player->getPosition().x;
							y = player->getPosition().y - (playerSide - rockSide);

							player->setPosition(glm::vec2(x, y));
						}
						break;
					}
					break;
				case CHEST:
					if (dir == BOTTOM_COLLISION && player->isFallingAss()) {
						
						int rewardType = rand() % 2; 

						if (rewardType == 0) {
							object[i]->setObjectType(HEART);
							object[i]->changeAnimation(HEART);
						}
						else {
							object[i]->setObjectType(COIN);
							object[i]->changeAnimation(COIN);
						}

						player->applyKillJump();   
					}
					else {
						float x, y;
						float playerSide, rockSide;

						switch (dir) {
							case RIGHT_COLLISION:
								if (player->getVelocity().y == 0 && player->getVelocity().x > 0) {
									// Right player side
									playerSide = player->getPosition().x + player->getPosCollision().x + player->getSizeCollision().x;
									// Left rock side
									rockSide = object[i]->getPosition().x + object[i]->getPosCollision().x;

									x = player->getPosition().x - (playerSide - rockSide);
									y = player->getPosition().y;

									player->setPosition(glm::vec2(x, y));
								}

								if (!player->isHoldingObj() && player->canPickUpObject() && Game::instance().interactKeyPressed()) {
									player->pickUpObject(object[i]);
								}
								break;
							case LEFT_COLLISION:
								if (player->getVelocity().y == 0 && player->getVelocity().x < 0) {
									// Left player side
									playerSide = player->getPosition().x + player->getPosCollision().x;
									// Right rock side
									rockSide = object[i]->getPosition().x + object[i]->getPosCollision().x + object[i]->getSizeCollision().x;

									x = player->getPosition().x - (playerSide - rockSide);
									y = player->getPosition().y;

									player->setPosition(glm::vec2(x, y));
								}

								if (!player->isHoldingObj() && player->canPickUpObject() && Game::instance().interactKeyPressed()) {
									player->pickUpObject(object[i]);
								}
								break;
							case BOTTOM_COLLISION:
								if (player->getVelocity().y > 0) {
									// Bottom player side
									playerSide = player->getPosition().y + player->getPosCollision().y + player->getSizeCollision().y;
									// Top rock side
									rockSide = object[i]->getPosition().y + object[i]->getPosCollision().y;

									x = player->getPosition().x;
									y = player->getPosition().y - (playerSide - rockSide);

									player->setPosition(glm::vec2(x, y));
								}
								player->hasGrounded();
								break;
							case TOP_COLLISION:
								if (player->getVelocity().y < 0) {
									// Top player side
									playerSide = player->getPosition().y + player->getPosCollision().y;
									// Bottom rock side
									rockSide = object[i]->getPosition().y + object[i]->getPosCollision().y + object[i]->getSizeCollision().y;

									x = player->getPosition().x;
									y = player->getPosition().y - (playerSide - rockSide);

									player->setPosition(glm::vec2(x, y));
								}
								break;
						}
						break;
					}
				case COIN:
					player->addPoints(100);  
					delete object[i];
					object.erase(object.begin() + i);
					--i;
					break;
				case HEART:
					player->addLife();
					delete object[i];
					object.erase(object.begin() + i);
					--i; // Redueix l'índex per evitar saltar un element després de l'eliminació
					break;
				case GEMSTONE:
					player->setVictoryStandAnimation();
					delete object[i];
					object.erase(object.begin() + i);
					--i;
					gameEnded = true;
					endGameDelay = 3.f;
					return;
					break;
			}

		}
	}
	
	// PLAYER x OGRE
	for (int i = 0; i < ogre.size(); ++i) {
		if (!ogre[i]->isEnemyDead()) {
			CollisionDir dir;

			if (dir = CollisionManager::instance().checkCollision(player->getPosition(), player->getPosCollision(), player->getSizeCollision(), ogre[i]->getPosition(), ogre[i]->getPosCollision(), ogre[i]->getSizeCollision()))
			{
				if (dir == BOTTOM_COLLISION && player->isFallingAss())
				{
					ogre[i]->die();
					player->applyKillJump();
					player->addPoints(50);
				}
				else
				{
					if (player->canTakeDamage() && !ogre[i]->isEnemyDead()) player->takeDamage();
				}
			}
		}
		
	}

	// PLAYER x BAT
	for (int i = 0; i < bat.size(); ++i) {
		if (bat[i]->isEnemyDead()) continue;
		if (!bat[i]->isEnemyDead()) {
			CollisionDir dir;

			if (dir = CollisionManager::instance().checkCollision(player->getPosition(), player->getPosCollision(), player->getSizeCollision(), bat[i]->getPosition(), bat[i]->getPosCollision(), bat[i]->getSizeCollision()))
			{
				if (dir == BOTTOM_COLLISION && player->isFallingAss())
				{
					bat[i]->die();
					player->applyKillJump();
					player->addPoints(70);
				}
				else
				{
					
					if (player->canTakeDamage() && !bat[i]->isEnemyDead()) player->takeDamage();
				}
			}
		}
	}

	// OBJECT x OGRE
	for (int i = 0; i < ogre.size(); ++i) {
		if (ogre[i]->isEnemyDead()) continue;
		for (int j = 0; j < object.size(); j++) {
			if (object[j]->getObjectType() == CHEST || object[j]->isObjPickedUp()) continue;
			CollisionDir dir;

			if (dir = CollisionManager::instance().checkCollision(ogre[i]->getPosition(), ogre[i]->getPosCollision(), ogre[i]->getSizeCollision(), object[j]->getPosition(), object[j]->getPosCollision(), object[j]->getSizeCollision()))
			{
				float x, y;
				float ogreSide, rockSide;

				switch (dir)
				{
				case RIGHT_COLLISION:
					if (object[j]->getVelocity().x != 0) {
						ogre[i]->die();
						player->addPoints(60);
					}
					else {
						// Right ogre side
						ogreSide = ogre[i]->getPosition().x + ogre[i]->getPosCollision().x + ogre[i]->getSizeCollision().x;
						// Left rock side
						rockSide = object[j]->getPosition().x + object[j]->getPosCollision().x;

						x = ogre[i]->getPosition().x - (ogreSide - rockSide);
						y = ogre[i]->getPosition().y;

						ogre[i]->setPosition(glm::vec2(x, y));

						ogre[i]->setMoveDir(LEFT);
					}
					break;
				case LEFT_COLLISION:
					if (object[j]->getVelocity().x != 0) {
						ogre[i]->die();
						player->addPoints(60);
					}
					else {
						// Left ogre side
						ogreSide = ogre[i]->getPosition().x + ogre[i]->getPosCollision().x;
						// Right rock side
						rockSide = object[j]->getPosition().x + object[j]->getPosCollision().x + object[j]->getSizeCollision().x;

						x = ogre[i]->getPosition().x - (ogreSide - rockSide);
						y = ogre[i]->getPosition().y;

						ogre[i]->setPosition(glm::vec2(x, y));

						ogre[i]->setMoveDir(RIGHT);
					}
					break;
				case BOTTOM_COLLISION:
					if (object[j]->getVelocity().y != 0) {
						ogre[i]->die();
						player->addPoints(60);
					}
					else {
						// Bottom ogre side
						ogreSide = ogre[i]->getPosition().y + ogre[i]->getPosCollision().y + ogre[i]->getSizeCollision().y;
						// Top rock side
						rockSide = object[j]->getPosition().y + object[j]->getPosCollision().y;

						x = ogre[i]->getPosition().x;
						y = ogre[i]->getPosition().y - (ogreSide - rockSide);

						ogre[i]->setPosition(glm::vec2(x, y));
					}
					break;
				case TOP_COLLISION:
					if (object[j]->getVelocity().y != 0) {
						ogre[i]->die();
						player->addPoints(60);
					}
					else {
						// Top ogre side
						ogreSide = ogre[i]->getPosition().y + ogre[i]->getPosCollision().y;
						// Bottom rock side
						rockSide = object[j]->getPosition().y + object[j]->getPosCollision().y + object[j]->getSizeCollision().y;

						x = ogre[i]->getPosition().x;
						y = ogre[i]->getPosition().y - (ogreSide - rockSide);

						ogre[i]->setPosition(glm::vec2(x, y));
					}
					break;
				}
			}
		}
	}

	// OBJECT x BAT
	for (int i = 0; i < bat.size(); ++i) {
		if (bat[i]->isEnemyDead()) continue;

		for (int j = 0; j < object.size(); ++j) {
			if (object[j]->getObjectType() != MINERAL || object[j]->isObjPickedUp()) continue;

			CollisionDir dir;

			if (dir = CollisionManager::instance().checkCollision(bat[i]->getPosition(), bat[i]->getPosCollision(), bat[i]->getSizeCollision(), object[j]->getPosition(), object[j]->getPosCollision(), object[j]->getSizeCollision())) {
				if (object[j]->getVelocity().x != 0 || object[j]->getVelocity().y != 0) {
					bat[i]->die();
					player->addPoints(80);
				}
			}
		}
	}


	// OBJECT x OBJECT
	for (int i = 0; i < object.size(); ++i) {
		for (int j = 0; j < object.size(); j++) {
			if (i == j) continue;
			if (object[i]->isObjPickedUp()) continue;
			if (object[i]->getVelocity().x == 0 && object[i]->getVelocity().y == 0) continue;

			CollisionDir dir;

			if (dir = CollisionManager::instance().checkCollision(object[i]->getPosition(), object[i]->getPosCollision(), object[i]->getSizeCollision(), object[j]->getPosition(), object[j]->getPosCollision(), object[j]->getSizeCollision()))
			{
				float x, y;
				float rock1Side, rock2Side;

				switch (dir)
				{
				case RIGHT_COLLISION:
					// Right ogre side
					rock1Side = object[i]->getPosition().x + object[i]->getPosCollision().x + object[i]->getSizeCollision().x;
					// Left rock side
					rock2Side = object[j]->getPosition().x + object[j]->getPosCollision().x;

					x = object[i]->getPosition().x - (rock1Side - rock2Side);
					y = object[i]->getPosition().y;

					object[i]->setPosition(glm::vec2(x, y));

					break;
				case LEFT_COLLISION:
					// Left ogre side
					rock1Side = object[i]->getPosition().x + object[i]->getPosCollision().x;
					// Right rock side
					rock2Side = object[j]->getPosition().x + object[j]->getPosCollision().x + object[j]->getSizeCollision().x;

					x = object[i]->getPosition().x - (rock1Side - rock2Side);
					y = object[i]->getPosition().y;

					object[i]->setPosition(glm::vec2(x, y));

					break;
				case BOTTOM_COLLISION:
					// Bottom ogre side
					rock1Side = object[i]->getPosition().y + object[i]->getPosCollision().y + object[i]->getSizeCollision().y;
					// Top rock side
					rock2Side = object[j]->getPosition().y + object[j]->getPosCollision().y;

					x = object[i]->getPosition().x;
					y = object[i]->getPosition().y - (rock1Side - rock2Side);

					object[i]->setPosition(glm::vec2(x, y));

					object[i]->setVelocity(glm::vec2(0.f, 0.f));

					break;
				case TOP_COLLISION:
					// Top ogre side
					rock1Side = object[i]->getPosition().y + object[i]->getPosCollision().y;
					// Bottom rock side
					rock2Side = object[j]->getPosition().y + object[j]->getPosCollision().y + object[j]->getSizeCollision().y;

					x = object[i]->getPosition().x;
					y = object[i]->getPosition().y - (rock1Side - rock2Side);

					object[i]->setPosition(glm::vec2(x, y));

					break;
				}
			}
		}
	}
}

Player* Scene::getPlayer() {
	return player;
}

