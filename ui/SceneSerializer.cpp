#include "SceneSerializer.h"
#include "AssetManager.h"
#include "material.h"
#include "shader.h"

#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

static json Vec3ToJson(const glm::vec3& v) { return json::array({ v.x, v.y, v.z }); }
static glm::vec3 JsonToVec3(const json& j) { return glm::vec3(j[0].get<float>(), j[1].get<float>(), j[2].get<float>()); }

bool SceneSerializer::Save(Scene& scene, const std::string& path)
{
    json root;
    root["ground"]     = scene.ground;
    root["nextEntity"] = scene.nextEntity;

    json entitiesJson = json::array();

    for (Entity e : scene.entities) {
        json je;
        je["id"]   = e;
        je["name"] = scene.names.count(e) ? scene.names[e] : "Entity";

        if (scene.transforms.count(e)) {
            auto& t = scene.transforms[e];
            je["transform"] = {
                { "position", Vec3ToJson(t.position) },
                { "rotation", Vec3ToJson(t.rotation) }, // ADAPT: rename/remove if your TransformComponent differs
                { "scale",    Vec3ToJson(t.scale)    }
            };
        }

        if (scene.meshRenderers.count(e)) {
            auto& mr = scene.meshRenderers[e];
            std::string meshPath = mr.mesh ? AssetManager::Get().GetMeshPath(mr.mesh) : "";
            je["meshRenderer"] = {
                { "meshPath", meshPath },
                { "color", mr.material ? Vec3ToJson(mr.material->color) : Vec3ToJson(glm::vec3(1.0f)) }
            };
        }

    /*
      if (scene.colliders.count(e)) {
            auto& c = scene.colliders[e];
            je["collider"] = {
                { "halfExtents", Vec3ToJson(c.halfExtents) } 
            };
        }

    */  
        if (scene.physics.count(e)) {
            auto& rb = scene.physics[e];
            je["rigidBody"] = {
                { "mass",       rb.mass },        
                { "isStatic",   rb.isStatic },
                { "useGravity", rb.useGravity },
                { "velocity",   Vec3ToJson(rb.velocity) }
            };
        }

        if (scene.orbits.count(e)) {
            auto& o = scene.orbits[e];
            je["orbit"] = {
                { "target", o.target },
                { "radius", o.radius },
                { "speed",  o.speed },
                { "phase",  o.phase }
            };
        }

        entitiesJson.push_back(je);
    }

    root["entities"] = entitiesJson;

    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "[SceneSerializer] Cannot write: " << path << "\n";
        return false;
    }
    out << root.dump(2);
    return true;
}

bool SceneSerializer::Load(Scene& scene, const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "[SceneSerializer] Cannot open: " << path << "\n";
        return false;
    }

    json root;
    try {
        in >> root;
    } catch (const std::exception& ex) {
        std::cerr << "[SceneSerializer] Parse error: " << ex.what() << "\n";
        return false;
    }

    scene.entities.clear();
    scene.names.clear();
    scene.transforms.clear();
    scene.meshRenderers.clear();
    scene.colliders.clear();
    scene.physics.clear();
    scene.orbits.clear();

    scene.ground     = root.value("ground", (Entity)0);
    scene.nextEntity = root.value("nextEntity", (Entity)0);

    for (auto& je : root["entities"]) {
        Entity e = je["id"].get<Entity>();
        scene.entities.push_back(e);
        scene.names[e] = je.value("name", std::string("Entity"));

        if (je.contains("transform")) {
            TransformComponent t;
            t.position = JsonToVec3(je["transform"]["position"]);
            t.rotation = JsonToVec3(je["transform"]["rotation"]); // ADAPT if needed
            t.scale    = JsonToVec3(je["transform"]["scale"]);
            scene.transforms[e] = t;
        }

        if (je.contains("meshRenderer")) {
            MeshRendererComponent mr;
            std::string meshPath = je["meshRenderer"].value("meshPath", std::string(""));
            if (!meshPath.empty())
                mr.mesh = AssetManager::Get().LoadMesh(meshPath);

            Material* mat = AssetManager::Get().CreateMaterial();
            mat->color  = JsonToVec3(je["meshRenderer"]["color"]);
            mr.material = mat;

            scene.meshRenderers[e] = mr;
        }

       /*
       
       if (je.contains("collider")) {
            BoxColliderComponent c;
            c.halfExtents = JsonToVec3(je["collider"]["halfExtents"]); // ADAPT
            scene.colliders[e] = c;
        }
       
       */ 

        if (je.contains("rigidBody")) {
            RigidBodyComponent rb;
            rb.mass       = je["rigidBody"].value("mass", 1.0f); // ADAPT
            rb.isStatic   = je["rigidBody"].value("isStatic", false);
            rb.useGravity = je["rigidBody"].value("useGravity", true);
            rb.velocity   = JsonToVec3(je["rigidBody"]["velocity"]);
            scene.physics[e] = rb;
        }

        if (je.contains("orbit")) {
            OrbitComponent o;
            o.target = je["orbit"]["target"].get<Entity>();
            o.radius = je["orbit"]["radius"].get<float>();
            o.speed  = je["orbit"]["speed"].get<float>();
            o.phase  = je["orbit"]["phase"].get<float>();
            scene.orbits[e] = o;
        }
    }

    return true;
}
