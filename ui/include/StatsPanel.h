#pragma once

#include "imgui.h"
#include "GameState.h"

// PANEL: Stats
// Displays FPS, frame time, draw call counts, and current play state.
class StatsPanel
{
public:
    bool show = true;

    void Show(GameState* gs);
};
