#include "Character.h"
#include "Sprite.h"
#include "ComponentSystem.h"
#include "Map.h"
#include <list>

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Component(name) {

	_spriteList.clear();
	_textureFilename = textureFilename;
	_scriptFilename = scriptName;
	_moveTime = 1.0f;
}

Character::~Character() {
	
}

void Character::Init() {
	WCHAR textureFilename[256];
	WCHAR scriptFilename[256];

	wsprintf(textureFilename, L"%s.png", _textureFilename.c_str());

	{
		wsprintf(scriptFilename, L"%s_left.json", _scriptFilename.c_str());
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_right.json", _scriptFilename.c_str());
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_up.json", _scriptFilename.c_str());
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_down.json", _scriptFilename.c_str());
		Sprite* sprite = new Sprite(textureFilename, scriptFilename);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		_tileX = rand() % (map->GetWidth() - 1) + 1;
		_tileY = rand() % (map->GetHeight() - 1) + 1;

		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	InitMove();
}

void Character::Deinit() {
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
	_spriteList[(int)_currentDirection]->SetPosition(_x, _y);
	_spriteList[(int)_currentDirection]->Render();
}

void Character::Release() {
	for (int i = 0; i < _spriteList.size(); i++) {
		_spriteList[i]->Release();
	}
}

void Character::Reset() {
	for (int i = 0; i < _spriteList.size(); i++) {
		_spriteList[i]->Reset();
	}
}

void Character::MoveDeltaPosition(float deltaX, float deltaY) {
	_x += deltaX;
	_y += deltaY;
}

void Character::SetPosition(float posX, float posY) {
	_x = posX;
	_y = posY;
}

void Character::UpdateAI(float deltaTime) {
	if (false == _isMoving) {
		int direction = rand() % 4;
		MoveStart((eDirection)direction);
	}
}

void Character::InitMove() {
	_isMoving = false;
	_movingDuration = 0.0f;

	_currentDirection = eDirection::DOWN;
}

void Character::MoveStart(eDirection direction) {
	if (true == _isMoving) {
		return;
	}
	_currentDirection = direction;

	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	int newTileX = _tileX;
	int newTileY = _tileY;

	switch (direction) {
	case eDirection::LEFT: // left
		newTileX--;
		break;
	case eDirection::RIGHT: // right
		newTileX++;
		break;
	case eDirection::UP: // up
		newTileY--;
		break;
	case eDirection::DOWN: // down
		newTileY++;
		break;
	}
	std::list<Component*> collisionList;
	bool canMove = map->GetTileCollisionList(newTileX, newTileY, collisionList);
	if (false == canMove) {
		for (std::list<Component*>::iterator it = collisionList.begin(); it != collisionList.end(); it++) {
			ComponentSystem::GetInstance()->SendMessage(this, (*it), L"Collision");

		}
		return;
	}
	map->ResetTileComponent(_tileX, _tileY, this);
	_tileX = newTileX;
	_tileY = newTileY;

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

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);

		_moveDistancePerTimeX = 0.0f;
		_moveDistancePerTimeY = 0.0f;
	}
	else {
		_movingDuration += deltaTime;

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;

		_x += moveDistanceX;
		_y += moveDistanceY;
	}
}