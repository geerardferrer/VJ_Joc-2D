#include "CollisionManager.h"
#include <cmath>
#include <algorithm>

CollisionDir CollisionManager::checkCollision(const glm::fvec2 &entityPos1, const glm::fvec2 &entityCollision1, const glm::ivec2 &entitySize1, const glm::fvec2 &entityPos2, const glm::fvec2 &entityCollision2, const glm::ivec2 &entitySize2)
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
		float overlapY = std::min(bottom1 - top2, bottom2 - top1) * 0.3f; // factor para priorizar caidas por encima/debajo

		if (overlapX < overlapY)
		{
			return ((right1 - left2 < right2 - left1) ? RIGHT_COLLISION : LEFT_COLLISION);
		}
		else
		{
			return ((bottom1 - top2 < bottom2 - top1) ? BOTTOM_COLLISION : TOP_COLLISION);
		}
	}
	return NO_COLLISION;
}