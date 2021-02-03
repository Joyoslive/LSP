#pragma once
#include <memory>
#include <any>
#include <string>
#include "DXCore.h"



class DXShader
{
public:
	enum class Type
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS
	};

private:
	std::any m_shader;
	Type m_type;
	std::string m_shaderData;

public:
	DXShader(std::any shader, Type type, std::string shaderData);
	~DXShader();

	template<typename T>
	T* getShader() const;
	const Type& getType() const;

	const std::string& getShaderData() const;

};

template<typename T>
inline T* DXShader::getShader() const
{
	try
	{
		Microsoft::WRL::ComPtr<T> tmp = std::any_cast<Microsoft::WRL::ComPtr<T>>(m_shader);		// this assignment costs.. (reference count)	perhaps we can keep a naked COM for the Shaders instead of using ComPtr
		return tmp.Get();
	}
	catch (const std::bad_any_cast& e)
	{
		//std::cout << e.what();
		// --> Wrong type of shader! (Wrong T!)
		assert(false);
	}
	return nullptr;
}
