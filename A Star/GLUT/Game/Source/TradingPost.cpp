#include "TradingPost.h"

TradingPost::TradingPost()
{
}

TradingPost::~TradingPost()
{
}

void TradingPost::CreateTradingPost(int arg_x, int arg_y, int arg_size)
{
	mTradingPostX = arg_x;
	mTradingPostY = arg_y;
	mTradingPostSize = arg_size;
}

void TradingPost::DrawTradingPost()
{
	double halfside = mTradingPostSize / 2;
	glPushMatrix();
	glTranslatef(mTradingPostX, mTradingPostY, 0);
	// bot
	glBegin(GL_LINES);
	glVertex3f(-halfside, halfside, 0.0);
	glVertex3f(halfside, halfside, 0.0);
	glEnd();
	// top
	glBegin(GL_LINES);
	glVertex3f(-halfside, -halfside, 0.0);
	glVertex3f(halfside, -halfside, 0.0);
	glEnd();
	// left
	glBegin(GL_LINES);
	glVertex3f(-halfside, -halfside, 0.0);
	glVertex3f(-halfside, halfside, 0.0);
	glEnd();
	// right
	glBegin(GL_LINES);
	glVertex3f(halfside, -halfside, 0.0);
	glVertex3f(halfside, halfside, 0.0);
	glEnd();
	// color
	glColor4f(0.05f, 0.4f, 0.75f, 1.0f); // Blue
	// draw quad
	glBegin(GL_QUADS);
	glVertex2d(halfside, halfside);
	glVertex2d(halfside, -halfside);
	glVertex2d(-halfside, -halfside);
	glVertex2d(-halfside, halfside);
	glEnd();
	glPopMatrix();
}

int TradingPost::GetX()
{
	return mTradingPostX;
}

int TradingPost::GetY()
{
	return mTradingPostY;
}

void TradingPost::SetPos(int arg_x, int arg_y)
{
	mTradingPostX = arg_x;
	mTradingPostY = arg_y;
}

