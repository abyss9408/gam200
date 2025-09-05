/******************************************************************************/
/*!
\file   BinaryCollision.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
		Header file for binary collision processing

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// JUST COPIED OVER FOR NOW - Shawn

#pragma once

namespace PopplioPhysics
{
	/******************************************************************************/
	/*!
		This function snaps the value sent as parameter to the center of the cell.
		It is used when a sprite is colliding with a collision area from one
		or more side.
		To snap the value sent by "Coordinate", find its integral part by type
		casting it to an integer, then add 0.5 (which is half the cell's width
		or height)
	 */
	 /******************************************************************************/
	void SnapToCell(float& coordinate);

	/******************************************************************************/
	/*!
		This function creates 2 hot spots on each side of the object instance,
		and checks if each of these hot spots is in a collision area (which means
		the cell if falls in has a value of 1).
		At the beginning of the function, a "Flag" integer should be initialized to 0.
		Each time a hot spot is in a collision area, its corresponding bit
		in "Flag" is set to 1.
		Finally, the function returns the integer "Flag"
		The position of the object instance is received as PosX and PosY
		The size of the object instance is received as scaleX and scaleY

		Note: This function assume the object instance's size is 1 by 1
			  (the size of 1 tile)

		Creating the hotspots:
			-Handle each side separately.
			-2 hot spots are needed for each collision side.
			-These 2 hot spots should be positioned on 1/4 above the center
			and 1/4 below the center

		Example: Finding the hots spots on the left side of the object instance

		f32 x1, y1, x2, y2;

		-hotspot 1
		x1 = PosX + scaleX/2	To reach the right side
		y1 = PosY + scaleY/4	To go up 1/4 of the height

		-hotspot 2
		x2 = PosX + scaleX/2	To reach the right side
		y2 = PosY - scaleY/4	To go down 1/4 of the height
	 */
	 /******************************************************************************/
	int CheckInstanceBinaryMapCollision(const float& PosX, const float& PosY,
		const float& scaleX, const float& scaleY, char** grid2D, const size_t& gridHeight, bool(*comp)(const char&));

	/******************************************************************************/
	/*!
		Collision response
	 */
	 /******************************************************************************/
	void AffectCollision(float& PosX, float& PosY,
		const float& scaleX, const float& scaleY, char** grid2D, const size_t& gridHeight, bool(*comp)(const char&));
}