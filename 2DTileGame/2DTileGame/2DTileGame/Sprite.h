#pragma once
#include <vector>
#include <d3dx9.h>

class Frame;
class Texture;

class Sprite
{
private:
	std::vector<Frame*> _frameList;
	Texture* _srcTexture;

	int _currentFrame;
	float _frameTime;
	LPCWSTR _textureName;
	LPCWSTR _scriptName;

	float _x, _y;

public:
	Sprite(LPCWSTR textureName, LPCWSTR scriptName);
	~Sprite();

public:
	void Init();
	void Init(int srcX, int srcY, int width, int height, float frameDelay);
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	void SetPosition(float posX, float posY);

	//DirextX
private:
	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;
};