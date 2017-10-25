#include "ResourceManager.h"

using namespace std;
ResourceManager* ResourceManager::_instance = 0;

ResourceManager::ResourceManager() {
	
}

ResourceManager::~ResourceManager() {

}

ResourceManager* ResourceManager::GetInstance() {
	if (_instance == NULL) 
		_instance = new ResourceManager();
	return _instance;
}

Texture* ResourceManager::LoadTexture(LPCWSTR textureFilename) {
	map<LPCWSTR, Texture*>::iterator it = _textureMap.find(textureFilename);
	if (it != _textureMap.end()) {
		return it->second;
	}

	Texture* texture = new Texture();
	texture->Init(textureFilename);

	_textureMap[textureFilename] = texture;

	return texture;
}

vector<string> ResourceManager::LoadScript(LPCWSTR scriptFilename) {
	map<LPCWSTR, vector<string>>::iterator it = _scriptMap.find(scriptFilename);
	if (it != _scriptMap.end()) {
		return it->second;
	}

	vector<string> scriptTextList;

	char record[1000];
	ifstream infile(scriptFilename);
	while (!infile.eof()) {
		infile.getline(record, 100);
		scriptTextList.push_back(record);
	}
	_scriptMap[scriptFilename] = scriptTextList;

	return scriptTextList;
}