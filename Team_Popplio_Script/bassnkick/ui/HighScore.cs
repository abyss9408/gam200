/******************************************************************************/
/*!
\file   HighScore.cs
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/04/06
\brief
        This script displays the highest score achieved for each level.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using Team_Popplio.Attributes;
using System.Collections.Generic;
using BassNKick.Lib;
using Microsoft.CodeAnalysis.VisualBasic.Syntax;

namespace BassNKick
{
    public class HighScore : PopplioMonoScript
    {

        private Text2D[] scoreTexts = new Text2D[3];

        public override void Setup()
        {
            // Find text components under this entity
            int index = 0;
            foreach (Entity child in This.Hierarchy.Children)
            {
                if (!child.HasComponent<Text2D>()) continue;

                Text2D textComponent = child.GetComponent<Text2D>();

                // Check if this is a score text (showing "0" initially)
                if (textComponent.Text == "0" && index < 3)
                {
                    scoreTexts[index] = textComponent;
                    index++;
                }
            }

            UpdateScoreDisplay();
        }

        private void UpdateScoreDisplay()
        {
            // Make sure we have valid text components
            if (scoreTexts == null)
            {
                Logger.Warning("HighScore: scoreTexts array is not properly initialized");
                return;
            }

            // Get scores from Profile
            var tracks = Profile.playerTracks;

            if (tracks == null)
            {
                Logger.Warning("HighScore: playerTracks is null in Profile");
                return;
            }

            // Update each score text
            for (int i = 0; i < scoreTexts.Length; i++)
            {
                if (scoreTexts[i] != null)
                {
                    float score = (i < tracks.Count) ? tracks[i].score : 0f;
                    //float scorePercentage = score / 1000000f;
                    //if (scorePercentage >= 1f) // S Rank
                    //    scoreTexts[i].Color = Color.Magenta;
                    //else if (scorePercentage >= 0.9f) // A
                    //    scoreTexts[i].Color = Color.Yellow;
                    //else if (scorePercentage >= 0.7f) // B
                    //    scoreTexts[i].Color = Color.Blue;
                    //else
                    //    scoreTexts[i].Color = Color.Gray;

                    scoreTexts[i].Text = score.ToString();

                }
            }
        }
    }
}
