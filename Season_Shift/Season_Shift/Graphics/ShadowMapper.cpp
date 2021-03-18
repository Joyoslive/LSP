#include "pch.h"
#include "ShadowMapper.h"
#include "Model.h"
#include "../Camera.h"
#include "LineDrawer.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

struct OrthoInfo
{
	float l, r, t, b, n, f;
};


float min(float a, float b)
{
	if (a > b) return b;
	return a;
}

float max(float a, float b)
{
	if (a > b) return a;
	return b;
}


ShadowMapper::ShadowMapper(std::shared_ptr<GfxRenderer> renderer) :
    m_renderer(renderer),
    m_firstTime(true)
{

    createResources();
}

void ShadowMapper::createResources()
{
    auto dev = m_renderer->getDXDevice();

    // Buffer for view matrix for each split
    m_bufferInfoCB = dev->createConstantBuffer(sizeof(BufferInfo), true, true);

    // Load shader and input layout
    m_vs = dev->createShader("DefaultVS.cso", DXShader::Type::VS);
    m_ps = dev->createShader("NullPS.cso", DXShader::Type::PS);
    std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
    ilDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0 });
    ilDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
    ilDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
    m_il = dev->createInputLayout(ilDesc, m_vs->getShaderData());

	D3D11_DEPTH_STENCIL_DESC dssDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	m_dss = dev->createDepthStencilState(dssDesc);




}


void ShadowMapper::setCascadeSettings(const std::vector<std::pair<float, unsigned int>>& cascadeEndDistancesAndResolution)
{
    if (m_firstTime)
    {
        for (auto cascadeInfo : cascadeEndDistancesAndResolution)
        {
			D3D11_VIEWPORT vp = { 0.f, 0.f, static_cast<float>(cascadeInfo.second) , static_cast<float>(cascadeInfo.second), 0.f, 1.f };
            m_cascades.push_back({ Matrix::Identity, cascadeInfo.first, cascadeInfo.second, nullptr, vp });
        }

        // must be here because the argument is dependent on the Camera which can vary.
        // Textures for shadow map
        for (int i = 0; i < m_cascades.size(); ++i)
        {
            DXTexture::Desc desc{};
            desc.type = DXTexture::Type::TEX2D;

            desc.desc2D.Width = m_cascades[i].viewport.Width;
            desc.desc2D.Height = m_cascades[i].viewport.Height;
            desc.desc2D.MipLevels = 1;
            desc.desc2D.ArraySize = 1;
            desc.desc2D.Format = DXGI_FORMAT_R32_TYPELESS;
            desc.desc2D.SampleDesc.Count = 1;
            desc.desc2D.SampleDesc.Quality = 0;
            desc.desc2D.Usage = D3D11_USAGE_DEFAULT;
            desc.desc2D.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            desc.desc2D.CPUAccessFlags = 0;
            desc.desc2D.MiscFlags = 0;

            m_cascades[i].texture = m_renderer->getDXDevice()->createTexture(desc, nullptr);
        }

        m_firstTime = false;
    }
    else
    {
        if (cascadeEndDistancesAndResolution.size() != m_cascades.size()) assert(false);

        for (int i = 0; i < m_cascades.size(); ++i)
        {
            m_cascades[i].cascadeEnd = cascadeEndDistancesAndResolution[i].first;
            m_cascades[i].resolution = cascadeEndDistancesAndResolution[i].second;
        }
    }



}

