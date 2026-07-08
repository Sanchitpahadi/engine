#pragma once

#include "imgui.h"
#include "GameState.h"

// PANEL: Hierarchy

// Lists every entity in the scene. Lets you select, rename, add, and delete.

// Component creation/destruction is always delegated to Scene, never done here.
class HierarchyPanel
{
public:
    bool show = true;

    void Show(GameState* gs);
};
