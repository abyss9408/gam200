/******************************************************************************/
/*!
\file   MatVec.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the header file for the Mat2 & vec2 class API that includes both.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Matrix2D.h"
#include "Vector2D.h"

namespace PopplioMath
{
	/**************************************************************************/
	/*!
		This function multiplies the matrix lhs with the vector rhs
		and returns the result as a vector
	*/
	/**************************************************************************/
	//Vector2D operator * (const M3x3f& lhs, const Vector2D& rhs)
	//{ return Vector2D(	lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * 1.0f, 
	//					lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * 1.0f); }

	/**************************************************************************/
	/*!
		This function multiplies the matrix lhs with the vector rhs
		and returns the result as a vector
	*/
	/**************************************************************************/
	//Vector2D operator * (const M3x3i& lhs, const Vector2D& rhs)
	//{ return Vector2D(	lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * 1,
	//					lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * 1); }
}
