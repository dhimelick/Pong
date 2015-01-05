#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"


void Game::start()
{
	if(gameState != Uninitialized)
		return;

	SFMLSoundProvider soundProvider;
	ServiceLocator::registerServiceLocator(&soundProvider);

	generator.seed(time(NULL));
	center = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	mainWindow.create(sf::VideoMode(1024,768,32), "Pong");
	//mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);

	// Create paddle 1
	PlayerPaddle* player1 = new PlayerPaddle(1);
	//player1->setPosition(SCREEN_WIDTH / 2, 700);
	player1->setPosition(300, 700);

	gameObjectManager.add("Paddle1", player1);

	// Create paddle 2
	PlayerPaddle* player2 = new PlayerPaddle(2);
	//player1->setPosition(SCREEN_WIDTH / 2, 700);
	player2->setPosition(350, 68);

	gameObjectManager.add("Paddle2", player2);

	// Create ball
	GameBall* ball = new GameBall();
	ball->setPosition(center);

	gameObjectManager.add("Ball", ball);


	gameState = Game::ShowingSplash;
	
	clock.restart();
	while(!isExiting())
	{
		elapsed = clock.restart().asSeconds();
		gameLoop();
	}

	mainWindow.close();
}

bool Game::isExiting()
{
	if(gameState == Game::Exiting) 
		return true;
	else 
		return false;
}

void Game::gameLoop()
{
	switch(gameState)
	{
		case Game::ShowingSplash:
		{
			showSplashScreen();
			break;
		}
		case Game::ShowingMenu:
		{
			showMenu();
			break;
		}
		case Game::Playing:
		{
			sf::Event currentEvent;
			while (mainWindow.pollEvent(currentEvent))
			{
				if (currentEvent.type == sf::Event::Closed)
				{
					gameState = Game::Exiting;
					break;
				}

				if (currentEvent.type == sf::Event::KeyPressed)
				{
					if (currentEvent.key.code == sf::Keyboard::Escape)
					{
						showMenu();
						break;
					}
					if (currentEvent.key.code == sf::Keyboard::Space)
					{
						dynamic_cast<GameBall*>(gameObjectManager.get("Ball"))->resetBall();
						break;
					}
				}
			}
			mainWindow.clear(sf::Color(0, 0, 0));
			gameObjectManager.updateAll(elapsed);
			gameObjectManager.drawAll(mainWindow);
			mainWindow.display();
			break;
		}
	}
}

void Game::showSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.show(mainWindow);
	gameState = Game::ShowingMenu;
}

void Game::showMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.show(mainWindow);
	switch (result)
	{
		case MainMenu::Exit:
			gameState = Game::Exiting;
			break;
		case MainMenu::Play:
			gameState = Game::Playing;
			break;
	}
}

float Game::getElapsed()
{
	return elapsed;
}

sf::Vector2f Game::getCenter()
{
	return center;
}

float Game::getRandomFloat(float lower, float upper)
{
	std::uniform_real_distribution<float> distribution(lower, upper);
	return distribution(generator);
}

GameObjectManager& Game::getGameObjectManager()
{
	return gameObjectManager;
}


Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
GameObjectManager Game::gameObjectManager;
sf::Clock Game::clock;
float Game::elapsed;
sf::Vector2f Game::center;
std::default_random_engine Game::generator;