#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <set>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 TileMap::getMapSize() const { return mapSize; }


	bool collisionMoveLeft(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize) const;
	bool collisionMoveRight(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize) const;
	bool collisionMoveDown(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize) const;
	bool collisionMoveUp(glm::fvec2 &entityPos, const glm::fvec2 &colliderPos, const glm::ivec2 &entitySize, const glm::ivec2 &colliderSize) const;


	bool tileIsTravessable(int tile) const;
	bool tileIsTopTravessable(int tile) const;

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	std::set<int> travessableTiles, travessableTopTiles;

};


#endif // _TILE_MAP_INCLUDE


