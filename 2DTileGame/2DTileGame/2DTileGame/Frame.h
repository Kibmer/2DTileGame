#pragma once
#include <d3dx9.h>

class Texture;

class Frame
{
private:
	LPD3DXSPRITE _sprite;
	//IDirect3DTexture9* _texture;
	Texture* _texture;

	//	D3DXIMAGE_INFO _textureInfo;
	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

	int _width;
	int _height;

	float _frameDelay;
	
	float _x;
	float _y;

public:
	Frame();
	~Frame();

	//x,y값만 바뀜->한 텍스처로 각각 다른 영역을 
	void Init(Texture* texture, int x, int y, int width, int height, float frameDelay);
	void DeInit();
	void Render();
	void Release();
	void Reset();
	float GetFrameDelay();

	void SetPosition(float x,float y);
};
