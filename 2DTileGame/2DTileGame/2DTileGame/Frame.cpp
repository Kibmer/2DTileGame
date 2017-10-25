#include "GameSystem.h"

#include "Frame.h"
#include "Texture.h"

Frame::Frame() :
	_frameDelay(0.0f)
{

}
Frame::~Frame()
{

}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float frameDelay)
{
	_sprite = GameSystem::GetInstance()->GetSprite();
	_texture = texture;

	_width = width;
	_height = height;

	_srcTextureRect.left = x;
	_srcTextureRect.top = y;
	_srcTextureRect.right = _srcTextureRect.left + _width;
	_srcTextureRect.bottom = _srcTextureRect.top + _height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	_frameDelay = frameDelay;
}
void Frame::DeInit()
{

}
void Frame::Render()
{
	//sprite 출력전 모양 조정(위치, 크기, 회전)
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)_width / 2.0f, (float)_height / 2.0f);
	D3DXVECTOR2 translate = D3DXVECTOR2(_x - (float)_width / 2.0f, _y - (float)_height / 2.0f); //위치
	D3DXVECTOR2 scaling = D3DXVECTOR2(1.0f, 1.0f); //크기

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f, //회전
		&scaling,
		&spriteCenter,
		0.0f,
		&translate
	);

	//생성행렬적용
	_sprite->SetTransform(&matrix);

	//그려줘
	_sprite->Draw(_texture->GetTextureDX(), &_srcTextureRect, NULL, NULL, _textureColor); //pivot은 직접 조정해줄거라 null, textureColor 흰색->그대로 보여주겠다
}
void Frame::Release()
{

}
void Frame::Reset()
{

}
float Frame::GetFrameDelay() {
	return _frameDelay;
}

void Frame::SetPosition(float x, float y) {
	_x = x;
	_y = y;
}