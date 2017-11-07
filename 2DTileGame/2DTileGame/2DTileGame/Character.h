#pragma once
#include <d3dx9.h>
#include "Component.h"
#include <vector>
#include <string>

class Sprite;

class Character : public Component {
private:
	std::vector<Sprite*> _spriteList;
	std::wstring _scriptFilename;
	std::wstring _textureFilename;
	float _x;
	float _y;

public:
	Character(LPCWSTR name, LPCWSTR scriptName ,LPCWSTR textureFilename);
	virtual ~Character();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	//transform
public:
	virtual void MoveDeltaPosition(float deltaX, float deltaY);
	void SetPosition(float posX, float posY);

public:
	virtual void UpdateAI(float deltaTime);
	virtual void UpdateMove(float deltaTime);

public:
	enum eDirection {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

protected:
	float _targetX;
	float _targetY;
	/*float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;*/

	bool _isMoving;
	float _moveTime;
	float _movingDuration;

	eDirection _currentDirection;

public:
	void InitMove();
	void MoveStart(eDirection direction);
};