/******************************************************************************/
/*!
\file   TestUI.cs
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/02/06
\brief
This script is a test class for UI interaction in the Popplio engine.
It is responsible for handling UI components dynamically. It verifies UI component 
existence, retrieves and logs UI properties, and registers UI click callbacks for
interaction testing.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;
using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using System.Collections.Generic;
using System.Data.Linq.Mapping;

namespace BassNKick.test
{
    public class TestUI : PopplioMonoScript
    {
        private UI? uiComponent = null;

        public override void Setup()
        {
            Logger.Info("TestUI.cs | Setup()");

            if (This.HasComponent<UI>())
            {
                uiComponent = This.GetComponent<UI>();
                Logger.Info("UIComponent Found!");
            }
            else
            {
                Logger.Error("UIComponent NOT Found on Entity!");
            }
        }

        public override void Init()
        {
            Logger.Info("TestUI.cs | Init()");

            if (uiComponent == null)
            {
                Logger.Error("UIComponent is NULL, cannot initialize.");
                return;
            }

            Logger.Info("=== UIComponent Properties ===");
            Logger.Info($"UI Type: {uiComponent.Type}");
            Logger.Info($"UI Active: {uiComponent.Active}");
            Logger.Info($"Has Hover Effect: {uiComponent.HasHoverEffect}");
            Logger.Info($"Is Hovered: {uiComponent.IsHovered}");

            if (uiComponent.HasHoverEffect)
            {
                Logger.Info($"Hover Frame (X,Y): ({uiComponent.HoverFrameX}, {uiComponent.HoverFrameY})");
            }

            Logger.Info($"Frame (X,Y): ({uiComponent.FrameX}, {uiComponent.FrameY})");
            Logger.Info($"Total Columns: {uiComponent.TotalColumns}");
            Logger.Info($"Total Rows: {uiComponent.TotalRows}");
            Logger.Info($"Sheet Width: {uiComponent.SheetWidth}");
            Logger.Info($"Sheet Height: {uiComponent.SheetHeight}");

            if (uiComponent.Type == UIType.BUTTON)
            {
                //Logger.Info("=== Testing OnClick Actions ===");

                //List<string> existingActions = new List<string>(uiComponent.GetOnClickActions());
                //Logger.Info($"Existing OnClick Actions: {(existingActions.Count > 0 ? string.Join(", ", existingActions) : "None")}");

                //Logger.Info("Adding Actions: 'MEOWMEOW' ");
                //uiComponent.AddOnClickAction("MEOWMEOW");

                //List<string> updatedActions = new List<string>(uiComponent.GetOnClickActions());
                //Logger.Info($"Updated OnClick Actions: {(updatedActions.Count > 0 ? string.Join(", ", updatedActions) : "None")}");

                //Logger.Info("Removing 'MEOWMEOW'");
                //uiComponent.RemoveOnClickAction("MEOWMEOW");
                //List<string> afterRemoveActions = new List<string>(uiComponent.GetOnClickActions());
                //Logger.Info($"After Removing 'MEOWMEOW': {(afterRemoveActions.Count > 0 ? string.Join(", ", afterRemoveActions) : "None")}");

                //Logger.Info("Overwriting Actions: [ 'RUN', 'ATTACK']");
                //string[] newActions = { "RUN", "ATTACK" };
                //uiComponent.SetOnClickActions(newActions);

                //List<string> finalActions = new List<string>(uiComponent.GetOnClickActions());
                //Logger.Info($"Final OnClick Actions after SetOnClickActions: {(finalActions.Count > 0 ? string.Join(", ", finalActions) : "None")}");

                Logger.Info("Registering UI Callbacks from C#");

                if (This.HasComponent<UI>())
                {
                    UI uiComponent = This.GetComponent<UI>();

                    // Correct registration (fixes delegate issue)
                    uiComponent.RegisterOnClickCallback("MEOWWWWWERRS", MeowFunction);
                    uiComponent.RegisterOnClickCallback("JUMP", JumpFunction);

                    Logger.Info("Registered Callbacks Successfully.");
                }
                else
                {
                    Logger.Error("No UIComponent found on entity!");
                }

            }

            if (uiComponent.Type == UIType.SLIDER_TRACK)
            {
                Logger.Info($"Initial Slider Value: {uiComponent.SliderValue}");
                uiComponent.SliderValue = 0.75f;
                Logger.Info($"Updated Slider Value: {uiComponent.SliderValue}");
            }
        }

        public override void Update()
        {
            if (uiComponent == null) return;

            Logger.Info("=== UIComponent Live Update ===");
            Logger.Info($"UI Type: {uiComponent.Type}");
            Logger.Info($"UI Active: {uiComponent.Active}");
            Logger.Info($"Is Hovered: {uiComponent.IsHovered}");

            if (uiComponent.Type == UIType.BUTTON && Input.IsMouseButtonDown(Input.MouseButton.Left) && uiComponent.IsHovered)
            {
                List<string> actions = new List<string>(uiComponent.GetOnClickActions());
                Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

                int entityID = uiComponent.GetEntityID(); //

                if (actions.Contains("QUIT"))
                {
                    uiComponent.ExecuteUIAction("QUIT");
                }

                if (actions.Contains("MEOWWWWWERRS"))
                {
                    MeowFunction(entityID);
                }
                if (actions.Contains("JUMP"))
                {
                    JumpFunction(entityID);
                }

            }

            if (uiComponent.Type == UIType.SLIDER_TRACK)
            {
                Logger.Info($"Current Slider Value: {uiComponent.SliderValue}");
            }
        }

        public override void End()
        {
            Logger.Info("TestUI.cs | End()");
        }

        public void MeowFunction(int entityID)
        {
            Logger.Info($"Meow function triggered for entity ID: {entityID}!");
        }

        public void JumpFunction(int entityID)
        {
            Logger.Info($"JUMP for entity ID: {entityID}!");
        }
    }
}

