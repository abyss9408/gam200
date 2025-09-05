/******************************************************************************/
/*!
\file   Components.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Hafawati Binte Mohd Ali
\contribution Shawn - 90%, Hafawati - 10%
\par    Course : CSD2401 / CSD2450 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the C# API Bridge for the Native Component library in C++

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Notes:

// Change events
// required to update the native side when the C# side changes member variables

// ================================================================ //

using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;

using Team_Popplio.Libraries;
using Team_Popplio;

namespace Team_Popplio.Components
{
	#region Utility =================================================
	public static class Utility
	{
		// copy all except entity id
		public static void Copy<T>(T source, T destination)
		{
			Team_Popplio.Utility.CopyAllExceptProperties(source, destination, new List<string>());
		}

		// copy everything
		public static void RefCopy<T>(T source, T destination)
		{
			Team_Popplio.Utility.CopyAllExcept(source, destination, new List<string>());
		}
	}
	#endregion // Utility // ================================

	public class Component 
	{
		public virtual void End() { }
	}

	#region Transform Component ================================================= 
	public class Transform2D : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private Libraries.Vector2<float> _Position;
		private Libraries.Vector2<float> _Scale;

		// constructor
		public Transform2D(int entity)
		{
			NativeID = entity;

			_Position = new Libraries.Vector2<float>();
			_Scale = new Libraries.Vector2<float>();

			_Position.OnChange += HandlePositionChange;
			_Scale.OnChange += HandleScaleChange;
		}

		private void HandlePositionChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetPositionX(NativeID, _Position.X);
					//UpdateLocalTransform(NativeID);
					break;
				case 1:
					SetPositionY(NativeID, _Position.Y);
					//UpdateLocalTransform(NativeID);
					break;
			}
		}

		private void HandleScaleChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetScaleX(NativeID, _Scale.X);
					//UpdateLocalTransform(NativeID);
					break;
				case 1:
					SetScaleY(NativeID, _Scale.Y);
					//UpdateLocalTransform(NativeID);
					break;
			}
		}

		public override void End()
		{
			_Position.OnChange -= HandlePositionChange;
			_Scale.OnChange -= HandleScaleChange;
		}

		public Libraries.Vector2<float> Position
		{
			get
			{
				_Position.X = GetPositionX(NativeID);
				_Position.Y = GetPositionY(NativeID);
				
				return _Position;
			}
			set
			{
				SetPositionX(NativeID, value.X);
				SetPositionY(NativeID, value.Y);
				//UpdateLocalTransform(NativeID);
				_Position = value;
			}
		}

		public Libraries.Vector2<float> Scale
		{
			get
			{
				_Scale.X = GetScaleX(NativeID);
				_Scale.Y = GetScaleY(NativeID);

				return _Scale;
			}
			set
			{
				SetScaleX(NativeID, value.X);
				SetScaleY(NativeID, value.Y);
				//UpdateLocalTransform(NativeID);
				_Scale = value;
			}
		}

		public double Rotation
		{
			get { return GetRotation(NativeID); }
			set { SetRotation(NativeID, value); } //UpdateLocalTransform(NativeID); }
		}

		public void Translate(Libraries.Vector2<float> translation)
		{
			Position += translation;
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetPositionX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPositionX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetPositionY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPositionY(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetScaleX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetScaleX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetScaleY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetScaleY(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double GetRotation(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetRotation(int instance, double value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void UpdateLocalTransform(int instance);
	}
	#endregion // Transform Component // ================================

	#region Render Component ================================================= 
	public class Render2D : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private Libraries.Color _Color;

		public Libraries.Color Color
		{
			get
			{
				_Color.R = GetColorR(NativeID);
				_Color.G = GetColorG(NativeID);
				_Color.B = GetColorB(NativeID);
				_Color.A = GetColorA(NativeID);

				return _Color;
			}
			set
			{
				SetColorR(NativeID, value.R);
				SetColorG(NativeID, value.G);
				SetColorB(NativeID, value.B);
				SetColorA(NativeID, value.A);

				_Color = value;
			}
		}

		// constructor
		public Render2D(int entity)
		{
			NativeID = entity;
			_Color = new Libraries.Color();
			_Color.OnChange += HandleColorChange;
		}

		private void HandleColorChange(object sender, int variable)
		{
			Color col = (Color)sender;
			switch (variable)
			{
				case 0:
					//SetColorR(NativeID, _Color.R);
					SetColorR(NativeID, col.R);
					break;
				case 1:
					//SetColorG(NativeID, _Color.G);
					SetColorG(NativeID, col.G);
					break;
				case 2:
					//SetColorB(NativeID, _Color.B);
					SetColorB(NativeID, col.B);
					break;
				case 3:
					//SetColorA(NativeID, _Color.A);
					SetColorA(NativeID, col.A);
					break;
			}
		}

		public override void End()
		{
			_Color.OnChange -= HandleColorChange;
		}

		public enum Mesh
		{
			Triangle,
			Quad,
			Circle,
			FullscreenQuad
		};

		public Mesh MeshType
		{
			get { return (Mesh)GetMeshType(NativeID); }
			set { SetMeshType(NativeID, (int)value); }
		}

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public string Shader
		{
			get { return GetShader(NativeID); }
			set { SetShader(NativeID, value); }
		}

		public string Texture
		{
			get { return GetTexture(NativeID); }
			set { SetTexture(NativeID, value); }
		}

		public bool UseInstancing
		{
			get { return GetInstancing(NativeID); }
			set { SetInstancing(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorR(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorR(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorG(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorG(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorB(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorB(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorA(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorA(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetMeshType(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMeshType(int instance, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetShader(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetShader(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetTexture(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTexture(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetInstancing(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetInstancing(int instance, bool value);
	}
	#endregion // Render Component // ================================

	#region RigidBody Component =================================================
	public class RigidBody2D : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private Libraries.Vector2<float> _Velocity;
		private Libraries.Vector2<float> _Acceleration;
		private Libraries.Vector2<float> _Force;

		public RigidBody2D(int entity)
		{
			NativeID = entity;

			_Velocity = new Libraries.Vector2<float>();
			_Acceleration = new Libraries.Vector2<float>();
			_Force = new Libraries.Vector2<float>();

			_Velocity.OnChange += HandleVelocityChange;
			_Acceleration.OnChange += HandleAccelerationChange;
			_Force.OnChange += HandleForceChange;
		}

		private void HandleVelocityChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetVelocityX(NativeID, _Velocity.X);
					break;
				case 1:
					SetVelocityY(NativeID, _Velocity.Y);
					break;
			}
		}

		private void HandleAccelerationChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetAccelerationX(NativeID, _Acceleration.X);
					break;
				case 1:
					SetAccelerationY(NativeID, _Acceleration.Y);
					break;
			}
		}

		private void HandleForceChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetForceX(NativeID, _Force.X);
					break;
				case 1:
					SetForceY(NativeID, _Force.Y);
					break;
			}
		}

		public override void End()
		{
			_Velocity.OnChange -= HandleVelocityChange;
			_Acceleration.OnChange -= HandleAccelerationChange;
			_Force.OnChange -= HandleForceChange;
		}

		public Libraries.Vector2<float> Velocity
		{
			get
			{
				_Velocity.X = GetVelocityX(NativeID);
				_Velocity.Y = GetVelocityY(NativeID);
				return _Velocity;
			}
			set
			{
				SetVelocityX(NativeID, value.X);
				SetVelocityY(NativeID, value.Y);
				_Velocity = value;
			}
		}

		public Libraries.Vector2<float> Acceleration
		{
			get
			{
				_Acceleration.X = GetAccelerationX(NativeID);
				_Acceleration.Y = GetAccelerationY(NativeID);
				return _Acceleration;
			}
			set
			{
				SetAccelerationX(NativeID, value.X);
				SetAccelerationY(NativeID, value.Y);
				_Acceleration = value;
			}
		}

		public Libraries.Vector2<float> Force
		{
			get
			{
				_Force.X = GetForceX(NativeID);
				_Force.Y = GetForceY(NativeID);
				return _Force;
			}
			set
			{
				SetForceX(NativeID, value.X);
				SetForceY(NativeID, value.Y);
				_Force = value;
			}
		}

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public float Mass
		{
			get { return GetMass(NativeID); }
			set { SetMass(NativeID, value); }
		}
		public float GravityScale
		{
			get { return GetGravityScale(NativeID); }
			set { SetGravityScale(NativeID, value); }
		}
		public bool UseGravity
		{
			get { return GetUseGravity(NativeID); }
			set { SetUseGravity(NativeID, value); }
		}
		public float DragCoefficient
		{
			get { return GetDragCoefficient(NativeID); }
			set { SetDragCoefficient(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetVelocityX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetVelocityX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetVelocityY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetVelocityY(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAccelerationX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAccelerationX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAccelerationY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAccelerationY(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetForceX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetForceX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetForceY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetForceY(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMass(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMass(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetGravityScale(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetGravityScale(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetUseGravity(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetUseGravity(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetDragCoefficient(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetDragCoefficient(int instance, float value);
	}
	#endregion // RigidBody Component // ================================

	#region BoxCollider Component =================================================
	public class BoxCollider2D : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private Libraries.Vector2<float> _Size;
		private Libraries.Vector2<float> _Offset;

		public BoxCollider2D(int entity)
		{
			NativeID = entity;
			_Size = new Libraries.Vector2<float>();
			_Offset = new Libraries.Vector2<float>();
			_Size.OnChange += HandleSizeChange;
			_Offset.OnChange += HandleOffsetChange;
		}

		private void HandleSizeChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetSizeX(NativeID, _Size.X);
					break;
				case 1:
					SetSizeY(NativeID, _Size.Y);
					break;
			}
		}

		private void HandleOffsetChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetOffsetX(NativeID, _Offset.X);
					break;
				case 1:
					SetOffsetY(NativeID, _Offset.Y);
					break;
			}
		}

		public override void End()
		{
			_Size.OnChange -= HandleSizeChange;
			_Offset.OnChange -= HandleOffsetChange;
		}

		public Libraries.Vector2<float> Size
		{
			get
			{
				_Size.X = GetSizeX(NativeID);
				_Size.Y = GetSizeY(NativeID);
				return _Size;
			}
			set
			{
				SetSizeX(NativeID, value.X);
				SetSizeY(NativeID, value.Y);
				_Size = value;
			}
		}

		public Libraries.Vector2<float> Offset
		{
			get
			{
				_Offset.X = GetOffsetX(NativeID);
				_Offset.Y = GetOffsetY(NativeID);
				return _Offset;
			}
			set
			{
				SetOffsetX(NativeID, value.X);
				SetOffsetY(NativeID, value.Y);
				_Offset = value;
			}
		}

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public bool IsTrigger
		{
			get { return GetTrigger(NativeID); }
			set { SetTrigger(NativeID, value); }
		}

		public bool IsEnabled
		{
			get { return GetEnabled(NativeID); }
			set { SetEnabled(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSizeX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSizeX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSizeY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSizeY(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetOffsetX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetOffsetX(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetOffsetY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetOffsetY(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetTrigger(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTrigger(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetEnabled(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetEnabled(int instance, bool value);
	}
	#endregion // BoxCollider Component // ================================

	#region Text Component =================================================
	public class Text2D : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private Libraries.Color _Color;

		public Libraries.Color Color
		{
			get
			{
				_Color.R = GetColorR(NativeID);
				_Color.G = GetColorG(NativeID);
				_Color.B = GetColorB(NativeID);
				_Color.A = GetColorA(NativeID);

				return _Color;
			}
			set
			{
				SetColorR(NativeID, value.R);
				SetColorG(NativeID, value.G);
				SetColorB(NativeID, value.B);
				SetColorA(NativeID, value.A);

				_Color = value;
			}
		}

		private void HandleColorChange(object sender, int variable)
		{
			Color col = (Color)sender;
			switch (variable)
			{
				case 0:
					//SetColorR(NativeID, _Color.R);
					SetColorR(NativeID, col.R);
					break;
				case 1:
					//SetColorG(NativeID, _Color.G);
					SetColorG(NativeID, col.G);
					break;
				case 2:
					//SetColorB(NativeID, _Color.B);
					SetColorB(NativeID, col.B);
					break;
				case 3:
					//SetColorA(NativeID, _Color.A);
					SetColorA(NativeID, col.A);
					break;
			}
		}

		public Text2D(int entity)
		{
			NativeID = entity;
			_Color = new Libraries.Color();
			_Color.OnChange += HandleColorChange;
		}

		public override void End()
		{
			_Color.OnChange -= HandleColorChange;
		}

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public string Text
		{
			get { return GetText(NativeID); }
			set { SetText(NativeID, value); }
		}
		public string FontName
		{
			get { return GetFontName(NativeID); }
			set { SetFontName(NativeID, value); }
		}
		public uint FontSize
		{
			get { return GetFontSize(NativeID); }
			set { SetFontSize(NativeID, value); }
		}
        public float Width
        {
            get { return GetWidth(NativeID); }
        }
        public float Height
        {
            get { return GetHeight(NativeID); }
        }

        // bridges

        [MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorR(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorR(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorG(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorG(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorB(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorB(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetColorA(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetColorA(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetText(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetText(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetFontName(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFontName(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern uint GetFontSize(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFontSize(int instance, uint value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetWidth(int entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetHeight(int entity);

    }
    #endregion // Text Component // ================================

    #region Logic Component =================================================
    public class Logic : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		//protected static ConcurrentDictionary<int, PopplioMonoScript> Scripts = 
		//	new ConcurrentDictionary<int, PopplioMonoScript>(); // set by C++ side

		public Logic(int entity)
		{
			NativeID = entity;
			//_Script = null;
		}

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public string ScriptName
		{
			get { return GetScriptName(NativeID); }
			//set { SetScriptName(NativeID, value); }
		}

		public int ScriptLanguage
		{
			get { return GetScriptLanguage(NativeID); }
			//set { SetScriptLanguage(NativeID, value); }
		}

		public void SetScript(PopplioMonoScript script)
		{
			if (script == null) return;

			Type type = script.GetType();
			Logger.EngineInfo($"C# | Setting script for entity {NativeID}");
			SetScriptName(NativeID, type.ToString().Replace(".", "::"));
			SetScriptLanguage(NativeID, 1); // 1 = C#
			SetScriptObj(NativeID, script);
		}

		public PopplioMonoScript? Script
		{
			get 
			{ 
				if (PopplioMonoScript.Scripts.ContainsKey(NativeID))
					return PopplioMonoScript.Scripts[NativeID];
				return null;
			}
			//set 
			//{
			//	if (_Script != null) return;

			//	Logger.EngineInfo($"C# | Setting script for entity {NativeID}");
			//	_Script = value;
			//}
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetScriptName(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetScriptName(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetScriptLanguage(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetScriptLanguage(int instance, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetScriptObj(int instance, PopplioMonoScript script);
	}
	#endregion // Logic Component // ================================

	#region Animation Component ================================================= 
	public class Animation2D : Component, IDisposable
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

		private int NativeID = -1; // entity id

		private Libraries.Vector2<int> _CurrentFrame;

		// constructor
		public Animation2D(int entity)
		{
			NativeID = entity;

			_CurrentFrame = new Libraries.Vector2<int>();

			_CurrentFrame.OnChange += HandleCurrentFrameChange;
		}

		public override void End()
		{
			_CurrentFrame.OnChange -= HandleCurrentFrameChange;
		}

		public int AnimStartX(string id, int? val = null)
		{
			if (val != null) SetAnimStartX(NativeID, id, (int)val);
			return GetAnimStartX(NativeID, id);
		}
		public int AnimStartY(string id, int? val = null)
		{
			if (val != null) SetAnimStartY(NativeID, id, (int)val);
			return GetAnimStartY(NativeID, id);
		}

		public int AnimEndX(string id, int? val = null)
		{
			if (val != null) SetAnimEndX(NativeID, id, (int)val);
			return GetAnimEndX(NativeID, id);
		}
		public int AnimEndY(string id, int? val = null)
		{
			if (val != null) SetAnimEndY(NativeID, id, (int)val);
			return GetAnimEndY(NativeID, id);
		}

		public float AnimSpeed(string id, float? val = null)
		{
			if (val != null) SetAnimSpeed(NativeID, id, (float)val);
			return GetAnimSpeed(NativeID, id);
		}

		public string AnimName(string id, string? val = null)
		{
			if (val != null) SetAnimName(NativeID, id, (string)val);
			return GetAnimName(NativeID, id);
		}

		public float AnimFrameWidth(string id, float? val = null)
		{
			if (val != null) SetAnimFrameWidth(NativeID, id, (float)val);
			return GetAnimFrameWidth(NativeID, id);
		}
		public float AnimFrameHeight(string id, float? val = null)
		{
			if (val != null) SetAnimFrameHeight(NativeID, id, (float)val);
			return GetAnimFrameHeight(NativeID, id);
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetAnimStartX(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimStartX(int instance, string id, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetAnimStartY(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimStartY(int instance, string id, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetAnimEndX(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimEndX(int instance, string id, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetAnimEndY(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimEndY(int instance, string id, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAnimSpeed(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimSpeed(int instance, string id, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetAnimName(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimName(int instance, string id, string value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAnimFrameWidth(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimFrameWidth(int instance, string id, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetAnimFrameHeight(int instance, string id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimFrameHeight(int instance, string id, float value);

		// -------------------------------

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public string CurrentAnimation 
		{
			get { return GetCurrentAnim(NativeID); }
			//set { SetCurrentAnim(NativeID, value); }
		}

		public void SetCurrentAnimation(string id, bool onlyOnce)
		{
			SetAnimation(NativeID, id, onlyOnce);
		}

		public float SheetWidth
		{
			get { return GetSheetWidth(NativeID); }
			set { SetSheetWidth(NativeID, value); }
		}
		public float SheetHeight
		{
			get { return GetSheetHeight(NativeID); }
			set { SetSheetHeight(NativeID, value); }
		}

		public float TimeElapsedCurrentCycle
		{
			get { return GetTimeElapsedCurrentCycle(NativeID); }
			//set { SetTimeElapsed(NativeID, value); }
		}

		public float TimeElapsedSinceLastResume
		{
			get { return GetTimeElapsedSinceLastResume(NativeID); }
		}

		private void HandleCurrentFrameChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetFrameX(NativeID, _CurrentFrame.X);
					break;
				case 1:
					SetFrameY(NativeID, _CurrentFrame.Y);
					break;
			}
		}

		public Vector2<int> CurrentFrame
		{
			get
			{
				_CurrentFrame.X = GetFrameX(NativeID);
				_CurrentFrame.Y = GetFrameY(NativeID);
				return _CurrentFrame;
			}
			set
			{
				SetFrameX(NativeID, value.X);
				SetFrameY(NativeID, value.Y);
				_CurrentFrame = value;
			}
		}

		public bool IsPaused
		{
			get { return GetPaused(NativeID); }
			//set { SetPaused(NativeID, value); }
		}

		public bool IsPlaying
		{
			get { return GetPlaying(NativeID); }
			//set { SetPaused(NativeID, value); }
		}

		public bool PlayOnce
		{
			get { return GetPlayOnce(NativeID); }
			set { SetPlayOnce(NativeID, value); }
		}

		public void Pause()
		{
			Pause(NativeID);
		}

        public void Stop()
        {
            Stop(NativeID);
        }

		public string[] Animations
		{
			get { return GetAnimations(NativeID); }
		}

        public void Play()
        {
            Play(NativeID);
        }

		public void Resume()
		{
			Resume(NativeID);
		}

        // bridges

        [MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetCurrentAnim(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetAnimation(int instance, string value, bool playOnce);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSheetWidth(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSheetWidth(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSheetHeight(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSheetHeight(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetTimeElapsedCurrentCycle(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetTimeElapsedSinceLastResume(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTimeElapsed(int instance, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetFrameX(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFrameX(int instance, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetFrameY(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFrameY(int instance, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetPaused(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetPlaying(int instance);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetPlayOnce(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPlayOnce(int instance, bool value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Pause(int instance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Stop(int instance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Play(int instance);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Resume(int instance);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string[] GetAnimations(int instance);
	}
	#endregion

	#region Audio Component =================================================
	public class Audio : Component, IDisposable
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

		private int NativeID = -1; // entity id
		public event EventHandler<int>? OnChange;

		//int NativeID
		//{
		//	get { return _NativeID; }
		//	set
		//	{
		//		if (!Entity.Exists(Entity.GetEntity(value))) return;
		//		_NativeID = value;
		//	}
		//}

		//public Libraries.Audio Data;

		// constructor
		public Audio(int entity)
		{
			NativeID = entity;
			//Data = new Libraries.Audio(entity);
		}

		// Component

		public bool IsActive
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public string Name
		{
			get { return GetName(NativeID); }
			set { SetName(NativeID, value); }
		}

		public float Volume
		{
			get { return GetVolume(NativeID); }
			set { SetVolume(NativeID, value); }
		}

		public bool Loop
		{
			get { return GetLoop(NativeID); }
			set { SetLoop(NativeID, value); }
		}

		public float BPM
		{
			get { return GetBPM(NativeID); }
			set { SetBPM(NativeID, value); }
		}

		public Tuple<int, int> TimeSignature
		{
			get { return new Tuple<int, int>(GetTimeSignatureTop(NativeID), GetTimeSignatureBottom(NativeID)); }
			set { SetTimeSignature(NativeID, value.Item1, value.Item2); }
		}

		public bool PlayOnAwake
		{
			get { return GetPlayOnAwake(NativeID); }
			set { SetPlayOnAwake(NativeID, value); }
		}

		public int ChannelGroup
		{
			get { return GetChannelGroup(NativeID); }
			set { SetChannelGroup(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string GetName(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetName(int instance, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetVolume(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetVolume(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetLoop(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLoop(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetBPM(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetBPM(int instance, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetTimeSignatureTop(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetTimeSignatureBottom(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTimeSignature(int instance, int top, int bottom);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetPlayOnAwake(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetPlayOnAwake(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetChannelGroup(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetChannelGroup(int instance, int value);

		// AudioSystem | Audio/AudioSystem.h

		public static uint DefaultUnit
		{
			get => GetDefaultUnit();
		}

		public bool IsPlaying
		{
			get => GetIsPlaying(NativeID);
		}

		public bool IsPaused
		{
			get => GetIsPaused(NativeID);
		}

		public uint Length
		{
			get => GetLength(NativeID);
		}

		public float LengthF
		{
			get => (float)Length / 1000f;
		}

		public double LengthD
		{
			get => (double)Length / 1000d;
		}

		public float BeatInterval
		{
			get => GetBeatInterval(NativeID);
		}

		public uint CurrentTime
		{
			get => GetCurrentTime(NativeID);
			set => SetCurrentTime(NativeID, value);
		}

		public float CurrentTimeF
		{
			get => (float)CurrentTime / 1000f;
			set => CurrentTime = (uint)(value * 1000f);
		}

		public double CurrentTimeD
		{
			get => (double)CurrentTime / 1000d;
			set => CurrentTime = (uint)(value * 1000d);
		}

		public int OffBeat
		{
			get => GetOffBeat(NativeID);
		}

		public void Play()
		{
			Play(NativeID);
			OnChange?.Invoke(this, -1);
		}

		public void Pause()
		{
			Pause(NativeID);
			OnChange?.Invoke(this, -1);
		}

		public void Stop()
		{
			Stop(NativeID);
			OnChange?.Invoke(this, -1);
		}

		// TODO | Try and use true rectangular array instead of jagged array
		public bool GetSpectrumData(out float[] fft, int channel, float magMod)
		{
			//fft = new float[2,64];
			//fft = new float[2][];
			try
			{
				fft = GetSpectrum(NativeID, channel, magMod);
				return true;
			}
			catch (Exception e)
			{
				Logger.EngineError($"C# | Failed to get spectrum data | {e}");
				fft = new float[32];
				//fft = new float[2][];
				//fft[0] = new float[64];
				//fft[1] = new float[64];
				return false;
			}
		}

		public bool Mute
		{
			get => GetMute(NativeID);
			set => SetMute(NativeID, value);
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern uint GetDefaultUnit();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetIsPlaying(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetIsPaused(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern uint GetLength(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetBeatInterval(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern uint GetCurrentTime(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetCurrentTime(int nativeID, uint time);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetOffBeat(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Play(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Pause(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Stop(int nativeID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float[] GetSpectrum(int nativeID, int channel, float magMod);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMute(int nativeID, bool m);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetMute(int nativeID);

		// Static methods

		public static float ChannelMasterVolume
		{
			get => GetMasterVolume();
			set => SetMasterVolume(value);
		}

		public static float ChannelBGMVolume
		{
			get => GetBGMVolume();
			set => SetBGMVolume(value);
		}

		public static float ChannelSFXVolume
		{
			get => GetSFXVolume();
			set => SetSFXVolume(value);
		}

		public static float GetChannelGroupVolume(int group)
		{
			return GetGroupVolume(group);
		}
		public static float SetChannelGroupVolume(int group, float volume)
		{
			SetGroupVolume(group, volume);
			return GetGroupVolume(group);
		}

		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMasterVolume();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMasterVolume(float volume);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetBGMVolume();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetBGMVolume(float volume);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSFXVolume();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSFXVolume(float volume);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetGroupVolume(int group);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetGroupVolume(int group, float volume);
	}
	#endregion // Audio Component // ================================

	#region Hierarchy Component ================================================= 
	public class Hierarchy : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		private int[] _ChildrenId;
		private ECS.Entity[] _Children;
		private List<ECS.Entity> _ChildrenList;
		private ConcurrentDictionary<int, ECS.Entity> _ChildrenDictId;
		private ConcurrentDictionary<string, ECS.Entity> _ChildrenDictName;

		//private bool ChildrenIdChanged;
		private bool ChildrenChanged;
		private bool ChildrenListChanged;
		private bool ChildrenDictIdChanged;
		private bool ChildrenDictNameChanged;

		private Libraries.Vector2<float> _LocalPos;
		private Libraries.Vector2<float> _LocalScale;

		// constructor
		public Hierarchy(int entity)
		{
			NativeID = entity;

			_ChildrenId			= new int[] { };
			_Children			= new ECS.Entity[] { };
			_ChildrenList		= new List<ECS.Entity>();
			_ChildrenDictId		= new ConcurrentDictionary<int, ECS.Entity>();
			_ChildrenDictName	= new ConcurrentDictionary<string, ECS.Entity>();

			//ChildrenIdChanged = false;
			ChildrenChanged			= false;
			ChildrenListChanged		= false;
			ChildrenDictIdChanged	= false;
			ChildrenDictNameChanged = false;

			_LocalPos = new Libraries.Vector2<float>();
			_LocalScale = new Libraries.Vector2<float>();

			_LocalPos.OnChange += HandleLocalPosChange;
			_LocalScale.OnChange += HandleLocalScaleChange;
		}

		public override void End()
		{
			_LocalPos.OnChange -= HandleLocalPosChange;
			_LocalScale.OnChange -= HandleLocalScaleChange;
		}

		public ECS.Entity Parent
		{
			get { return ECS.Entity.GetEntity(GetParent(NativeID)); }
			set 
			{ 
				if (value.GetId() == -1)
				{
					SetParent(NativeID, value.GetId());
					return;
				}

				if (!ECS.Entity.Exists(value))
				{
					Logger.EngineError($"C# | Hierarchy | Tried to set non-existent entity {value.GetId()} as parent");
					return;
				}

				if (value.GetId() == NativeID)
				{
					Logger.EngineError($"C# | Hierarchy | Tried to set entity {value.GetId()} as its own parent");
					return;
				}

				SetParent(NativeID, value.GetId()); 
			}
		}

		public ECS.Entity AddChild(ECS.Entity e)
		{
			AddChild(NativeID, e.GetId());
			_ChildrenId = GetChildren(NativeID);
			ChildrenChanged			= true;
			ChildrenListChanged		= true;
			ChildrenDictIdChanged	= true;
			ChildrenDictNameChanged = true;
			return e;
		}

		public void RemoveChild(ECS.Entity e)
		{
			RemoveChild(NativeID, e.GetId());
			_ChildrenId = GetChildren(NativeID);
			ChildrenChanged = true;
			ChildrenListChanged = true;
			ChildrenDictIdChanged = true;
			ChildrenDictNameChanged = true;
		}

		private static int[]? childrenIdBuffer;

		public int[] ChildrenId // fastest
		{
			get
			{
				int[] temp = GetChildren(NativeID);

				if (_ChildrenId != temp)
				{
					_ChildrenId = temp;

					ChildrenChanged = true;
					ChildrenListChanged = true;
					ChildrenDictIdChanged = true;
					ChildrenDictNameChanged = true;
				}

				return _ChildrenId;
			}
		}

		public ECS.Entity[] Children 
		{
			get
			{
				//bool changed = (_ChildrenId != GetChildren(NativeID));

				//if (changed)
				//{
				//	_ChildrenId = GetChildren(NativeID);

				//	ChildrenChanged = true;
				//	ChildrenListChanged = true;
				//	ChildrenDictIdChanged = true;
				//	ChildrenDictNameChanged = true;
				//}

				childrenIdBuffer = ChildrenId;

				if (ChildrenChanged)
				{
					Logger.EngineDebug("C# | Hierarchy | Children changed");

					_Children = new ECS.Entity[_ChildrenId.Length];
					for (int i = 0; i < _ChildrenId.Length; i++)
					{
						_Children[i] = ECS.Entity.GetEntity(_ChildrenId[i]);
					}
					ChildrenChanged = false;
				}

				return _Children;
			}
		}

		public List<ECS.Entity> ChildrenList
		{
			get
			{
				//bool changed = (_ChildrenId != GetChildren(NativeID));
				//if (changed)
				//{
				//	_ChildrenId = GetChildren(NativeID);

				//	ChildrenChanged = true;
				//	ChildrenListChanged = true;
				//	ChildrenDictIdChanged = true;
				//	ChildrenDictNameChanged = true;
				//}

				childrenIdBuffer = ChildrenId;

				if (ChildrenListChanged)
				{
					_ChildrenList = new List<ECS.Entity>();
					for (int i = 0; i < _ChildrenId.Length; i++)
					{
						_ChildrenList.Add(ECS.Entity.GetEntity(_ChildrenId[i]));
					}
					ChildrenListChanged = false;
				}
				return _ChildrenList;
			}
		}

		public ConcurrentDictionary<int, ECS.Entity> ChildrenDictId
		{
			get
			{
				//bool changed = (_ChildrenId != GetChildren(NativeID));
				//if (changed)
				//{
				//	_ChildrenId = GetChildren(NativeID);
				//	ChildrenChanged = true;
				//	ChildrenListChanged = true;
				//	ChildrenDictIdChanged = true;
				//	ChildrenDictNameChanged = true;
				//}

				childrenIdBuffer = ChildrenId;

				if (ChildrenDictIdChanged)
				{
					_ChildrenDictId = new ConcurrentDictionary<int, ECS.Entity>();
					for (int i = 0; i < _ChildrenId.Length; i++)
					{
						_ChildrenDictId.TryAdd(_ChildrenId[i], ECS.Entity.GetEntity(_ChildrenId[i]));
					}
					ChildrenDictIdChanged = false;
				}
				return _ChildrenDictId;
			}
		}

		public ConcurrentDictionary<string, ECS.Entity> ChildrenDictName
		{
			get
			{
				//bool changed = (_ChildrenId != GetChildren(NativeID));
				//if (changed)
				//{
				//	_ChildrenId = GetChildren(NativeID);
				//	ChildrenChanged = true;
				//	ChildrenListChanged = true;
				//	ChildrenDictIdChanged = true;
				//	ChildrenDictNameChanged = true;
				//}

				childrenIdBuffer = ChildrenId;

				if (ChildrenDictNameChanged)
				{
					_ChildrenDictName = new ConcurrentDictionary<string, ECS.Entity>();
					for (int i = 0; i < _ChildrenId.Length; i++)
					{
						_ChildrenDictName.TryAdd(
							ECS.Entity.GetEntity(_ChildrenId[i]).Name(), ECS.Entity.GetEntity(_ChildrenId[i]));
					}
					ChildrenDictNameChanged = false;
				}
				return _ChildrenDictName;
			}
		}

		public bool InheritActive
		{
			get { return GetInheritActive(NativeID); }
			set { SetInheritActive(NativeID, value); }
		}

		public bool InheritTransform
		{
			get { return GetInheritTransform(NativeID); }
			set { SetInheritTransform(NativeID, value); }
		}

		private void HandleLocalPosChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetLocalPosX(NativeID, _LocalPos.X);
					break;
				case 1:
					SetLocalPosY(NativeID, _LocalPos.Y);
					break;
			}
		}

		private void HandleLocalScaleChange(object sender, int variable)
		{
			switch (variable)
			{
				case 0:
					SetLocalScaleX(NativeID, _LocalScale.X);
					break;
				case 1:
					SetLocalScaleY(NativeID, _LocalScale.Y);
					break;
			}
		}

		public Libraries.Vector2<float> LocalPosition2D
		{
			get
			{
				_LocalPos.X = GetLocalPosX(NativeID);
				_LocalPos.Y = GetLocalPosY(NativeID);
				return _LocalPos;
			}
			set
			{
				SetLocalPosX(NativeID, value.X);
				SetLocalPosY(NativeID, value.Y);
				_LocalPos = value;
			}
		}

		public Libraries.Vector2<float> LocalScale2D
		{
			get
			{
				_LocalPos.X = GetLocalScaleX(NativeID);
				_LocalPos.Y = GetLocalScaleY(NativeID);
				return _LocalScale;
			}
			set
			{
				SetLocalScaleX(NativeID, value.X);
				SetLocalScaleY(NativeID, value.Y);
				_LocalScale = value;
			}
		}

		public double LocalRotation2D
		{
			get { return GetLocalRotation(NativeID); }
			set { SetLocalRotation(NativeID, value); }
		}

		// bridges

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetParent(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetParent(int entity, int parent);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddChild(int entity, int child);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void RemoveChild(int entity, int child);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int[] GetChildren(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetInheritActive(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetInheritActive(int entity, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetInheritTransform(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetInheritTransform(int entity, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetLocalPosX(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLocalPosX(int entity, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetLocalPosY(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLocalPosY(int entity, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetLocalScaleX(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLocalScaleX(int entity, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetLocalScaleY(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLocalScaleY(int entity, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern double GetLocalRotation(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLocalRotation(int entity, double value);
	}
    #endregion // Hierarchy Component // ================================

    #region UI Component =================================================

    public class UI : Component, IDisposable
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

        protected int NativeID = -1; // entity id

        public UI(int entity)
        {
            NativeID = entity;
        }

		public int GetEntityID() 
		{
			return NativeID;
		}

    public bool Active
        {
            get { return GetActive(NativeID); }
            set { SetActive(NativeID, value); }
        }

        public UIType Type
        {
            get { return (UIType)GetUIType(NativeID); }
            set { SetUIType(NativeID, (int)value); }
        }

        public bool Checked
        {
            get { return GetChecked(NativeID); }
            set { SetChecked(NativeID, value); }
        }

        public float SliderValue
        {
            get { return GetSliderValue(NativeID); }
            set { SetSliderValue(NativeID, value); }
        }

        public int LinkedEntity
        {
            get { return GetLinkedEntity(NativeID); }
            set { SetLinkedEntity(NativeID, value); }
        }

		public void RegisterOnClickCallback(string actionName, Action<int> callback)
		{
			RegisterUICallback(NativeID, actionName, callback);
		}

		public void AddOnClickAction(string action)
		{
			AddOnClickAction(NativeID, action);
		}

		public void RemoveOnClickAction(string action)
		{
			RemoveOnClickAction(NativeID, action);
		}

		public string[] GetOnClickActions()
		{
			return GetOnClickActions(NativeID);
		}

		public void SetOnClickActions(string[] actions)
		{
			SetOnClickActions(NativeID, actions);
		}

		public bool HasHoverEffect
		{
			get { return GetHasHoverEffect(NativeID); }
			set { SetHasHoverEffect(NativeID, value); }
		}

		public bool IsHovered
		{
			get { return GetIsHovered(NativeID); }
			set { SetIsHovered(NativeID, value); }
		}

		public int FrameX
		{
			get { return GetFrameX(NativeID); }
			set { SetFrameX(NativeID, value); }
		}

		public int FrameY
		{
			get { return GetFrameY(NativeID); }
			set { SetFrameY(NativeID, value); }
		}

		public int HoverFrameX
		{
			get { return GetHoverFrameX(NativeID); }
			set { SetHoverFrameX(NativeID, value); }
		}

		public int HoverFrameY
		{
			get { return GetHoverFrameY(NativeID); }
			set { SetHoverFrameY(NativeID, value); }
		}

		public int TotalColumns
		{
			get { return GetTotalColumns(NativeID); }
			set { SetTotalColumns(NativeID, value); }
		}

		public int TotalRows
		{
			get { return GetTotalRows(NativeID); }
			set { SetTotalRows(NativeID, value); }
		}

		public float SheetWidth
		{
			get { return GetSheetWidth(NativeID); }
			set { SetSheetWidth(NativeID, value); }
		}

		public float SheetHeight
		{
			get { return GetSheetHeight(NativeID); }
			set { SetSheetHeight(NativeID, value); }
		}

		public void ExecuteUIAction(string action)
		{
			ExecuteUIActionInternal(NativeID, action);
		}

		// Internal Call Bindings
		[MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetActive(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetActive(int entity, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetUIType(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetUIType(int entity, int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetChecked(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetChecked(int entity, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetSliderValue(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSliderValue(int entity, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetLinkedEntity(int entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinkedEntity(int entity, int linkedEntity);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddOnClickAction(int entity, string action);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void RemoveOnClickAction(int entity, string action);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string[] GetOnClickActions(int entity);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetOnClickActions(int entity, string[] actions);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetHasHoverEffect(int entity);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetHasHoverEffect(int entity, bool value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetIsHovered(int entity);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetIsHovered(int entity, bool value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetFrameX(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFrameX(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetFrameY(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetFrameY(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetHoverFrameX(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetHoverFrameX(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetHoverFrameY(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetHoverFrameY(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetTotalColumns(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTotalColumns(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetTotalRows(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTotalRows(int entity, int value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSheetWidth(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSheetWidth(int entity, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetSheetHeight(int entity);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetSheetHeight(int entity, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void RegisterUICallback(int entityID, string name, object callback);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void ExecuteUIActionInternal(int entity, string action);
	}

    public enum UIType
    {
        BUTTON,
        CHECKBOX,
        SLIDER_TRACK,
        SLIDER_HANDLE
    }

	#endregion

	#region Particle Component =================================================
	public class Particle : Component, IDisposable
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

		protected int NativeID = -1; // entity id

		public class Emitter
		{
			// DO NOT MUTATE
			private int index = -1;
			private int nativeId = -1;

			//private Libraries.Vector2<float> _Position;

			public Emitter(int i, int n)
			{
				index = i;
				nativeId = n;
			}

			public void Kill()
			{
				KillEmitter(nativeId, index);
			}

			public bool Active
			{
				get { return GetActiveEmitter(nativeId, index); }
				set { SetActiveEmitter(nativeId, index, value); }
			}

			public bool Loop
			{
				get { return GetLoopEmitter(nativeId, index); }
				set { SetLoopEmitter(nativeId, index, value); }
			}

			public float ElapsedTime
			{
				get { return GetElapsedTimeEmitter(nativeId, index); }
				set { SetElapsedTimeEmitter(nativeId, index, value); }
			}

			public float Rate
			{
				get { return GetRateEmitter(nativeId, index); }
				set { SetRateEmitter(nativeId, index, value); }
			}

			public float TLoop
			{
				get { return GetTLoopEmitter(nativeId, index); }
				set { SetTLoopEmitter(nativeId, index, value); }
			}

			public float MinLifeTime
			{
				get { return GetMinLifeTimeEmitter(nativeId, index); }
				set { SetMinLifeTimeEmitter(nativeId, index, value); }
			}

			public float MaxLifeTime
			{
				get { return GetMaxLifeTimeEmitter(nativeId, index); }
				set { SetMaxLifeTimeEmitter(nativeId, index, value); }
			}

			public float MinRot
			{
				get { return GetMinRotEmitter(nativeId, index); }
				set { SetMinRotEmitter(nativeId, index, value); }
			}

			public float MaxRot
			{
				get { return GetMaxRotEmitter(nativeId, index); }
				set { SetMaxRotEmitter(nativeId, index, value); }
			}

			public float MinSpinVel
			{
				get { return GetMinSpinVelEmitter(nativeId, index); }
				set { SetMinSpinVelEmitter(nativeId, index, value); }
			}

			public float MaxSpinVel
			{
				get { return GetMaxSpinVelEmitter(nativeId, index); }
				set { SetMaxSpinVelEmitter(nativeId, index, value); }
			}

			public float MinSpinAccel
			{
				get { return GetMinSpinAccelEmitter(nativeId, index); }
				set { SetMinSpinAccelEmitter(nativeId, index, value); }
			}

			public float MaxSpinAccel
			{
				get { return GetMaxSpinAccelEmitter(nativeId, index); }
				set { SetMaxSpinAccelEmitter(nativeId, index, value); }
			}

			public float MinSpinDirAccel
			{
				get { return GetMinSpinDirAccelEmitter(nativeId, index); }
				set { SetMinSpinDirAccelEmitter(nativeId, index, value); }
			}

			public float MaxSpinDirAccel
			{
				get { return GetMaxSpinDirAccelEmitter(nativeId, index); }
				set { SetMaxSpinDirAccelEmitter(nativeId, index, value); }
			}

			// todo remaining properties
			// todo remaining framework of interface (ECS, etc)

			public float MinPosX
			{
				get { return GetMinPosXEmitter(nativeId, index); }
				set { SetMinPosXEmitter(nativeId, index, value); }
			}

			public float MaxPosX
			{
				get { return GetMaxPosXEmitter(nativeId, index); }
				set { SetMaxPosXEmitter(nativeId, index, value); }
			}

			public float MinPosY
			{
				get { return GetMinPosYEmitter(nativeId, index); }
				set { SetMinPosYEmitter(nativeId, index, value); }
			}

			public float MaxPosY
			{
				get { return GetMaxPosYEmitter(nativeId, index); }
				set { SetMaxPosYEmitter(nativeId, index, value); }
			}

			public float MinVelX
			{
				get { return GetMinVelXEmitter(nativeId, index); }
				set { SetMinVelXEmitter(nativeId, index, value); }
			}

			public float MaxVelX
			{
				get { return GetMaxVelXEmitter(nativeId, index); }
				set { SetMaxVelXEmitter(nativeId, index, value); }
			}

			public float MinVelY
			{
				get { return GetMinVelYEmitter(nativeId, index); }
				set { SetMinVelYEmitter(nativeId, index, value); }
			}

			public float MaxVelY
			{
				get { return GetMaxVelYEmitter(nativeId, index); }
				set { SetMaxVelYEmitter(nativeId, index, value); }
			}

			public float MinAccelX
			{
				get { return GetMinAccelXEmitter(nativeId, index); }
				set { SetMinAccelXEmitter(nativeId, index, value); }
			}

			public float MaxAccelX
			{
				get { return GetMaxAccelXEmitter(nativeId, index); }
				set { SetMaxAccelXEmitter(nativeId, index, value); }
			}

			public float MinAccelY
			{
				get { return GetMinAccelYEmitter(nativeId, index); }
				set { SetMinAccelYEmitter(nativeId, index, value); }
			}

			public float MaxAccelY
			{
				get { return GetMaxAccelYEmitter(nativeId, index); }
				set { SetMaxAccelYEmitter(nativeId, index, value); }
			}

			public float MinDirAccelX
			{
				get { return GetMinDirAccelXEmitter(nativeId, index); }
				set { SetMinDirAccelXEmitter(nativeId, index, value); }
			}

			public float MaxDirAccelX
			{
				get { return GetMaxDirAccelXEmitter(nativeId, index); }
				set { SetMaxDirAccelXEmitter(nativeId, index, value); }
			}

			public float MinDirAccelY
			{
				get { return GetMinDirAccelYEmitter(nativeId, index); }
				set { SetMinDirAccelYEmitter(nativeId, index, value); }
			}

			public float MaxDirAccelY
			{
				get { return GetMaxDirAccelYEmitter(nativeId, index); }
				set { SetMaxDirAccelYEmitter(nativeId, index, value); }
			}

			public float MinScaleX
			{
				get { return GetMinScaleXEmitter(nativeId, index); }
				set { SetMinScaleXEmitter(nativeId, index, value); }
			}

			public float MaxScaleX
			{
				get { return GetMaxScaleXEmitter(nativeId, index); }
				set { SetMaxScaleXEmitter(nativeId, index, value); }
			}

			public float MinScaleY
			{
				get { return GetMinScaleYEmitter(nativeId, index); }
				set { SetMinScaleYEmitter(nativeId, index, value); }
			}

			public float MaxScaleY
			{
				get { return GetMaxScaleYEmitter(nativeId, index); }
				set { SetMaxScaleYEmitter(nativeId, index, value); }
			}

			public float MinScaleVelX
			{
				get { return GetMinScaleVelXEmitter(nativeId, index); }
				set { SetMinScaleVelXEmitter(nativeId, index, value); }
			}

			public float MaxScaleVelX
			{
				get { return GetMaxScaleVelXEmitter(nativeId, index); }
				set { SetMaxScaleVelXEmitter(nativeId, index, value); }
			}

			public float MinScaleVelY
			{
				get { return GetMinScaleVelYEmitter(nativeId, index); }
				set { SetMinScaleVelYEmitter(nativeId, index, value); }
			}

			public float MaxScaleVelY
			{
				get { return GetMaxScaleVelYEmitter(nativeId, index); }
				set { SetMaxScaleVelYEmitter(nativeId, index, value); }
			}

			public float MinScaleAccelX
			{
				get { return GetMinScaleAccelXEmitter(nativeId, index); }
				set { SetMinScaleAccelXEmitter(nativeId, index, value); }
			}

			public float MaxScaleAccelX
			{
				get { return GetMaxScaleAccelXEmitter(nativeId, index); }
				set { SetMaxScaleAccelXEmitter(nativeId, index, value); }
			}

			public float MinScaleAccelY
			{
				get { return GetMinScaleAccelYEmitter(nativeId, index); }
				set { SetMinScaleAccelYEmitter(nativeId, index, value); }
			}

			public float MaxScaleAccelY
			{
				get { return GetMaxScaleAccelYEmitter(nativeId, index); }
				set { SetMaxScaleAccelYEmitter(nativeId, index, value); }
			}

			public float MinScaleDirAccelX
			{
				get { return GetMinScaleDirAccelXEmitter(nativeId, index); }
				set { SetMinScaleDirAccelXEmitter(nativeId, index, value); }
			}

			public float MaxScaleDirAccelX
			{
				get { return GetMaxScaleDirAccelXEmitter(nativeId, index); }
				set { SetMaxScaleDirAccelXEmitter(nativeId, index, value); }
			}

			public float MinScaleDirAccelY
			{
				get { return GetMinScaleDirAccelYEmitter(nativeId, index); }
				set { SetMinScaleDirAccelYEmitter(nativeId, index, value); }
			}

			public float MaxScaleDirAccelY
			{
				get { return GetMaxScaleDirAccelYEmitter(nativeId, index); }
				set { SetMaxScaleDirAccelYEmitter(nativeId, index, value); }
			}

			public float MinColR
			{
				get { return GetMinColREmitter(nativeId, index); }
				set { SetMinColREmitter(nativeId, index, value); }
			}

			public float MaxColR
			{
				get { return GetMaxColREmitter(nativeId, index); }
				set { SetMaxColREmitter(nativeId, index, value); }
			}

			public float MinColG
			{
				get { return GetMinColGEmitter(nativeId, index); }
				set { SetMinColGEmitter(nativeId, index, value); }
			}

			public float MaxColG
			{
				get { return GetMaxColGEmitter(nativeId, index); }
				set { SetMaxColGEmitter(nativeId, index, value); }
			}

			public float MinColB
			{
				get { return GetMinColBEmitter(nativeId, index); }
				set { SetMinColBEmitter(nativeId, index, value); }
			}

			public float MaxColB
			{
				get { return GetMaxColBEmitter(nativeId, index); }
				set { SetMaxColBEmitter(nativeId, index, value); }
			}

			public float MinColA
			{
				get { return GetMinColAEmitter(nativeId, index); }
				set { SetMinColAEmitter(nativeId, index, value); }
			}

			public float MaxColA
			{
				get { return GetMaxColAEmitter(nativeId, index); }
				set { SetMaxColAEmitter(nativeId, index, value); }
			}

			public float MinColVelR
			{
				get { return GetMinColVelREmitter(nativeId, index); }
				set { SetMinColVelREmitter(nativeId, index, value); }
			}

			public float MaxColVelR
			{
				get { return GetMaxColVelREmitter(nativeId, index); }
				set { SetMaxColVelREmitter(nativeId, index, value); }
			}

			public float MinColVelG
			{
				get { return GetMinColVelGEmitter(nativeId, index); }
				set { SetMinColVelGEmitter(nativeId, index, value); }
			}

			public float MaxColVelG
			{
				get { return GetMaxColVelGEmitter(nativeId, index); }
				set { SetMaxColVelGEmitter(nativeId, index, value); }
			}

			public float MinColVelB
			{
				get { return GetMinColVelBEmitter(nativeId, index); }
				set { SetMinColVelBEmitter(nativeId, index, value); }
			}

			public float MaxColVelB
			{
				get { return GetMaxColVelBEmitter(nativeId, index); }
				set { SetMaxColVelBEmitter(nativeId, index, value); }
			}

			public float MinColVelA
			{
				get { return GetMinColVelAEmitter(nativeId, index); }
				set { SetMinColVelAEmitter(nativeId, index, value); }
			}

			public float MaxColVelA
			{
				get { return GetMaxColVelAEmitter(nativeId, index); }
				set { SetMaxColVelAEmitter(nativeId, index, value); }
			}

			public float MinColAccelR
			{
				get { return GetMinColAccelREmitter(nativeId, index); }
				set { SetMinColAccelREmitter(nativeId, index, value); }
			}

			public float MaxColAccelR
			{
				get { return GetMaxColAccelREmitter(nativeId, index); }
				set { SetMaxColAccelREmitter(nativeId, index, value); }
			}

			public float MinColAccelG
			{
				get { return GetMinColAccelGEmitter(nativeId, index); }
				set { SetMinColAccelGEmitter(nativeId, index, value); }
			}

			public float MaxColAccelG
			{
				get { return GetMaxColAccelGEmitter(nativeId, index); }
				set { SetMaxColAccelGEmitter(nativeId, index, value); }
			}

			public float MinColAccelB
			{
				get { return GetMinColAccelBEmitter(nativeId, index); }
				set { SetMinColAccelBEmitter(nativeId, index, value); }
			}

			public float MaxColAccelB
			{
				get { return GetMaxColAccelBEmitter(nativeId, index); }
				set { SetMaxColAccelBEmitter(nativeId, index, value); }
			}

			public float MinColAccelA
			{
				get { return GetMinColAccelAEmitter(nativeId, index); }
				set { SetMinColAccelAEmitter(nativeId, index, value); }
			}

			public float MaxColAccelA
			{
				get { return GetMaxColAccelAEmitter(nativeId, index); }
				set { SetMaxColAccelAEmitter(nativeId, index, value); }
			}

			public float MinColDirAccelR
			{
				get { return GetMinColDirAccelREmitter(nativeId, index); }
				set { SetMinColDirAccelREmitter(nativeId, index, value); }
			}

			public float MaxColDirAccelR
			{
				get { return GetMaxColDirAccelREmitter(nativeId, index); }
				set { SetMaxColDirAccelREmitter(nativeId, index, value); }
			}

			public float MinColDirAccelG
			{
				get { return GetMinColDirAccelGEmitter(nativeId, index); }
				set { SetMinColDirAccelGEmitter(nativeId, index, value); }
			}

			public float MaxColDirAccelG
			{
				get { return GetMaxColDirAccelGEmitter(nativeId, index); }
				set { SetMaxColDirAccelGEmitter(nativeId, index, value); }
			}

			public float MinColDirAccelB
			{
				get { return GetMinColDirAccelBEmitter(nativeId, index); }
				set { SetMinColDirAccelBEmitter(nativeId, index, value); }
			}

			public float MaxColDirAccelB
			{
				get { return GetMaxColDirAccelBEmitter(nativeId, index); }
				set { SetMaxColDirAccelBEmitter(nativeId, index, value); }
			}

			public float MinColDirAccelA
			{
				get { return GetMinColDirAccelAEmitter(nativeId, index); }
				set { SetMinColDirAccelAEmitter(nativeId, index, value); }
			}

			public float MaxColDirAccelA
			{
				get { return GetMaxColDirAccelAEmitter(nativeId, index); }
				set { SetMaxColDirAccelAEmitter(nativeId, index, value); }
			}
		}

		public Emitter[] emitters;

		public Particle(int entity)
		{
			NativeID = entity;

			emitters = new Emitter[Constants.PARTICLE_SYSTEM_EMISSION_MAX];
			for (int i = 0; i < Constants.PARTICLE_SYSTEM_EMISSION_MAX; i++)
			{
				emitters[i] = new Emitter(i, NativeID);
			}
		}

		public int GetEntityID()
		{
			return NativeID;
		}

		public bool Active
		{
			get { return GetActive(NativeID); }
			set { SetActive(NativeID, value); }
		}

		public int RenderOption
		{
			get { return GetRenderOption(NativeID); }
			set { SetRenderOption(NativeID, value); }
		}

		public bool UsePhysics
		{
			get { return GetUsePhysics(NativeID); }
			set { SetUsePhysics(NativeID, value); }
		}

		public void SetUsePhysics(bool value, float gravityX, float gravityY, float drag)
		{
			SetUsePhysics(NativeID, value, gravityX, gravityY, drag);
		}

		public int SpawnParticle()
		{
			return SpawnParticle(NativeID);
		}

		public void KillParticle(int index)
		{
			KillParticle(NativeID, index);
		}

		public int SpawnEmitter()
		{
			return SpawnEmitter(NativeID);
		}

		// binds // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActive(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActive(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int GetRenderOption(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetRenderOption(int instance, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetUsePhysics(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetUsePhysics(int instance, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetUsePhysics(int instance, bool value, float gravityX, float gravityY, float drag);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int SpawnParticle(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void KillParticle(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int SpawnEmitter(int instance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void KillEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetActiveEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetActiveEmitter(int instance, int index, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool GetLoopEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetLoopEmitter(int instance, int index, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetElapsedTimeEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetElapsedTimeEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetRateEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetRateEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetTLoopEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTLoopEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinLifeTimeEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinLifeTimeEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxLifeTimeEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxLifeTimeEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinRotEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinRotEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxRotEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxRotEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinSpinVelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinSpinVelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxSpinVelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxSpinVelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinSpinAccelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinSpinAccelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxSpinAccelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxSpinAccelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinPosXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinPosXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxPosXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxPosXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinPosYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinPosYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxPosYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxPosYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinVelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinVelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxVelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxVelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinVelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinVelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxVelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxVelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleVelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleVelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleVelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleVelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleVelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleVelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleVelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleVelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColVelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColVelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColVelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColVelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColVelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColVelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColVelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColVelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColVelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColVelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColVelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColVelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColVelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColVelAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColVelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColVelAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColAccelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColAccelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColAccelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColAccelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColAccelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColAccelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColAccelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColAccelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColAccelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColAccelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColAccelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColAccelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColAccelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColAccelAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColAccelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColAccelAEmitter(int instance, int index, float value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinSpinDirAccelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinSpinDirAccelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxSpinDirAccelEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxSpinDirAccelEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinDirAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinDirAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxDirAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxDirAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinDirAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinDirAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxDirAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxDirAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleDirAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleDirAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleDirAccelXEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleDirAccelXEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinScaleDirAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinScaleDirAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxScaleDirAccelYEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxScaleDirAccelYEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColDirAccelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColDirAccelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColDirAccelREmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColDirAccelREmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColDirAccelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColDirAccelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColDirAccelGEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColDirAccelGEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColDirAccelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColDirAccelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColDirAccelBEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColDirAccelBEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMinColDirAccelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMinColDirAccelAEmitter(int instance, int index, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float GetMaxColDirAccelAEmitter(int instance, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetMaxColDirAccelAEmitter(int instance, int index, float value);
	}
	#endregion
	// ================================================================== //
}
