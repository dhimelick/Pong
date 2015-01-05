#pragma once
#include "VisibleGameObject.h"

class GameBall : public VisibleGameObject
{
public:
	GameBall();
	virtual ~GameBall();
	void update(float);
	void resetBall();

private:
	float elapsedTimeSinceStart;
	sf::Vector2f velocity;

	float prevX;
	float prevY;

};

