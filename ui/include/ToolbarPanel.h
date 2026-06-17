#pragma once

#include "imgui.h"
#include "GameState.h"

// PANEL: Toolbar
// Top bar with Save / Load / New and Play / Pause / Stop controls.
class ToolbarPanel
{
public:
    // Pass GameState by pointer so every panel shares the same instance.
    void Show(GameState* gs);
};
