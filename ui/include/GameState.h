#pragma once

#include <string>
#include <functional>
#include <iostream>

#include "scene.h"

// GAME STATE — shared across all UI panels
struct GameState {
    Scene* scene = nullptr;

    bool isPlaying        = false;
    bool isPaused         = false;
    int  selectedObjectID = -1;
    std::string currentScenePath = "Untitled";

    std::function<void()> onSave = nullptr;
    std::function<void()> onLoad = nullptr;
    std::function<void()> onNew  = nullptr;

    void SaveScene() {
        std::cout << "[SAVED] Scene: " << currentScenePath << std::endl;
        if (onSave) onSave();
    }
    void LoadScene() {
        std::cout << "[LOADED] Scene from disk" << std::endl;
        if (onLoad) onLoad();
    }
    void NewScene() {
        selectedObjectID = -1;
        currentScenePath = "Untitled";
        if (onNew) onNew();
    }
};