ShadowMapper::OrthoMatrices ShadowMapper::createOrthos(const std::shared_ptr<Camera>& cam, const DirectX::SimpleMath::Matrix& lightCamView)
{
	/*
	Define three view frustums (near, middle, far)
	*/
	const int cascadeCount = 3;

	OrthoInfo orthos[cascadeCount];


	// near is previous far
	//float frustumPlaneDist[4] = { m_cascades[0].cascadeStart, 
	//	m_cascades[1].cascadeStart, 
	//	(m_cascades[1].cascadeStart + m_cascades[2].cascadeStart) / 2, 
	//	m_cascades[2].cascadeStart };

	float frustumPlaneDist[4] = { cam->getNearPlane(), 
		m_cascades[0].cascadeEnd, 
		m_cascades[1].cascadeEnd, 
		m_cascades[2].cascadeEnd };

	// To calculate the frustum corners --> Trigonometry. We calculate a factor that is common for all cascade calculations

	// 1. Lets get the horizontal fov and vertical fov separately
	float horizontalFov = cam->getFieldOfView();
	float verticalFov = atanf(tanf(horizontalFov / 2.f) * (9.f / 16.f)) * 2.f;		// Wiki: Calculate vertical FOV from horizontal. Assuming player camera has 16:9 aspect ratio here!

	float halfTanHori = tanf(horizontalFov / 2.f);
	float halfTanVert = tanf(verticalFov / 2.f);


	for (int ga = 0; ga < cascadeCount; ++ga)
	{
		float nearDist = frustumPlaneDist[ga];
		float farDist = frustumPlaneDist[ga + 1];


		// Calculate the eight corners of current cascade. Note that it is symmetrical! (Persepective view frustum)

		// Current (only numbers! Since they are symmetrical, the signs will be added later when defining the frustum points)
		float xNear = nearDist * halfTanHori;
		float yNear = nearDist * halfTanVert;

		float xFar = farDist * halfTanHori;
		float yFar = farDist * halfTanVert;

		Vector4 frustumPoints[8] =
		{
			// near
			Vector4(xNear, yNear, nearDist, 1.f),
			Vector4(-xNear, yNear, nearDist, 1.f),
			Vector4(xNear, -yNear, nearDist, 1.f),
			Vector4(-xNear, -yNear, nearDist, 1.f),

			// far
			Vector4(xFar, yFar, farDist, 1.f),
			Vector4(-xFar, yFar, farDist, 1.f),
			Vector4(xFar, -yFar, farDist, 1.f),
			Vector4(-xFar, -yFar, farDist, 1.f)

		};

		// We need to transform from view space to world space (so we can eventually get into light space)
		Matrix view = cam->getViewMatrix();
		Matrix invView = view.Invert();

		// Transform frustum points to world space
		for (int x = 0; x < 8; ++x)
		{
			frustumPoints[x] = DirectX::XMVector4Transform(frustumPoints[x], invView);

		}

		// Transform frustum points to light space
		for (int x = 0; x < 8; ++x)
		{
			frustumPoints[x] = DirectX::XMVector4Transform(frustumPoints[x], lightCamView);
		}

		// Find min/max for X, Y, Z respectively that bounds this cascade! (so we can define a "rectangle" to work with that encapsulates what the viewer sees
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();

		// Get min/max
		for (int x = 0; x < 8; ++x)
		{
			minX = min(minX, frustumPoints[x].x);
			maxX = max(maxX, frustumPoints[x].x);
			minY = min(minY, frustumPoints[x].y);
			maxY = max(maxY, frustumPoints[x].y);
			minZ = min(minZ, frustumPoints[x].z);
			maxZ = max(maxZ, frustumPoints[x].z);
		}

		orthos[ga].l = minX;
		orthos[ga].r = maxX;

		orthos[ga].t = maxY;
		orthos[ga].b = minY;

		orthos[ga].n = minZ;
		orthos[ga].f = maxZ;


	}

	OrthoMatrices mats;
	for (int a = 0; a < 3; ++a)
	{
		// Offsets to make the OBB bigger
	
		float l;
		float r;
		float t;
		float b;
		float n;
		float f;

		if (a == 0)
		{
			l = orthos[a].l - 60.f;
			r = orthos[a].r + 60.f;
			t = orthos[a].t + 40.f;
			b = orthos[a].b - 60.f;
			n = orthos[a].n - 200.f;
			f = orthos[a].f + 200.f;
		}
		else if (a == 2)
		{
			//l = orthos[a].l - 150.f;
			//r = orthos[a].r + 150.f;
			//t = orthos[a].t + 30.f;
			//b = orthos[a].b - 30.f;
			//n = orthos[a].n;
			//f = orthos[a].f;

			l = orthos[a].l - 150.f;
			r = orthos[a].r + 150.f;
			t = orthos[a].t + 150.f;
			b = orthos[a].b - 150.f;
			n = orthos[a].n - 200.f;
			f = orthos[a].f + 200.f;
		}
		else
		{
			//l = orthos[a].l - 150.f;
			//r = orthos[a].r + 150.f;
			//t = orthos[a].t + 70.f;
			//b = orthos[a].b - 70.f;
			//n = orthos[a].n - 200.f;
			//f = orthos[a].f + 200.f;

			l = orthos[a].l - 150.f;
			r = orthos[a].r + 150.f;
			t = orthos[a].t + 150.f;
			b = orthos[a].b - 150.f;
			n = orthos[a].n - 200.f;
			f = orthos[a].f + 200.f;
		}



		//float l = orthos[a].l - 150.f;
		//float r = orthos[a].r + 150.f;
		//float t = orthos[a].t + 150.f;
		//float b = orthos[a].b - 150.f;
		//float n = orthos[a].n - 200.f;
		//float f = orthos[a].f + 200.f;


		DirectX::XMFLOAT4X4 mat;

		ZeroMemory(&mat, sizeof(DirectX::XMFLOAT4X4));

		mat._11 = 2.f / (r - l);
		mat._41 = (-r - l) / (r - l);

		mat._22 = 2.f / (t - b);
		mat._42 = (-t - b) / (t - b);


		mat._33 = 1.f / (f - n);
		mat._43 = -n / (f - n);

		mat._44 = 1.f;

		DirectX::XMMATRIX res = DirectX::XMLoadFloat4x4(&mat);

		res = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		mats.orthoMats[a] = res;

		//if (a == 0)
		//{
		//	mats.close = res;
		//	std::wstring zmin = L"BACK Z VALUE:" + std::to_wstring(b) + L"\n";
		//	OutputDebugStringW(zmin.c_str());
		//}
		//else if (a == 1)
		//	mats.middle = res;
		//else
		//	mats.away = res;
	}
	
	// add..
	for (int i = 0; i < m_cascades.size(); ++i)
	{
		m_cascades[i].projMat = mats.orthoMats[i];
	}

	return mats;

	// Below are debug (ignore for now)
	float l = -30.f;
	float r = 30.f;
	float t = 15.f;
	float b = -15.f;
	float n = -100.f;
	float f = 75.f;

	/*
	Standard mapping (a, b) to (c, d):	We go from box in viewspace to ndc! (box to box) simple linear mapping

	c + ( (X-a)/(b-a) ) * (d - c)

	--> -1 + ( (X-a)/(b-a) ) * 2

	--> -1 + (2x-2a)/(b-a)

	--> (-b + a + 2x - 2a) / (b-a)

	--> 2x/(b-a) + -b-a/(b-a)			--> Dívided into two parts and fill in matrix!

	*/

	DirectX::XMFLOAT4X4 mat;

	ZeroMemory(&mat, sizeof(DirectX::XMFLOAT4X4));

	mat._11 = 2.f / (r - l);
	mat._41 = (-r - l) / (r - l);

	mat._22 = 2.f / (t - b);
	mat._42 = (-t - b) / (t - b);

	/*
	Special case, mapping from (a, b) to (0, 1)

	c + ( (X-a)/(b-a) ) * (d - c)

	--> 0 + ( (X-a)/(b-a) ) * (1 - 0)
	--> ( (X-a)/(b-a) )

	--> X/(b-a) - a/(b-a)

	*/

	mat._33 = 1.f / (f - n);
	mat._43 = -n / (f - n);

	mat._44 = 1.f;

	DirectX::XMMATRIX res = DirectX::XMLoadFloat4x4(&mat);

	return { res, res, res };

}

