#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <glm/glm.hpp>

namespace {

struct Key {
    int v, vt, vn;
    bool operator==(const Key& o) const { return v == o.v && vt == o.vt && vn == o.vn; }
};
struct KeyHash {
    size_t operator()(const Key& k) const {
        return std::hash<long long>()(((long long)k.v << 40) ^ ((long long)k.vt << 20) ^ (long long)(unsigned int)k.vn);
    }
};

int FixIndex(int idx, size_t count) {
    if (idx > 0) return idx - 1;
    if (idx < 0) return (int)count + idx;
    return -1;
}

} // namespace

bool LoadOBJ(const std::string& path, std::vector<float>& outVertices, std::vector<unsigned int>& outIndices)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ObjLoader] Cannot open: " << path << "\n";
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::unordered_map<Key, unsigned int, KeyHash> uniqueVerts;
    outVertices.clear();
    outIndices.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string tag;
        ss >> tag;

        if (tag == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (tag == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        } else if (tag == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (tag == "f") {
            std::vector<unsigned int> faceIndices;
            std::string vert;
            while (ss >> vert) {
                int vi = 0, ti = 0, ni = 0;
                size_t firstSlash = vert.find('/');
                if (firstSlash == std::string::npos) {
                    vi = std::stoi(vert);
                } else {
                    size_t secondSlash = vert.find('/', firstSlash + 1);
                    vi = std::stoi(vert.substr(0, firstSlash));
                    if (secondSlash == std::string::npos) {
                        std::string tStr = vert.substr(firstSlash + 1);
                        if (!tStr.empty()) ti = std::stoi(tStr);
                    } else {
                        std::string tStr = vert.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                        std::string nStr = vert.substr(secondSlash + 1);
                        if (!tStr.empty()) ti = std::stoi(tStr);
                        if (!nStr.empty()) ni = std::stoi(nStr);
                    }
                }

                Key key{ FixIndex(vi, positions.size()),
                         ti ? FixIndex(ti, uvs.size()) : -1,
                         ni ? FixIndex(ni, normals.size()) : -1 };

                auto it = uniqueVerts.find(key);
                if (it != uniqueVerts.end()) {
                    faceIndices.push_back(it->second);
                    continue;
                }

                glm::vec3 pos = (key.v  >= 0 && key.v  < (int)positions.size()) ? positions[key.v]  : glm::vec3(0.0f);
                glm::vec2 uv  = (key.vt >= 0 && key.vt < (int)uvs.size())       ? uvs[key.vt]        : glm::vec2(0.0f);
                glm::vec3 nrm = (key.vn >= 0 && key.vn < (int)normals.size())   ? normals[key.vn]    : glm::vec3(0.0f, 1.0f, 0.0f);

                unsigned int newIndex = (unsigned int)(outVertices.size() / 8);
                outVertices.push_back(pos.x); outVertices.push_back(pos.y); outVertices.push_back(pos.z);
                outVertices.push_back(uv.x);  outVertices.push_back(uv.y);
                outVertices.push_back(nrm.x); outVertices.push_back(nrm.y); outVertices.push_back(nrm.z);

                uniqueVerts[key] = newIndex;
                faceIndices.push_back(newIndex);
            }

            // Fan triangulation for n-gon faces
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                outIndices.push_back(faceIndices[0]);
                outIndices.push_back(faceIndices[i]);
                outIndices.push_back(faceIndices[i + 1]);
            }
        }
        // mtllib / usemtl / g / o / s intentionally ignored
    }

    if (outVertices.empty()) {
        std::cerr << "[ObjLoader] No vertices parsed from: " << path << "\n";
        return false;
    }

    return true;
}
