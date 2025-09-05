/******************************************************************************/
/*!
\file   QueueSystem.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for engine Queue System
        Enables enqueueing of functions for processing

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "QueueSystem.h"

namespace Popplio
{
    QueueSystem::QueueSystem() : queue(), isProcessing(false), editorQueue(), isProcessingEditor(false) {}

    // Runtime

    void QueueSystem::Add(std::function<void()> const& func)
    {
        queue.push(func);
    }

    void QueueSystem::Process()
    {
        isProcessing = true;
        while (!queue.empty())
        {
            queue.front()();
            queue.pop();
        }
        isProcessing = false;
    }

    void QueueSystem::Clear()
    {
        std::queue<std::function<void()>> empty;
        queue.swap(empty);
    }

    // Editor

    void QueueSystem::AddEditor(std::function<void()> const& func)
    {
        editorQueue.push(func);
    }

    void QueueSystem::ProcessEditor()
    {
        isProcessingEditor = true;
        while (!editorQueue.empty())
        {
            editorQueue.front()();
            editorQueue.pop();
        }
        isProcessingEditor = false;
    }

    void QueueSystem::ClearEditor()
    {
        std::queue<std::function<void()>> empty;
        editorQueue.swap(empty);
    }
}