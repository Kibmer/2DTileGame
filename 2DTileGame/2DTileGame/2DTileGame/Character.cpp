#include "Character.h"
#include "Sprite.h"
#include "ComponentSystem.h"
#include "Map.h"

Character::Character(LPCWSTR name) : Component(name) {
	_targetX = 0.0f;
	_targetY = 0.0f;
	_moveDistancePerTimeX = 0.0f;
	_moveDistancePerTimeY = 0.0f;
	_spriteList.clear();
}

Character::~Character() {
	
}

void Character::Init() {
	WCHAR textureFilename[256];
	WCHAR scriptFilename[256];
	/*wsprintf(textureFilename, L"%s.png", _name);
	wsprintf(scriptFilename, L"%s.json", _name);
	_sprite = new Sprite(textureFilename, scriptFilename);
	_sprite->Init();*/

	{
		wsprintf(textureFilename, L"%s.png", _name);
		wsprintf(scriptFilename, L"%s_left.json", _name);
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFilename, L"%s.png", _name);
		wsprintf(scriptFilename, L"%s_right.json", _name);
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFilename, L"%s.png", _name);
		wsprintf(scriptFilename, L"%s_up.json", _name);
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFilename, L"%s.png", _name);
		wsprintf(scriptFilename, L"%s_down.json", _name);
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		_tileX = 7;
		_tileY = 3;
		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	InitMove();
}

void Character::Deinit() {
	/*if (_sprite != NULL) {
		_sprite->Deinit();
		delete _sprite;
		_sprite = NULL;
	}*/
	for (int i = 0; i < _spriteList.size(); i++) {
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Character::Update(float deltaTime) {
	_spriteList[(int)_currentDirection]->Update(deltaTime);

	UpdateAI(deltaTime);
	UpdateMove(deltaTime);
}

void Character::Render() {
	/*_sprite->SetPosition(_x, _y);
	_sprite->Render();*/
	_spriteList[(int)_currentDirection]->SetPosition(_x, _y);
	_spriteList[(int)_currentDirection]->Render();
}

void Character::Release() {
	//_sprite->Release();
	for (int i = 0; i < _spriteList.size(); i++) {
		_spriteList[i]->Release();
	}
}

void Character::Reset() {
	//_sprite->Reset();
	for (int i = 0; i < _spriteList.size(); i++) {
		_spriteList[i]->Reset();
	}
}

void Character::MoveDeltaPosition(float deltaX, float deltaY) {
	_x += deltaX;
	_y += deltaY;
}

void Character::UpdateAI(float deltaTime) {

}

void Character::InitMove() {
	_isMoving = false;
	_movingDuration = 0.0f;
	_moveTime = 1.0f;

	_currentDirection = eDirection::DOWN;
}

void Character::MoveStart(eDirection direction) {
	if (true == _isMoving) {
		return;
	}
	_currentDirection = direction;

	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	map->ResetTileComponent(_tileX, _tileY, this);

	switch (direction) {
	case eDirection::LEFT: // left
		_tileX--;
		if (_tileX < 0) {
			_tileX = 0;
		}
		break;
	case eDirection::RIGHT: // right
		_tileX++;
		if (_tileX > 20) {
			_tileX = 0;
		}
		break;
	case eDirection::UP: // up
		_tileY--;
		if (_tileY < 0) {
			_tileY = 0;
		}
		break;
	case eDirection::DOWN: // down
		_tileY++;
		if (_tileX > 20) {
			_tileX = 0;
		}
		break;
	}
/*
	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);
	map->SetTileComponent(_tileX, _tileY, this);
*/

	// 자연스러운 이동을 위한 보간
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		//이동거리
	    _targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);
		
		float distanceX = _targetX - _x;
		float distanceY = _targetY - _y;

		// 최소 이동거리
		_moveDistancePerTimeX = distanceX / _moveTime;
		_moveDistancePerTimeY = distanceY / _moveTime;
	}

	_isMoving = true;

}

void Character::UpdateMove(float deltaTime) {
	if (false == _isMoving) {
		return;
	}

	if (_moveTime <= _movingDuration) {
		_movingDuration = 0.0f;
		_isMoving = false;
		_x = _targetX;
		_y = _targetY;
	}
	else {
		_movingDuration += deltaTime;

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;

		_x += moveDistanceX;
		_y += moveDistanceY;
	}
}