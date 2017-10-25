#pragma once
#include <d3dx9.h>
#include "Component.h"

class Sprite;

class Character : public Component {
private:
	Sprite* _sprite;

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

public:
	void UpdateAI(float deltaTime);

private:
	bool _isMoving;
	float _moveTime;
	float _movingDuration;

public:
	void Character::InitMove();
};