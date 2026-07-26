#pragma once
#include <string>
#include <vector>

/*
Parses a Wavefront .obj into a flat vertex buffer ,
layout: pos.xyz, uv.xy, normal.xyz -> 8 floats/vertex (same as cubeVertices)
Handles v/vt/vn, f with v, v/vt, v//vn, v/vt/vn, negative (relative) indices
and n-gon faces via fan triangulation. No mtllib/material support (materials
*/ 

bool LoadOBJ(const std::string& path,
             std::vector<float>& outVertices,
             std::vector<unsigned int>& outIndices);
