/******************************************************************************/
/*!
\file   LevelJson.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for loading level data from json for game

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Linq;
using System.Collections.Generic;

namespace BassNKick.Lib
{
	// Note Information
	public class Note
	{
		public int id;							// id
		//public int barstart;
		public int bar;							// bar position
		public int beat;						// beat position on bar
		public int beatfractiontop;             // sub beat position numerator
		public int beatfractionbottom;          // sub beat position denominator
		public int beatlength;					// length of note in beats
		public int beatlengthfractiontop;       // sub beat length numerator
		public int beatlengthfractionbottom;    // sub beat length denominator
		public int lane;                        // lane
		public int type;                        // note type | 0 = tap / 1 = hold / 2 = directional up / 3 = directional down
		public bool rainbow;					// is rainbow note | has more score + vfx
		//public int musiclayer;                  // music layer (deprecated)

		// runtime
		public double time;						// track pos in seconds
	}

	// Chart Information
	public class Chart
	{
		public int id;						// Chart ID

		public string name;					// Chart Name (usually track name)
		public int difficulty;              // Chart Difficulty

		public float bpm;					// Track BPM
		public int timeSignatureTop;        // Time Signature Top
		public int timeSignatureBottom;     // Time Signature Bottom
		public float duration;              // Track Duration

		public string audioFile;            // Audio File Path

		// notes
		public List<Note> notes;

		public List<Note> lane1;
		public List<Note> lane2;
		public List<Note> lane3;

		// runtime
		public List<Note>? remainingNotes;
		public List<Note>? unspawnedNotes;

		public Chart()
		{
			id = 0;

			name = "";
			difficulty = 0;

			bpm = 0f;
			timeSignatureTop = 0;
			timeSignatureBottom = 0;
			duration = 0f;

			audioFile = "";

			notes = new List<Note>();

			lane1 = new List<Note>();
			lane2 = new List<Note>();
			lane3 = new List<Note>();

			//remainingNotes = new List<Note>();
		}
	}

	public static class LevelJson
	{
		//public static Coroutine initNotes;

		public static Chart LoadChart(string path)
		{
			Chart chart = new Chart();
			try
			{
				// Load json file
				if (Json.LoadJson(path) == IntPtr.Zero)
					throw new Exception("Failed to get json file");

				// Parse json file

				// todo: load info into Settings.cs

				Logger.Critical("LevelJson | Loading Chart " + path);

				chart.name = Json.LoadString("name");
				chart.duration = Json.LoadFloat("duration");
				chart.bpm = Json.LoadFloat("bpm");
				chart.timeSignatureTop = Json.LoadInt("time sig top");
				chart.timeSignatureBottom = Json.LoadInt("time sig bot");

				chart.audioFile = Json.LoadString("audio file path");
				Json.LoadString("save file path");

				IntPtr[] notes = Json.LoadObjArray("notes");

				// Parse notes
				foreach (IntPtr note in notes)
				{
					//IntPtr note = Json.LoadObjWithObj(i);

					// Create new note
					Note newNote = new Note();

					// Parse note data
					newNote.id = Json.LoadObjInt(note, "id");
					//newNote.barstart = Json.LoadObjInt(note, "barstart");
					newNote.bar = Json.LoadObjInt(note, "bar");

					float temp = Json.LoadObjFloat(note, "beat");

					newNote.beat = (int)temp;
					newNote.beatfractiontop = (temp != 0f ?
						(int)Math.Round((temp - newNote.beat) * chart.timeSignatureBottom) : 0);
					newNote.beatfractionbottom = chart.timeSignatureBottom; // no varied time sig currently
					//newNote.beatfractiontop = Json.LoadObjInt(note, "beatfractiontop");
					//newNote.beatfractionbottom = Json.LoadObjInt(note, "beatfractionbot");

					temp = Json.LoadObjFloat(note, "beatlength");

					newNote.beatlength = (int)temp;
					newNote.beatlengthfractiontop = (temp != 0f ?
						(int)Math.Round((temp - newNote.beatlength) * chart.timeSignatureBottom) : 0);
					newNote.beatlengthfractionbottom = chart.timeSignatureBottom; // no varied time sig currently
					//newNote.beatlengthfractiontop = Json.LoadObjInt(note, "beatlengthfractiontop");
					//newNote.beatlengthfractionbottom = Json.LoadObjInt(note, "beatlengthfractionbot");

					newNote.lane = Json.LoadObjInt(note, "lane"); 

					switch (newNote.lane)// correction
					{
						case 0:
							newNote.lane = 3;
							break;
						case 1:
							newNote.lane = 2;
							break;
						case 2:
							newNote.lane = 1;
							break;
					}

					newNote.type = Json.LoadObjInt(note, "notetype");
					newNote.rainbow = Json.LoadObjBool(note, "rainbow");
					//newNote.musiclayer = Json.LoadObjInt(note, "musiclayer");

					// Add note to chart
					chart.notes.Add(newNote);

					// Add note to lane
					switch (newNote.lane)
					{
						case 1:
							chart.lane1.Add(newNote);
							break;
						case 2:
							chart.lane2.Add(newNote);
							break;
						case 3:
							chart.lane3.Add(newNote);
							break;
					}

					Logger.Info("Loaded note " + newNote.id + " of type " + newNote.type + " at " + newNote.bar + ":" + 
						newNote.beat + ":" + newNote.beatfractiontop + ":" + newNote.beatfractionbottom + " | " +
						newNote.beatlength + ":" + newNote.beatlengthfractiontop + ":" + newNote.beatlengthfractionbottom +
						" on lane " + newNote.lane);
				}

				// Close json file
				Json.Close();
				Json.ClearObjs();

				// Add runtime info
				//chart.remainingNotes = new List<Note>();
				//chart.unspawnedNotes = new List<Note>();
				//chart.remainingNotes = new List<Note>(chart.notes);
				//chart.unspawnedNotes = new List<Note>(chart.notes);

				Logger.Critical("LevelJson | Finished Loading");
			}
			catch (System.Exception e)
			{
				// Log error
				Logger.EngineError($"C# | Failed to parse chart data | {e.Message}");
			}
			// Return chart data
			return chart;
		}

		// Initialize chart
		public static void InitChart(Chart chart, GameManager gameManager)
		{
			if (gameManager.track == null) throw new System.Exception("Track is null");
			if (gameManager.settings == null) throw new System.Exception("Settings is null");

			// Initialize time for notes

			//Logger.Info(gameManager.track.beatTimeLength.ToString());
			//Logger.Info(gameManager.settings.timeSigTop.ToString());

			foreach (Note note in chart.notes)
			{
				note.time = gameManager.track.beatTimeLength * 
					(double)((float)(note.bar * gameManager.settings.timeSigTop + note.beat));

				if (note.beatfractionbottom > 0 && note.beatfractiontop > 0)
					note.time += gameManager.track.beatTimeLength *
						(double)((float)note.beatfractiontop / (float)note.beatfractionbottom);

				// Add note to lane
				//switch (note.lane)
				//{
				//	case 1:
				//		chart.lane1.Add(note);
				//		break;
				//	case 2:
				//		chart.lane2.Add(note);
				//		break;
				//	case 3:
				//		chart.lane3.Add(note);
				//		break;
				//}

				// Add runtime info
				//chart.remainingNotes.Add(note);
				//chart.unspawnedNotes.Add(note);
			}

			// Add runtime info
			chart.remainingNotes = new List<Note>(chart.notes);
			chart.unspawnedNotes = new List<Note>(chart.notes);

			// Sort notes

			chart.lane1 = chart.lane1.OrderBy(o => o.time).ToList();
			chart.lane2 = chart.lane2.OrderBy(o => o.time).ToList();
			chart.lane3 = chart.lane3.OrderBy(o => o.time).ToList();

			chart.remainingNotes = chart.remainingNotes.OrderBy(o => o.time).ToList();
			chart.unspawnedNotes = chart.unspawnedNotes.OrderBy(o => o.time).ToList();

			foreach (Note n in chart.remainingNotes)
			{
				Logger.Info("Note " + n.id + " at " + n.time);
			}

			int noteC = chart.notes.Count;
			//noteC = 300;
			gameManager.feverObj.GetScript<Fever>().SetProgPerNote((int)((float)noteC * gameManager.settings.feverProgMod));
		}

		// Set note properties
		public static void SetNote(NoteProperties note, Note n)
		{
			note.id = n.id;
			note.bar = n.bar;
			note.beat = n.beat;
			note.beatFractionTop = n.beatfractiontop;
			note.beatFractionBot = n.beatfractionbottom;
			note.beatLength = n.beatlength;
			note.beatLengthFractionTop = n.beatlengthfractiontop;
			note.beatLengthFractionBot = n.beatlengthfractionbottom;
			note.type = n.type;
			note.rainbow = n.rainbow;
			//note.musicLayer = n.musiclayer;
			note.lane = n.lane;
		}

		// Build chart (deprecated)
		public static List<Entity> BuildChart(Chart chart, Entity lane1, Entity lane2, Entity lane3, 
			Entity note, GameManager gameManager)
		{
			List<Note> notes = chart.notes;
			List<Entity> newNotes = new List<Entity>();

			foreach (Note n in notes)
			{
				Entity newNote = Entity.Instantiate(note);

				NoteProperties noteProperties = newNote.GetScript<NoteProperties>();
				//noteProperties.barStart = n.barstart;

				SetNote(noteProperties, n);

				noteProperties.trackObj = gameManager.TrackObj;
				noteProperties.gameUIObj = gameManager.uiObj;

				switch (n.lane)
				{
					case 1:
						newNote.Hierarchy.Parent = lane1;
						//noteProperties.lane = 1;
						break;
					case 2:
						newNote.Hierarchy.Parent = lane2;
						//noteProperties.lane = 2;
						break;
					case 3:
						newNote.Hierarchy.Parent = lane3;
						//noteProperties.lane = 3;
						break;
				}

				newNotes.Add(newNote);
			}

			return newNotes;
		}
	}
}
