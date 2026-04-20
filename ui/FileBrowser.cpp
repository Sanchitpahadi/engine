#include "FileBrowser.h"
#include <imgui.h>
#include <algorithm>

void FileBrowser::Init(const std::string& rootDir) {
    root       = rootDir;
    currentDir = fs::path(rootDir);
    ScanDir(currentDir);
}

void FileBrowser::SetOnSelect(SelectCallback cb) {
    onSelect = cb;
}

void FileBrowser::ScanDir(const fs::path& dir) {
    entries.clear();
    selected.clear();
    currentDir = dir;
    try {
        for (auto& e : fs::directory_iterator(dir))
            entries.push_back(e.path());
        std::sort(entries.begin(), entries.end(), [](auto& a, auto& b){
            // folders first, then files
            bool aDir = fs::is_directory(a);
            bool bDir = fs::is_directory(b);
            if (aDir != bDir) return aDir > bDir;
            return a.filename() < b.filename();
        });
    } catch (...) {}
}

std::string FileBrowser::ConsumePending() {
    std::string p = pendingPath;
    pendingPath.clear();
    return p;
}

void FileBrowser::Render() {
    ImGui::Begin("File Browser", &open);

    // -- breadcrumb / back button --
    if (currentDir != fs::path(root)) {
        if (ImGui::Button("<- Back"))
            ScanDir(currentDir.parent_path());
        ImGui::SameLine();
    }
    ImGui::TextDisabled("%s", currentDir.string().c_str());
    ImGui::Separator();

    // -- file list --
    for (auto& entry : entries) {
        std::string name = entry.filename().string();
        bool isDir       = fs::is_directory(entry);

        // icon prefix
        std::string label = (isDir ? "[D] " : "[F] ") + name;

        bool isSelected = (selected == entry.string());
        if (ImGui::Selectable(label.c_str(), isSelected)) {
            if (isDir) {
                ScanDir(entry);         // navigate into folder
            } else {
                selected = entry.string();
            }
        }
    }

    ImGui::Separator();

    // -- selected file + apply --
    if (!selected.empty()) {
        ImGui::Text("Selected: %s", fs::path(selected).filename().string().c_str());
        if (ImGui::Button("Apply Texture")) {
            pendingPath = selected;
            if (onSelect) onSelect(selected);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear"))
            selected.clear();
    } else {
        ImGui::TextDisabled("No file selected");
    }

    ImGui::End();
}