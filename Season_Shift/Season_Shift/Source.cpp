#include "Window.h"
#include "Graphics/Graphics.h"
#include <string>
#include "GameObject.h"



bool b = true;
int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	Window win(inst, L"Season Shift", 1280, 720);	

	Graphics gph(win.getHWND(), win.getClientWidth(), win.getClientHeight());

	Ref<GameObject> go1 = std::make_shared<GameObject>();
	go1->AddComponent(std::make_shared<Transform>());
	go1->getTransform()->setPosition(DirectX::XMFLOAT3(0, 0, 0));
	go1->AddComponent(std::make_shared<Collider>(go1->getTransform()->getPosition()));

	Ref<GameObject> go2 = std::make_shared<GameObject>();
	go2->AddComponent(std::make_shared<Transform>());
	go2->getTransform()->setPosition(DirectX::XMFLOAT3(3, 0, 0));
	go2->AddComponent(std::make_shared<Collider>(go2->getTransform()->getPosition()));

	DirectX::BoundingSphere sp1 = go1->getComponentType<Collider>()->getCollider();
	DirectX::BoundingSphere sp2 = go2->getComponentType<Collider>()->getCollider();
	
	std::wstring outPut = L"no intersect";
	if (sp1.Intersects(sp2))
	{
		outPut = L"intersect";
	}
	OutputDebugString(outPut.c_str());


	MSG msg = { };
	while (!win.isClosed())
	{
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Do stuff
		gph.render();

	}




	return 0;
}