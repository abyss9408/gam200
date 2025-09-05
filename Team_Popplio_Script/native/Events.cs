/******************************************************************************/
/*!
\file   Events.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the C# API for Events in C++

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

using Team_Popplio;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.ECS;

namespace Team_Popplio
{
	public static class Events
	{
		#region Events
		public static event Action<int, int, double>? OnCollision2DEnter;
		public static event Action<int, int, double>? OnCollision2DStay;
		public static event Action<int, int, double>? OnCollision2DExit;

		public static void OnCollision2D(int entity1, int entity2, double collisionTime, int action)
		{
			// Call the function
			switch (action)
			{
				case -1:
					OnCollision2DEnter?.Invoke(entity1, entity2, collisionTime);
					break;
				case 0:
					OnCollision2DStay?.Invoke(entity1, entity2, collisionTime);
					break;
				case 1:
					OnCollision2DExit?.Invoke(entity1, entity2, collisionTime);
					break;
			}
		}

		public static event Action<int, int, double>? OnTrigger2DEnter;
		public static event Action<int, int, double>? OnTrigger2DStay;
		public static event Action<int, int, double>? OnTrigger2DExit;

		public static void OnTrigger2D(int entity1, int entity2, double triggerTime, int action)
		{
			// Call the function
			switch (action)
			{
				case -1:
					OnTrigger2DEnter?.Invoke(entity1, entity2, triggerTime);
					break;
				case 0:
					OnTrigger2DStay?.Invoke(entity1, entity2, triggerTime);
					break;
				case 1:
					OnTrigger2DExit?.Invoke(entity1, entity2, triggerTime);
					break;
			}
		}

		public static event Action<int, int>? OnEntityChangeEvent;

		public static void OnEntityChange(int id, int change)
		{
			switch (change)
			{
				case 0: // Added
					Logger.EngineDebug($"Entity {id} added to dictionary");
					ECS.Entity.AddEntityToDict(id);
					break;
				case 1: // Removed
					Logger.EngineDebug($"Entity {id} removed from dictionary");
					ECS.Entity.RemoveEntityFromDict(id);
					break;
				case 2: // Modified
					
					break;
			}

			// Call the native function
			OnEntityChangeEvent?.Invoke(id, change);
		}

		public static event Action<int>? OnEntityCreateEvent;

		public static void OnEntityCreate(int entity)
		{
			// Call the native function
			try
			{
				//Entity.GetEntity(entity).GetScript().OnCreate();

				OnEntityCreateEvent?.Invoke(entity);
			}
			catch (Exception e)
			{
				Logger.EngineDebug($"C# | Failed to call OnCreate on entity {entity} | {e.Message}");
			}
		}

		public static event Action<int>? OnEntityDestroyEvent;

		public static void OnEntityDestroy(int entity)
		{
			// Call the native function
			try
			{
				//Entity.GetEntity(entity).GetScript().OnDestroy();

				OnEntityDestroyEvent?.Invoke(entity);
			}
			catch (Exception e)
			{
				Logger.EngineDebug($"C# | Failed to call OnDestroy on entity {entity} | {e.Message}");
			}
		}

		public static event Action<int>? OnEnableEvent;

		public static void OnEnable(int entity)
		{
			// Call the native function
			//Entity.GetEntity(entity).GetScript().OnEnable();

			OnEnableEvent?.Invoke(entity);
		}

		public static event Action<int>? OnDisableEvent;

		public static void OnDisable(int entity)
		{
			// Call the native function
			//Entity.GetEntity(entity).GetScript().OnDisable();

			OnDisableEvent?.Invoke(entity);
		}

		public static event Action? OnAppQuitEvent;

		public static void OnAppQuit()
		{
			// Call the native function
			OnAppQuitEvent?.Invoke();
		}
		#endregion
	}
}
