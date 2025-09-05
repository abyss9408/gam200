/******************************************************************************/
/*!
\file   Physics.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This implementation file contains the definitions for the physics-related 
		functions and classes declared in Physics.h. It provides the functionality 
		for detecting collisions between various objects (points, rectangles, and 
		circles) and managing gravitational effects on entities.

		The functions include:
		- PointRectangleCollision: Determines if a point lies within a rectangle.
		- RectangleToRectangleCollision: Checks for collisions between two rectangles.
		- CircleToCircleCollision: Checks for collisions between two circles.
		- AABB_RectangleRectangleCollision: Performs collision detection between two AABBs,
		  taking into account their velocities and the time of collision.

		Added BoxToCircleCollision to detect collision between a circle and a box collider
		component

		The Gravity class is also defined here, with methods to apply gravity to
		an entity's velocity over a specified time interval. 


Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Physics.h"

namespace PopplioPhysics
{
	static bool StaticCollisionIntersection_RectRect(const AABB& a, const AABB& b)
	{
		if (a.max.x < b.min.x || a.min.x > b.max.x)
		{
			return false;
		}
		if (a.max.y < b.min.y || a.min.y > b.max.y)
		{
			return false;
		}
		return true;
	}

	bool PointRectangleCollision(PopplioMath::Vec2f const& point, float left, float right, float top, float bottom)
	{
		if (point.x < left || point.x > right) {
			return false;
		}

		if (point.y < bottom || point.y > top) {
			return false;
		}

		return true;
	}

	bool RectangleToRectangleCollision(float leftA, float rightA, float topA, float bottomA, float leftB, float rightB, float topB, float bottomB)
	{
		if (leftA > rightB || leftB > rightA)
		{
			return false;
		}
		else if (topA < bottomB || topB < bottomA)
		{
			return false;
		}

		return true;

	}

	bool CircleToCircleCollision(PopplioMath::Vec2f const& centerA, float radiusA, PopplioMath::Vec2f centerB, float radiusB)
	{
		float radiusSumSq = (radiusA + radiusB) * (radiusA + radiusB);

		//calculate squared distance between centers
		float dx = centerA.x - centerB.x;
		float dy = centerA.y - centerB.y;

		float centersDistanceSq = (dx * dx) + (dy * dy);

		//check collision
		return centersDistanceSq <= radiusSumSq;
	}

	bool BoxToCircleCollision(float boxLeft, float boxRight, float boxTop, float boxBottom, PopplioMath::Vec2f const& circleCenter, float circleRadius)
	{
		//find the closest point on the box to the circle center
		//removed std::max and std::min by using if-else statements instead
		//float closeToX = std::max(boxLeft, std::min(circleCenter.x, boxRight));
		//float closeToY = std::max(boxBottom, std::min(circleCenter.y, boxTop));
		float closeToX;
		if (circleCenter.x < boxLeft)
			closeToX = boxLeft;
		else if (circleCenter.x > boxRight)
			closeToX = boxRight;
		else
			closeToX = circleCenter.x;
		

		//now to find closest Y point
		float closeToY;
		if (circleCenter.y < boxBottom)
			closeToY = boxBottom;
		else if (circleCenter.y > boxTop)
			closeToY = boxTop;
		else
			closeToY = circleCenter.y;

		//calculate the distance between the closest point and circle center
		float dx = closeToX - circleCenter.x;
		float dy = closeToY - circleCenter.y;
		float distanceSquared = (dx * dx) + (dy * dy);

		return distanceSquared <= (circleRadius * circleRadius);
	}

	bool AABB_RectangleRectangleCollision(const AABB& aabb1, const PopplioMath::Vec2f& vel1, const AABB& aabb2, const PopplioMath::Vec2f& vel2,
		float& firstTimeOfCollision, float GlobalDeltaTime)
	{
		//static collision check
		if (StaticCollisionIntersection_RectRect(aabb1, aabb2))
		{
			firstTimeOfCollision = 0.0f;
			return true;
		}

		//initialize
		float tFirst = 0.0f;
		float tLast = GlobalDeltaTime;
		float dFirstX, dFirstY;
		float dLastX, dLastY;

		float VB_x = vel2.x - vel1.x; //relative velocity along x-axis
		float VB_y = vel2.y - vel1.y; //relative velocity along y-axis

		dFirstX = aabb1.max.x - aabb2.min.x;
		dFirstY = aabb1.max.y - aabb2.min.y;
		dLastX = aabb1.min.x - aabb2.max.x;
		dLastY = aabb1.min.y - aabb2.max.y;

		//check x-axis

		if (VB_x < 0.0f) {
			if (dFirstX > 0.0f) {
				return false; //no collision and B is moving away
			}

			if (dLastX < 0.0f) {
				tFirst = std::max(dLastX / VB_x, tFirst); 
				tLast = std::min(dFirstX / VB_x, tLast);
			}
		}
		else if (VB_x > 0.0f) {
			if (dFirstX < 0.0f) {
				return false; //no collision and B is moving away
			}

			if (dLastX > 0.0f) {
				tFirst = std::max(dFirstX / VB_x, tFirst); 
				tLast = std::min(dLastX / VB_x, tLast);
			}

		}
		else {
			if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x)
			{
				return false; //no collision and no intersection
			}
		}

		//now check the y-axis
		if (VB_y < 0.0f)
		{
			if (dFirstY > 0.0f) {
				return false;
			}

			if (dLastY < 0.0f) {
				tFirst = std::max(dLastY / VB_y, tFirst);
				tLast = std::min(dFirstY / VB_y, tLast);

			}
		}
		else if (VB_y > 0.0f) {
			if (dFirstY < 0.0f) {
				return false;
			}

			if (dLastY > 0.0f) {
				tFirst = std::max(dFirstY / VB_y, tFirst);
				tLast = std::min(dLastY / VB_y, tLast);
			}
		}
		else {
			if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y)
			{
				return false;
			}
		}

		//check if there is any valid time interval for collision
		if (tFirst > tLast) 
		{
			return false;
		}

		//return collision result
		firstTimeOfCollision = tFirst; //update with tFirst

		return true; //there is a collision


	}

	//overload constructor for Gravity
	Gravity::Gravity(double gravityAcceleration) : gravity(gravityAcceleration) {}

	//void Gravity::ApplyGravity(Popplio::RigidBodyComponent& rigidBody, double deltaTime) {
	//	if (rigidBody.useGravity) {
	//		float gravityForce = -static_cast<float>(gravity *
	//			rigidBody.gravityScale * rigidBody.mass);
	//		rigidBody.ApplyForce(0.0f, gravityForce);
	//	}
	//}

}