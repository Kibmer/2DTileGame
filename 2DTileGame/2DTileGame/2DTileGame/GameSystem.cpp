#include <stdio.h>
#include <string>
#include "GameSystem.h"
#include "GameTimer.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "ComponentSystem.h"
#include "Component.h"
#include <list>

GameSystem* GameSystem::_instance = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:

	case WM_KEYDOWN:
		GameSystem::GetInstance()->KeyDown(wParam);
		if (VK_ESCAPE == wParam) {
			ComponentSystem::GetInstance()->RemoveAllComponents();
			DestroyWindow(hWnd);
		}
		
		//Scroll Test
		/*if (VK_LEFT == wParam) {
			GameSystem::GetInstance()->MapScrollTest(3.0f, 0.0f);
		}
		if (VK_RIGHT == wParam) {
			GameSystem::GetInstance()->MapScrollTest(-3.0f, 0.0f);
		}
		if (VK_UP == wParam) {
			GameSystem::GetInstance()->MapScrollTest(0.0f, 3.0f);
		}
		if (VK_DOWN == wParam) {
			GameSystem::GetInstance()->MapScrollTest(0.0f, -3.0f);
		}*/
		return 0;

	case WM_KEYUP:
		//GameSystem::GetInstance()->MapScrollTest(0.0f, 0.0f);
		GameSystem::GetInstance()->KeyUp(wParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameSystem* GameSystem::GetInstance()
{
	if (_instance == NULL)
		_instance = new GameSystem();
	return _instance;
}
GameSystem::GameSystem()
{
	_gameTimer = new GameTimer();
	_isFullScreen = false;

	//_map = NULL;
	//_player = NULL;
	//_npc = NULL;
}
GameSystem::~GameSystem()
{
}

bool GameSystem::InitSystem(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; //윈도 각 창으로 역할 넘김
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance; //현재 어플리케이션의 핸들을 연결
							  //GUI
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BASE"; 

	if (!RegisterClass(&wc))
	{
		return false;
	}

	DWORD style;
	if (_isFullScreen)
	{


		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}

	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	_hWnd = CreateWindow(L"BASE", L"TITLE", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0); //윈도우 생성
	
	if (_isFullScreen == false)
	{
		RECT clientRect;
		GetClientRect(_hWnd, &clientRect);

		int addWidth = _clientWidth - clientRect.right;
		int addHeight = _clientHeight - clientRect.bottom;
		int finalwidth = _clientWidth + addWidth;
		int finalheight = _clientHeight + addHeight;

		MoveWindow(_hWnd, 0, 0, finalwidth, finalheight, TRUE);
	}

	ShowWindow(_hWnd, nCmdShow);

	UpdateWindow(_hWnd);

	if (false == InitDirect3D())
	{
		return false;
	}

	InitInput();

	_componentList.clear();

	Map* map = new Map(L"Map");
	_componentList.push_back(map);
	
	Player* player = new Player(L"player", L"player", L"player");
	_componentList.push_back(player);

	NPC* npc = new NPC(L"npc", L"npc", L"character_sprite2");
	_componentList.push_back(npc);
	/*for (int i = 0; i < 1; i++) {
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		NPC* npc = new NPC(name, L"npc", L"character_sprite2");
		_componentList.push_back(npc);
	}*/

	
	Monster* monster = new Monster(L"monster", L"monster",  L"character_sprite2");
	_componentList.push_back(monster);

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++) {
		(*it)->Init();
	}

	//map->InitViewer(npc);
	map->InitViewer(player);

	return true;
}

