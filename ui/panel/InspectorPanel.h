#pragma once
#include "imgui.h"
#include "GameState.h"
#include <glm/glm.hpp>

class InspectorPanel {
public:                            
    bool show = true;
    void Show(GameState* gs);

private:
    void ShowProperties(Entity e, GameState* gs);  
};