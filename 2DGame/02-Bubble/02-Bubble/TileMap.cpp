#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"

using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	string tile;

	travessableTiles = { 0,4,5,12,23,30,31,32,39,41,42,46,48,52,53,54,55,56,57,60,61,62,63,65,67,68,69,70,71,72,73,76,77,78,79,81,83,84,85,86,87,88,89,94,95,100,101,102,103,104,105,109,110,111,112,113,114,115,116,117,118,120,128 };
	travessableTopTiles = { 14,15,16,27,43,44,45,91,92,107,108 };

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			if (i == mapSize.x - 1) getline(fin, tile);
			else getline(fin, tile, ',');
			map[j*mapSize.x + i] = stoi(tile) + 1;
		}
#ifndef _WIN32
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

int TileMap::getTileAt(const glm::fvec2 &pos) const {
	int tileX = pos.x / tileSize;
	int tileY = pos.y / tileSize;

	return map[tileY * mapSize.x + tileX] - 1;
}

bool TileMap::tileIsTravessable(int tile) const {
	return !(travessableTiles.find(tile) == travessableTiles.end());
}

bool TileMap::tileIsTopTravessable(int tile) const {
	return !(travessableTopTiles.find(tile) == travessableTopTiles.end());
}


// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize, EntityType entity) const
{
	int x, y0, y1;

	glm::fvec2 pos = entityPos + colliderPos;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + colliderSize.y - 1) / tileSize;

	for (int y = y0; y <= y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		
		switch (entity)
		{
		case EnemyType:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile) || tile == 128) {
				entityPos.x = (x + 1) * tileSize - colliderPos.x;
				return true;
			}
			break;
		default:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile)) {
				entityPos.x = (x + 1) * tileSize - colliderPos.x;
				return true;
			}
			break;
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize, EntityType entity) const
{
	int x, y0, y1;

	glm::fvec2 pos = entityPos + colliderPos;

	x = (pos.x + colliderSize.x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + colliderSize.y - 1) / tileSize;

	for (int y = y0; y <= y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		
		switch (entity)
		{
		case EnemyType:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile) || tile == 128) {
				entityPos.x = tileSize * x - entitySize.x + colliderPos.x;
				return true;
			}
			break;
		default:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile)) {
				entityPos.x = tileSize * x - entitySize.x + colliderPos.x;
				return true;
			}
			break;
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize, EntityType entity) const
{
	int x0, x1, y;

	glm::fvec2 pos = entityPos + colliderPos;

	x0 = pos.x / tileSize;
	x1 = (pos.x + colliderSize.x - 1) / tileSize;
	y = (pos.y + colliderSize.y) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];

		switch (entity)
		{
		case EnemyType:
			if (!tileIsTravessable(tile) || tile == 128) {
				entityPos.y = tileSize * y - entitySize.y + colliderPos.y;
				return true;
			}
			break;
		default:
			if (!tileIsTravessable(tile)) {
				entityPos.y = tileSize * y - entitySize.y + colliderPos.y;
				return true;
			}
			break;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize, EntityType entity) const
{
	int x0, x1, y;

	glm::fvec2 pos = entityPos + colliderPos;

	x0 = pos.x / tileSize;
	x1 = (pos.x + colliderSize.x - 1) / tileSize;
	y = pos.y / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];

		switch (entity)
		{
		case EnemyType:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile) || tile == 128) {
				entityPos.y = (y + 1) * tileSize - colliderPos.y;
				return true;
			}
			break;
		default:
			if (!tileIsTravessable(tile) && !tileIsTopTravessable(tile)) {
				entityPos.y = (y + 1) * tileSize - colliderPos.y;
				return true;
			}
			break;
		}
	}

	return false;
}


