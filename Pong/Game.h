#pragma once
#include "PlayerPaddle.h"
#include "GameObjectManager.h"

class Game
{
public:
	static void start();
	static float getElapsed();

	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;

private:
	static bool isExiting();
	static void gameLoop();

	static void showSplashScreen();
	static void showMenu();

	enum GameState { Uninitialized, ShowingSplash, Paused, 
					ShowingMenu, Playing, Exiting };
	
	static GameState gameState;
	static sf::RenderWindow mainWindow;
	static sf::Clock clock;
	static float elapsed;
	
	static GameObjectManager gameObjectManager;
};