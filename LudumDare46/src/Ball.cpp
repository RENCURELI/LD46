#include "Ball.h"

Ball::Ball(float posX, float posY, float startRadius, int baseMultX, int baseMultY, float baseSpeed, int baseLives)
{
	x = posX;
	y = posY;
	radius = startRadius;
	multX = baseMultX;
	multY = baseMultY;
	speed = baseSpeed;
	lives = baseLives;
}

void Ball::BallMove(float dirX, float dirY)
{
	fXOffset = dirX;
	fYOffset = dirY;
}
