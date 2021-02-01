#include "DXShader.h"


DXShader::DXShader(std::any shader, DXShader::Type type, std::string data) :
	m_shader(shader),
	m_type(type),
	m_shaderData(data)
{
}

DXShader::~DXShader()
{
}

const DXShader::Type& DXShader::getType() const
{
	return m_type;
}

const std::string& DXShader::getShaderData() const
{
	return m_shaderData;
}
