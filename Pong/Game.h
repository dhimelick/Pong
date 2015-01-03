#pragma once
#include "PlayerPaddle.h"
#include "GameBall.h"
#include "GameObjectManager.h"

class Game
{
public:
	static void start();
	static float getElapsed();
	static sf::Vector2f getCenter();
	static GameObjectManager& getGameObjectManager();

	static float getRandomFloat(float lower, float upper);

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
	static sf::Vector2f center;
	
	static GameObjectManager gameObjectManager;
	static std::default_random_engine generator;
};