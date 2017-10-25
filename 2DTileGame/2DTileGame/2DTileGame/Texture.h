#pragma once
#include <d3dx9.h>

class Texture {
private:
	D3DXIMAGE_INFO _textureInfo;
	IDirect3DTexture9* _texture;

public:
	Texture();
	~Texture();

	void Init(LPCWSTR fileName);

	void Release();
	void DeInit();

	IDirect3DTexture9* GetTextureDX();
};