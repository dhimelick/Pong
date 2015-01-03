#include "stdafx.h"
#include "GameBall.h"
#include "Game.h"


GameBall::GameBall() :
elapsedTimeSinceStart(00)
{
	load("images/ball_metal_gold.png");
	assert(isObjectLoaded());

	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);

	resetVelocity();
}


GameBall::~GameBall()
{
}

void GameBall::update(float elapsedTime)
{
	elapsedTimeSinceStart += elapsedTime;

	// Delay game from starting until 3 seconds have passed
	if (elapsedTimeSinceStart < 1.5)
		return;

	float distToTop = getPosition().y;
	float distToBottom = Game::SCREEN_HEIGHT - getPosition().y;
	float distToLeft = getPosition().x;
	float distToRight = Game::SCREEN_WIDTH - getPosition().x;

	float moveByX = velocity.x * elapsedTime;
	float moveByY = velocity.y * elapsedTime;
	float newX = getPosition().x + moveByX;
	float newY = getPosition().y + moveByY;

	// check top and bottom
	if (getPosition().y - getBoundingRect().height / 2 < 0
		|| getPosition().y + getBoundingRect().height / 2 > Game::SCREEN_HEIGHT)
	{
		getSprite().setPosition(Game::getCenter());
		elapsedTimeSinceStart = 0;
		resetVelocity();
		return;
	}

	// check left
	if (getPosition().x - getBoundingRect().width / 2 + moveByX < 0)
	{
		newX = abs(moveByX) - (distToLeft - getBoundingRect().width / 2) + (getBoundingRect().width / 2);
		velocity.x *= -1;
	} 
	// check right
	else if (getPosition().x + getBoundingRect().width / 2 + moveByX > Game::SCREEN_WIDTH)
	{
		newX = Game::SCREEN_WIDTH - (moveByX - (distToRight - getBoundingRect().width / 2)) - (getBoundingRect().width / 2);
		velocity.x *= -1;
	}

	getSprite().setPosition(newX, newY);
}

void GameBall::resetVelocity()
{
	float velX = Game::getRandomFloat(-400, 400);
	float velY = Game::getRandomFloat(200, 400);
	velocity = sf::Vector2f(velX, velY);
}
