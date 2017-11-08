#include "Monster.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Character(name, scriptName, textureFilename){

}

Monster::~Monster() {

}

void Monster::UpdateAI(float deltaTime) {
	if (false == _isMoving) {
		/*
		나를 기준으로,
		+- 몇 칸의 타일을 검사할 지 결정
		   검사하는 타일 범위를 구한다.
		범위내 모든 타일을 검사
			NPC or Player가 있으면
				적을 찾았다.
			못 찾았으면
				평소 캐릭터 움직임
		*/

		Component* findEnemy = NULL;
		// 탐색거리에 적이 있는지 확인
		{//반복
			if (component->GetType() == eComponentType::CT_NPC || eComponentType::CT_PLAYER) {
				findEnemy = component;
				break;
			}
		}

		if (NULL != findEnemy) {
			//추격 방향 설정
			eDirection direction = eDirection::NONE;
			MoveStart(direction);
		}
		else {
			Character::UpdateAI(deltaTime);
		}
	}
}