#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class FileBrowser {
public:
    // Callback: called when user clicks "Apply" on a file
    using SelectCallback = std::function<void(const std::string& path)>;

    void Init(const std::string& rootDir = "assets/textures");
    void Render();                          // call inside ImGui frame
    void SetOnSelect(SelectCallback cb);    // wire up what happens on pick

    bool HasPending() const { return !pendingPath.empty(); }
    std::string ConsumePending();           // returns path, clears it

private:
    void ScanDir(const fs::path& dir);

    std::string              root;
    fs::path                 currentDir;
    std::vector<fs::path>    entries;       // files + folders in currentDir
    std::string              selected;
    std::string              pendingPath;
    SelectCallback           onSelect;
    bool                     open = true;
};