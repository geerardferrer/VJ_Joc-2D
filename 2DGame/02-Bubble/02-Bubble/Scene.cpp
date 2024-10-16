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

	for (int i = 0; i < NUM_ROCKS; ++i) rock.push_back(new Rock());

	for (int i = 0; i < rock.size(); ++i)
	{
		rock[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		rock[i]->setTileMap(map);
	}
	
	rock[0]->setPosition(glm::vec2(37 * map->getTileSize(), (72 * map->getTileSize())));
	rock[1]->setPosition(glm::vec2(56 * map->getTileSize(), (76 * map->getTileSize())));
	rock[2]->setPosition(glm::vec2(77 * map->getTileSize(), (75 * map->getTileSize())));
	rock[3]->setPosition(glm::vec2(71 * map->getTileSize(), (94 * map->getTileSize())));
	rock[4]->setPosition(glm::vec2(118 * map->getTileSize(), (70 * map->getTileSize())));
	rock[5]->setPosition(glm::vec2(150 * map->getTileSize(), (118 * map->getTileSize())));
	rock[6]->setPosition(glm::vec2(142 * map->getTileSize(), (118 * map->getTileSize())));
	rock[7]->setPosition(glm::vec2(44 * map->getTileSize(), (161 * map->getTileSize())));

	for (int i = 0; i < NUM_OGRES; ++i) ogre.push_back(new OgreEnemy());

	for (int i = 0; i < ogre.size(); ++i)
	{
		ogre[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ogre[i]->setTileMap(map);
	}

	ogre[0]->setPosition(glm::vec2(37 * map->getTileSize(), (73 * map->getTileSize())));
	ogre[1]->setPosition(glm::vec2(56 * map->getTileSize(), (77 * map->getTileSize())));
	ogre[2]->setPosition(glm::vec2(77 * map->getTileSize(), (76 * map->getTileSize())));
	ogre[3]->setPosition(glm::vec2(71 * map->getTileSize(), (95 * map->getTileSize())));
	ogre[4]->setPosition(glm::vec2(118 * map->getTileSize(), (71 * map->getTileSize())));
	ogre[5]->setPosition(glm::vec2(150 * map->getTileSize(), (119 * map->getTileSize())));
	ogre[6]->setPosition(glm::vec2(142 * map->getTileSize(), (119 * map->getTileSize())));
	ogre[7]->setPosition(glm::vec2(44 * map->getTileSize(), (162 * map->getTileSize())));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int i = 0; i < rock.size(); ++i) rock[i]->update(deltaTime);
	for (int i = 0; i < ogre.size(); ++i) ogre[i]->update(deltaTime);

	glm::vec2 playerPos = player->getPosition();
	float halfWidth = SCREEN_WIDTH / 2.0f;
	float halfHeight = SCREEN_HEIGHT / 2.0f;

	// Calcular el ancho del mapa
	float mapWidth = map->getMapSize().x * map->getTileSize(); // Método para obtener el tamaño del mapa

															   // Calcular la posición de la cámara en el eje X
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



