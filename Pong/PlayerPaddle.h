#pragma once
#include "VisibleGameObject.h"

class PlayerPaddle : public VisibleGameObject
{
public:
	PlayerPaddle(int playerNum);
	~PlayerPaddle();

	void update(float elapsedTime);
	void draw(sf::RenderWindow& renderWindow);

	float getVelocity() const;

private:
	float velocity;
	float maxVelocity;
	int playerNum;
	sf::Keyboard::Key leftKey;
	sf::Keyboard::Key rightKey;
};

