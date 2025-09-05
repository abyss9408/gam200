/******************************************************************************/
/*!
\file   TestInspector.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Inspector functions for Team_Popplio engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;

namespace BassNKick
{
	public class TestInspector : PopplioMonoScript
	{
		[Inspector("TestBool")]
		public bool testBool = true;

		[Inspector("TestInt")]
		public int testInt = 3;

		[Inspector("TestFloat")]
		public float testFloat = 2.1f;

		[Inspector("TestDouble")]
		public double testDouble = 5.3;

		[Inspector("TestChar")]
		public char testChar = 'C';

		[Inspector("TestString")]
		public string testString = "Hello World";

		[Inspector("TestEntity")]
		public Entity testEntity = Entity.EntityField();

		[Inspector("TestPrefab")]
		public Prefab testPrefab = Prefab.GetPrefab();

		[Inspector("TestBoolArr")]
		public bool[] testBoolArr = new bool[] { true, false, false };

		[Inspector("TestIntArr")]
		public int[] testIntArr = { 1, 4, 3, 2 };

		[Inspector("TestFloatArr")]
		public float[] testFloatArr = { 2.1f, 3.4f, 5.6f };

		[Inspector("TestDoubleArr")]
		public double[] testDoubleArr = { 5.3, 2.3, 5.44432 };

		[Inspector("TestCharArr")]
		public char[] testCharArr = { 'A', 'H', 'L' };

		[Inspector("TestStringArr")]
		public string[] testStringArr = { "Hello World", "Frick", "Oof", "Dang" };

		[Inspector("TestEntityArr")]
		public Entity[] testEntityArr = { Entity.EntityField(), Entity.GetEntity(0), Entity.GetEntity(3) };

		[Inspector("TestBoolList")]
		public List<bool> testBoolList = new List<bool>();

		[Inspector("TestIntList")]
		public List<int> testIntList = new List<int>();

		[Inspector("TestFloatList")]
		public List<float> testFloatList = new List<float>();

		[Inspector("TestDoubleList")]
		public List<double> testDoubleList = new List<double>();

		[Inspector("TestCharList")]
		public List<char> testCharList = new List<char>();

		[Inspector("TestStringList")]
		public List<string> testStringList = new List<string>();

		[Inspector("TestEntityList")]
		public List<Entity> testEntityList = new List<Entity>();

		public override void Setup()
		{
			Logger.Info("TestInspector.cs: Setup");
			Logger.Info(testBool.ToString());
			Logger.Info(testInt.ToString());
			Logger.Info(testFloat.ToString());
			Logger.Info(testDouble.ToString());
			Logger.Info(testChar.ToString());
			Logger.Info(testString);
			//int temp = testEntity.GetId();
			//Logger.Info(temp.ToString());
			Logger.Info(testEntity.ToString());
			//Logger.Info(testEntity.GetId().ToString());
			Logger.Info(testPrefab.Name);

			Logger.Info(testBoolArr[0].ToString());
			Logger.Info(testIntArr[1].ToString());
			Logger.Info(testFloatArr[0].ToString());
			Logger.Info(testDoubleArr[1].ToString());
			Logger.Info(testCharArr[0].ToString());
			Logger.Info(testStringArr[1].ToString());
			Logger.Info(testEntityArr[1].ToString());

			if (testBoolList.Count > 0) Logger.Info(testBoolList[0].ToString());
			if (testIntList.Count > 0) Logger.Info(testIntList[0].ToString());
			if (testFloatList.Count > 0) Logger.Info(testFloatList[0].ToString());
			if (testDoubleList.Count > 0) Logger.Info(testDoubleList[0].ToString());
			if (testCharList.Count > 0) Logger.Info(testCharList[0].ToString());
			if (testStringList.Count > 0) Logger.Info(testStringList[0]);
			if (testEntityList.Count > 0) Logger.Info(testEntityList[0].ToString());
		}

		public override void Init()
		{
			Logger.Info("TestInspector.cs: Entity Inspector");
		}

		public override void Update()
		{

		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
