/******************************************************************************/
/*!
\file   Keybinds.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for cross scene keybind settings

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using Team_Popplio.Extern;

namespace BassNKick.Lib
{
	public static class Keybinds
	{
		// game
		public static GLFW calibrate = GLFW.KEY_F;
		public static GLFW pause = GLFW.KEY_ESCAPE;
		public static GLFW back = GLFW.KEY_ESCAPE;
		public static GLFW confirm = GLFW.KEY_ENTER;
		public static GLFW lane1 = GLFW.KEY_R;
		public static GLFW lane2 = GLFW.KEY_F;
		public static GLFW lane3 = GLFW.KEY_V;
		public static GLFW exit = GLFW.KEY_DELETE;
		public static GLFW press = GLFW.KEY_SPACE;
		public static GLFW options = GLFW.KEY_BACKSPACE;

		// debug
		public static GLFW mainMenu = GLFW.KEY_1;
		public static GLFW levelSelect = GLFW.KEY_2;
		public static GLFW results = GLFW.KEY_3;
		public static GLFW track1 = GLFW.KEY_4;
		public static GLFW track2 = GLFW.KEY_5;
		public static GLFW track3 = GLFW.KEY_6;
		public static GLFW reload = GLFW.KEY_7;

		public static GLFW showFPS = GLFW.KEY_0;
	}
}
