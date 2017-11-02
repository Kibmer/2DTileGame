#pragma once
#include <d3dx9.h>
#include <vector>
#include "Component.h"

class Sprite;
class TileCell;

class Map : public Component {
private:
	std::vector<std::vector<TileCell*>> _tileMap;
	int _width;
	int _height;

	float _deltaX;
	float _deltaY;
	float _startX;
	float _startY;

	int _tileSize;

	std::vector<Sprite*> _spriteList;

public:
	Map(LPCWSTR mapName);
	~Map();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Scroll(float deltaX, float deltaY);

	int GetPositionX(int tileX, int tileY);
	int GetPositionY(int tileX, int tileY);

	void SetTileComponent(int tileX, int tileY, Component* component, bool isRender);
	void ResetTileComponent(int tileX, int tileY, Component* component);

	bool CanMoveTileMap(int tileX, int tileY);
	void InitViewer(Component* viewer);
};
