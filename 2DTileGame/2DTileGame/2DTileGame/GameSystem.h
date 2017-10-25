#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#define RELEASE_COM(x) {if(x != NULL) {x->Release(); x = NULL;}}
#define SAFE_DELETE(x) {if(x) {delete x; x = NULL;}}

//#define MAP_WIDTH 100
//#define MAP_HEIGHT 100

class Map;
class Character;
class GameTimer;

class GameSystem
{
private:
	static GameSystem* _instance;
public:
	static GameSystem* GetInstance();

private:
	GameSystem();
	~GameSystem();

public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int Update();

	int GetClientWidth();
	int GetClientHeight();

public:
	bool InitDirect3D();
	void CheckDeviceLost();

	//Direct3D
private:
	HWND _hWnd;

	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;
	D3DPRESENT_PARAMETERS _d3dpp;

	bool _isFullScreen;

	int _clientWidth = 1920;
	int _clientHeight = 1080;

	//Game
private:
	GameTimer* _gameTimer;
	float _frameDuration;
	
public:
	LPD3DXSPRITE GetSprite();
	LPDIRECT3DDEVICE9 GetDevice();
	
	//Map
private:
	/*Sprite* _testTileMap[MAP_HEIGHT][MAP_WIDTH];
	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;*/

	Map* _map;
	Character* _character;

public:
	void MapScrollTest(float deltaX, float deltaY);
};
