#include "stdafx.h"
#include "GameBall.h"
#include "Game.h"

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
	if (paddle1 != NULL)
	{
		sf::Rect<float> paddleRect = paddle1->getBoundingRect();

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
		

		// slope
		float slope = 0;
		if (refMoveByX != 0)
		{
			slope = moveByY / refMoveByX;
		}
		
		// ball equation from middle
		// ball-paddle intersections
		float intercept = ballY - (slope * ballX);
		float topIntersect = ballX;
		if (slope != 0)
		{
			topIntersect = (paddleRect.top - intercept) / slope;						// x coord of ball path at y = paddle top edge
		}
		//float leftIntersect = slope * paddleRect.left + intercept;							// y coord of ball path at x = paddle left edge
		//float rightIntersect = slope * (paddleRect.left + paddleRect.width) + intercept;	// y coord of ball path at x = paddle right edge


		// ball y-intercepts
		float interceptBR = br.y - (slope * br.x);
		float interceptBL = bl.y - (slope * bl.x);
		float interceptTR = tr.y - (slope * tr.x);
		float interceptTL = tl.y - (slope * tl.x);

		// top right corner of ball only needs left side of paddle
		float leftIntersectTR = slope * paddleRect.left + interceptTR;

		// top left corner of ball only needs right side of paddle
		float rightIntersectTL = slope * (paddleRect.left + paddleRect.width) + interceptTL;

		// bottom right corner of ball needs top and left of paddle
		float topIntersectBR = br.x;
		if (slope != 0)
		{
			topIntersectBR = (paddleRect.top - interceptBR) / slope;						
		}
		float leftIntersectBR = slope * paddleRect.left + interceptBR;

		// bottom left corner of ball needs top and right of paddle
		float topIntersectBL = bl.x;
		if (slope != 0)
		{
			topIntersectBL = (paddleRect.top - interceptBL) / slope;						
		}
		float rightIntersectBL = slope * (paddleRect.left + paddleRect.width) + interceptBL;

		//if BR topintersect is within top of paddle OR if BL topintersect is within top of paddle -> top, also check distance to paddle. also check sign of yvel
		//else if BR OR TR leftintersect is within left of paddle -> left, check dist, and check sign of xvel
		//else if BL OR TL right intersect is within right of paddle -> right, check dist, and check sign of xvel

		float BRcheck = (sqrt(pow(topIntersectBR - br.x, 2) + pow(paddleRect.top - br.y, 2)));
		float BRmovecheck = (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)));
		float BLcheck = (sqrt(pow(topIntersectBL - bl.x, 2) + pow(paddleRect.top - bl.y, 2)));
		float BLmovecheck = (sqrt(pow(refMoveByX, 2) + pow(moveByY, 2)));

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
			}
		}
		


		//else if ((leftIntersectBR > paddleRect.top && leftIntersectBR <= paddleRect.top + paddleRect.height)
		//	|| (leftIntersectTR > paddleRect.top && leftIntersectTR <= paddleRect.top + paddleRect.height))
		//{
		//	// left bounce
		//	newX = paddleRect.left - (refMoveByX - (paddleRect.left - (ballX + ballR))) - ballR;
		//	velocity.x = -abs(velocity.x) + paddle1->getVelocity();
		//}
		//else if ((rightIntersectBL > paddleRect.top && rightIntersectBL <= paddleRect.top + paddleRect.height)
		//	|| (rightIntersectTL > paddleRect.top && rightIntersectTL <= paddleRect.top + paddleRect.height))
		//{
		//	// right bounce
		//	newX = (paddleRect.left + paddleRect.width) + (refMoveByX - ((ballX - ballR) - (paddleRect.left + paddleRect.width))) + ballR;
		//	velocity.x = abs(velocity.x) + paddle1->getVelocity();
		//}


		//sf::Rect<float> thing()

		//if (ballY + moveByY + ballR >= paddleRect.top && ballY + moveByY - ballR <= paddleRect.top + paddleRect.height)
		//{
		//	if (ballY + ballR < paddleRect.top
		//		&& topIntersect >= paddleRect.left
		//		&& topIntersect <= paddleRect.left + paddleRect.width)
		//	{
		//		// top bounce
		//		newY = paddleRect.top - (moveByY - (paddleRect.top - (ballRect.top + ballRect.height))) - (ballRect.height / 2);
		//		velocity.y *= -1;
		//		velocity.x += paddle1->getVelocity();
		//	}
		//}
		//	else if (ballX + ballR + refMoveByX >= paddleRect.left					// right of ball will pass left of paddle
		//		&& ballX + ballR + refMoveByX < paddleRect.left + paddleRect.width	// right of ball will not pass right of paddle
		//		&& leftIntersect >= paddleRect.top
		//		&& leftIntersect <= paddleRect.top + paddleRect.height)				// left intersect contained on paddle left edge
		//	{
		//		// left bounce
		//		newX = paddleRect.left - (refMoveByX - (paddleRect.left - (ballX + ballR))) - ballR;
		//		velocity.x = -abs(velocity.x) + paddle1->getVelocity();
		//	}
		//	else if (ballX + refMoveByX - ballR <= paddleRect.left + paddleRect.width
		//		&& ballX + refMoveByX + ballR > paddleRect.left
		//		&& rightIntersect >= paddleRect.top
		//		&& rightIntersect <= paddleRect.top + paddleRect.height)
		//	{
		//		// right bounce
		//		newX = (paddleRect.left + paddleRect.width) + (refMoveByX - ((ballX - ballR) - (paddleRect.left + paddleRect.width))) + ballR;

		//		velocity.x = abs(velocity.x) + paddle1->getVelocity();
		//	}
		//}
	}

	// check left wall
	if (getPosition().x - ballRect.width / 2 + moveByX < 0)
	{
		newX = -moveByX - (distToLeft - ballRect.width / 2) + (ballRect.width / 2);
		velocity.x *= -1;
	} 
	// check right wall
	else if (getPosition().x + ballRect.width / 2 + moveByX > Game::SCREEN_WIDTH)
	{
		newX = Game::SCREEN_WIDTH - (moveByX - (distToRight - ballRect.width / 2)) - (ballRect.width / 2);
		velocity.x *= -1;
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
	velocity = sf::Vector2f(-50, 100);
}
