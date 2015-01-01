#include "stdafx.h"
#include "PlayerPaddle.h"
#include "Game.h"


PlayerPaddle::PlayerPaddle() :
velocity(0),
maxVelocity(600.0f)
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity -= 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity += 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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

	if (pos.x < getSprite().getLocalBounds().width / 2 || pos.x >(Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2))
	{
		velocity = -velocity;
	}

	getSprite().move(velocity * elapsedTime, 0);
}