#pragma once
#include <memory>
#include <any>
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

public:
	DXShader(std::any shader);
	~DXShader();

	template<typename T>
	const Microsoft::WRL::ComPtr<T>& getShader() const;

};

template<typename T>
inline const Microsoft::WRL::ComPtr<T>& DXShader::getShader() const
{
	Microsoft::WRL::ComPtr<T> toRet = nullptr;

	try
	{
		return std::any_cast<Microsoft::WRL::ComPtr<T>>(m_shader);
	}
	catch (const std::bad_any_cast& e)
	{
		//std::cout << e.what();
		// --> Wrong type of shader! (Wrong T!)
		assert(false);
	}
}
