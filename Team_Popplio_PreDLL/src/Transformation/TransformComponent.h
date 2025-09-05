/******************************************************************************/
/*!
\file   TransformComponent.h
\author Team Popplio
\author Pearl Goh (100%)
\par    Course : CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This source file defines the TransformComponent structure, which includes
		position, scale, and rotation properties for game objects.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Math/Vector2D.h"
#include "../Math/Matrix2D.h"

namespace Popplio
{
    enum class Anchor {
        TopLeft,
        TopCenter,
        TopRight,
        MiddleLeft,
        MiddleCenter,
        MiddleRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

	struct TransformComponent
	{
		PopplioMath::Vec2f position;
		PopplioMath::Vec2f scale;
		double rotation;
		PopplioMath::M3x3f modelMatrix;
        Anchor anchor; 
		//public:

		TransformComponent(float xpos = 0, float ypos = 0, float xscl = 1, float yscl = 1, double rot = 0, Anchor anch = Anchor::MiddleCenter)
			: position(xpos, ypos), scale(xscl, yscl), rotation(rot), modelMatrix(), anchor(anch) {}

        //PopplioMath::Vec2f const& Position() const { return position; }
        //PopplioMath::Vec2f& Position() { return position; }
        //PopplioMath::Vec2f const& Scale() const { return scale; }
        //PopplioMath::Vec2f& Scale() { return scale; }
        //double const& Rotation() const { return rotation; }
        //double& Rotation() { return rotation; }
        //PopplioMath::M3x3f const& ModelMatrix() const { return modelMatrix; }
        //PopplioMath::M3x3f& ModelMatrix() { return modelMatrix; }
	};
}