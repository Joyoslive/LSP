#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<DXBuffer> vb, std::shared_ptr<DXBuffer> ib) :
	m_vb(vb),
	m_ib(ib)
{
}

Mesh::~Mesh()
{
}

const std::shared_ptr<DXBuffer>& Mesh::getVB()
{
	return m_vb;
}

const std::shared_ptr<DXBuffer>& Mesh::getIB()
{
	return m_ib;
}
