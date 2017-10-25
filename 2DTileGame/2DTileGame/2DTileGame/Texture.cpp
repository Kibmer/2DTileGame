#include "Texture.h"
#include "GameSystem.h"

Texture::Texture() {

}
Texture::~Texture() {

}

void Texture::Init(LPCWSTR fileName) {
	//���Ϸ� �̹��� ���� �ʺ� ������
	HRESULT hr = D3DXGetImageInfoFromFile(fileName, &_textureInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"ImageInfo Error", L"ImageInfo Error", MB_OK);
		return;
	}
	//����
	hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance()->GetDevice(),
		fileName, //�о�� ����
		_textureInfo.Width,
		_textureInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN, //�˾Ƽ�
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255), //bmp�� load�� ��,��->��������
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