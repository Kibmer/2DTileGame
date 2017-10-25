#pragma once

#include <list>

class Sprite;
class Component;

class TileCell {
private:
	Sprite* _sprite;
	float _posX;
	float _posY;

	std::list<Component*> _componentList;

public:
	TileCell();
	~TileCell();

	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	float GetPositionX();
	float GetPositionY();

	void SetSprite(Sprite* sprite);

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
};