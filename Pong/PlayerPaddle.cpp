#include "stdafx.h"
#include "PlayerPaddle.h"
#include "Game.h"


PlayerPaddle::PlayerPaddle() :
velocity(0),
maxVelocity(750.0f)
{
	load("images/paddle_metal_purple.png");
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		velocity = -maxVelocity;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
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