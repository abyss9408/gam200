/******************************************************************************/
/*!
\file   Attributes.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for the engine's C# attributes

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Team_Popplio.Attributes
{
	#region Inspector
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
	public class InspectorAttribute : Attribute
	{
		public string? Name { get; private set; }

		public InspectorAttribute(string? name = null)
		{
			Name = name;
		}
	}

	[AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
	public class InspectorIgnoreAttribute : Attribute
	{
		public InspectorIgnoreAttribute() { }
	}

	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
	public class InspectorHeaderAttribute : Attribute
	{
		public string? Name { get; private set; }
		public InspectorHeaderAttribute(string? name = null)
		{
			Name = name;
		}
	}
	#endregion
}
