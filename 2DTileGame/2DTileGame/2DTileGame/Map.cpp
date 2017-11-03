#include <fstream>

#include "GameSystem.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "Map.h"

Map::Map(LPCWSTR mapName) : Component(mapName)
{
	_deltaX = _deltaY = _startX = _startY = 0.0f;
	_spriteList.clear();
}
Map::~Map()
{

}

void Map::Init()
{
	//맵용 스프라이트 리스트 작업
	int srcX = 0;
	int srcY = 0;
	_tileSize = 32;
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			Sprite* sprite = new Sprite(L"terrain.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, _tileSize, _tileSize, 1.0f);
			_spriteList.push_back(sprite);
			srcX += _tileSize;
		}
		srcX = 0;
		srcY += _tileSize;
	}

	_width = 32;
	_height = 32;

	//Load Map Script 1층
	{
		int line = 0;
		char record[1024];
		std::ifstream infile("MapData_layer1.csv");
		while (!infile.eof()) {
			infile.getline(record, 1024);

			char* token = strtok(record, ",");
			switch (line) {
			case 0:
				if (NULL != token) {
					token = strtok(NULL, ",");
					_width = atoi(token);
					token = strtok(NULL, ",");
					_height = atoi(token);
				}
				break;
			case 1:
				break;
			default:
				//map data
				if (NULL != token) {
					//std::vector<Sprite*> rowList;
					std::vector<TileCell*> rowList;
					for (int x = 0; x < _width; x++)
					{
						int index = atoi(token);
						TileCell* tileCell = new TileCell();
						WCHAR componentName[256];
						wprintf(componentName, L"map_layer01_%d_%d", line, x);
						TileObject* tileObject = new TileObject(componentName, _spriteList[index]);
						tileObject->SetCanMove(true);
						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
				}
			}
			line++;
		}
	}

	// 2층 로드

	{
		int line = 0;
		int row = 0;

		char record[1024];
		std::ifstream infile("MapData_layer2.csv");
		while (!infile.eof()) {
			infile.getline(record, 1024);

			char* token = strtok(record, ",");
			switch (line) {
			case 0:
				break;
			case 1:
				break;
			default:
				//map data
				if (NULL != token) {
					//std::vector<Sprite*> rowList;
					std::vector<TileCell*> rowList = _tileMap[row];
					for (int x = 0; x < _width; x++)
					{
						int index = atoi(token);
						if (0 <= index) {

							TileCell* tileCell = rowList[x];
							WCHAR componentName[256];
							wsprintf(componentName, L"map_layer02_%d_%d", line, x);
							TileObject* tileObject = new TileObject(componentName, _spriteList[index]);
							tileObject->SetCanMove(false);
							tileCell->AddComponent(tileObject, true);
						}
						token = strtok(NULL, ",");
					}
					row++;
				}
			}
			line++;
		}
	}

	//_startX += _deltaX;
	//_startY += _deltaY;
	//float posX = _startX;
	//float posY = _startY;

	//for (int y = 0; y < _height; y++)
	//{
	//	for (int x = 0; x < _width; x++)
	//	{
	//		_tileMap[y][x]->SetPosition(posX, posY);
	//		posX += _tileSize; //tileSize = 32
	//	}
	//	posX = _startX;
	//	posY += _tileSize;
	//}
	/*
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			Sprite* sprite;
			int randValue = rand() % 4;
			switch (randValue)
			{
			case 0:
				sprite = new Sprite(L"character_sprite.png", L"jsontest_01.json");
				break;
			case 1:
				sprite = new Sprite(L"character_sprite.png", L"jsontest_02.json");
				break;
			case 2:
				sprite = new Sprite(L"character_sprite.png", L"jsontest_03.json");
				break;
			case 3:
				sprite = new Sprite(L"character_sprite.png", L"jsontest_04.json");
				break;
			}
			sprite->Init();
			_testTileMap[y][x] = sprite;
		}
	}
	*/
}
void Map::Deinit()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Deinit();
			delete _tileMap[y][x];
		}
	}
}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->MoveDeltaPosition(_deltaX, _deltaY);
			_tileMap[y][x]->Update(deltaTime);
		}
	}
}
void Map::Render()
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 1;
	int maxX = _viewer->GetTileX() - (midX / _tileSize) + 1;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 1;
	int maxY = _viewer->GetTileY() - (midX / _tileSize) + 1;

	// 범위가 벗어날 경우 보정
	if (minX < 0)
		minX = 0;
	if (_width <= maxX)
		maxX = _width;
	if (minY < 0)
		minY = 0;
	if (_height <= maxY)
		maxY = _height;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Render();
		}
	}
}
void Map::Release()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Release();
		}
	}
}
void Map::Reset()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Reset();
		}
	}
}

void Map::Scroll(float deltaX, float deltaY)
{
	_deltaX = deltaX;
	_deltaY = deltaY;
}

int Map::GetPositionX(int tileX, int tileY) {
	return _tileMap[tileY][tileX]->GetPositionX();
}

int Map::GetPositionY(int tileX, int tileY) {
	return _tileMap[tileY][tileX]->GetPositionY();
}

void Map::SetTileComponent(int tileX, int tileY, Component* component, bool isRender) {
	_tileMap[tileX][tileY]->AddComponent(component, false);
}

void Map::ResetTileComponent(int tileX, int tileY, Component* component) {
	_tileMap[tileX][tileY]->RemoveComponent(component);
}

bool Map::CanMoveTileMap(int tileX, int tileY) {
	if (tileX < 0)
		return false;
	if (_width <= tileX)
		return false;
	if (tileY < 0)
		return false;
	if (_width <= tileY)
		return false;

	return _tileMap[tileY][tileX]->CanMove();
}


void Map::InitViewer(Component* viewer) {
	_viewer = viewer;
	// 뷰어를 중심으로 렌더링할 영역을 구한다.
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;
	/*
	int minX = _viewer->GetTileX() - (midX / _tileSize) - 1;
	int maxX = _viewer->GetTileX() - (midX / _tileSize) + 1;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 1;
	int maxY = _viewer->GetTileY() - (midX / _tileSize) + 1;

	// 범위가 벗어날 경우 보정
	if (minX < 0)
		minX = 0;
	if (_width <= maxX)
		maxX = _width - 1;
	if (minY < 0)
		minY = 0;
	if (_height <= maxY)
		maxY = _height - 1;
	*/
	// 뷰어의 위치를 기준으로 시작 픽셀 위치를 계산
	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	// 해당 위치에 맞게 타일을 그려줌
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}