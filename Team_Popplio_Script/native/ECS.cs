/******************************************************************************/
/*!
\file   ECS.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the C# API for the ECS library in C++

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Collections.Concurrent;

using Team_Popplio;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Mono.Debugger.Soft;
using static Mono.Security.X509.X520;

namespace Team_Popplio.ECS
{
	public class ECS
	{

	}

	public class Entity : ECS, IDisposable
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

		#region C++ Interface

		private static void ResetEntities() // called by C++ code to reset entities when loading new scene
		{
			ConcurrentDictionary<int, Entity> existingEntities = 
				new ConcurrentDictionary<int, Entity>();

			foreach (var pair in Entities)
			{
				if (Exists(pair.Key)) existingEntities.TryAdd(pair.Key, pair.Value);
			}

			Entities.Clear();
			Entities = existingEntities;

			Logger.EngineInfo("C# | Reset entities");
		}

		private static void AddEntity(int id) // called by C++ code to add non-script-attached entities
		{
			if (!Entities.ContainsKey(id)) new Entity(id);
				//if (!Entities.TryAdd(id, new Entity(id)))
				//	Logger.EngineError($"C# | Team_Popplio.ECS::Entity | Failed to add entity {id}");
		}

		private static void RemoveEntity(int id) // called by C++ code to add non-script-attached entities
		{
			if (Entities.ContainsKey(id)) 
				if (!Entities.TryRemove(id, out Entity e))
					Logger.EngineError($"C# | Team_Popplio.ECS::Entity | Failed to remove entity {id}");
		}

		//private static Entity InitEntity(int id) // used by C++ code to initialize entities at start (setup)
		//{
		//	AddEntity(id);
		//	return GetEntity(id);
		//}

		//public static void SetEntityField(int id, Entity entity)
		//{
		//	entity = new Entity(id);
		//}

		private static void SetEntityId(int id, Entity entity)
		{
			if (entity == null) entity = new Entity();
			entity.SetEntity(id);
		}

		private static Entity[] AddEntitiesToArr(int[] id)
		{
			Entity[] arr = new Entity[id.Length];
			for (int i = 0; i < id.Length; i++)
			{
				arr[i] = GetEntity(id[i]);
			}
			return arr;
		}

		#endregion

		#region Engine

		public static void AddEntityToDict(int id)
		{
			AddEntity(id);
		}

		public static void RemoveEntityFromDict(int id)
		{
			RemoveEntity(id);
		}

		#endregion

		#region Static Variables / Methods

		// use concurrentdictionary as it is thread-safe, and C# script initialization is multi-threaded,
		// so it crashes if the script initializes multiple same entries at initialization
		private static ConcurrentDictionary<int, Entity> Entities = new ConcurrentDictionary<int, Entity>();

		public static EntityRef EntityField()
		{
			return new EntityRef();
		}

		public static Entity GetNullEntity()
		{
			if (!Entities.ContainsKey(-1)) AddEntity(-1);
			return Entities[-1];
		}

		public static Entity GetEntity(int id)
		{
			if (!Exists(id))
			{
				if (!Entities.ContainsKey(-1)) AddEntity(-1);
				return Entities[-1];
			}
			
			if (!Entities.ContainsKey(id)) AddEntity(id);

			return Entities[id];
		}

		public static uint GetEntityCount()
		{
			return (uint)GetEntCount(); // Entities.Count;
		}

		public static Entity Instantiate(Entity? entity = null)
		{
			if (entity == null)
			{
				int newEnt = InstEnt(-1);
				return GetEntity(newEnt);
			}
			else
			{
				int newEnt = InstEnt(entity.GetId());
				return GetEntity(newEnt);
			}
		}

		public static void Destroy(Entity entity)
		{
			//entity.Kill();
			DelEnt(entity.GetId());
		}

		public static ConcurrentDictionary<int, Entity> GetEntities()
		{
			return Entities;
		}

		public static int GetEntityId(Entity e)
		{
			return e.GetId();
		}

		public static void EndEntities(ConcurrentDictionary<int, Entity> ent)
		{
			foreach (var pair in ent)
			{
				pair.Value.End();
			}
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int InstEnt(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void DelEnt(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetEntCount();

		#endregion

		#region ECS Entity
		//private IntPtr NativeInst; // pointer to native instance
		private int _NativeID = -1;

		private int NativeID // ID of the entity
		{
			get
			{
				//if (this != Entities[_NativeID])
				//{
				//	foreach (KeyValuePair<int, Entity> pair in Entities)
				//	{
				//		if (pair.Value == this)
				//		{
				//			_NativeID = pair.Key;
				//			return _NativeID;
				//		}
				//	}
				//	//if (_NativeID < 0)
				//	//{
				//	//	this.
				//	//}

				//	//AddEntityToDict(_NativeID);
				//}
				return _NativeID;
			}
			//set
			//{
			//	//if (Entities.ContainsKey(value)) Entities.TryRemove(_NativeID);
			//	if (_NativeID < 0) Logger.Warning(value.ToString());
			//	_NativeID = value;
			//	//Entities.TryAdd(value, this);
			//	if (!Entities.ContainsKey(value)) Entities.TryAdd(value, this);
			//}
		}

		private void SetNativeID(int id)
		{
			_NativeID = id;
			//Entities.TryAdd(value, this);
			if (!Entities.ContainsKey(id)) Entities.TryAdd(id, this);
		}

		public Entity() 
		{
			//Logger.EngineDebug("C# | Created invalid entity [-1]");
			//AddEntity(-1); // add invalid entity to dictionary

			Mono.AddGC(this);
		}

		public Entity(int NativeInst)
		{
			//if (!Exists(NativeInst)) Logger.EngineDebug($"C# | Created non-existent entity {NativeInst}");

			SetNativeID(NativeInst);
			Mono.AddGC(this);
		}

		~Entity()
		{
			//if (Entities.ContainsKey(NativeID)) Entities.TryRemove(NativeID);
			//if (Exists()) OnScriptDestroy(NativeID);
			Dispose(false);
			Mono.RemoveGC(this);
		}

		public void SetEntity(int NativeInst)
		{
			SetNativeID(NativeInst);
		}

		//public Entity GetEntity()
		//{
		//	return this;
		//}

		public override string ToString()
		{
			return $"Entity: {NativeID} | {Name()}";
		}

		public void Kill()
		{
			Kill(NativeID);
		}

		public int GetId()
		{
			return NativeID;
		}

		public string Name()
		{
			if (NativeID < 0 || !Exists(NativeID)) return "[Invalid Entity]";
			return GetTag(NativeID);
		}

		public void Tag(string tag)
		{
			Tag(NativeID, tag);
		}

		public bool HasTag(string tag)
		{
			return HasTag(NativeID, tag);
		}

		public void Group(string group)
		{
			Group(NativeID, group);
		}

		public bool BelongsToGroup(string group)
		{
			if (NativeID < 0 || !Exists(NativeID))
			{
				//Logger.EngineError(NativeID.ToString());
				return false;
			}
			return BelongsToGroup(NativeID, group);
		}

		public bool Exists()
		{
			return Exists(NativeID);
		}

		public static bool Exists(Entity e)
		{
			return e.Exists();
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Kill(int instance);
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//private extern int GetId(IntPtr instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetTag(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Tag(int instance, string tag);
        [MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasTag(int instance, string tag);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Group(int instance, string group);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool BelongsToGroup(int instance, string group);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool Exists(int instance);
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//private static extern void OnScriptDestroy(int instance);
		#endregion

		// Components // -----------------------------------------------------

		#region Active
		public bool Active
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int entity, bool value);
		#endregion

		#region Transform
		private Components.Transform2D? _Transform2D;

		public Components.Transform2D Transform2D
		{
			get
			{
				//if (!HasComponent<Components.Transform2D>())
				//	throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Transform Component");

				_Transform2D ??= new Components.Transform2D(NativeID); // [??=] is compound assignment operator?

				return _Transform2D;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Transform Component via C# Script!");
			}
		}
		#endregion

		#region Render
		private Components.Render2D? _Render;

		protected Components.Render2D Render
		{
			get
			{
				if (!HasComponent<Components.Render2D>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Render Component");

				_Render ??= new Components.Render2D(NativeID); // [??=] is compound assignment operator?

				return _Render;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Render Component via C# Script!");
			}
		}
		#endregion

		#region RigidBody
		private Components.RigidBody2D? _RigidBody2D;

		protected Components.RigidBody2D RigidBody2D
		{
			get
			{
				if (!HasComponent<Components.RigidBody2D>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent RigidBody Component");
				_RigidBody2D ??= new Components.RigidBody2D(NativeID); // [??=] is compound assignment operator?
				return _RigidBody2D;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set RigidBody Component via C# Script!");
			}
		}
		#endregion

		#region BoxCollider
		private Components.BoxCollider2D? _BoxCollider2D;

		protected Components.BoxCollider2D BoxCollider2D
		{
			get
			{
				if (!HasComponent<Components.BoxCollider2D>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent BoxCollider Component");
				_BoxCollider2D ??= new Components.BoxCollider2D(NativeID); // [??=] is compound assignment operator?
				return _BoxCollider2D;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set BoxCollider Component via C# Script!");
			}
		}
		#endregion

		#region Text
		private Components.Text2D? _Text2D;

		protected Components.Text2D Text2D
		{
			get
			{
				if (!HasComponent<Components.Text2D>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Text Component");
				_Text2D ??= new Components.Text2D(NativeID); // [??=] is compound assignment operator?
				return _Text2D;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Text Component via C# Script!");
			}
		}
		#endregion

		#region Logic
		private Components.Logic? _Logic;

		protected Components.Logic Logic
		{
			get
			{
				if (!HasComponent<Components.Logic>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Logic Component");
				_Logic ??= new Components.Logic(NativeID); // [??=] is compound assignment operator?
				return _Logic;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Logic Component via C# Script!");
			}
		}
		#endregion

		#region Animation
		private Components.Animation2D? _Anim;

		protected Components.Animation2D Anim
		{
			get
			{
				if (!HasComponent<Components.Animation2D>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Animation Component");

				_Anim ??= new Components.Animation2D(NativeID); // [??=] is compound assignment operator?

				return _Anim;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Animation Component via C# Script!");
			}
		}
		#endregion

		#region Audio
		private Components.Audio? _Audio;

		protected Components.Audio Audio
		{
			get
			{
				if (!HasComponent<Components.Audio>())
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Audio Component");

				_Audio ??= new Components.Audio(NativeID); // [??=] is compound assignment operator?

				return _Audio;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Audio Component via C# Script!");
			}
		}
		#endregion

		#region Hierarchy
		private Components.Hierarchy? _Hierarchy;

		public Components.Hierarchy Hierarchy
		{
			get
			{
				if (!HasComponent<Hierarchy>())
				{
					throw new Exception("Team_Popplio.ECS::Entity | Non-Existent Hierarchy Component");
				}
					//AddParentComponent(NativeID);

				_Hierarchy ??= new Components.Hierarchy(NativeID); // [??=] is compound assignment operator?

				return _Hierarchy;
			}
			set
			{
				throw new Exception("Team_Popplio.ECS::Entity | Cannot set Hierarchy Component via C# Script!");
			}
		}
		#endregion

		#region Persistent
		public bool Persistent
		{
			get { return GetPersist(NativeID); }
			set { SetPersist(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetPersist(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPersist(int entity, bool value);
        #endregion

        #region UI
        private Components.UI? _UI;

        public Components.UI UI
        {
            get
            {
                if (!HasComponent<Components.UI>())
                    throw new Exception("Team_Popplio.ECS::Entity | Non-Existent UI Component");

                _UI ??= new Components.UI(NativeID);
                return _UI;
            }
            set
            {
                throw new Exception("Team_Popplio.ECS::Entity | Cannot set UI Component via C# Script!");
            }
        }
        #endregion


        #region Generics
        public T GetComponent<T>() where T : Component
		{
			Type type = typeof(T);

			// Check if the component exists
			if (!HasComponent<T>())
				throw new Exception($"Team_Popplio.ECS::Entity.GetComponent | Non-Existent {type.Name} Component");

			// Dynamically resolve the component type

			if (type == typeof(Components.Render2D))
				return Render as T ?? throw new Exception($"Failed to cast Render to {typeof(T).Name}");

			if (type == typeof(Components.RigidBody2D))
				return RigidBody2D as T ?? throw new Exception($"Failed to cast RigidBody2D to {typeof(T).Name}");

			if (type == typeof(Components.BoxCollider2D))
				return BoxCollider2D as T ?? throw new Exception($"Failed to cast BoxCollider2D to {typeof(T).Name}");

			if (type == typeof(Components.Text2D))
				return Text2D as T ?? throw new Exception($"Failed to cast Text2D to {typeof(T).Name}");

			if (type == typeof(Components.Logic))
				return Logic as T ?? throw new Exception($"Failed to cast Logic to {typeof(T).Name}");

			if (type == typeof(Components.Animation2D))
				return Anim as T ?? throw new Exception($"Failed to cast Animation to {typeof(T).Name}");

			if (type == typeof(Components.Audio))
			{
				return Audio as T ?? throw new Exception($"Failed to cast Audio to {typeof(T).Name}");
			}

            if (type == typeof(Components.UI))
                return UI as T ?? throw new Exception($"Failed to cast UI to {typeof(T).Name}");

            throw new Exception($"Team_Popplio.ECS::Entity.GetComponent | Unsupported Component Type: {type.Name}");
		}


		public bool HasComponent<T>() where T : Component
		{
			Type type = typeof(T);

			// Dynamically resolve the native method for the component type

			if (type == typeof(Components.Render2D))
				return HasRenderComponent(NativeID);

			if (type == typeof(Components.RigidBody2D))
				return HasRigidBodyComponent(NativeID);

			if (type == typeof(Components.BoxCollider2D))
				return HasBoxColliderComponent(NativeID);

			if (type == typeof(Components.Text2D))
				return HasTextComponent(NativeID);

			if (type == typeof(Components.Logic))
				return HasLogicComponent(NativeID);

			if (type == typeof(Components.Animation2D))
				return HasAnimationComponent(NativeID);

			if (type == typeof(Components.Audio))
				return HasAudioComponent(NativeID);

			if (type == typeof(Components.Hierarchy))
				return HasParentComponent(NativeID);

            if (type == typeof(Components.UI))
                return HasUIComponent(NativeID);

            throw new Exception($"Team_Popplio.ECS::Entity.HasComponent | Unsupported Component Type: {type.Name}");
		}

		public void AddComponent<T>() where T : Component
		{
			Type type = typeof(T);

			// Check if the component exists
			if (HasComponent<T>())
				throw new Exception($"Team_Popplio.ECS::Entity.AddComponent | {type.Name} Component already exists");

			// Dynamically resolve the component type

			if (type == typeof(Components.Render2D))
				AddRenderComponent(NativeID);

			else if (type == typeof(Components.RigidBody2D))
				AddRigidBodyComponent(NativeID);

			else if (type == typeof(Components.BoxCollider2D))
				AddBoxColliderComponent(NativeID);

			else if (type == typeof(Components.Text2D))
				AddTextComponent(NativeID);

			else if (type == typeof(Components.Logic))
				AddLogicComponent(NativeID);

			else if (type == typeof(Components.Animation2D))
				AddAnimationComponent(NativeID);

			else if (type == typeof(Components.Audio))
				AddAudioComponent(NativeID);

			else if (type == typeof(Components.Hierarchy))
				AddParentComponent(NativeID);

			else if (type == typeof(Components.UI))
                AddUIComponent(NativeID);

            else throw new Exception($"Team_Popplio.ECS::Entity.AddComponent | Unsupported Component Type: {type.Name}");
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasRenderComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasRigidBodyComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasBoxColliderComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasTextComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasLogicComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasAnimationComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasAudioComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasParentComponent(int entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasUIComponent(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddRenderComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddRigidBodyComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddBoxColliderComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddTextComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddLogicComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddAnimationComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddAudioComponent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddParentComponent(int entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AddUIComponent(int entity);
        #endregion

		// call at scene end
		public void End()
		{
			Transform2D.End();

			if (HasComponent<Render2D>())
			{
				GetComponent<Render2D>().End();
			}
			if (HasComponent<RigidBody2D>())
			{
				GetComponent<RigidBody2D>().End();
			}
			if (HasComponent<BoxCollider2D>())
			{
				GetComponent<BoxCollider2D>().End();
			}
			if (HasComponent<Text2D>())
			{
				GetComponent<Text2D>().End();
			}
			if (HasComponent<Logic>())
			{
				GetComponent<Logic>().End();
			}
			if (HasComponent<Animation2D>())
			{
				GetComponent<Animation2D>().End();
			}
			if (HasComponent<Audio>())
			{
				GetComponent<Audio>().End();
			}
			if (HasComponent<Hierarchy>())
			{
				Hierarchy.End();
			}
			if (HasComponent<UI>())
			{
				GetComponent<UI>().End();
			}
		}

        // Scripts // -----------------------------------------------------

        #region Scripts
        public PopplioMonoScript GetScript()
		{
			//if (!HasScript())
			//	throw new Exception("Team_Popplio.ECS::Entity.GetScript | Non-Existent Script");

			try
			{
				if (!HasScript())
					throw new Exception("Team_Popplio.ECS::Entity.GetScript | Non-Existent Script");

				return Logic.Script as PopplioMonoScript ?? 
					throw new Exception($"Failed to cast Logic to {typeof(PopplioMonoScript).Name}");
			}
			catch (Exception e)
			{
				Logger.EngineError($"Team_Popplio.ECS::Entity.GetScript | {e.Message}");
				throw new Exception($"Team_Popplio.ECS::Entity.GetScript | {e.Message}");
				//throw new Exception($"Team_Popplio.ECS::Entity.GetScript | {e.Message}");
			}
		}

		public bool HasScript()
		{
			try
			{
				if (!HasComponent<Logic>()) return false;
				return GetComponent<Logic>().Script?.GetType().IsSubclassOf(typeof(PopplioMonoScript)) ??
					 throw new Exception("Team_Popplio.ECS::Entity.HasScript | null boolean");
			}
			catch (Exception e)
			{
				Logger.EngineError($"Team_Popplio.ECS::Entity.HasScript | {e.Message}");
				throw new Exception($"Team_Popplio.ECS::Entity.HasScript | {e.Message}");
				//throw new Exception($"Team_Popplio.ECS::Entity.HasScript | {e.Message}");
			}
		}
		#endregion

		#region Generics
		public T GetScript<T>() where T : PopplioMonoScript
		{
			//foreach (var script in PopplioMonoScript.Scripts)
			//{
			//	Logger.Critical(script.Key.ToString());
			//	Logger.Critical(script.Value.ToString());
			//}

			Type type = typeof(T);

			// Check if the script exists
			if (!HasScript<T>())
				throw new Exception($"Team_Popplio.ECS::Entity.GetScript | Non-Existent {type.Name} Script");

			// Dynamically resolve the script type

			try
			{
				return GetComponent<Logic>().Script as T ??
				//return PopplioMonoScript.Scripts[NativeID] as T ?? 
					throw new Exception($"Failed to cast Logic to {typeof(T).Name}");
			}
			catch //(Exception e)
			{
				try
				{
					return PopplioMonoScript.Scripts[NativeID] as T ??
					//return GetComponent<Logic>().Script as T ??
						throw new Exception($"Failed to cast Logic to {typeof(T).Name}");
				}
				catch (Exception e2)
				{
					throw new Exception($"Team_Popplio.ECS::Entity.GetScript<T> | {e2.Message}");
				}
				//throw new Exception($"Team_Popplio.ECS::Entity.GetScript<T> | {e.Message}");
			}

			throw new Exception($"Team_Popplio.ECS::Entity.GetScript<T> | Unsupported Script Type: {type.Name}");
		}

		public bool HasScript<T>() where T : PopplioMonoScript
		{
			Type type = typeof(T);

			// Dynamically resolve the native method for the script type

			if (!HasComponent<Logic>())
			{
				Logger.EngineError("Team_Popplio.ECS::Entity.HasScript<T> | Logic Component does not exist");
				return false;
			}

			try
			{
				if (!type.IsSubclassOf(typeof(PopplioMonoScript)))
				{
					Logger.EngineError("Team_Popplio.ECS::Entity.HasScript<T> | Type is not a script");
					return false;
				}
				//Logger.Warning(this.GetComponent<Logic>().Script.ToString());
				if (PopplioMonoScript.Scripts.ContainsKey(NativeID)) return true;
			}
			catch (Exception e)
			{
				throw new Exception($"Team_Popplio.ECS::Entity.HasScript<T> | {e.Message}");
			}

			Logger.EngineError("Team_Popplio.ECS::Entity.HasScript<T> | Script is invalid or does not exist");
			return false;
			//throw new Exception($"Team_Popplio.ECS::Entity.HasScript<T> | Unsupported Script Type: {type.Name}");
		}
		#endregion

		// ??? // ++++++++++++++++++
	}

	public class EntityRef : Entity
	{

	}

	public class Prefab : ECS, IDisposable
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

		private static ConcurrentDictionary<string, Prefab> Prefabs = new ConcurrentDictionary<string, Prefab>();

		private static void AddPrefab(string id)
		{
			if (!Prefabs.ContainsKey(id)) Prefabs.TryAdd(id, new Prefab(id));
		}

		public static Prefab GetPrefab()
		{
			if (!Prefabs.ContainsKey("")) AddPrefab("");
			return Prefabs[""];
		}

		public static Prefab GetPrefab(string id)
		{
			if (!HasPrefab(id))
			{
				if (!Prefabs.ContainsKey("")) AddPrefab("");
				return Prefabs[""];
			}

			if (!Prefabs.ContainsKey(id)) AddPrefab(id);

			return Prefabs[id];
		}

		private string _NameID = "";

		public string Name // ID of the prefab
		{
			get => _NameID;
			set
			{
				if (!HasPrefab(value))
				{
					Logger.EngineDebug($"C# | Team_Popplio.ECS::Prefab | Non-Existent Prefab: {value}");
					return;
				}

				_NameID = value;
			}
		}

		public static PrefabRef PrefabField()
		{
			return new PrefabRef();
		}

		protected Prefab()
		{
			//Logger.EngineDebug($"C# | Team_Popplio.ECS::Prefab | Constructed Non-Existent Prefab");
		}

		public Prefab(string name)
		{
			if (!HasPrefab(name))
			{
				Logger.EngineDebug($"C# | Team_Popplio.ECS::Prefab | Non-Existent Prefab: {name}");
				Dispose();
				return;
			}
			Name = name;
		}

		public static Entity Instantiate(Prefab prefab)
		{
			int newEnt = InstantiatePrefab(prefab.Name);
			return Entity.GetEntity(newEnt);
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasPrefab(string name);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int InstantiatePrefab(string name);
	}

	public class PrefabRef : Prefab
	{

	}
}
