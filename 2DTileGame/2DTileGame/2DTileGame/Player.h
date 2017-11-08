#pragma once

#include "Character.h"

class Player : public Character {
public:
	Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~Player();
	
	//AI
public:
	void UpdateAI(float deltaTime);

public:
	/*void UpdateMove(float deltaTime);
	void MoveDeltaPosition(float deltaX, float deltaY);*/
};