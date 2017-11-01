#include "Component.h"
#include "TileCell.h"
#include <list>

TileCell::TileCell() {

}

TileCell::~TileCell() {

}

void TileCell::Deinit() {
	
}
void TileCell::Update(float deltaTime) {
	
}
void TileCell::Render() {
	

	for (std::list<Component*>::iterator it = _renderList.begin(); it != _renderList.end(); it++)
	{
		(*it)->Render();
	}
}
void TileCell::Release() {
	
}
void TileCell::Reset() {
	
}

void TileCell::SetPosition(float posX, float posY) {
	_posX = posX;
	_posY = posY;
	//_sprite->SetPosition(posX, posY);

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->SetPosition(posX, posY);
	}
}

//void TileCell::SetSprite(Sprite* sprite) {
//	_sprite = sprite;
//}


void TileCell::MoveDeltaPosition(float deltaX, float deltaY) {
	_posX += deltaX;
	_posY += deltaY;
	//_sprite->SetPosition(_posX, _posY);

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->MoveDeltaPosition(deltaX, deltaY);
	}
}

float TileCell::GetPositionX() {
	return _posX;
}
float TileCell::GetPositionY() {
	return _posY;
}

void TileCell::AddComponent(Component* component, bool isRender) {
	_componentList.push_back(component);

	if (isRender) {
		_renderList.push_back(component);
	}
}
void TileCell::RemoveComponent(Component* component) {
	_componentList.remove(component);
	_renderList.remove(component);
}

bool TileCell::CanMove() {
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		if (false == (*it)->CanMove())
			return false;
	}

	return true;
}