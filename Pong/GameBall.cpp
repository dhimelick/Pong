#include "stdafx.h"
#include "GameBall.h"
#include "Game.h"
#include "ServiceLocator.h"

GameBall::GameBall() :
elapsedTimeSinceStart(00)
{
	load("images/ball_metal_gold.png");
	assert(isObjectLoaded());

	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);

	resetBall();
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

	// save bounding rect
	sf::Rect<float> ballRect = getBoundingRect();

	// check top and bottom
	if (getPosition().y - ballRect.height / 2 < 0
		|| getPosition().y + ballRect.height / 2 > Game::SCREEN_HEIGHT)
	{

		resetBall();
		return;
	}

	// check paddle collision
	PlayerPaddle* paddle1 = dynamic_cast<PlayerPaddle*>(Game::getGameObjectManager().get("Paddle1"));
	PlayerPaddle* paddle2 = dynamic_cast<PlayerPaddle*>(Game::getGameObjectManager().get("Paddle2"));
	if (paddle1 != NULL && paddle2 != NULL)
	{
		sf::Rect<float> paddleRect = paddle1->getBoundingRect();
		sf::Rect<float> paddle2Rect = paddle2->getBoundingRect();

		float ballX = getSprite().getPosition().x;
		float ballY = getSprite().getPosition().y;
		float ballR = ballRect.height / 2;

		// ball corners
		sf::Vector2f br(ballX + ballR, ballY + ballR);
		sf::Vector2f bl(ballX - ballR, ballY + ballR);
		sf::Vector2f tr(ballX + ballR, ballY - ballR);
		sf::Vector2f tl(ballX - ballR, ballY - ballR);

		// change frame of reference
		float refMoveByX = moveByX - paddle1->getVelocity() * elapsedTime;
		float refMoveByX2 = moveByX - paddle2->getVelocity() * elapsedTime;
		
		// slope
		float slope = 0;
		if (refMoveByX != 0)
		{
			slope = moveByY / refMoveByX;
		}
		float slope2 = 0;
		if (refMoveByX2 != 0)
		{
			slope2 = moveByY / refMoveByX2;
		}
		
		// ball y-intercepts
		float interceptBR = br.y - (slope * br.x);
		float interceptBL = bl.y - (slope * bl.x);
		float interceptTR = tr.y - (slope * tr.x);
		float interceptTL = tl.y - (slope * tl.x);

		float interceptBR2 = br.y - (slope2 * br.x);
		float interceptBL2 = bl.y - (slope2 * bl.x);
		float interceptTR2 = tr.y - (slope2 * tr.x);
		float interceptTL2 = tl.y - (slope2 * tl.x);

		// PADDLE 1 - top right corner of ball only needs left side of paddle
		float leftIntersectTR = slope * paddleRect.left + interceptTR;

		//PADDLE 2 - top right corner of ball checks bottom and left
		float leftIntersectTR2 = slope2 * paddle2Rect.left + interceptTR2;
		float bottomIntersectTR2 = br.x;
		if (slope2 != 0)
		{
			bottomIntersectTR2 = ((paddle2Rect.top + paddle2Rect.height)- interceptTR2) / slope2;
		}

		// PADDLE 1 - top left corner of ball only needs right side of paddle
		float rightIntersectTL = slope * (paddleRect.left + paddleRect.width) + interceptTL;

		// PADDLE 2 - top left corner of ball checks bottom and right
		float rightIntersectTL2 = slope2 * (paddle2Rect.left + paddle2Rect.width)+ interceptTL2;
		float bottomIntersectTL2 = br.x;
		if (slope2 != 0)
		{
			bottomIntersectTL2 = ((paddle2Rect.top + paddle2Rect.height) - interceptTL2) / slope2;
		}

		// PADDLE 1 - bottom right corner of ball needs top and left of paddle
		float topIntersectBR = br.x;
		if (slope != 0)
		{
			topIntersectBR = (paddleRect.top - interceptBR) / slope;						
		}
		float leftIntersectBR = slope * paddleRect.left + interceptBR;

		// PADDLE 2 - bottom right corner of ball hits left of paddle
		float leftIntersectBR2 = slope2 * (paddle2Rect.left) + interceptBR2;

		// PADDLE 1 - bottom left corner of ball needs top and right of paddle
		float topIntersectBL = bl.x;
		if (slope != 0)
		{
			topIntersectBL = (paddleRect.top - interceptBL) / slope;						
		}
		float rightIntersectBL = slope * (paddleRect.left + paddleRect.width) + interceptBL;

		//PADDLE 2 - bottom left corner of ball checks right side of paddle
		float rightIntersectBL2 = slope2 * (paddle2Rect.left + paddle2Rect.width) + interceptBL2;

		//if BR topintersect is within top of paddle OR if BL topintersect is within top of paddle -> top, also check distance to paddle. also check sign of yvel
		//else if BR OR TR leftintersect is within left of paddle -> left, check dist, and check sign of xvel
		//else if BL OR TL right intersect is within right of paddle -> right, check dist, and check sign of xvel

		//float BRcheck = (sqrt(pow(topIntersectBR - br.x, 2) + pow(paddleRect.top - br.y, 2)));
		//float BRmovecheck = (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)));
		//float BLcheck = (sqrt(pow(topIntersectBL - bl.x, 2) + pow(paddleRect.top - bl.y, 2)));
		//float BLmovecheck = (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)));

		//paddle2
		if (velocity.y < 0)
		{

			float TR2dist = (sqrt(pow(bottomIntersectTR2 - tr.x, 2) + pow((paddle2Rect.top + paddle2Rect.height) - tr.y, 2)));
			float move1dist = (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2)));
			float TL2dist = (sqrt(pow(bottomIntersectTL2 - tl.x, 2) + pow((paddle2Rect.top + paddle2Rect.height) - tl.y, 2)));
			float move2dist = (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2)));


			if (((bottomIntersectTR2 >= paddle2Rect.left && bottomIntersectTR2 <= paddle2Rect.left + paddle2Rect.width)
				|| (bottomIntersectTL2 >= paddle2Rect.left && bottomIntersectTL2 <= paddle2Rect.left + paddle2Rect.width))
				&& ((sqrt(pow(bottomIntersectTR2 - tr.x, 2) + pow((paddle2Rect.top + paddle2Rect.height) - tr.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2))))
 				|| (sqrt(pow(bottomIntersectTL2 - tl.x, 2) + pow((paddle2Rect.top + paddle2Rect.height) - tl.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2))))))
			{
				// bottom bounce
				newY = (paddle2Rect.top + paddle2Rect.height) + (moveByY - ((paddle2Rect.top + paddle2Rect.height) - ballRect.top)) + (ballRect.height / 2);
				velocity.y *= -1;
				velocity.x += paddle2->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}
			else if (((leftIntersectBR2 > paddle2Rect.top && leftIntersectBR2 <= paddle2Rect.top + paddle2Rect.height)
				|| (leftIntersectTR2 > paddle2Rect.top && leftIntersectTR2 <= paddle2Rect.top + paddle2Rect.height))
				&& ((sqrt(pow(paddle2Rect.left - br.x, 2) + pow(leftIntersectBR2 - br.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2))))
				|| (sqrt(pow(paddle2Rect.left - tr.x, 2) + pow(leftIntersectTR2 - tr.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2)))))
				&& (velocity.x - paddle2->getVelocity() > 0))
			{
				// left bounce
				newX = paddle2Rect.left - (refMoveByX2 - (paddle2Rect.left - (ballX + ballR))) - ballR;
				velocity.x = -abs(velocity.x) + paddle2->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}
			else if (((rightIntersectBL2 > paddle2Rect.top && rightIntersectBL2 <= paddle2Rect.top + paddle2Rect.height)
				|| (rightIntersectTL2 > paddle2Rect.top && rightIntersectTL2 <= paddle2Rect.top + paddle2Rect.height))
				&& ((sqrt(pow((paddle2Rect.left + paddle2Rect.width) - bl.x, 2) + pow(rightIntersectBL2 - bl.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2))))
				|| (sqrt(pow((paddle2Rect.left + paddle2Rect.width) - tl.x, 2) + pow(rightIntersectTL2 - tl.y, 2)) <= (sqrt(pow(refMoveByX2, 2) + pow(moveByY, 2)))))
				&& (velocity.x - paddle2->getVelocity() < 0))
			{
				// right bounce
				newX = (paddle2Rect.left + paddle2Rect.width) + (refMoveByX2 - ((ballX - ballR) - (paddle2Rect.left + paddle2Rect.width))) + ballR;
				velocity.x = abs(velocity.x) + paddle2->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}


		}



		//paddle1
		if (velocity.y > 0)
		{
			if (((topIntersectBR >= paddleRect.left && topIntersectBR <= paddleRect.left + paddleRect.width) 
				|| (topIntersectBL >= paddleRect.left && topIntersectBL <= paddleRect.left + paddleRect.width))
				&& ((sqrt(pow(topIntersectBR - br.x, 2) + pow(paddleRect.top - br.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2))))
				|| (sqrt(pow(topIntersectBL - bl.x, 2) + pow(paddleRect.top - bl.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2))))))
			{
				// top bounce
				newY = paddleRect.top - (moveByY - (paddleRect.top - (ballRect.top + ballRect.height))) - (ballRect.height / 2);
				velocity.y *= -1;
				velocity.x += paddle1->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}
			else if (((leftIntersectBR > paddleRect.top && leftIntersectBR <= paddleRect.top + paddleRect.height)
				|| (leftIntersectTR > paddleRect.top && leftIntersectTR <= paddleRect.top + paddleRect.height))
				&& ((sqrt(pow(paddleRect.left - br.x, 2) + pow(leftIntersectBR - br.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2))))
				|| (sqrt(pow(paddleRect.left - tr.x, 2) + pow(leftIntersectTR - tr.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)))))
				&& (velocity.x - paddle1->getVelocity() > 0))
			{
				// left bounce
				newX = paddleRect.left - (refMoveByX - (paddleRect.left - (ballX + ballR))) - ballR;
				velocity.x = -abs(velocity.x) + paddle1->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}
			else if (((rightIntersectBL > paddleRect.top && rightIntersectBL <= paddleRect.top + paddleRect.height)
				|| (rightIntersectTL > paddleRect.top && rightIntersectTL <= paddleRect.top + paddleRect.height))
				&& ((sqrt(pow((paddleRect.left +paddleRect.width) - bl.x, 2) + pow(rightIntersectBL - bl.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2))))
				|| (sqrt(pow((paddleRect.left + paddleRect.width) - tl.x, 2) + pow(rightIntersectTL - tl.y, 2)) <= (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)))))
				&& (velocity.x - paddle1->getVelocity() < 0))
			{
				// right bounce
				newX = (paddleRect.left + paddleRect.width) + (refMoveByX - ((ballX - ballR) - (paddleRect.left + paddleRect.width))) + ballR;
				velocity.x = abs(velocity.x) + paddle1->getVelocity();
				ServiceLocator::getAudio()->playSound("audio/blip.wav");
			}
		}
		
	}

	// check left wall
	if (getPosition().x - ballRect.width / 2 + moveByX < 0)
	{
		newX = -moveByX - (distToLeft - ballRect.width / 2) + (ballRect.width / 2);
		velocity.x *= -1;
		ServiceLocator::getAudio()->playSound("audio/blop.wav");
	} 
	// check right wall
	else if (getPosition().x + ballRect.width / 2 + moveByX > Game::SCREEN_WIDTH)
	{
		newX = Game::SCREEN_WIDTH - (moveByX - (distToRight - ballRect.width / 2)) - (ballRect.width / 2);
		velocity.x *= -1;
		ServiceLocator::getAudio()->playSound("audio/blop.wav");
	}

	prevX = getPosition().x;
	prevY = getPosition().y;
	getSprite().setPosition(newX, newY);
}

void GameBall::resetBall()
{
	getSprite().setPosition(Game::getCenter());
	elapsedTimeSinceStart = 0;
	float velX = Game::getRandomFloat(-400, 400);
	float velY = Game::getRandomFloat(200, 400);
	velocity = sf::Vector2f(velX, velY);
}
