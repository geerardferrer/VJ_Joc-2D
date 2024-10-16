#include <iostream>
#include <cmath>
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
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	background = Background::createBackground("images/Background.png", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), texProgram);
	background->setPosition(glm::vec2(4 * map->getTileSize(), 70 * map->getTileSize()));

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//ogre[0] = new OgreEnemy();
	//ogre[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	//ogre[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 10) * map->getTileSize(), ((INIT_PLAYER_Y_TILES)* map->getTileSize())));
	//ogre[0]->setTileMap(map);

	for (int i = 0; i < NUM_OGRES; ++i) ogre.push_back(new OgreEnemy());

	for (int i = 0; i < ogre.size(); ++i)
	{
		ogre[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ogre[i]->setTileMap(map);
	}

	ogre[0]->setPosition(glm::vec2(36 * map->getTileSize(), (71 * map->getTileSize())));
	ogre[1]->setPosition(glm::vec2(55 * map->getTileSize(), (75 * map->getTileSize())));
	ogre[2]->setPosition(glm::vec2(76 * map->getTileSize(), (74 * map->getTileSize())));
	ogre[3]->setPosition(glm::vec2(70 * map->getTileSize(), (93 * map->getTileSize())));
	ogre[4]->setPosition(glm::vec2(117 * map->getTileSize(), (69 * map->getTileSize())));
	ogre[5]->setPosition(glm::vec2(149 * map->getTileSize(), (117 * map->getTileSize())));
	ogre[6]->setPosition(glm::vec2(141 * map->getTileSize(), (117 * map->getTileSize())));
	ogre[7]->setPosition(glm::vec2(43 * map->getTileSize(), (160 * map->getTileSize())));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int i = 0; i < ogre.size(); ++i) ogre[i]->update(deltaTime);

	glm::vec2 playerPos = player->getPosition();
	float halfWidth = SCREEN_WIDTH / 2.0f;
	float halfHeight = SCREEN_HEIGHT / 2.0f;

	// Calcular el ancho del mapa
	float mapWidth = map->getMapSize().x * map->getTileSize(); // M�todo para obtener el tama�o del mapa

															   // Calcular la posici�n de la c�mara en el eje X
	float cameraX = playerPos.x;

	
	if (cameraX < halfWidth) {
		cameraX = halfWidth; 
	}
	else if (cameraX > mapWidth - halfWidth) {
		cameraX = mapWidth - halfWidth; 
	}

	projection = glm::ortho(cameraX - halfWidth, cameraX + halfWidth,
		playerPos.y + halfHeight, playerPos.y - halfHeight);
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
	//background->render();
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



