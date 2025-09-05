/******************************************************************************/
/*!
\file   QueueSystem.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine Queue System
        Enables enqueueing of functions for processing

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../ECS/ECS.h"

#include <queue>
#include <functional>
#include <memory>

namespace Popplio
{
    class QueueSystem : public System
    {
    private:
        // Runtime
        std::queue<std::function<void()>> queue;
        bool isProcessing;

        // Editor
        std::queue<std::function<void()>> editorQueue;
        bool isProcessingEditor;
    public:

        QueueSystem();

        // Runtime

        void Add(std::function<void()> const& func);

        void Process();

        void Clear();

        bool IsProcessing() const { return isProcessing; }

        // Editor

        void AddEditor(std::function<void()> const& func);

        void ProcessEditor();

        void ClearEditor();

        bool IsProcessingEditor() const { return isProcessingEditor; }
    };
}