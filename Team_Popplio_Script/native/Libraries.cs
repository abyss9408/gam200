/******************************************************************************/
/*!
\file   Libraries.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the C# API for libraries in C++

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Concurrent;
using System.Numerics;
using System.Runtime.CompilerServices;
using Microsoft.CSharp;
using System.Collections.Generic;

using Team_Popplio.Extern;

namespace Team_Popplio.Libraries
{
	#region Logger | Logging/Logger.h
	public static class Logger //: IDisposable
	{
		//private bool _disposed = false;
		////private UnmanagedResource _resource;

		//public void Dispose()
		//{
		//	Dispose(true);
		//	GC.SuppressFinalize(this);
		//}

		//protected virtual void Dispose(bool disposing)
		//{
		//	if (!_disposed)
		//	{
		//		if (disposing)
		//		{
		//			// Dispose managed resources
		//			//_resource?.Close();
		//		}

		//		// Dispose unmanaged resources
		//		// Free native resources if any

		//		_disposed = true;
		//	}
		//}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Critical(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Warning(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Debug(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Error(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Info(string message);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EngineCritical(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EngineWarning(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EngineDebug(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EngineError(string message);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EngineInfo(string message);
	}
	#endregion

	#region Time | Utilities/Timer.h
	public static class Time //: IDisposable
	{
		//private bool _disposed = false;
		////private UnmanagedResource _resource;

		//public void Dispose()
		//{
		//	Dispose(true);
		//	GC.SuppressFinalize(this);
		//}

		//protected virtual void Dispose(bool disposing)
		//{
		//	if (!_disposed)
		//	{
		//		if (disposing)
		//		{
		//			// Dispose managed resources
		//			//_resource?.Close();
		//		}

		//		// Dispose unmanaged resources
		//		// Free native resources if any

		//		_disposed = true;
		//	}
		//}

		public static double DeltaTime
		{
			get { return GetDeltaTime(); }
		}

		public static double FixedDeltaTime
		{
			get { return GetFixedDeltaTime(); }
		}

		public static double ElapsedTime
		{
			get { return GetTime(); }
		}

		public static float DeltaTimeF
		{
			get { return (float)GetDeltaTime(); }
		}

		public static float FixedDeltaTimeF
		{
			get { return (float)GetFixedDeltaTime(); }
		}

		public static float ElapsedTimeF
		{
			get { return (float)GetTime(); }
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double GetDeltaTime();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double GetFixedDeltaTime();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double GetTime();

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void SetFixedDeltaTime(double value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ResetFixedDeltaTime();
	}
	#endregion

	#region Rand | Utilities/Rand.h
	public static class Rand
	{
		public static void SetSeed(uint seed) => SetRand(seed);
		public static int Gen() => GetRand();
		public static int Gen(int min, int max) => RandInt(min, max);
		public static float Gen(float min, float max) => RandFloat(min, max);

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetRand(uint seed);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetRand();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int RandInt(int min, int max);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float RandFloat(float min, float max);
	}
	#endregion

	#region Animation | Animation/AnimationSystem.h
	public static class Animation
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void PlayAll();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void PauseAll();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void StopAll();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ResumeAll();
	}
	#endregion

	#region Input | Input/InputSystem.h
	public static class Input //: IDisposable
	{
		//private bool _disposed = false;
		////private UnmanagedResource _resource;

		//public void Dispose()
		//{
		//	Dispose(true);
		//	GC.SuppressFinalize(this);
		//}

		//protected virtual void Dispose(bool disposing)
		//{
		//	if (!_disposed)
		//	{
		//		if (disposing)
		//		{
		//			// Dispose managed resources
		//			//_resource?.Close();
		//		}

		//		// Dispose unmanaged resources
		//		// Free native resources if any

		//		_disposed = true;
		//	}
		//}

		// wrappers // -----------------------------------------------------

		public enum KeyState
		{
			None,
			Triggered,  // Just pressed this frame
			Down,       // Held down
			Released    // Just released this frame
		};

		public enum MouseButton
		{
			Left = GLFW.MOUSE_LEFT,
			Right = GLFW.MOUSE_RIGHT,
			Middle = GLFW.MOUSE_MIDDLE
		};

		// Keyboard Input Methods
		public static bool IsKeyTriggered(GLFW key) => KeyTriggered((int)key);
		public static bool IsKeyDown(GLFW key) => KeyDown((int)key);
		public static bool IsKeyReleased(GLFW key) => KeyReleased((int)key);

		static List<GLFW> keysT = new List<GLFW>();
		static List<GLFW> keysD = new List<GLFW>();
		static List<GLFW> keysR = new List<GLFW>();

		public static List<GLFW> GetKeysTriggered()
		{
			int[] keys = KeysTriggered();
			keysT.Clear();
			for (int i = 0; i < keys.Length; i++)
			{
				keysT.Add((GLFW)keys[i]);
			}
			return keysT;
		}
		public static List<GLFW> GetKeysDown()
		{
			int[] keys = KeysDown();
			keysD.Clear();
			for (int i = 0; i < keys.Length; i++)
			{
				keysD.Add((GLFW)keys[i]);
			}
			return keysD;
		}
		public static List<GLFW> GetKeysReleased()
		{
			int[] keys = KeysReleased();
			keysR.Clear();
			for (int i = 0; i < keys.Length; i++)
			{
				keysR.Add((GLFW)keys[i]);
			}
			return keysR;
		}

		// Mouse Input Methods
		public static bool IsMouseButtonTriggered(MouseButton button) => MouseButtonTriggered((int)button);
		public static bool IsMouseButtonDown(MouseButton button) => MouseButtonDown((int)button);
		public static bool IsMouseButtonReleased(MouseButton button) => MouseButtonReleased((int)button);

		// Notes: might incur overhead due to new VVVVV

		// Mouse Position Methods
		public static Vector2<float> GetMouse() => new Vector2<float>(MouseX(), MouseY());
		public static Vector2<float> GetMouseDelta() => new Vector2<float>(MouseDeltaX(), MouseDeltaY());
		public static Vector2<float> GetMousePrevious() => new Vector2<float>(MousePrevX(), MousePrevY());

		// Mouse Scroll Methods
		public static Vector2<float> GetMouseScroll() => new Vector2<float>(MouseScrollX(), MouseScrollY());

		// Frame Timing Methods
		public static double GetInputLatency(GLFW key) => InputLatency((int)key);

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool KeyTriggered(int key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool KeyDown(int key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool KeyReleased(int key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int[] KeysTriggered();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int[] KeysDown();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int[] KeysReleased();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool MouseButtonTriggered(int button);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool MouseButtonDown(int button);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool MouseButtonReleased(int button);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseX();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseY();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseDeltaX();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseDeltaY();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MousePrevX();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MousePrevY();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseScrollX();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float MouseScrollY();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double InputLatency(int key);
	}
	#endregion

	#region Json | RapidJson
	public static class Json
	{
		// bridges // -----------------------------------------------------
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr LoadJson(string path);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr GetCurrentDoc();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern string GetCurrentDocName();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern int LoadInt(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern float LoadFloat(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool LoadBool(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern string LoadString(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Close();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ClearObjs();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr[] LoadObjArray(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr LoadObjWithObj(IntPtr obj);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr LoadObj(string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern int LoadObjInt(IntPtr objPtr, string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern float LoadObjFloat(IntPtr objPtr, string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool LoadObjBool(IntPtr objPtr, string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern string LoadObjString(IntPtr objPtr, string key);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr LoadObjObj(IntPtr objPtr, string key);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr SaveJson1();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr NewObjValue();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr NewArrayValue();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void AddToArray(IntPtr array, IntPtr value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr SaveBool(string key, bool value, IntPtr member);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr SaveInt(string key, int value, IntPtr member);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr SaveFloat(string key, float value, IntPtr member);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern IntPtr SaveString(string key, string value, IntPtr member);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void SaveObj(string key, IntPtr value, IntPtr array);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool SaveJson2(string filename);
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//public static extern void InitValues();
	}
	#endregion

	#region Vector2 | Math/Vector2D.h (WIP)
	public class Vector2<T> : IDisposable
	{
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

		// wrappers // -----------------------------------------------------

		private T? _X, _Y;

		public event EventHandler<int>? OnChange; // 0 = X, 1 = Y

		public T? X
		{
			get => _X;
			set
			{
				_X = value;
				OnChange?.Invoke(this, 0);
			}
		}

		public T? Y
		{
			get => _Y;
			set
			{
				_Y = value;
				OnChange?.Invoke(this, 1);
			}
		}

		public Vector2()
		{
			X = default;
			Y = default;
		}
		public Vector2(T x, T y)
		{
			X = x;
			Y = y;
		}
		public Vector2(Vector2<T> rhs)
		{
			X = rhs.X;
			Y = rhs.Y;
		}

		static Vector2<T> _Zero = new Vector2<T>((dynamic?)0, (dynamic?)0);
		static Vector2<T> _One = new Vector2<T>((dynamic?)1, (dynamic?)1);
		static Vector2<T> _Up = new Vector2<T>((dynamic?)0, (dynamic?)1);
		static Vector2<T> _Down = new Vector2<T>((dynamic?)0, (dynamic?)-1);
		static Vector2<T> _Left = new Vector2<T>((dynamic?)-1, (dynamic?)0);
		static Vector2<T> _Right = new Vector2<T>((dynamic?)1, (dynamic?)0);

		// TODO fix
		public static Vector2<T> Zero { get => _Zero; }
		public static Vector2<T> One { get => _One; }
		public static Vector2<T> Up { get => _Up; }
		public static Vector2<T> Down { get => _Down; }
		public static Vector2<T> Left { get => _Left; }
		public static Vector2<T> Right { get => _Right; }

		public override string ToString()
		{
			return $"Vector2 | X: {X}, Y: {Y}";
		}

		// operators

		public static Vector2<T> operator+(Vector2<T> lhs, Vector2<T> rhs)
		{
			return new Vector2<T>((dynamic?)lhs.X + (dynamic?)rhs.X, (dynamic?)lhs.Y + (dynamic?)rhs.Y);
		}

		public static Vector2<T> operator -(Vector2<T> lhs, Vector2<T> rhs)
		{
			return new Vector2<T>((dynamic?)lhs.X - (dynamic?)rhs.X, (dynamic?)lhs.Y - (dynamic?)rhs.Y);
		}

		public static Vector2<T> operator *(Vector2<T> lhs, Vector2<T> rhs)
		{
			return new Vector2<T>((dynamic?)lhs.X * (dynamic?)rhs.X, (dynamic?)lhs.Y * (dynamic?)rhs.Y);
		}

		public static Vector2<T> operator /(Vector2<T> lhs, Vector2<T> rhs)
		{
			return new Vector2<T>((dynamic?)lhs.X / (dynamic?)rhs.X, (dynamic?)lhs.Y / (dynamic?)rhs.Y);
		}

		public static Vector2<T> operator *(Vector2<T> lhs, T rhs)
		{
			return new Vector2<T>((dynamic?)lhs.X * (dynamic?)rhs, (dynamic?)lhs.Y * (dynamic?)rhs);
		}

		public static Vector2<T> operator *(T rhs, Vector2<T> lhs)
		{
			return new Vector2<T>((dynamic?)lhs.X * (dynamic?)rhs, (dynamic?)lhs.Y * (dynamic?)rhs);
		}

		// bridges // -----------------------------------------------------

		//[MethodImpl(MethodImplOptions.InternalCall)]
		//public static extern void Critical(string message);
	}
	#endregion

	#region Color | Graphic/Color.h (WIP)
	public class Color : IDisposable
	{
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

		// Wrapper // -----------------------------------------------------

		private float _R;
		private float _G;
		private float _B;
		private float _A;

		public event EventHandler<int>? OnChange; // 0 = R, 1 = G, 2 = B, 3 = A

		public float R
		{
			get => _R;
			set
			{
				_R = value;
				OnChange?.Invoke(this, 0);
			}
		}

		public float G
		{
			get => _G;
			set
			{
				_G = value;
				OnChange?.Invoke(this, 1);
			}
		}

		public float B
		{
			get => _B;
			set
			{
				_B = value;
				OnChange?.Invoke(this, 2);
			}
		}

		public float A
		{
			get => _A;
			set
			{
				_A = value;
				OnChange?.Invoke(this, 3);
			}
		}

		public Color (float r = 1f, float g = 1f, float b = 1f, float a = 1f)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}

		public Color(int r, int g, int b, int a)
		{
			R = r / 255f;
			G = g / 255f;
			B = b / 255f;
			A = a / 255f;
		}

		public Color(Color rhs)
		{
			R = rhs.R;
			G = rhs.G;
			B = rhs.B;
			A = rhs.A;
		}

		// Common Colors
		private static Color _White =		new Color(1f, 1f, 1f, 1f);
		private static Color _Black =		new Color(0f, 0f, 0f, 1f);
		private static Color _Red =			new Color(1f, 0f, 0f, 1f);
		private static Color _Green =		new Color(0f, 1f, 0f, 1f);
		private static Color _Blue =		new Color(0f, 0f, 1f, 1f);
		private static Color _Yellow =		new Color(1f, 1f, 0f, 1f);
		private static Color _Cyan =		new Color(0f, 1f, 1f, 1f);
		private static Color _Magenta =		new Color(1f, 0f, 1f, 1f);
		private static Color _Gray =		new Color(0.5f, 0.5f, 0.5f, 1f);
		private static Color _LightGray =	new Color(0.75f, 0.75f, 0.75f, 1f);
		private static Color _DarkGray =	new Color(0.25f, 0.25f, 0.25f, 1f);
		private static Color _Transparent = new Color(0f, 0f, 0f, 0f);

		public static Color White		{ get => _White; }
		public static Color Black		{ get => _Black; }
		public static Color Red			{ get => _Red; }
		public static Color Green		{ get => _Green; }
		public static Color Blue		{ get => _Blue; }
		public static Color Yellow		{ get => _Yellow; }
		public static Color Cyan		{ get => _Cyan; }
		public static Color Magenta		{ get => _Magenta; }
		public static Color Gray		{ get => _Gray; }
		public static Color LightGray	{ get => _LightGray; }
		public static Color DarkGray	{ get => _DarkGray; }
		public static Color Transparent { get => _Transparent; }

		// bridges // -----------------------------------------------------

		//[MethodImpl(MethodImplOptions.InternalCall)]
		//public static extern void Critical(string message);
	}
	#endregion

	#region Audio | Audio/AudioSystem.h (WIP)
	//public static class Audio //: IDisposable
	//{
	//	private bool _disposed = false;
	//	//private UnmanagedResource _resource;

	//	public void Dispose()
	//	{
	//		Dispose(true);
	//		GC.SuppressFinalize(this);
	//	}

	//	protected virtual void Dispose(bool disposing)
	//	{
	//		if (!_disposed)
	//		{
	//			if (disposing)
	//			{
	//				// Dispose managed resources
	//				//_resource?.Close();
	//			}

	//			// Dispose unmanaged resources
	//			// Free native resources if any

	//			_disposed = true;
	//		}
	//	}

	//	// Wrapper // -----------------------------------------------------

	//	private int NativeID = -1; // native id of entity of the audio component this audio is attached to
	//	public event EventHandler<int>? OnChange;

	//	public Audio(int nativeID)
	//	{
	//		NativeID = nativeID;
	//	}
	//}
	#endregion

	#region Scene | Scene/SceneManageSystem.h (WIP)
	public class SceneManager : IDisposable
	{
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

		public static void LoadScene(string sceneName, bool reloadMono = false)
		{
			//ConcurrentDictionary<int, Entity> ent = 
			//	new ConcurrentDictionary<int, Entity>(ECS.Entity.GetEntities());

			Load(sceneName, reloadMono);

			//ECS.Entity.EndEntities(ent);
		}
		public static void ReloadScene(bool reloadMono = false)
		{
			//ConcurrentDictionary<int, Entity> ent =
			//	new ConcurrentDictionary<int, Entity>(ECS.Entity.GetEntities());

			Reload(reloadMono);

			//ECS.Entity.EndEntities(ent);
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Load(string sceneName, bool reloadMono);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern string GetCurrent();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern string GetPrevious();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Reload(bool reloadMono);
	}
	#endregion

	#region Math
	public static class Mathematics
	{
		public static float Distance(Vector2<float> v1, Vector2<float> v2)
		{
			return Dist2D(v1.X, v1.Y, v2.X, v2.Y);
		}

		// bridges // -----------------------------------------------------
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Dist2D(float x1, float y1, float x2, float y2);
		
	}
	#endregion

	#region Engine | Engine/Engine.h
	public static class Engine
	{
		// bridges // -----------------------------------------------------
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern double GetFPS();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void AppQuit();
	}
	#endregion

	#region Camera | Camera/GameCamera.h
	public static class Camera
	{
		public static Vector2<float> Position
		{
			get { return new Vector2<float>(GetPosX(), GetPosY()); }
			set { SetPosition(value.X, value.Y); }
		}

		public static float Zoom
		{
			get { return GetZoom(); }
			set { SetZoom(value); }
		}

		public static float AspectRatio
		{
			get { return GetAspectRatio(); }
		}

		public static void SetTarget(Entity target)
		{
			SetZoom(target.GetId());
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetPosX();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetPosY();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPosition(float x, float y);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetZoom();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetZoom(float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAspectRatio();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTarget(int entity);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ClearTarget();
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool HasTarget();
	}
	#endregion
}
