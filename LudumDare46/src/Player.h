#pragma once

class Player
{
public:
	Player(float x, float y, float width, float height, float r, float g, float b);
	//~Player();

	float x;
	float y;
	float width;
	float height;
	float r;
	float g;
	float b;
	float fXOffset, fYOffset;

	void PlayerMove(float dir);
	
};