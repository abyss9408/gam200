/******************************************************************************/
/*!
\file   DotNetHotReload.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing .NET hot reload functionality
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
using System.Diagnostics;
using System.Reflection;

using Team_Popplio.Libraries;

namespace Team_Popplio
{
	public class DotNetHotReload : IDisposable
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

		//private static DotNetHotReload instance; // singleton instance

		// callback for hot reload events
		public event Action OnHotReloadTriggered;

		private static FileSystemWatcher? watcher;

		private void NewInstance()
		{
			//instance = new DotNetHotReload();
			string directoryPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"..\..\Team_Popplio_Script";
			watcher = new FileSystemWatcher(directoryPath)
			{
				NotifyFilter = NotifyFilters.LastWrite
							   | NotifyFilters.FileName
							   | NotifyFilters.DirectoryName,

				// specify specific file extensions to watch
				Filter = "*.cs"
			};

			// setup event handlers
			watcher.Changed += OnFileChanged;
			watcher.Created += OnFileChanged;
			watcher.Deleted += OnFileChanged;
			watcher.Renamed += OnFileRenamed;

			// start watching
			watcher.EnableRaisingEvents = true;
		}

		public DotNetHotReload()
		{
			//instance = this;

			NewInstance();

			OnHotReloadTriggered += OnHotReload;
		}

		private void OnFileChanged(object sender, FileSystemEventArgs e)
		{
			Logger.EngineInfo($"File {e.ChangeType}: {e.FullPath} has changed!");

			// trigger build
			TriggerBuild(e.FullPath);
		}

		private void OnFileRenamed(object sender, RenamedEventArgs e)
		{
			Logger.EngineInfo($"File renamed from {e.OldFullPath} to {e.FullPath}!");

			// trigger build
			TriggerBuild(e.FullPath);
		}

		// Singleton access
		//public static DotNetHotReload Instance
		//{
		//	get
		//	{
		//		//instance ??= new DotNetHotReload();
		//		return instance;
		//	}
		//}

		private void OnHotReload() // hot reload event handler
		{

		}

		//private void OnHotReloadDetected(Type[] changedTypes)
		//{
		//	try
		//	{
		//		// Invoke custom reload logic
		//		OnHotReloadTriggered?.Invoke();

		//		// Log changed types
		//		foreach (var type in changedTypes)
		//		{
		//			Console.WriteLine($"Hot Reload: Type changed - {type.FullName}");
		//		}
		//	}
		//	catch (Exception ex)
		//	{
		//		Console.WriteLine($"Hot Reload Error: {ex.Message}");
		//	}
		//}

		public void TriggerBuild(string projectPath)
		{
			try
			{
				var process = new Process
				{
					StartInfo = new ProcessStartInfo
					{
						FileName = "dotnet",
						Arguments = $"build \"{projectPath}\"",
						RedirectStandardOutput = true,
						UseShellExecute = false,
						CreateNoWindow = true
					}
				};

				process.OutputDataReceived += (sender, e) =>
				{
					if (!string.IsNullOrEmpty(e.Data))
						Console.WriteLine(e.Data);
				};

				process.ErrorDataReceived += (sender, e) =>
				{
					if (!string.IsNullOrEmpty(e.Data))
						Console.Error.WriteLine(e.Data);
				};

				process.Start();
				process.BeginOutputReadLine();
				process.BeginErrorReadLine();
				process.WaitForExit();

				// trigger reload event after successful build
				if (process.ExitCode == 0)
				{
					OnHotReloadTriggered?.Invoke();
				}
			}
			catch (Exception ex)
			{
				Logger.EngineError($"Build Error: {ex.Message}");
			}
		}

		// manually trigger reload
		public void TriggerReload()
		{
			OnHotReloadTriggered?.Invoke();
		}
	}
}
