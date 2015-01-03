#include "stdafx.h"
#include "PlayerPaddle.h"
#include "Game.h"


PlayerPaddle::PlayerPaddle() :
velocity(0),
maxVelocity(75.0f)
{
	load("images/paddle.png");
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

	if (pos.x + velocity < 0)
	{
		getSprite().setPosition(getSprite().getLocalBounds().width / 2, pos.y);
	}
	else if (pos.x + velocity > Game::SCREEN_WIDTH)
	{
		getSprite().setPosition((Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2), pos.y);
	}
	else
	{
		getSprite().move(velocity * elapsedTime * 10, 0);
	}
}