#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"

void Game::start(void)
{
	if(gameState != Uninitialized)
		return;

	mainWindow.create(sf::VideoMode(1024,768,32), "Pong");
	mainWindow.setFramerateLimit(60);

	player1.load("images/paddle.png");
	player1.setPosition(1024 / 2 - 45, 700);

	gameState = Game::ShowingSplash;
	
	while(!isExiting())
	{
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
				mainWindow.clear(sf::Color(0, 0, 0));
				player1.draw(mainWindow);
				mainWindow.display();

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

Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
PlayerPaddle Game::player1;