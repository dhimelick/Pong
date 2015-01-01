#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"

void Game::start(void)
{
	if(_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode(1024,768,32),"Pong");
	_gameState = Game::ShowingSplash;
	
	while(!isExiting())
	{
		gameLoop();
	}

	_mainWindow.close();
}

bool Game::isExiting()
{
	if(_gameState == Game::Exiting) 
		return true;
	else 
		return false;
}

void Game::gameLoop()
{
	sf::Event currentEvent;
	while(_mainWindow.pollEvent(currentEvent))
	{
	
		switch(_gameState)
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
				while (_mainWindow.pollEvent(currentEvent))
				{
					_mainWindow.clear(sf::Color(0, 0, 0));
					_mainWindow.display();

					if (currentEvent.type == sf::Event::Closed)
					{
						_gameState = Game::Exiting;
					}
					if (currentEvent.type == sf::Event::KeyPressed)
					{
						if (currentEvent.key.code == sf::Keyboard::Escape)
						{
							showMenu();
						}
					}
				}
				break;
			}
		}
	}
}

void Game::showSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.show(_mainWindow);
	_gameState = Game::ShowingMenu;
}

void Game::showMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.show(_mainWindow);
	switch (result)
	{
		case MainMenu::Exit:
			_gameState = Game::Exiting;
			break;
		case MainMenu::Play:
			_gameState = Game::Playing;
			break;
	}
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;