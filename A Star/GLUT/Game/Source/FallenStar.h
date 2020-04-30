#pragma once
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes

class FallenStar
{
public:
	FallenStar();
	~FallenStar();
	void CreateFallenStar(int arg_x, int arg_y, int arg_size);
	void DrawFallenStar();
	int GetX();
	int GetY();
	void SetPos(int arg_x, int arg_y);
private:
	int mFallenStarX;
	int mFallenStarY;
	int mFallenStarSize;
};
