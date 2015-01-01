#pragma once
#include "VisibleGameObject.h"

class PlayerPaddle :
	public VisibleGameObject
{
public:
	PlayerPaddle();
	~PlayerPaddle();

	void update(float elapsedTime);
	void draw(sf::RenderWindow& renderWindow);

	float getVelocity() const;

private:
	float velocity;
	float maxVelocity;
};

