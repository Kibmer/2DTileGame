#pragma once

#include "Character.h"

class Player : public Character {
public:
	Player(LPCWSTR name, LPCWSTR textureFilename);
	~Player();
	
	//AI
public:
	void UpdateAI(float deltaTime);
	void UpdateMove(float deltaTime);
	void MoveDeltaPosition(float deltaX, float deltaY);
};