/******************************************************************************/
/*!
\file   ScoreUI.cs
\author Hafawati Binte Mohd Ali
\par    Course : CSD2450
\par    Section : A
\date   2025/02/06
\brief
        This script manages the display of score-related UI elements in the game.

        The `ScoreUI` class extends `PopplioMonoScript` and dynamically updates text
        components to reflect the player's score. It retrieves score data from an
        associated `Score` script and assigns values to UI elements representing
        different scoring categories (Perfect, Nice, Bad, Miss).

        Features:
        - Retrieves a `Score` script from a linked entity (`scoreObj`).
        - Finds and assigns `Text2D` components for displaying score values.
        - Updates text elements in real-time based on the current score.
        - Uses the entity hierarchy to locate relevant UI components dynamically.
        - Hides both score values and labels when R, F, or V key is pressed.

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
using Team_Popplio.Attributes;
using Team_Popplio.Libraries;

using BassNKick.Lib;
using Team_Popplio.Extern;

namespace BassNKick
{
    public class ScoreUI : PopplioMonoScript
    {
        [Inspector("Score System")]
        public Entity scoreObj = Entity.EntityField();
        private Score? score;

        private Text2D? perfectScoreText;
        private Text2D? niceScoreText;
        private Text2D? badScoreText;
        private Text2D? missScoreText;

        //private Text2D? perfectLabelText;
        //private Text2D? niceLabelText;
        //private Text2D? badLabelText;
        //private Text2D? missLabelText;

        public override void Setup()
        {
            if (scoreObj != null && scoreObj.Exists() && scoreObj.HasScript<Score>())
                score = scoreObj.GetScript<Score>();

            // Find text components under the ScoreUI parent entity
            foreach (Entity child in This.Hierarchy.Children)
            {
                if (!child.HasComponent<Text2D>()) continue;

                Text2D textComponent = child.GetComponent<Text2D>();

                // Assign based on position Y
                if (textComponent.Text == "0")
                {
                    if (child.Transform2D.Position.Y > 300)
                        perfectScoreText = textComponent;
                    else if (child.Transform2D.Position.Y > 250)
                        niceScoreText = textComponent;
                    else if (child.Transform2D.Position.Y > 200)
                        badScoreText = textComponent;
                    else
                        missScoreText = textComponent;
                }
                //else
                //{
                //    if (child.Transform2D.Position.Y > 300)
                //        perfectLabelText = textComponent;
                //    else if (child.Transform2D.Position.Y > 250)
                //        niceLabelText = textComponent;
                //    else if (child.Transform2D.Position.Y > 200)
                //        badLabelText = textComponent;
                //    else
                //        missLabelText = textComponent;
                //}
            }
        }

        public override void Update()
        {
            if (score == null) return;

            //bool hideUI = Input.IsKeyDown(Keybinds.lane1) || Input.IsKeyDown(Keybinds.lane2) || Input.IsKeyDown(Keybinds.lane3);


            // Hide the Score UI and Labels when a key is pressed, but still update scores
            if (perfectScoreText != null)
            {
                //perfectScoreText.IsActive = !hideUI;
                perfectScoreText.Text = $"{score.perfect}";
            }
            if (niceScoreText != null)
            {
                //niceScoreText.IsActive = !hideUI;
                niceScoreText.Text = $"{score.nice}";
            }
            if (badScoreText != null)
            {
                //badScoreText.IsActive = !hideUI;
                badScoreText.Text = $"{score.bad}";
            }
            if (missScoreText != null)
            {
                //missScoreText.IsActive = !hideUI;
                missScoreText.Text = $"{score.miss}";
            }

            // Hide labels as well
            //if (perfectLabelText != null) perfectLabelText.IsActive = !hideUI;
            //if (niceLabelText != null) niceLabelText.IsActive = !hideUI;
            //if (badLabelText != null) badLabelText.IsActive = !hideUI;
            //if (missLabelText != null) missLabelText.IsActive = !hideUI;
        }
    }
}
