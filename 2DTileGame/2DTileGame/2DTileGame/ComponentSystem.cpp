#include <stdio.h>
#include "ComponentSystem.h"

ComponentSystem* ComponentSystem::_instance = NULL;

ComponentSystem* ComponentSystem::GetInstance() {
	if (_instance == NULL)
		_instance = new ComponentSystem();
	return _instance;
}

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem() {

}

void ComponentSystem::Addcomponent(std::wstring name, Component* com) {
	if (NULL != com) {
		std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
		if (it == _componentMap.end()) {
			_componentMap[name] = com;
		}
	}
}

void ComponentSystem::RemoveAllComponents() {
	for (std::map<std::wstring, Component*>::iterator it = _componentMap.begin(); 
		it != _componentMap.end(); 
		it++) {
		delete it->second;
	}
	_componentMap.clear();
}

Component* ComponentSystem::FindComponent(std::wstring name) {
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it != _componentMap.end()) {
		return it->second;
	}
	return 0;
}

void ComponentSystem::SendMessage(Component* sender, Component* receiver, std::wstring messeage) {
	receiver->ReceiveMessage(sender, messeage);
}