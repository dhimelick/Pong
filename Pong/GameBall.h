#pragma once
#include "VisibleGameObject.h"

class GameBall : public VisibleGameObject
{
public:
	GameBall();
	virtual ~GameBall();
	void update(float);

private:
	float elapsedTimeSinceStart;
	sf::Vector2f velocity;

	void resetVelocity();
};

