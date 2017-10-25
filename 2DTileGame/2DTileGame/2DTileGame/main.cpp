#include "GameSystem.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	GameSystem::GetInstance()->InitSystem(hInstance, nCmdShow);
	return GameSystem::GetInstance()->Update();
}
