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
	player->update(deltaTime);
	glm::vec2 playerPos = player->getPosition();

	for (int i = 0; i < rock.size(); ++i) rock[i]->update(deltaTime);
	for (int i = 0; i < ogre.size(); ++i) {
		ogre[i]->setPlayerPosition(playerPos);
		ogre[i]->update(deltaTime);

		//COLLISION PLAYER/OGRE
		if (checkCollision(playerPos, player->getPosCollision(), player->getSizeCollision(), ogre[i]->getPosition(), ogre[i]->getPosCollision(), ogre[i]->getSizeCollision()))
		{
			manageCollision(i);
		}
	}


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

bool Scene::checkCollision(const glm::fvec2 &playerPos, const glm::fvec2 &playerCollision, const glm::ivec2 &playerSize,
	const glm::fvec2 &enemyPos, const glm::fvec2 &enemyCollision, const glm::ivec2 &enemySize)
{
	// Càlcul de les posicions de la hitbox del player i l'enemic
	glm::fvec2 playerHitboxPos = playerPos + playerCollision;
	glm::fvec2 enemyHitboxPos = enemyPos + enemyCollision;

	// Comprovació de col·lisió en l'eix X
	bool collisionX = (playerHitboxPos.x + playerSize.x > enemyHitboxPos.x) && (enemyHitboxPos.x + enemySize.x > playerHitboxPos.x);

	// Comprovació de col·lisió en l'eix Y
	bool collisionY = (playerHitboxPos.y + playerSize.y > enemyHitboxPos.y) && (enemyHitboxPos.y + enemySize.y > playerHitboxPos.y);

	// Si hi ha col·lisió en els dos eixos, retorna true
	return collisionX && collisionY;
}


void Scene::manageCollision(int enemyIndex)
{
	glm::vec2 playerPos = player->getPosition();
	glm::vec2 enemyPos = ogre[enemyIndex]->getPosition();

	// Comprovem si el jugador ha caigut sobre l'enemic
	//if (playerPos.y + player->getSize().y <= enemyPos.y + 10) // Ajust segons sigui necessari
	//{
		// El jugador cau sobre l'enemic -> l'enemic mor
		//ogre[enemyIndex]->die();

		// Determinar si és LEFT o RIGHT en funció de la posició del jugador
		if (playerPos.x < enemyPos.x)
		{
			
			ogre[enemyIndex]->startDeathAnimation("LEFT");
		}
		else
		{
			ogre[enemyIndex]->startDeathAnimation("RIGHT");
		}

		//player->jump(); // El jugador fa un salt després de matar l'enemic
	//}
	//else
	//{
		// Si no ha caigut sobre l'enemic, el jugador perd una vida
		//player->loseLife();
	//}
}




