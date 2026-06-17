#pragma once

#include "imgui.h"
#include "GameState.h"
#include <glm/glm.hpp>

// PANEL: Properties (sub-panel used by Inspector)
// Renders editable component fields for whatever components exist on an entity.
// Never creates or destroys components — read/write only.
class PropertiesPanel
{
public:
    // Call from InspectorPanel after it resolves the selected entity.
    void Show(Entity e, GameState* gs);
};
