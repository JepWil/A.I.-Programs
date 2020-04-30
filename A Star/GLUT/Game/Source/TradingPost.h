#pragma once
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes

class TradingPost
{
public:
	TradingPost();
	~TradingPost();
	void CreateTradingPost(int arg_x, int arg_y, int arg_size);
	void DrawTradingPost();
	void SetPos(int arg_x, int arg_y);
	int GetX();
	int GetY();
private:
	int mTradingPostX;
	int mTradingPostY;
	int mTradingPostSize;
};