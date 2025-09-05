/******************************************************************************/
/*!
\file   EditorMode.cpp
\author Team Popplio
\author			Bryan Ang Wei Ze
\co-author      Ng Jun Heng Shawn
\contribution	Bryan - 50% | Shawn - 50%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/07
\brief
        This is the source file for editor's toggles
        These include toggle functions for:
        Running the editor, toggling fullscreen mode for editor, etc.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "../Graphic/RenderSystem.h"
#include "Editor.h"

namespace Popplio
{
    void Editor::ToggleEditor()
    {
        isEditorEnabled = !isEditorEnabled;

        //if (!isEditorEnabled)
        //{
        //    // When hiding editor, automatically start the scene if not already playing
        //    if (gameIsRunning == State::EDITING || gameIsRunning == State::STOPPED)
        //    {
        //        StartScene();
        //    }
        //}
        //else
        //{
        //    if (gameIsRunning == State::PLAYING || gameIsRunning == State::PAUSED) EndScene();
        //}
    }

    void Editor::ToggleRun()
    {
        if (editorState == State::EDITING || editorState == State::STOPPED) StartScene();
        else if (editorState == State::PLAYING || editorState == State::PAUSED) EndScene();
    }
}