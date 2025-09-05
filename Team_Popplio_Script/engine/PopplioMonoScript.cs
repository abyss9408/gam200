/******************************************************************************/
/*!
\file   PopplioMonoScript.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the base class for Team_Popplio engine scripting API
		Base class for all C# scripts in the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using Microsoft.CodeAnalysis.Scripting;
using Mono.CSharp;
using System;
using System.Collections.Concurrent;
using System.Windows.Forms.Design;
using Team_Popplio;
using Team_Popplio.Components;

//[DllImport("Team_Popplio_EngineDLL.dll", CallingConvention = CallingConvention.Cdecl)]

namespace Team_Popplio
{
	/*
    *   base C# class for all scripts in the engine
    */
	public class PopplioMonoScript : IDisposable
	{
		#region IDisposable
		private bool _disposed = false;
		//private UnmanagedResource _resource;

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
			if (!_disposed)
			{
				if (disposing)
				{
					// Dispose managed resources
					//_resource?.Close();
				}

				// Dispose unmanaged resources
				// Free native resources if any

				_disposed = true;
			}
		}
		#endregion

		protected static ConcurrentDictionary<int, PopplioMonoScript> _Scripts =
			new ConcurrentDictionary<int, PopplioMonoScript>(); // set by C++ side

		public static ConcurrentDictionary<int, PopplioMonoScript> Scripts
		{
			get => _Scripts;
		}

		#region C++ interface
#pragma warning disable CS0649 // disable unused variable warning
#pragma warning disable CS0169

		private static void ResetScripts() // call when loading new scene
		{
			ConcurrentDictionary<int, PopplioMonoScript> existingScripts =
				new ConcurrentDictionary<int, PopplioMonoScript>();
			foreach (var script in _Scripts)
			{
				if (Entity.Exists(script.Key)) existingScripts.TryAdd(script.Key, script.Value);
			}
			_Scripts.Clear();
			_Scripts = existingScripts;

			Logger.EngineInfo("C# | Reset scripts");

			//foreach (var script in _Scripts)
			//{
			//	Logger.Critical(script.Key.ToString());
			//	Logger.Critical(script.Value.ToString());
			//}
		}

		private void SetScript() // called by C++
		{
			//EntP = data;
			//EntID = id;
			//Ent = ECS.Entity.GetEntity(EntID);
			//This.GetComponent<Logic>().Script = this;
			if (!_Scripts.ContainsKey(EntID)) _Scripts.TryAdd(EntID, this);
		}

		//internal Team_Popplio.Popplio.EntityRef e;

		private IntPtr EntP; // set by c++ code
		protected int EntID = -1;

#pragma warning restore CS0169
#pragma warning restore CS0649
		#endregion

		// Variables // -----------------------------------------------------

		private ECS.Entity? Ent; // = ECS.Entity.GetEntity(); //new ECS.Entity();

		// Properties // -----------------------------------------------------

		public ECS.Entity This // TODO FIX // only returns 0 for now (probably not properly pointing to correct entity)
		{
			get => Entity.GetEntity(EntID);
			private set
			{
				//Ent = Entity.GetEntity(EntID);
				//Ent.SetEntity(EntID);
				Ent = Entity.GetEntity(EntID);
				_Scripts.TryAdd(EntID, this);
			}
			//private set => Ent.SetEntity(EntID); // set by c++ code (MonoAPI.cpp)
		}
		//public int entityID
		//{
		//	get => PopplioScriptAPI.MonoNative.GetTypeData(pEntity).Id; // call MonoNative.cpp
		//}

		// Functions // -----------------------------------------------------

		//public void SetEntity(Team_Popplio.Popplio.Entity ent) { e = ent; }

		public PopplioMonoScript()
		{
			Events.OnCollision2DEnter += _OnCollision2DEnter;
			Events.OnCollision2DStay += _OnCollision2DStay;
			Events.OnCollision2DExit += _OnCollision2DExit;
			Events.OnTrigger2DEnter += _OnTrigger2DEnter;
			Events.OnTrigger2DStay += _OnTrigger2DStay;
			Events.OnTrigger2DExit += _OnTrigger2DExit;
			Events.OnAppQuitEvent += OnAppQuit;
			Events.OnDisableEvent += _OnDisable;
			Events.OnEnableEvent += _OnEnable;
			Events.OnEntityCreateEvent += _OnCreate;
			Events.OnEntityDestroyEvent += _OnDestroy;
		}

		~PopplioMonoScript()
		{
			_Scripts.TryRemove(EntID, out _);
			Dispose();
		}

		static PopplioMonoScript nullInstance = new PopplioMonoScript();

		public static PopplioMonoScript GetScript()
		{
			return nullInstance;
		}

		#region Loop
		/*
		*   Setup() - called once when the scene is loaded
		*/
		virtual public void Setup()			{ }
		/*
		*   Setup() - called once when the scene is loaded after Setup()
		*/
		virtual public void Init()			{ }
		/*
		*   Update() - called every frame
		*/
		virtual public void Update()		{ }
		/*
		*   Update() - called every fixed frame
		*/
		virtual public void FixedUpdate()	{ }
		/*
		*   End() - called on scene exit
		*/
		virtual public void End()			{ }
		#endregion

		#region Events
		private void _OnCollision2DEnter(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnCollision2DEnter(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnCollision2DEnter(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnCollision2DEnter(ECS.Entity other, double time) { }

		private void _OnCollision2DStay(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnCollision2DStay(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnCollision2DStay(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnCollision2DStay(ECS.Entity other, double time) { }

		private void _OnCollision2DExit(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnCollision2DExit(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnCollision2DExit(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnCollision2DExit(ECS.Entity other, double time) { }

		private void _OnTrigger2DEnter(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnTrigger2DEnter(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnTrigger2DEnter(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnTrigger2DEnter(ECS.Entity other, double time) { }

		private void _OnTrigger2DStay(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnTrigger2DStay(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnTrigger2DStay(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnTrigger2DStay(ECS.Entity other, double time) { }

		private void _OnTrigger2DExit(int e1, int e2, double t)
		{
			if (EntID == e1)
				OnTrigger2DExit(ECS.Entity.GetEntity(e2), t);
			else if (EntID == e2)
				OnTrigger2DExit(ECS.Entity.GetEntity(e1), t);
		}

		virtual public void OnTrigger2DExit(ECS.Entity other, double time) { }

		private void _OnDisable(int id)
		{
			if (EntID == id)
				OnDisable();
		}

		private void _OnEnable(int id)
		{
			if (EntID == id)
				OnEnable();
		}

		private void _OnCreate(int id)
		{
			if (EntID == id)
				try { OnCreate(); }
				catch (Exception e)
				{
					Logger.EngineError($"C# | Failed to call OnCreate() | {e.Message}");
				}
		}

		private void _OnDestroy(int id)
		{
			if (EntID == id)
				try { OnDestroy(); }
				catch (Exception e)
				{
					Logger.EngineError($"C# | Failed to call OnDestroy() | {e.Message}");
				}
		}

		virtual public void OnCreate() { }

		virtual public void OnDestroy() { }

		virtual public void OnEnable() { }

		virtual public void OnDisable() { }

		virtual public void OnAppQuit() { }
		#endregion
	}
}
