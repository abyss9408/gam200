/******************************************************************************/
/*!
\file   UndoRedo.cpp
\author Team Popplio
\author			Ng Jun Heng Shawn
\contribution	Shawn - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for undo and redo functionality in the editor
        Implements undo and redo functionality for changes in entities and components
        Add undo and redo functionality here for subsequent components added

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Editor.h"

#include "Changes.h"

#include "../Utilities/Any.h"

#include "../Cloning/CloneSystem.h"

#include "EntitySelectionEvents.h"

namespace Popplio
{
    void Editor::RenderUndoRedoMenu()
    {
        if (ImGui::MenuItem("Undo")) Undo();

        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();

        if (ImGui::MenuItem("Redo")) Redo();
    }

    template <typename T>
    void Editor::UndoComponent(SceneChangeEvent<std::any>& latestChange)
    {
        for (size_t i{}; i < latestChange.changes.size(); ++i)
        {
            switch (latestChange.changes[i]) // component created
            {
                case ChangeAction::ADD_COMPONENT:
                {
                    if (latestChange.entities[i].HasComponent<T>())
                        latestChange.entities[i].RemoveComponent<T>();

                    break;
                }
                case ChangeAction::MODIFY_COMPONENT:
                {
                    latestChange.entities[i].GetComponent<T>() =
                        PopplioUtil::AnyTo<T>(latestChange.oldState[i]);

                    // update for audio components
                    if (std::is_same_v<T, AudioComponent>) 
                        latestChange.entities[i].GetComponent<AudioComponent>().Update(reg.GetSystem<AudioSystem>());
                    break;
                }
                case ChangeAction::REMOVE_COMPONENT:
                {
                    if (!latestChange.entities[i].HasComponent<T>())
                        latestChange.entities[i].AddComponent<T>();
                    latestChange.entities[i].GetComponent<T>() =
                        PopplioUtil::AnyTo<T>(latestChange.oldState[i]);

                    break;
                }
            }
        }
    }

    template <typename T>
    void Editor::RedoComponent(SceneChangeEvent<std::any>& latestUndo)
    {
        for (size_t i{}; i < latestUndo.changes.size(); ++i)
        {
            switch (latestUndo.changes[i]) // component created
            {
                case ChangeAction::ADD_COMPONENT:
                {
                    if (!latestUndo.entities[i].HasComponent<T>())
                        latestUndo.entities[i].AddComponent<T>();
                    latestUndo.entities[i].GetComponent<T>() =
                        PopplioUtil::AnyTo<T>(latestUndo.newState[i]);

                    break;
                }
                case ChangeAction::MODIFY_COMPONENT:
                {
                    latestUndo.entities[i].GetComponent<T>() =
                        PopplioUtil::AnyTo<T>(latestUndo.newState[i]);

                    // update for audio components
                    if (std::is_same_v<T, AudioComponent>) 
                        latestUndo.entities[i].GetComponent<AudioComponent>().Update(reg.GetSystem<AudioSystem>());
                    break;
                }
                case ChangeAction::REMOVE_COMPONENT:
                {
                    if (latestUndo.entities[i].HasComponent<T>())
                        latestUndo.entities[i].RemoveComponent<T>();

                    break;
                }
            }
        }
    }

    void Editor::UndoEntity(SceneChangeEvent<std::any>& latestChange)
    {
        for (size_t i{}; i < latestChange.changes.size(); ++i)
        {
            switch (latestChange.changes[i])
            {
                case ChangeAction::ADD_ENTITY:
                {
                    auto& e = latestChange.entities[i];

                    // ensure when adding new entity, the change still gets detected if same entity id
                    if (lastNewEntity == e) lastNewEntity.reset();
                    if (latestNewEntity == e) latestNewEntity.reset();

                    if (reg.EntityExists(e.GetId())) // might have error due to how entity IDs are assigned
                    {
                        layerManager.RemoveEntityFromCurrentLayer(e);
                        reg.KillEntity(e);
                        if (selectedEntity.has_value() && selectedEntity.value() == e)
                        {
                            selectedEntity.reset();
                            eventBus.EmitEvent<EntityDeselectedEvent>();
                        }
                    }

                    break;
                }
                case ChangeAction::MODIFY_ENTITY:
                {
                    auto& e = latestChange.entities[i];
                    //e = PopplioUtil::AnyTo<Entity>(latestChange.oldState[i]);

                    // tag
                    if (latestChange.typeName[i] == static_cast<std::string>(typeid(std::string).name()) + "0")
                    {
                        if (reg.EntityHasAnyTag(e)) reg.RemoveEntityTag(e);
                        e.Tag(PopplioUtil::AnyTo<std::string>(latestChange.oldState[i]));
                    }
                    // group
                    else if (latestChange.typeName[i] == static_cast<std::string>(typeid(std::string).name()) + "1")
                    {
                        if (reg.EntityBelongsToAnyGroup(e)) reg.RemoveEntityGroup(e);
                        e.Group(PopplioUtil::AnyTo<std::string>(latestChange.oldState[i]));
                    }

                    break;
                }
                case ChangeAction::REMOVE_ENTITY:
                {
                    auto& original = latestChange.entities[i];
                    std::optional<Entity> e;
                    if (!reg.EntityExists(original.GetId())) // might have error due to how entity IDs are assigned
                    {
                        e = reg.CreateEntity(); // might have issues if entity additional info arent copied

                    }
                    else e = reg.GetEntityById(original.GetId());

                    reg.GetSystem<CloneSystem>().CloneComponents(original, *e);

                    latestNewEntity = *e; // to signal change event

                    break;
                }
            }
        }
    }

    void Editor::RedoEntity(SceneChangeEvent<std::any>& latestUndo)
    {
        for (size_t i{}; i < latestUndo.changes.size(); ++i)
        {
            switch (latestUndo.changes[i])
            {
                case ChangeAction::ADD_ENTITY:
                {
                    auto& original = latestUndo.entities[i];
                    std::optional<Entity> e;
                    if (!reg.EntityExists(original.GetId())) // might have error due to how entity IDs are assigned
                    {
                        e = reg.CreateEntity(); // might have issues if entity additional info arent copied

                    }
                    else e = reg.GetEntityById(original.GetId());

                    reg.GetSystem<CloneSystem>().CloneComponents(original, *e);

                    latestNewEntity = *e; // to signal change event

                    break;
                }
                case ChangeAction::MODIFY_ENTITY:
                {
                    auto& e = latestUndo.entities[i];
                    //e = PopplioUtil::AnyTo<Entity>(latestUndo.newState[i]);

                    // tag
                    if (latestUndo.typeName[i] == static_cast<std::string>(typeid(std::string).name()) + "0")
                    {
                        if (reg.EntityHasAnyTag(e)) reg.RemoveEntityTag(e);
                        std::string val = PopplioUtil::AnyTo<std::string>(latestUndo.newState[i]);
                        if (!val.empty()) e.Tag(val);
                        //else reg.RemoveEntityTag(e);
                    }
                    // group
                    else if (latestUndo.typeName[i] == static_cast<std::string>(typeid(std::string).name()) + "1")
                    {
                        if (reg.EntityBelongsToAnyGroup(e)) reg.RemoveEntityGroup(e);
                        std::string val = PopplioUtil::AnyTo<std::string>(latestUndo.newState[i]);
                        if (!val.empty()) e.Group(val);
                        //else reg.RemoveEntityGroup(e);
                    }

                    break;
                }
                case ChangeAction::REMOVE_ENTITY:
                {
                    auto& e = latestUndo.entities[i];

                    // ensure when adding new entity, the change still gets detected if same entity id
                    if (lastNewEntity == e) lastNewEntity.reset();
                    if (latestNewEntity == e) latestNewEntity.reset();

                    if (reg.EntityExists(e.GetId())) // might have error due to how entity IDs are assigned
                    {
                        layerManager.RemoveEntityFromCurrentLayer(e);
                        reg.KillEntity(e);
                        if (selectedEntity.has_value() && selectedEntity.value() == e)
                        {
                            selectedEntity.reset();
                            eventBus.EmitEvent<EntityDeselectedEvent>();
                        }
                    }

                    break;
                }
            }
        }
    }

    void Editor::Undo()
    {
        if (editorState != State::EDITING)
        {
            Logger::Debug("Cannot undo in current state");
            return;
        }

        if (undoStack.empty())
        {
            Logger::Debug("Undo stack is empty");
            return;
        }

        auto& latestChange = undoStack.back();

        if (latestChange.newState.size() == 0)
        {
            Logger::Debug("Invalid undo stack: empty entity");
        }
        else
        {
            switch (latestChange.changes[0])
            {
                case ChangeAction::ADD_COMPONENT:
                case ChangeAction::MODIFY_COMPONENT:
                case ChangeAction::REMOVE_COMPONENT:
                {
                    // add for every component

                    if (latestChange.oldState[0].type() == typeid(TransformComponent))
                    {
                        Logger::Debug("Undoing transform change...");
                        UndoComponent<TransformComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(ActiveComponent))
                    {
                        Logger::Debug("Undoing active change...");
                        UndoComponent<ActiveComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(RenderComponent))
                    {
                        Logger::Debug("Undoing render change...");
                        UndoComponent<RenderComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(RigidBodyComponent))
                    {
                        Logger::Debug("Undoing rigidbody change...");
                        UndoComponent<RigidBodyComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(BoxColliderComponent))
                    {
                        Logger::Debug("Undoing boxcollider change...");
                        UndoComponent<BoxColliderComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(TextComponent))
                    {
                        Logger::Debug("Undoing text change...");
                        UndoComponent<TextComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(AnimationComponent))
                    {
                        Logger::Debug("Undoing animation change...");
                        UndoComponent<AnimationComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(LogicComponent))
                    {
                        Logger::Debug("Undoing logic change...");
                        UndoComponent<LogicComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(AudioComponent))
                    {
                        Logger::Debug("Undoing audio change...");
                        UndoComponent<AudioComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(ParentComponent))
                    {
                        Logger::Debug("Undoing parent change...");
                        UndoComponent<ParentComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(PersistOnLoadComponent))
                    {
                        Logger::Debug("Undoing persist change...");
                        UndoComponent<PersistOnLoadComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(UIComponent))
                    {
                        Logger::Debug("Undoing ui change...");
                        UndoComponent<UIComponent>(latestChange);
                    }
                    else if (latestChange.oldState[0].type() == typeid(ParticleComponent))
                    {
                        Logger::Debug("Undoing particle change...");
                        UndoComponent<ParticleComponent>(latestChange);
                    }

                    //else if (latestChange.oldState[0].type() == typeid(Entity))
                    //{
                    //    Logger::Debug("Undoing entity change...");
                    //    UndoEntity(latestChange);
                    //}
                    break;
                }
                case ChangeAction::ADD_ENTITY:
                case ChangeAction::MODIFY_ENTITY:
                case ChangeAction::REMOVE_ENTITY:
                {
                    Logger::Debug("Undoing entity change...");
                    UndoEntity(latestChange);
                    break;
                }
            }
        }

        // add to redo stack and remove from undo stack
        redoStack.push_back(latestChange);
        undoStack.pop_back();
    }
    void Editor::Redo()
    {
        if (editorState != State::EDITING)
        {
            Logger::Debug("Cannot redo in current state");
            return;
        }

        if (redoStack.empty())
        {
            Logger::Debug("Redo stack is empty");
            return;
        }

        auto& latestUndo = redoStack.back();

        if (latestUndo.newState.size() == 0)
        {
            Logger::Debug("Invalid redo stack: empty entity");
        }
        else
        {
            switch (latestUndo.changes[0])
            {
                case ChangeAction::ADD_COMPONENT:
                case ChangeAction::MODIFY_COMPONENT:
                case ChangeAction::REMOVE_COMPONENT:
                {
                    // add for every component

                    if (latestUndo.oldState[0].type() == typeid(TransformComponent))
                    {
                        Logger::Debug("Redoing transform undo...");
                        RedoComponent<TransformComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(ActiveComponent))
                    {
                        Logger::Debug("Redoing active undo...");
                        RedoComponent<ActiveComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(RenderComponent))
                    {
                        Logger::Debug("Redoing render undo...");
                        RedoComponent<RenderComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(RigidBodyComponent))
                    {
                        Logger::Debug("Redoing rigidbody undo...");
                        RedoComponent<RigidBodyComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(BoxColliderComponent))
                    {
                        Logger::Debug("Redoing boxcollider undo...");
                        RedoComponent<BoxColliderComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(TextComponent))
                    {
                        Logger::Debug("Redoing text undo...");
                        RedoComponent<TextComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(AnimationComponent))
                    {
                        Logger::Debug("Redoing animation undo...");
                        RedoComponent<AnimationComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(LogicComponent))
                    {
                        Logger::Debug("Redoing logic undo...");
                        RedoComponent<LogicComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(AudioComponent))
                    {
                        Logger::Debug("Redoing audio undo...");
                        RedoComponent<AudioComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(ParentComponent))
                    {
                        Logger::Debug("Redoing parent undo...");
                        RedoComponent<ParentComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(PersistOnLoadComponent))
                    {
                        Logger::Debug("Redoing persist undo...");
                        RedoComponent<PersistOnLoadComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(UIComponent))
                    {
                        Logger::Debug("Redoing ui undo...");
                        RedoComponent<UIComponent>(latestUndo);
                    }
                    else if (latestUndo.oldState[0].type() == typeid(ParticleComponent))
                    {
                        Logger::Debug("Redoing particle undo...");
                        RedoComponent<ParticleComponent>(latestUndo);
                    }

                    //else if (latestUndo.oldState[0].type() == typeid(Entity))
                    //{
                    //    Logger::Debug("Redoing entity undo...");
                    //    RedoEntity(latestUndo);
                    //}

                    break;
                }
                case ChangeAction::ADD_ENTITY:
                case ChangeAction::MODIFY_ENTITY:
                case ChangeAction::REMOVE_ENTITY:
                {
                    Logger::Debug("Redoing entity undo...");
                    RedoEntity(latestUndo);
                    break;
                }
            }
        }

        // add to redo stack and remove from undo stack
        undoStack.push_back(latestUndo);
        redoStack.pop_back();
    }
}