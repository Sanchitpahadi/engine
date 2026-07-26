#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <functional>
namespace fs = std::filesystem;

class FileBrowser {
public:
    using SelectCallback = std::function<void(const std::string& path)>;

    void Init(const std::string& rootDir = "assets/textures"); // starting folder only, not a hard boundary
    void Render();                          // call inside ImGui frame
    void SetOnSelect(SelectCallback cb);    // wire up what happens on pick
    void GoTo(const std::string& path);     // jump to any folder directly

    bool HasPending() const { return !pendingPath.empty(); }
    std::string ConsumePending();           // returns path, clears it

private:
    void ScanDir(const fs::path& dir);

    std::string              root;          // starting folder, shown as a shortcut button
    fs::path                 currentDir;
    std::vector<fs::path>    entries;       // files + folders in currentDir
    std::string              selected;
    std::string              pendingPath;
    SelectCallback           onSelect;
    bool                     open = true;
    char                     pathBuf[512] = "";
};