#pragma once
#include <memory>
#include "DX/DXBuffer.h"

class Mesh
{
private:
	std::shared_ptr<DXBuffer> m_vb;
	std::shared_ptr<DXBuffer> m_ib;

public:
	Mesh(std::shared_ptr<DXBuffer> vb, std::shared_ptr<DXBuffer> ib);
	~Mesh();
	const std::shared_ptr<DXBuffer>& getVB();
	const std::shared_ptr<DXBuffer>& getIB();
};

