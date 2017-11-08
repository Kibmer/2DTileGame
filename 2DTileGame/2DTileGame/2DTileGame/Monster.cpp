#include "Monster.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Character(name, scriptName, textureFilename){

}

Monster::~Monster() {

}

void Monster::UpdateAI(float deltaTime) {
	if (false == _isMoving) {
		/*
		���� ��������,
		+- �� ĭ�� Ÿ���� �˻��� �� ����
		   �˻��ϴ� Ÿ�� ������ ���Ѵ�.
		������ ��� Ÿ���� �˻�
			NPC or Player�� ������
				���� ã�Ҵ�.
			�� ã������
				��� ĳ���� ������
		*/

		Component* findEnemy = NULL;
		// Ž���Ÿ��� ���� �ִ��� Ȯ��
		{//�ݺ�
			if (component->GetType() == eComponentType::CT_NPC || eComponentType::CT_PLAYER) {
				findEnemy = component;
				break;
			}
		}

		if (NULL != findEnemy) {
			//�߰� ���� ����
			eDirection direction = eDirection::NONE;
			MoveStart(direction);
		}
		else {
			Character::UpdateAI(deltaTime);
		}
	}
}