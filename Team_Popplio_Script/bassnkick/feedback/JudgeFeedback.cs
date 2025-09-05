/******************************************************************************/
/*!
\file   JudgeFeedback.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the judgement feedback

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using Team_Popplio;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;

namespace BassNKick
{
	public class JudgeFeedback : PopplioMonoScript
	{
		[Inspector("Glow 1")]
		public Entity glow1 = Entity.EntityField();
		[Inspector("Glow 2")]
		public Entity glow2 = Entity.EntityField();
		[Inspector("Glow 3")]
		public Entity glow3 = Entity.EntityField();
	}
}
