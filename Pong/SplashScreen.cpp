#include "stdafx.h"
#include "SplashScreen.h"

void SplashScreen::show(sf::RenderWindow& renderWindow)
{
	sf::Texture texture;
	if (texture.loadFromFile("images/SplashScreen.png") != true)
	{
		return;
	}
	sf::Sprite sprite(texture);

	renderWindow.draw(sprite);
	renderWindow.display();

	sf::Event event;
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed)
			{
				return;
			}
		}
	}
}