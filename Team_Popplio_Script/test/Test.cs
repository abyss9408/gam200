/******************************************************************************/
/*!
\file   Test.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test C# - C++ interop for Team_Popplio engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.CompilerServices;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;

namespace BassNKick
{
	public class Test : PopplioMonoScript
	{
		// test //
		public float MyPublicFloatVar = 5.0f;
#pragma warning disable CS0414
		private float MyPrivateFloatVar = 10.0f;
#pragma warning restore CS0414

		private string m_Name = "Popplio";

		public string Name
		{
			get => m_Name;
			set
			{
				m_Name = value;
				MyPublicFloatVar += 5.0f;
			}
		}

		public float MyPublicFloatProperty
		{
			get => MyPublicFloatVar;
			set => MyPublicFloatVar = value;
		}

		public void PrintFloatVar()
		{
			Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
		}

		[MethodImpl(MethodImplOptions.NoInlining)]
		public void PrintThunk()
		{
			Console.WriteLine("Thunk");
		}

		public float ReturnFloatVar()
		{
			return MyPublicFloatVar;
		}

		public void IncrementFloatVar(float value)
		{
			MyPublicFloatVar += value;
		}

		public int IncrementInt(int value)
		{
			return value;
		}
		// test //

		public override void Setup()
		{
			Logger.Info("Test.cs | Setup()");
			Logger.Info("Test.cs | " + EntID);
			Logger.Info("Test.cs | " + This.GetId());
			//Console.WriteLine("Console: Test.cs | Setup()");
		}

		public override void Init()
		{
			Logger.Info("Test.cs | Init()");
			Logger.Info("" + This.GetId());
			This.Kill();
			//Console.WriteLine("Test.cs | Init()");
		}

		public override void Update()
		{
			
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			Logger.Info("Test.cs | End()");
			//Console.WriteLine("Test.cs | End()");
		}

		public override void OnDestroy()
		{
			Logger.Info("Test.cs | Destroy()");
		}
	}
}
