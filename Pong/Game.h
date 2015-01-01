#pragma once

class Game
{
public:
	static void start();

private:
	static bool isExiting();
	static void gameLoop();

	static void showSplashScreen();
	static void showMenu();

	enum GameState { Uninitialized, ShowingSplash, Paused, 
					ShowingMenu, Playing, Exiting };
	
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
};