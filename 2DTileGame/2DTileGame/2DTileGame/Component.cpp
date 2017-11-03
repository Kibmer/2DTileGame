#include "ComponentSystem.h"
#include "Component.h"

Component::Component(LPCWSTR name) {
	_name = name;
	ComponentSystem::GetInstance()->Addcomponent(_name, this);
	_canMove = false;
}

Component::~Component() {

}