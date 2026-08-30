#pragma once 

struct OrbitParams {
    Entity entity;
    float radius;
    float speed;
    float phase = 0.0f;
};
std::vector<OrbitParams>orbits;
