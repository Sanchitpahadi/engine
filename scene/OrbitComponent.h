#pragma once

struct OrbitComponent
{
    Entity target   = (Entity)-1; // the entity being orbited (e.g. the sun)
    float  radius   = 5.0f;       // orbit distance from target
    float  speed    = 1.0f;       
    float  phase    = 0.0f;       
    glm::vec3 axis  = glm::vec3(0.0f, 1.0f, 0.0f); // orbit plane normal Y-up = flat horizontal orbit
};