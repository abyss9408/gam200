/******************************************************************************/
/*!
\file   pch.h
\author Team Popplio
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
        Precompiled Header

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// STL includes
#include <filesystem>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <type_traits>
//#include <random>
#include <ctime>
#include <exception>
#include <iomanip>


// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rapidjson/document.h>
#include <imgui/imgui.h>

// Common project lib headers
#include "../Math/MathLib.h"
#include "../Logging/Logger.h"
#include "../EventBus/EventBus.h"
#include "../EventBus/Event.h"
#include "../ECS/ECS.h"
#include "../Input/InputSystem.h"

// Components
#include "../Animation/AnimationComponent.h"
#include "../Transformation/TransformComponent.h"
#include "../Physics/RigidBodyComponent.h"
#include "../Collision/BoxColliderComponent.h"
#include "../Collision/CircleColliderComponent.h"
#include "../Graphic/GraphicComponent.h"
#include "../PrefabManagement/PrefabInstanceComponent.h"
#include "../Script/LogicComponent.h"
#include "../Script/ActiveComponent.h"
#include "../Audio/AudioComponent.h"
#include "../Hierarchy/ParentComponent.h"
#include "../UI/UIComponent.h"

// Function registry for UI actions
#include "../UI/FunctionRegistry.h"

// Events
#include "../Editor/EntitySelectionEvents.h"
#include "../Cloning/CloneEntityEvent.h"
#include "../Serialization/SceneLoadedEvent.h"
#include "../Collision/CollisionEvent.h"

// Utilities
#include "../Utilities/Constants.h"
#include "../Utilities/Paths.h"