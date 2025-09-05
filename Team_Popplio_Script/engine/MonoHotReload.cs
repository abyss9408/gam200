/******************************************************************************/
/*!
\file   MonoHotReload.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing mono hot reload functionality
		This class is responsible for detecting changes in the script files and trigger a rebuild of the project

		Currently does not work TODO: FIX

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.IO;
using System.Reflection;

using Team_Popplio.Libraries;

namespace Team_Popplio
{
	public class MonoHotReload : IDisposable
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

		private string assemblyPath;
		private DateTime lastWrite;
		private FileSystemWatcher observer;
		private Assembly? currentAssembly;
		private object reloadLock = new object();

		public MonoHotReload(string path) // constructor
		{
			assemblyPath = path;
			lastWrite = File.GetLastWriteTime(assemblyPath);

			// set up the file system watcher
			observer = new FileSystemWatcher(Path.GetDirectoryName(assemblyPath));
			observer.Filter = Path.GetFileName(assemblyPath);
			observer.Changed += OnAssemblyChanged;
			observer.EnableRaisingEvents = true;
		}


		// when assembly changes, update
		private void OnAssemblyChanged(object sender, FileSystemEventArgs e)
		{
			lock (reloadLock)
			{
				var currentTime = File.GetLastWriteTime(e.FullPath);
				if (currentTime != lastWrite)
				{
					try // try reload
					{
						ReloadAssembly();
						lastWrite = currentTime; // update last write time
					}
					catch (Exception except)
					{
						Logger.EngineError("Error reloading assembly: " + except.Message);
					}
				}
			}
		}

		// reload assembly
		private void ReloadAssembly()
		{
			currentAssembly = null; // unload prev assembly

			currentAssembly = Assembly.LoadFrom(assemblyPath); // load new assembly

			// invoke reload method
			var entryType = currentAssembly.GetType("Team_Popplio.PopplioMonoScript"); // get entry type
			var reloadMethod = entryType.GetMethod("OnHotReload"); // get reload method
			if (reloadMethod != null)
			{
				var instance = Activator.CreateInstance(entryType); // create instance
				reloadMethod.Invoke(instance, null); // invoke reload method
			}

			Logger.EngineInfo("Assembly reloaded successfully!");
		}

		// called on hot reload
		public void OnHotReload()
		{
			Logger.EngineInfo("C# Changes detected, hot reloaded and applied!");
		}
	}
}
