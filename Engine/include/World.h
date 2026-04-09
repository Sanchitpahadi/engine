// World.h
#include "Chunk.h"
#include <unordered_map>
#include <string>
#include <cmath>

class World {
public:
    int renderDistance = 8; // chunks in each direction (covers 300 blocks at 16*8=128... use 10 for 160)

    void Update(float playerX, float playerZ);
    void Render();

private:
    std::unordered_map<std::string, Chunk*> chunks;

    std::string Key(int cx, int cz) {
        return std::to_string(cx) + "," + std::to_string(cz);
    }
};