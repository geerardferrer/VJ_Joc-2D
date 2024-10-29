#ifndef _COLLISION_MANAGER_INCLUDE
#define _COLLISION_MANAGER_INCLUDE


#include <glm/glm.hpp>
#include <vector>

enum CollisionDir
{
	NO_COLLISION, LEFT_COLLISION, RIGHT_COLLISION, TOP_COLLISION, BOTTOM_COLLISION
};

class CollisionManager
{

private:
	CollisionManager() {}

public:
	static CollisionManager &instance()
	{
		static CollisionManager CM;

		return CM;
	}

	CollisionDir checkCollision(const glm::fvec2 &entityPos1, const glm::fvec2 &entityCollision1, const glm::ivec2 &entitySize1, const glm::fvec2 &entityPos2, const glm::fvec2 &entityCollision2, const glm::ivec2 &entitySize2);

private:

};


#endif // _COLLISION_MANAGER_INCLUDE

