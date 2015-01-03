#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"

void Game::start()
{
	if(gameState != Uninitialized)
		return;

	mainWindow.create(sf::VideoMode(1024,768,32), "Pong");
	//mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);

	PlayerPaddle *player1 = new PlayerPaddle();
	player1->setPosition(1024 / 2 - 45, 700);

	gameObjectManager.add("Paddle1", player1);

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

Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
GameObjectManager Game::gameObjectManager;
sf::Clock Game::clock;
float Game::elapsed;