int GameSystem::Update()
{
	_gameTimer->Reset();
	_frameDuration = 0.0f;
	//메세지 루프
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//윈도우
			TranslateMessage(&msg);
			DispatchMessage(&msg); //실제할당
		}
		else
		{
			_gameTimer->Update();

			float deltaTime = _gameTimer->GetDeltaTime();
			float secPerFrame = 1.0f / 60.0f;
			_frameDuration += deltaTime;

			/*_map->Update(deltaTime);
			_player->Update(deltaTime);
			_npc->Update(deltaTime);
			_monster->Update(deltaTime);*/

			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++) {
				(*it)->Update(deltaTime);
			}

			if (secPerFrame <= _frameDuration)
			{
				wchar_t timeCheck[256];
				//swprintf(timeCheck, L"deltaTime %f\n", _frameDuration);
				OutputDebugString(timeCheck);

				_frameDuration = 0.0f;
				_device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(53, 41, 42), 0.0f, 0);
				
				_device3d->BeginScene();
				
				_sprite->Begin(D3DXSPRITE_ALPHABLEND);
				
				/*_map->Render();
				_player->Render();
				_npc->Render();
				_monster->Render();
*/
				for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++) {
					(*it)->Render();
				}

				_sprite->End();
				
				_device3d->EndScene();
				
				CheckDeviceLost();

				_device3d->Present(NULL, NULL, NULL, NULL);
			}
		}
	}

	return (int)msg.wParam;
}
int GameSystem::GetClientWidth()
{
	return 	_clientWidth;
}
int GameSystem::GetClientHeight()
{
	return _clientHeight;
}


bool GameSystem::InitDirect3D()
{
	LPDIRECT3D9 direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
	{
		MessageBox(0, L"Direct3DCreate9 Error", L"Direct3DCreate9 Error", MB_OK);
		return false;
	}

	ZeroMemory(&_d3dpp, sizeof(_d3dpp));
	_d3dpp.BackBufferWidth = _clientWidth;
	_d3dpp.BackBufferHeight = _clientHeight;

	if (_isFullScreen)
	{
		_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	}

	_d3dpp.BackBufferCount = 1;
	_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dpp.hDeviceWindow = _hWnd;
	_d3dpp.Windowed = (!_isFullScreen);
	_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;

	HRESULT hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (FAILED(hr))
	{
		MessageBox(0, L"GetDeviceCaps Error", L"GetDeviceCaps Error", MB_OK);
		return false;
	}

	DWORD behavior;
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, behavior, &_d3dpp, &_device3d);
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateDevice Error", L"CreateDevice Error", MB_OK);
		return false;
	}

	hr = D3DXCreateSprite(_device3d, &_sprite);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3DXCreateSprite Error", L"D3DXCreateSprite Error", MB_OK);
		return false;
	}
	return true;
}
void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();
	if (FAILED(hr)) //유효한 상태가 아니라면 failed가 fail되었다. device 오류 
	{
		if (D3DERR_DEVICELOST == hr) //device로스트 상태가 되어 현재는 복구 불가능->실패는 아니야
		{
			Sleep(100); //100tick = 0.1sec
			return;
		}
		else if (D3DERR_DEVICENOTRESET == hr) //device 다시 작동 가능
		{
			/*_map->Release();
			_player->Release();
			_npc->Release();
			_monster->Release();*/
			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++) {
				(*it)->Release();
			}

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);
			
			/*_map->Reset();
			_player->Reset();
			_npc->Reset();
			_monster->Reset();*/

			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++) {
				(*it)->Reset();
			}
		}
	}
}
LPD3DXSPRITE GameSystem::GetSprite() {
	return _sprite;
}

void GameSystem::InitInput() {
	for (int i = 0; i < 256; i++) {
		_keyState[i] = eKeyState::KEY_UP;
	}
}

void GameSystem::KeyDown(unsigned int keyCode) {
	_keyState[keyCode] = eKeyState::KEY_DOWN;
}
void GameSystem::KeyUp(unsigned int keyCode) {
	_keyState[keyCode] = eKeyState::KEY_UP;
}

bool GameSystem::IsKeyDown(unsigned int keyCode) {
	return(eKeyState::KEY_DOWN == _keyState[keyCode]);
}

LPDIRECT3DDEVICE9 GameSystem::GetDevice() {
	return _device3d;
}