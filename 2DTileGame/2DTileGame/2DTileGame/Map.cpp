#include <fstream>

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
						//rowList.push_back(_spriteList[index]);
						WCHAR componentName[256];
						wprintf(componentName, L"map_layer01_%d_%d", line, x);
						TileObject* tileObject = new TileObject(componentName, _spriteList[index]);
						tileCell->AddComponent(tileObject, true);
						//tileCell->SetSprite(_spriteList[index]);
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

	_startX += _deltaX;
	_startY += _deltaY;
	float posX = _startX;
	float posY = _startY;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize; //tileSize = 32
		}
		posX = _startX;
		posY += _tileSize;
	}
	//int index = 0;
	//for (int y = 0; y < _height; y++) //MAP_HEIGHT가 유동적으로 바뀔 수 있도록 리팩토링 -> 제이슨에서 map 사이즈를 가지고와서 적용
	//{
	//	std::vector<Sprite*> rowList;
	//	for (int x = 0; x < _width; x++)
	//	{
	//		//_tileMap[y][x] = _spriteList[index]; 
	//		rowList.push_back(_spriteList[index]);
	//		index++;
	//	}
	//	_tileMap.push_back(rowList);
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
	//_startX += _deltaX;
	//_startY += _deltaY;
	//float posX = _startX;
	//float posY = _startY;

	//for (int y = 0; y < _height; y++)
	//{
	//	for (int x = 0; x < _width; x++)
	//	{
	//		_tileMap[y][x]->SetPosition(posX, posY);
	//		_tileMap[y][x]->Render();
	//		posX += _tileSize; //tileSize = 32
	//	}
	//	posX = _startX;
	//	posY += _tileSize;
	//}
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