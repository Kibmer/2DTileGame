#pragma once
#include <d3dx9.h>
#include <string>
#include <map>
#include <string>
#include "Component.h"

class ComponentSystem {
private:
	static ComponentSystem* _instance;

public:
	static ComponentSystem* GetInstance();

private:
	ComponentSystem();

public:
	~ComponentSystem();

private:
	std::map<std::wstring, Component*> _componentMap;

public:
	void Addcomponent(std::wstring name, Component* com);
	void RemoveAllComponents();
	Component* FindComponent(std::wstring name);

public:
	void SendMessage(Component* sender, Component* receiver, std::wstring messeage);
};