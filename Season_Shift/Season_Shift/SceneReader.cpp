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
        vec.push_back(so);
    }

    return vec;
}
