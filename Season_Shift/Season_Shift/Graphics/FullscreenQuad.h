#pragma once
#include "Model.h"
#include "Graphics.h"

class FullscreenQuad
{
private:
	std::shared_ptr<DXBuffer> m_vertexBuffer;
	std::shared_ptr<DXBuffer> m_indexBuffer;
	std::shared_ptr<Mesh> m_mesh;
public:
	FullscreenQuad(DXDevice* gph);
	~FullscreenQuad();

	const std::shared_ptr<DXBuffer>& getVB() const;
	const std::shared_ptr<DXBuffer>& getIB() const;
};

