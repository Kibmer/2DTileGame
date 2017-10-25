#include "Texture.h"
#include "GameSystem.h"

Texture::Texture() {

}
Texture::~Texture() {

}

void Texture::Init(LPCWSTR fileName) {
	//파일로 이미지 폭과 너비를 가져옴
	HRESULT hr = D3DXGetImageInfoFromFile(fileName, &_textureInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"ImageInfo Error", L"ImageInfo Error", MB_OK);
		return;
	}
	//생성
	hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance()->GetDevice(),
		fileName, //읽어올 파일
		_textureInfo.Width,
		_textureInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN, //알아서
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255), //bmp로 load시 흰,검->투명값으로
		&_textureInfo,
		NULL,
		&_texture
	);
}

void Texture::Release() {

}
void Texture::DeInit() {

}

IDirect3DTexture9* Texture::GetTextureDX() {
	return _texture;
}