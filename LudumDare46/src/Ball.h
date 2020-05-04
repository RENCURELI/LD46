#pragma once

class Ball
{
public:
	Ball(float x, float y, float radius, int multX, int multY, float speed, int lives);
	
	float x;
	float y;
	float radius;
	int multX;
	int multY;
	float speed;
	int lives;
	float fXOffset, fYOffset;

	void BallMove(float dirX, float dirY);
};