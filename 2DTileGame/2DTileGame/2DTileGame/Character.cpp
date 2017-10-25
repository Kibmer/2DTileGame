#include "Character.h"
#include "Sprite.h"
#include "ComponentSystem.h"
#include "Map.h"

Character::Character(LPCWSTR name) : Component(name) {
	_sprite = NULL;
}

Character::~Character() {
	
}

void Character::Init() {
	WCHAR textureFilename[256];
	WCHAR scriptFilename[256];
	wsprintf(textureFilename, L"%s.png", _name);
	wsprintf(scriptFilename, L"%s.json", _name);
	_sprite = new Sprite(textureFilename, scriptFilename);
	_sprite->Init();

	/*_x = 100.0f;
	_y = 500.0f;*/
	{
		/*int mapTileSize = 32;
		int tileX = 2;
		int tileY = 2;

		_x = tileX * mapTileSize;
		_y = tileY * mapTileSize;*/

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		_tileX = 7;
		_tileY = 3;
		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this);
	}
	InitMove();
}

void Character::Deinit() {
	if (_sprite != NULL) {
		_sprite->Deinit();
		delete _sprite;
		_sprite = NULL;
	}
}

void Character::Update(float deltaTime) {
	_sprite->Update(deltaTime);

	UpdateAI(deltaTime);
}

void Character::Render() {
	_sprite->SetPosition(_x, _y);
	_sprite->Render();
}

void Character::Release() {
	_sprite->Release();
}

void Character::Reset() {
	_sprite->Reset();
}

void Character::UpdateAI(float deltaTime) {
	if (false == _isMoving) {
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		map->ResetTileComponent(_tileX, _tileY, this);

		int direction = rand() % 4;
		switch (direction) {
		case 0: // left
			_tileX--;
			if (_tileX < 0) {
				_tileX = 0;
			}
			break;
		case 1: // right
			_tileX++;
			if (_tileX > 20) {
				_tileX = 0;
			}
			break;
		case 2: // up
			_tileY--;
			if (_tileY < 0) {
				_tileY = 0;
			}
			break;
		case 3: // down
			_tileY++;
			if (_tileX > 20) {
				_tileX = 0;
			}
			break;
		}

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this);

		_isMoving = true;
	}
	else {
		if (_moveTime <= _movingDuration) {
			_movingDuration = 0.0f;
			_isMoving = false;
		}
		else {
			_movingDuration += deltaTime;
		}
	}
}

void Character::InitMove() {
	_isMoving = false;
	_movingDuration = 0.0f;
	_moveTime = 1.0f;
}