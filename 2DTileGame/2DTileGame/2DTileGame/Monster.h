#pragma once
#include "Character.h"

class Monster : public Character {
public:
	Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~Monster();

public:
	void UpdateAI(float deltaTime);
};