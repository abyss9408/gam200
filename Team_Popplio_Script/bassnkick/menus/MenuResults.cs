/******************************************************************************/
/*!
\file   MenuResults.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Hafawati Binte Mohd Ali
\contribution Shawn - 90%, Hafawati - 10%
\par    Course : CSD2401 / CSD2450 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the menu results after a level

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;
using BassNKick.Lib;
using Team_Popplio.Extern;

namespace BassNKick
{
    public class MenuResults : PopplioMonoScript
    {
        //[Inspector("Score Obj")]
        //Entity scoreObj = Entity.EntityField();
        //Score score;

        //public static Score score;

        //public static int currScore = 0;
        //public static int currMaxCombo = 0;
        //public static int perfect = 0;
        //public static int nice = 0;
        //public static int bad = 0;
        //public static int miss = 0;

        [Inspector("Result Obj")]
        Entity[] resultObj = { };

        List<Text2D> results = new List<Text2D>();
        List<float> resultVals = new List<float>();
        List<int> initialFontSize = new List<int>();

        float timer = 0f;

        float animSpeed = 2f;

        //float defaultFontSize = 28f;

        int currResult = 0;

        public Entity rankObj = Entity.EntityField();
        Render2D? rank;
        public Entity manaObj = Entity.EntityField();
        Render2D? mana;
        public Entity stellarObj = Entity.EntityField();
        Render2D? stellar;

        public Entity fcObj = Entity.EntityField();

        Vector2<float> initialRankSize = new Vector2<float>(1f, 1f);
        Vector2<float> initialManaSize = new Vector2<float>(1f, 1f);
        Vector2<float> initialStellarSize = new Vector2<float>(1f, 1f);
        Vector2<float> initialManaPos = new Vector2<float>(1f, 1f);

        float rankAnimSpeed = 5f;

        public override void Setup()
        {
            //score = scoreObj.GetScript<Score>();

            rank = rankObj.GetComponent<Render2D>();
            mana = manaObj.GetComponent<Render2D>();
            stellar = stellarObj.GetComponent<Render2D>();

            initialRankSize = rankObj.Transform2D.Scale;
            //initialManaSize = manaObj.Transform2D.Scale;
            initialStellarSize = stellarObj.Transform2D.Scale;
            initialManaPos = manaObj.Transform2D.Position;

            foreach (Entity res in resultObj)
            {
                results.Add(res.GetComponent<Text2D>());
                initialFontSize.Add((int)res.GetComponent<Text2D>().FontSize);
                res.Active = false;
            }
        }

        // Start is called before the first frame update
        public override void Init()
        {
            if (mana == null)
            {
                Logger.Error("Mana Sprite is null");
                return;
            }

            ///score = FindObjectOfType<Score>(); // TODO add find functions for scripts / components

            foreach (Text2D text in results) text.Text = "0";

            //resultVals.Add(Lib.StaticResults.currScore);
            resultVals.Add(Lib.StaticResults.scorePercent);
            resultVals.Add(Lib.StaticResults.currMaxCombo);
            resultVals.Add(Lib.StaticResults.perfect);
            resultVals.Add(Lib.StaticResults.nice);
            resultVals.Add(Lib.StaticResults.bad);
            resultVals.Add(Lib.StaticResults.miss);

            //resultVals.Add(score.currScore);
            //resultVals.Add(score.currMaxCombo); // convert to percentage of max score in M3
            //resultVals.Add(score.perfect);
            //resultVals.Add(score.nice);
            //resultVals.Add(score.bad);
            //resultVals.Add(score.miss);

            Logger.Critical("Current Score: " + Lib.StaticResults.currScore);
            Logger.Critical("Current Max Combo: " + resultVals[1]);
            Logger.Critical("Perfect: " + resultVals[2]);
            Logger.Critical("Nice: " + resultVals[3]);
            Logger.Critical("Bad: " + resultVals[4]);
            Logger.Critical("Miss: " + resultVals[5]);
            Logger.Critical("Score Percent: " + resultVals[0]);
            Logger.Critical("Rank: " + Lib.StaticResults.rank);

            string currentScene = SceneManager.GetPrevious();
            int trackID = 0;

            switch (currentScene)
            {
                case "Track1": trackID = 1; break;
                case "Track2": trackID = 2; break;
                case "Track3": trackID = 3; break;
                default:
                    trackID = 0;
                    break;
            }
            if (trackID != 0)
            {
                int finalScore = (int)Math.Round(Lib.StaticResults.scorePercent * 1000000f, MidpointRounding.AwayFromZero);
                BassNKick.Lib.Profile.UpdateTrackScore(trackID, finalScore);
                Logger.Critical("TrackID: " + trackID + " -> updated track score in profile.json");
            }
            else
            {
                Logger.Info("TrackID is 0, skipping UpdateTrackScore call");
            }

            if (rank == null) return;

            switch (Lib.StaticResults.rank)
            {
                case 0:
                    rank.Texture = "UI_ResultScreen_S_Rank";
                    mana.Texture = "UI_ResultScreen_Thumbsup";
                    manaObj.Transform2D.Scale = new Vector2<float>(618f, 646f);
                    break;
                case 1:
                    rank.Texture = "UI_ResultScreen_A_Rank";
                    mana.Texture = "UI_ResultScreen_Thumbsup";
                    manaObj.Transform2D.Scale = new Vector2<float>(618f, 646f);
                    break;
                case 2:
                    rank.Texture = "UI_ResultScreen_B_Rank";
                    mana.Texture = "UI_ResultScreen_Thumbsdown";
                    manaObj.Transform2D.Scale = new Vector2<float>(531f, 451f) * 1.2f;
                    manaObj.Transform2D.Position += new Vector2<float>(50f, -25f);
                    break;
                default:
                    rank.Texture = "UI_ResultScreen_F_Rank";
                    mana.Texture = "UI_ResultScreen_Thumbsdown";
                    manaObj.Transform2D.Scale = new Vector2<float>(531f, 451f) * 1.2f;
                    manaObj.Transform2D.Position += new Vector2<float>(50f, -25f);
                    break;
            }
            initialManaSize = manaObj.Transform2D.Scale;
        }

        // Update is called once per frame
        public override void Update()
        {
            // debug
            //if (Input.IsKeyDown(GLFW.KEY_1)) SceneManager.Load("MainMenu");
            //if (Input.IsKeyDown(GLFW.KEY_2)) SceneManager.Load("Track1");
            //if (Input.IsKeyDown(GLFW.KEY_3)) SceneManager.Load("Results");

            if (TempInputFix.IsKeyTriggered(Keybinds.back) || Input.IsKeyTriggered(Keybinds.press) ||
                Input.IsKeyTriggered(Keybinds.pause) || TempInputFix.IsMouseTriggered(Input.MouseButton.Left))
            {
                //SceneManager.Load("LevelSelect");

                if (SceneManager.GetPrevious() == "Track1")
                    StaticCutsceneData.PlayThisCutscene = "Cutscene_2_Level1end_1";
                else if (SceneManager.GetPrevious() == "Track2")
                    StaticCutsceneData.PlayThisCutscene = "Cutscene_3_Level2end_1";
                else if (SceneManager.GetPrevious() == "Track3")
                    StaticCutsceneData.PlayThisCutscene = "Finall";

                SceneManager.LoadScene("Cutscene");
                TempInputFix.ResetKeyCooldown();

            }

            AnimateResults();

            AnimateRank();

            if (timer < 0f && StaticResults.miss == 0)
            {
                if (!fcObj.Active) GameUI.StaticFC(fcObj);
                GameUI.UpdateFC(fcObj);
            }
        }

        public override void End()
        {
            manaObj.Transform2D.Position = initialManaPos;
        }

        void AnimateResults()
        {
            if (currResult >= resultVals.Count) return;

            if (resultObj[currResult].Active == false)
            {
                resultObj[currResult].Active = true;
            }

            timer += Time.DeltaTimeF * animSpeed;
            float clampedTimer = timer > 1f ? 1f : timer;

            //if (currResult == 0) defaultFontSize = 40f;
            //else if (currResult == 1) defaultFontSize = 45f;
            //else defaultFontSize = 28f;

            // to animate feedback
            results[currResult].FontSize = (uint)(initialFontSize[currResult] * UILibrary.EaseOutElastic(timer));

            if (currResult == 0) results[currResult].Text = (resultVals[currResult] * 1000000f * clampedTimer).ToString("0000000");
            else results[currResult].Text = ((int)(resultVals[currResult] * clampedTimer)).ToString();

            if (timer > 1f)
            {
                timer = 0f;
                ++currResult;
            }
        }

        void AnimateRank()
        {
            if (mana == null)
            {
                Logger.Error("Mana Sprite is null");
                return;
            }
            if (rank == null)
            {
                Logger.Error("Rank Sprite is null");
                return;
            }
            if (stellar == null)
            {
                Logger.Error("Stellar Sprite is null");
                return;
            }

            if (currResult < resultVals.Count) return;
            if (timer < 0f) return;

            timer += Time.DeltaTimeF * rankAnimSpeed;

            if (!rankObj.Active)
            {
                if (!manaObj.Active) manaObj.Active = true;

                mana.Color = new Color(1f, 1f, 1f, timer);
                manaObj.Transform2D.Scale = initialManaSize * UILibrary.EaseOutElastic(timer);

                if (timer > 1f)
                {
                    timer = 0f;
                    rankObj.Active = true;
                    mana.Color = Color.White;
                }
            }
            else if (!stellarObj.Active)
            {
                rank.Color = new Color(1f, 1f, 1f, timer);
                rankObj.Transform2D.Scale = initialRankSize * UILibrary.EaseOutElastic(timer);
                if (timer > 1f)
                {
                    timer = 0f;
                    if (Lib.StaticResults.rank == 0) stellarObj.Active = true;
                    else timer = -1f;
                    rank.Color = Color.White;
                }
            }
            else
            {
                stellar.Color = new Color(1f, 1f, 1f, timer);
                stellarObj.Transform2D.Scale = initialStellarSize * UILibrary.EaseOutElastic(timer);
                if (timer > 1f)
                {
                    timer = -1f;
                    stellar.Color = Color.White;
                }
            }
        }
    }
}
