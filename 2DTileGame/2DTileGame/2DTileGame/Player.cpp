#include "Player.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Map.h"

Player::Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Character(name, scriptName, textureFilename){
	_moveTime = 0.3f;
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

//void Player::UpdateMove(float deltaTime) {
//	if (false == _isMoving)
//		return;
//
//	if (_moveTime <= _movingDuration) {
//		_movingDuration = 0.0f;
//		_isMoving = false;
//
//		_moveDistancePerTimeX = 0.0f;
//		_moveDistancePerTimeY = 0.0f;
//	}
//	else {
//		_movingDuration += deltaTime;
//	}
//}
//
//void Player::MoveDeltaPosition(float deltaX, float deltaY) {
//
//}