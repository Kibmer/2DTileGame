#include <stdio.h>
#include <string>
#include "GameSystem.h"
#include "GameTimer.h"
#include "Map.h"
#include "Character.h"
#include "ComponentSystem.h"

GameSystem* GameSystem::_instance = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:

	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam) {
			ComponentSystem::GetInstance()->RemoveAllComponents();
			DestroyWindow(hWnd);
		}
		
		//Scroll Test
		if (VK_LEFT == wParam) {
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
		}
		return 0;

	case WM_KEYUP:
		GameSystem::GetInstance()->MapScrollTest(0.0f, 0.0f);
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

	_map = NULL;
	_character = NULL;
}
GameSystem::~GameSystem()
{
	if (NULL != _map) {
		_map->Deinit();
		delete _map;
		_map = NULL;
	}

	if (NULL != _character) {
		_character->Deinit();
		delete _character;
		_character = NULL;
	}
}

bool GameSystem::InitSystem(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; //���� �� â���� ���� �ѱ�
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance; //���� ���ø����̼��� �ڵ��� ����
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

	_hWnd = CreateWindow(L"BASE", L"TITLE", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0); //������ ����
	
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
	_map = new Map(L"Map");
	_map->Init();

	_character = new Character(L"testCharacter");
	_character->Init();

	return true;
}

int GameSystem::Update()
{
	_gameTimer->Reset();
	_frameDuration = 0.0f;
	//�޼��� ����
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//������
			TranslateMessage(&msg);
			DispatchMessage(&msg); //�����Ҵ�
		}
		else
		{
			_gameTimer->Update();

			float deltaTime = _gameTimer->GetDeltaTime();
			float secPerFrame = 1.0f / 60.0f;
			_frameDuration += deltaTime;

			_map->Update(deltaTime);
			_character->Update(deltaTime);
			
			if (secPerFrame <= _frameDuration)
			{
				wchar_t timeCheck[256];
				swprintf(timeCheck, L"deltaTime %f\n", _frameDuration);
				OutputDebugString(timeCheck);

				_frameDuration = 0.0f;
				_device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(53, 41, 42), 0.0f, 0);
				
				_device3d->BeginScene();
				
				_sprite->Begin(D3DXSPRITE_ALPHABLEND);
				
				_map->Render();
				_character->Render();
			
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
	if (FAILED(hr)) //��ȿ�� ���°� �ƴ϶�� failed�� fail�Ǿ���. device ���� 
	{
		if (D3DERR_DEVICELOST == hr) //device�ν�Ʈ ���°� �Ǿ� ����� ���� �Ұ���->���д� �ƴϾ�
		{
			Sleep(100); //100tick = 0.1sec
			return;
		}
		else if (D3DERR_DEVICENOTRESET == hr) //device �ٽ� �۵� ����
		{
			_map->Release();
			_character->Release();

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);
			
			_map->Reset();
			_character->Reset();
		}
	}
}
LPD3DXSPRITE GameSystem::GetSprite() {
	return _sprite;
}
LPDIRECT3DDEVICE9 GameSystem::GetDevice() {
	return _device3d;
}

void GameSystem::MapScrollTest(float deltaX, float deltaY) {
	_map->Scroll(deltaX, deltaY);
}