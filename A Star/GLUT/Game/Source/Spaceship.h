#pragma once
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes

class Spaceship
{
public:
	Spaceship();
	~Spaceship();
	void DrawShip();
	void CreateShip(int arg_x, int arg_y, int arg_size);
	void SetPos(int arg_x, int arg_y);
	int GetX();
	int GetY();
private:
	int mSpaceshipX;
	int mSpaceshipY;
	int mSpaceshipSize;
};