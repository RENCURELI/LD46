#include "Player.h"

Player::Player(float posX, float posY, float startWidth, float startHeight, float colR, float colG, float colB)
{
	x = posX;
	y = posY;
	width = startWidth;
	height = startHeight;
	r = colR;
	g = colG;
	b = colB;
	
	fXOffset = 0.0f;
	fYOffset = 0.0f;
}


void Player::PlayerMove(float dir)
{
	fYOffset = dir;
}
