#pragma once
#include <d3dx9.h>
#include <string>

enum eComponentType {
	CT_PLAYER,
	CT_MONSTER,
	CT_NPC,
	CT_TILE_OBJECT,
	CT_NONE
};

class Component {
protected:
	LPCWSTR _name;
	bool _canMove;
	Component* _viewer;

	int _tileX;
	int _tileY;

	float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;

public:
	Component() {
		_canMove = false;
	}
	Component(LPCWSTR name);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void Reset() = 0;

	virtual void MoveDeltaPosition(float deltaX, float deltaY) {}
	virtual void SetPosition(float posX, float posY) {}

	void SetCanMove(bool canMove) { _canMove = canMove; }
	bool CanMove() { return _canMove; }

	int GetTileX() { return _tileX; }
	int GetTileY() { return _tileY; }

	float GetMoveDeltaX() { return _moveDistancePerTimeX; };
	float GetMoveDeltaY() { return _moveDistancePerTimeY; };

	// Message
public:
	virtual void ReceiveMessage(Component* sender, std::wstring message);
};