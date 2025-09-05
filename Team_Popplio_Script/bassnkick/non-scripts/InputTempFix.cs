/******************************************************************************/
/*!
\file   InputTempFix.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for temporary fix to trigger input not detecting

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using Team_Popplio.Extern;

using System.Collections.Generic;
using System.Collections.Concurrent;

namespace BassNKick.Lib
{
    public static class TempInputFix
    {
        private const double timer = 0.001f;
        private static ConcurrentDictionary<GLFW, double> keyStates =
            new ConcurrentDictionary<GLFW, double>();
        private static ConcurrentDictionary<Input.MouseButton, double> mouseStates =
            new ConcurrentDictionary<Input.MouseButton, double>();

        public static void Update()
        {
            foreach (GLFW key in keyStates.Keys)
            {
                if (Input.IsKeyDown(key)) keyStates[key] += Time.DeltaTimeF; //Time.FixedDeltaTimeF;
            }
            foreach (Input.MouseButton key in mouseStates.Keys)
            {
                if (Input.IsMouseButtonDown(key)) mouseStates[key] += Time.DeltaTimeF; // Time.FixedDeltaTimeF;
            }
        }

        public static bool IsKeyTriggered(GLFW key)
        {
            if (!keyStates.ContainsKey(key)) keyStates[key] = 0.0;

            if (Input.IsKeyDown(key))
            {
                if (keyStates[key] < timer) return true;
            }
            else keyStates[key] = 0.0;
            if (keyStates[key] >= timer) return false;
            return false;
        }

        public static bool IsMouseTriggered(Input.MouseButton key)
        {
            if (!mouseStates.ContainsKey(key)) mouseStates[key] = 0.0;

            if (Input.IsMouseButtonDown(key))
            {
                if (mouseStates[key] < timer) return true;
            }
            else mouseStates[key] = 0.0;
            if (mouseStates[key] >= timer) return false;
            return false;
        }

        public static void ResetKeyCooldown()
        {
            foreach (GLFW key in keyStates.Keys)
            {
                if (Input.IsKeyDown(key)) keyStates[key] = 0.0f; //Time.FixedDeltaTimeF;
            }
            foreach (Input.MouseButton key in mouseStates.Keys)
            {
                if (Input.IsMouseButtonDown(key)) mouseStates[key] = 0.0f; // Time.FixedDeltaTimeF;
            }
        }
    }
}
