#include "FileBrowser.h"
#include <imgui.h>
#include <algorithm>
#include <cstring>

namespace {
void CopyToBuf(char* buf, size_t bufSize, const std::string& s) {
    std::strncpy(buf, s.c_str(), bufSize - 1);
    buf[bufSize - 1] = '\0';
}
}

void FileBrowser::Init(const std::string& rootDir) {
    root = rootDir;
    fs::path start;
    try {
        start = fs::absolute(fs::path(rootDir)).lexically_normal();
    } catch (...) {
        start = fs::path(rootDir).lexically_normal();
    }
    ScanDir(start);
}

void FileBrowser::SetOnSelect(SelectCallback cb) {
    onSelect = cb;
}

void FileBrowser::GoTo(const std::string& path) {
    try {
        fs::path p = fs::absolute(fs::path(path)).lexically_normal();
        if (fs::is_directory(p))
            ScanDir(p);
    } catch (...) {}
}

void FileBrowser::ScanDir(const fs::path& dir) {
    entries.clear();
    selected.clear();
    currentDir = dir.lexically_normal();
    try {
        for (auto& e : fs::directory_iterator(currentDir))
            entries.push_back(e.path().lexically_normal());
        std::sort(entries.begin(), entries.end(), [](auto& a, auto& b){
            // folders first, then files
            bool aDir = fs::is_directory(a);
            bool bDir = fs::is_directory(b);
            if (aDir != bDir) return aDir > bDir;
            return a.filename() < b.filename();
        });
    } catch (...) {}
    CopyToBuf(pathBuf, sizeof(pathBuf), currentDir.string());
}

std::string FileBrowser::ConsumePending() {
    std::string p = pendingPath;
    pendingPath.clear();
    return p;
}

void FileBrowser::Render() {
    ImGui::Begin("File Browser", &open);

    // -- free-form path entry: type anywhere, hit Go --
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 60.0f);
    ImGui::InputText("##gotopath", pathBuf, sizeof(pathBuf));
    ImGui::SameLine();
    if (ImGui::Button("Go"))
        GoTo(pathBuf);

    // -- up / shortcut buttons --
    fs::path parent = currentDir.parent_path();
    bool hasParent = !parent.empty() && parent != currentDir; // guards the old crash: no parent_path() past this point
    if (!hasParent) ImGui::BeginDisabled();
    if (ImGui::Button("<- Up one level"))
        ScanDir(parent);
    if (!hasParent) ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Start folder"))
        GoTo(root);

    ImGui::TextDisabled("%s", currentDir.string().c_str());
    ImGui::Separator();

    // -- file list --
    for (auto& entry : entries) {
        std::string name = entry.filename().string();
        bool isDir       = fs::is_directory(entry);
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
        if (ImGui::Button("Select")) {
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