// ChunkMesh.h
#ifndef CHUNK_H
#define CHUNK_H

#pragma once

#include "Mesh.h"
#include <vector>

class Chunk {
public:
    static const int SIZE = 16;

    Chunk(int cx, int cz);
    void BuildMesh();

    Mesh* mesh;

private:
    int chunkX, chunkZ;
    int blocks[SIZE][SIZE][SIZE];

    bool IsSolid(int x, int y, int z);   // ← add this
    void EmitFace(std::vector<float>& verts,
                  std::vector<unsigned int>& indices,
                  int x, int y, int z,
                  float nx, float ny, float nz,
                  const float corners[12]);  // ← add this
};

#endif
