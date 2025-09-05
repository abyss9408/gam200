/******************************************************************************/
/*!
\file   Profile.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Hafawati Binte Mohd Ali
\contribution Shawn - 60%, Hafawati - 40%
\par    Course : CSD2451 / CSD 2450
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for player profile info in BassNKick
		Scores, etc get deserialized from JSON file here

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;

namespace BassNKick.Lib
{
	public static class Profile
	{
		public struct Track
		{
			public int score;
			public int id;
			public bool unlocked;
		};
		
		public static string profileFile = "profile.json";
		public static string profileDefaultFile = "profile_default.json";

		#region Player
		// profile
		public static int playerID = 0;
		public static string playerName = "Guest";
		public static bool playerNoob = true;
		public static int playerAvatar = 0;
		public static int playerLevel = 0;
		public static float playerExp = 0f;
		public static List<Track>? playerTracks; // = new List<Track>();
		public static string currentCutscene = "";
		#endregion

		public static void Load()
		{
			try
			{
				if (Json.LoadJson(profileFile) == IntPtr.Zero)
				{
					Logger.Critical("Profile.cs | Profile not found, attempting to initialize defaults");
					Save();
					if (Json.LoadJson(profileFile) == IntPtr.Zero)
						throw new Exception("Failed to get Json file");
				}
				Logger.Critical("Profile.cs | Loading Settings " + profileFile);
				
				// load information
				playerID = Json.LoadInt("id");
				playerName = Json.LoadString("name");
				playerNoob = Json.LoadBool("noob");
				playerAvatar = Json.LoadInt("avatar");
				playerLevel = Json.LoadInt("level");
				playerExp = Json.LoadFloat("exp");

                // TODO FIX
                // Parse tracks
                //IntPtr[] tracks = Json.LoadObjArray("tracks");

                //foreach (IntPtr track in tracks)
                //{
                //	Track temp = new Profile.Track();
                //	temp.id = Json.LoadObjInt(track, "id");
                //	temp.score = Json.LoadObjFloat(track, "score");
                //	temp.unlocked = Json.LoadObjBool(track, "unlocked");
                //	playerTracks.Add(temp);
                //}

				if (playerTracks == null) playerTracks = new List<Track>();
				playerTracks.Clear();

                for (int i = 1; i <= 3; i++)
                {
                    int s = Json.LoadInt(i + "_score");     // e.g. "1_score"
                    bool u = Json.LoadBool(i + "_unlocked");    // e.g. "1_unlocked"
                    Track t = new Track()
                    {
                        id = i,
                        score = s,
                        unlocked = u
                    };
                    playerTracks.Add(t);
                    Logger.Critical("Profile.cs | Track: " + t.id + " | Score: " + t.score);

                }

				currentCutscene = Json.LoadString("current cutscene");

				// Close json file
				Json.Close();
				Json.ClearObjs();

				Logger.Critical("Profile.cs | Loaded Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading profile: " + e.Message);
			}
		}

		public static void Save()
		{
			try
			{
				Json.SaveJson1();

				// save information
				Json.SaveInt("id", playerID, IntPtr.Zero);
				Json.SaveString("name", playerName, IntPtr.Zero);
				Json.SaveBool("noob", playerNoob, IntPtr.Zero);
				Json.SaveInt("avatar", playerAvatar, IntPtr.Zero);
				Json.SaveInt("level", playerLevel, IntPtr.Zero);
				Json.SaveFloat("exp", playerExp, IntPtr.Zero);

				// TODO FIX
				//IntPtr tracks = Json.NewArrayValue();
				//foreach (Track track in playerTracks)
				//{
				//	IntPtr trackObj = Json.NewObjValue();
				//	Json.SaveInt("id", track.id, trackObj);
				//	Json.SaveFloat("score", track.score, trackObj);
				//	Json.SaveBool("unlocked", track.unlocked, trackObj);
				//	Json.SaveObj("", trackObj, tracks);
				//}
				//Json.SaveObj("tracks", tracks, IntPtr.Zero);

				if (playerTracks == null)
				{
					playerTracks = new List<Track>();
					playerTracks.Add(new Track() { id = 1, score = 0, unlocked = false });
					playerTracks.Add(new Track() { id = 2, score = 0, unlocked = false });
					playerTracks.Add(new Track() { id = 3, score = 0, unlocked = false });
					Logger.Error("playerTracks was null, creating new list");
				}

				Json.SaveInt("1_score", playerTracks[0].score, IntPtr.Zero);
                Json.SaveBool("1_unlocked", playerTracks[0].unlocked, IntPtr.Zero);
                Json.SaveInt("2_score", playerTracks[1].score, IntPtr.Zero);
                Json.SaveBool("2_unlocked", playerTracks[1].unlocked, IntPtr.Zero);
                Json.SaveInt("3_score", playerTracks[2].score, IntPtr.Zero);
                Json.SaveBool("3_unlocked", playerTracks[2].unlocked, IntPtr.Zero);

				Json.SaveString("current cutscene", currentCutscene, IntPtr.Zero);

				Json.SaveJson2(profileFile);
			}
			catch (Exception e)
			{
				Logger.Error("Error saving profile: " + e.Message);
			}
		}

        public static void UpdateTrackScore(int trackID, int newScore)
        {

            int index = trackID - 1;
            if (index < 0 || index >= 3)
            {
                Logger.Info("UpdateTrackScore() was called with invalid trackID=" + trackID);
                return;
            }

            if (playerTracks == null)
            {
                playerTracks = new List<Track>();
            }

            // If haven't allocate the list for 3 tracks
            while (playerTracks.Count < 3)
            {
                playerTracks.Add(new Track() { id = playerTracks.Count + 1, score = 0, unlocked = false });
            }

            // Overwrite track if new score is higher
            Track t = playerTracks[index];
            if (newScore > t.score)
            {
                t.score = newScore;
                t.unlocked = true;
                playerTracks[index] = t;
            }

            //Logger.Critical("Profile.cs | Track: " + t.id + " | Score: " + t.score);
			Save();
			//Logger.Critical("Done");
		}
    }
}
