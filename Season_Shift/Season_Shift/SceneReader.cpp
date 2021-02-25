#include "pch.h"
#include "SceneReader.h"
#include "json/json.hpp"
#include <fstream>
#include <sstream>

std::vector<SceneReader::SceneObject> SceneReader::readSceneFromFile(const std::string& path)
{
    std::vector<SceneObject> vec;

    std::ifstream ifs(path);
    if (!ifs)
        assert(false);

    // Read the json from file
    std::stringstream ss;
    std::string line;
    while (getline(ifs, line))
    {
        ss << line << "\n";
    }
    nlohmann::json json;
    ss >> json;

    for (auto& obj : json)
    {
        SceneObject so;
        so.name = obj["name"];
        so.position = {obj["pos"]["x"],   obj["pos"]["y"],   obj["pos"]["z"]};
        so.rotation = {obj["rot"]["x"],   obj["rot"]["y"],   obj["rot"]["z"]};
        so.scale = {obj["scale"]["x"], obj["scale"]["y"], obj["scale"]["z"]};

        so.meshFileName = obj["meshFileName"];
        so.meshDirectory = obj["meshDirectory"];
        so.meshDirectory += std::string("/");

        // If no box collider was present in Unity this will be [0, 0, 0]
        // This needs to be checked for in the scene by for example comparing the sum of x, y, and z with 0 
        // scale should never be negative
        so.boxCollider = {
            obj["boxCollider"]["x"] * so.scale.x,
            obj["boxCollider"]["y"] * so.scale.y,
            obj["boxCollider"]["z"] * so.scale.z
        };
        vec.push_back(so);
    }

    return vec;
}
