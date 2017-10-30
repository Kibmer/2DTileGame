#include "Player.h"
#include "GameSystem.h"
#include "ComponentSystem.h"

Player::Player(LPCWSTR name) : Character(name){
	_moveTime = 0.5f;
}
Player::~Player() {

}

void Player::UpdateAI(float deltaTime) {
	
	if (false == _isMoving) {
		if (GameSystem::GetInstance()->IsKeyDown(VK_UP)) {
			MoveStart(eDirection::UP);
		}
		if (GameSystem::GetInstance()->IsKeyDown(VK_DOWN)) {
			MoveStart(eDirection::DOWN);
		}
		if (GameSystem::GetInstance()->IsKeyDown(VK_LEFT)) {
			MoveStart(eDirection::LEFT);
		}
		if (GameSystem::GetInstance()->IsKeyDown(VK_RIGHT)) {
			MoveStart(eDirection::RIGHT);
		}
	}
}

void Player::UpdateMove(float deltaTime) {
	if (false == _isMoving)
		return;

	if (_moveTime <= _movingDuration) {
		_movingDuration = 0.0f;
		_isMoving = false;

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		/*_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);*/
	}
	else {
		_movingDuration += deltaTime;

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;

		/*_x += moveDistanceX;
		_y += moveDistanceY;*/
	}
}