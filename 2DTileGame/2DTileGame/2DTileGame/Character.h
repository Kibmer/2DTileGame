#pragma once
#include <d3dx9.h>
#include "Component.h"
#include <vector>

class Sprite;

class Character : public Component {
private:
	//Sprite* _sprite;
	std::vector<Sprite*> _spriteList;
	float _x;
	float _y;

	int _tileX;
	int _tileY;


public:
	Character(LPCWSTR name);
	~Character();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	//transform
public:
	void MoveDeltaPosition(float deltaX, float deltaY);

public:
	void UpdateAI(float deltaTime);
	void UpdateMove(float deltaTime);

public:
	enum eDirection {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

private:
	float _targetX;
	float _targetY;
	float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;

	bool _isMoving;
	float _moveTime;
	float _movingDuration;

	eDirection _currentDirection;

public:
	void InitMove();
	void MoveStart(eDirection direction);
};