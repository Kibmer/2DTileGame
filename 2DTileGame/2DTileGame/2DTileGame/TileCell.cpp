#include "Component.h"
#include "TileCell.h"
#include "Sprite.h"
#include <list>

TileCell::TileCell() {

}

TileCell::~TileCell() {

}

void TileCell::Deinit() {
	_sprite->Deinit();
}
void TileCell::Update(float deltaTime) {
	_sprite->Update(deltaTime);
}
void TileCell::Render() {
	_sprite->Render();

	for (std::list<Component*>::iterator it = _renderList.begin(); it != _renderList.end(); it++)
	{
		(*it)->Render();
	}
}
void TileCell::Release() {
	_sprite->Release();
}
void TileCell::Reset() {
	_sprite->Reset();
}

void TileCell::SetPosition(float posX, float posY) {
	_posX = posX;
	_posY = posY;
	_sprite->SetPosition(posX, posY);
}
void TileCell::SetSprite(Sprite* sprite) {
	_sprite = sprite;
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
	_componentList.push_back(component);
	_renderList.remove(component);
}

void TileCell::MoveDeltaPosition(float deltaX, float deltaY) {
	_posX += deltaX;
	_posY += deltaY;
	_sprite->SetPosition(_posX, _posY);

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->MoveDeltaPosition(deltaX, deltaY);
	}
}