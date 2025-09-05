#pragma once
#include "../EventBus/Event.h"
#include <string>

namespace Popplio
{
	// Event triggered when a scene has finished loading
	struct SceneLoadedEvent : public Event
	{
		std::string scenePath;
		SceneLoadedEvent(const std::string& name) : scenePath(name) {}
	};
}