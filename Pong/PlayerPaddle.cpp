#include "stdafx.h"
#include "PlayerPaddle.h"
#include "Game.h"


PlayerPaddle::PlayerPaddle(int playerNum) :
velocity(0),
maxVelocity(750.0f)
{
	if (playerNum == 1)
	{
		load("images/paddle_metal_purple.png");
		rightKey = sf::Keyboard::Key::Right;
		leftKey = sf::Keyboard::Key::Left;
	}
	else if (playerNum == 2)
	{
		load("images/paddle_metal_blue.png");
		rightKey = sf::Keyboard::Key::D;
		leftKey = sf::Keyboard::Key::A;
	}
	
	assert(isObjectLoaded());



	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}


PlayerPaddle::~PlayerPaddle()
{
}

void PlayerPaddle::draw(sf::RenderWindow& renderWindow)
{
	VisibleGameObject::draw(renderWindow);
}

float PlayerPaddle::getVelocity() const
{
	return velocity;
}

void PlayerPaddle::update(float elapsedTime)
{
	if (sf::Keyboard::isKeyPressed(leftKey))
	{
		velocity = -maxVelocity;
	}
	else if (sf::Keyboard::isKeyPressed(rightKey))
	{
		velocity = maxVelocity;
	}
	else
	{
		velocity = 0;
	}

	if (velocity > maxVelocity)
	{
		velocity = maxVelocity;
	}
	else if (velocity < -maxVelocity)
	{
		velocity = -maxVelocity;
	}

	sf::Vector2f pos = this->getPosition();

	float moveByX = velocity * elapsedTime;

	if (pos.x + moveByX < 0)
	{
		getSprite().setPosition(getSprite().getLocalBounds().width / 2, pos.y);
	}
	else if (pos.x + moveByX > Game::SCREEN_WIDTH)
	{
		getSprite().setPosition((Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2), pos.y);
	}
	else
	{
		getSprite().move(moveByX, 0);
	}
}