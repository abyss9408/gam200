/******************************************************************************/
/*!
\file   Utility.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing utilities for the engine's C# code

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Reflection;
using System.Collections.Generic;
using Mono.Linker.Steps;
using System.Threading.Tasks;

namespace Team_Popplio
{
	public static class Utility
	{
		public static void CopyAllExcept<T>(T source, T destination, List<string> excludedMemberName)
		{
			if (source == null || destination == null)
				throw new ArgumentNullException("Source and destination objects cannot be null.");

			// properties
			var properties = typeof(T).GetProperties(BindingFlags.Public | BindingFlags.Instance);

			foreach (var property in properties)
			{
				if (excludedMemberName.Contains(property.Name) || !property.CanWrite)
					continue;

				var value = property.GetValue(source);
				property.SetValue(destination, value);
			}

			// fields
			var fields = typeof(T).GetFields(BindingFlags.Public | BindingFlags.Instance | BindingFlags.NonPublic);
			foreach (var field in fields)
			{
				if (excludedMemberName.Contains(field.Name))
					continue;

				var value = field.GetValue(source);
				field.SetValue(destination, value);
			}
		}

		public static void CopyAllExceptProperties<T>(T source, T destination, List<string> excludedPropertyName)
		{
			if (source == null || destination == null)
				throw new ArgumentNullException("Source and destination objects cannot be null.");

			// properties
			var properties = typeof(T).GetProperties(BindingFlags.Public | BindingFlags.Instance);

			foreach (var property in properties)
			{
				if (excludedPropertyName.Contains(property.Name) || !property.CanWrite)
					continue;

				var value = property.GetValue(source);
				property.SetValue(destination, value);
			}
		}
	}

	public static class StaticCoroutine
	{
		private static readonly Queue<Func<Task>> funcs = new Queue<Func<Task>>();

		public static void AddStep(Func<Task> func)
		{
			funcs.Enqueue(func);
		}

		public static async Task Run()
		{
			while (funcs.Count > 0)
			{
				var step = funcs.Dequeue();
				await step();
			}
		}
	}

	public class Coroutine
	{
		private readonly Queue<Func<Task>> funcs = new Queue<Func<Task>>();

		public void AddStep(Func<Task> func)
		{
			funcs.Enqueue(func);
		}

		public async Task Run()
		{
			while (funcs.Count > 0)
			{
				var step = funcs.Dequeue();
				await step();
			}
		}
	}
}
