#include <fstream>
#include <reader.h>

#include "GameSystem.h"
#include "ResourceManager.h"

#include "Sprite.h"
#include "Frame.h"
#include "Texture.h"

Sprite::Sprite(LPCWSTR textureName, LPCWSTR scriptName) :
	_currentFrame(0), _frameTime(0.0f), _srcTexture(NULL),
	_textureName(textureName), _scriptName(scriptName)
{

}
Sprite::~Sprite()
{
	if (NULL != _srcTexture)
	{
		_srcTexture->DeInit();
		delete _srcTexture;
	}
}

void Sprite::Init()
{
	_device3d = GameSystem::GetInstance()->GetDevice();
	_sprite = GameSystem::GetInstance()->GetSprite();
	
	_srcTexture = ResourceManager::GetInstance()->LoadTexture(_textureName);

	//json parsing
	{
		std::vector<std::string> scriptTextList = ResourceManager::GetInstance()->LoadScript(_scriptName);

		for(int i = 0; i < scriptTextList.size(); i++)
		{
			std::string record = scriptTextList[i];

			Json::Value root;
			Json::Reader reader;
			bool isSuccess = reader.parse(record, root);
			if (isSuccess)
			{
				std::string texture = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double delay = root["framedelay"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_srcTexture, x, y, width, height, delay);
				_frameList.push_back(frame);
			}
		}
	}

	//Texture
	{
		_currentFrame = 0;
		_frameTime = 0.0f;
	}
}
void Sprite::Init(int srcX, int srcY, int width, int height, float frameDelay)
{
	_device3d = GameSystem::GetInstance()->GetDevice();
	_sprite = GameSystem::GetInstance()->GetSprite();

	_srcTexture = ResourceManager::GetInstance()->LoadTexture(_textureName);

	//json parsing->
	{
		Frame* frame = new Frame();
		frame->Init(_srcTexture, srcX, srcY, width, height, frameDelay);
		_frameList.push_back(frame);
	}

	//Texture
	_currentFrame = 0;
	_frameTime = 0.0f;
}
void Sprite::Deinit()
{
	std::vector<Frame*>::iterator it = _frameList.begin();
	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->DeInit();
		delete frame;
	}
	_frameList.clear();

	_srcTexture->DeInit();
}

void Sprite::Update(float deltaTime)
{
	_frameTime += deltaTime;
	if (_frameList[_currentFrame]->GetFrameDelay() <= _frameTime)
	{
		_frameTime = 0.0f;
		_currentFrame = (_currentFrame + 1) % _frameList.size();
	}
	/*
	누적된 시간이, 프레임 딜레이를 넘어가면
		다음 프레임
		누적된 시간은 다시 리셋
	*/
}
void Sprite::Render()
{
	if (_currentFrame < _frameList.size())
	{
		_frameList[_currentFrame]->SetPosition(_x, _y);
		_frameList[_currentFrame]->Render(); //1개만 출력
	}
}

void Sprite::Release()
{
	std::vector<Frame*>::iterator it = _frameList.begin();
	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->Release();
	}

	_srcTexture->Release();
}
void Sprite::Reset()
{
	Init();

	std::vector<Frame*>::iterator it = _frameList.begin();
	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->Reset();
	}
}

void Sprite::SetPosition(float posX, float posY)
{
	_x = posX;
	_y = posY;
}