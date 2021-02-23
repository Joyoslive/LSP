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
		std::string meshFileName;
		std::string meshDirectory;
		DirectX::SimpleMath::Vector3 boxCollider;
	};
private:

public:
	SceneReader() = default;
	~SceneReader() = default;
	/*!
	Reads a scene from a .json file.
	Includes position, rotation, scale, model paths, and box collider.
	If no box collider is present it will be (0, 0, 0) which needs to be checked for upon use.
	*/
	std::vector<SceneObject> readSceneFromFile(const std::string& path);
};