const std::vector<ShadowMapper::Cascade>& ShadowMapper::generateCascades(const std::vector<std::shared_ptr<Model>>& casters, const std::shared_ptr<Camera>& playerCamera, const DirectX::SimpleMath::Matrix& lightViewMatrix, 
	const std::shared_ptr<LineDrawer>& line)
{
	OrthoMatrices orthos = createOrthos(playerCamera, lightViewMatrix);

	// Bind 
	auto dev = m_renderer->getDXDevice();

	// Bind shader and resources
	dev->bindShader(m_vs, DXShader::Type::VS);
	dev->bindShader(m_ps, DXShader::Type::PS);
	dev->bindInputTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dev->bindInputLayout(m_il);
	dev->bindShaderConstantBuffer(DXShader::Type::VS, 0, m_bufferInfoCB);
	dev->bindDepthStencilState(m_dss);

	// for each cascade, draw all shadow casters
	for (int i = 0; i < m_cascades.size(); ++i)
	{
		dev->clearDepthTarget(m_cascades[i].texture);
		m_renderer->getDXDevice()->bindRenderTargets({ }, m_cascades[i].texture);

		//m_bufferInfo.viewMat = playerCamera->getViewMatrix();
		//m_bufferInfo.projMat = playerCamera->getProjectionMatrix();

		m_bufferInfo.viewMat = lightViewMatrix;
		m_bufferInfo.projMat = m_cascades[i].projMat;
		dev->bindViewports({ m_cascades[i].viewport });

		for (auto& mod : casters)
		{
			for (auto& mat : mod->getSubsetsMaterial())
			{
				m_bufferInfo.worldMat = mod->getTransform()->getWorldMatrix();
				m_bufferInfoCB->updateMapUnmap(&m_bufferInfo, sizeof(BufferInfo));


				dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
				dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);
			}
		}
	}

	// draw lines for each cascade
	for (int i = 0; i < m_cascades.size(); ++i)
	{
		m_renderer->getDXDevice()->bindRenderTargets({ }, m_cascades[i].texture);

		dev->bindViewports({ m_cascades[i].viewport });

		line->draw(playerCamera->getViewMatrix(), lightViewMatrix, m_cascades[i].projMat, true);
	}



	m_renderer->getDXDevice()->bindRenderTargets({ }, nullptr);


	// Return
	return m_cascades;
}
