#pragma once

#include <d3dx9.h>
#include "Texture.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>

class ResourceManager {
private:
	ResourceManager();

public:
	~ResourceManager();

private:
	static ResourceManager* _instance;

public:
	static ResourceManager* GetInstance();

private:
	std::map<LPCWSTR, Texture*> _textureMap;

public:
	Texture* LoadTexture(LPCWSTR TextureFilename);
	
	//Script
private:
	std::map<LPCWSTR, std::vector<std::string>> _scriptMap;

public:
	std::vector<std::string> LoadScript(LPCWSTR scriptFilename);
};