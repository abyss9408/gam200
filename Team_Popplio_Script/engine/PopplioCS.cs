/******************************************************************************/
/*!
\file   PopplioCS.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for initializing the engine's C# system-side functionalities

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

using Team_Popplio.Libraries;

namespace Team_Popplio
{
	public class PopplioCS // called by C++
	{
		private static MonoHotReload? monoHot;

		private static void Init(string assembly)
		{
			Logger.EngineInfo("PopplioCS.cs | Initializing Engine C# Systems...");

			//Console.WriteLine(Environment.Version);
			//Console.WriteLine(typeof(object).Assembly.GetName().Version);

			try
			{
				monoHot = new MonoHotReload(assembly);
			}
			catch (Exception ex)
			{
				Logger.EngineError($"Exception in MonoHotReload: {ex.Message}");
				Logger.EngineError($"Stack Trace: {ex.StackTrace}");
			}

			//Logger.EngineInfo("PopplioCS.cs | Initialized Engine C# Systems");
		}

		private static void CompleteInit()
		{
			Logger.EngineInfo($"PopplioCS.cs | Loaded {Entity.GetEntities().Count.ToString()} Entities");

			//foreach (Entity e in Entity.GetEntities().Values)
			//	Logger.EngineInfo("PopplioCS.cs | " + e.ToString());

			Logger.EngineInfo("PopplioCS.cs | Completed initialization of Engine C# Systems");
		}
	}
}
