#pragma once

#include "Character.h"

class NPC : public Character{
public:
	NPC(LPCWSTR name);
	~NPC();

public:
	void UpdateAI(float deltaTime);
};