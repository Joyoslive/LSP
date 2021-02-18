#pragma once

#include <string>
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>

class SceneReader
{
public:
	struct SceneObject
	{
		std::string name;
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 rotation;
		DirectX::SimpleMath::Vector3 scale;
	};
private:

public:
	SceneReader() = default;
	~SceneReader() = default;
	std::vector<SceneObject> readSceneFromFile(const std::string& path);
};

