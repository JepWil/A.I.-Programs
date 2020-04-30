#include "FallenStar.h"

FallenStar::FallenStar()
{

}

FallenStar::~FallenStar()
{
}

void FallenStar::CreateFallenStar(int arg_x, int arg_y, int arg_size)
{
	mFallenStarX = arg_x;
	mFallenStarY = arg_y;
	mFallenStarSize = arg_size;
}

void FallenStar::DrawFallenStar()
{
	double halfside = mFallenStarSize / 2;
	glPushMatrix();
	glTranslatef(mFallenStarX, mFallenStarY + 1, 0);
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
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f); // Purple/pink
	// draw quad
	glBegin(GL_QUADS);
	glVertex2d(halfside, halfside);
	glVertex2d(halfside, -halfside);
	glVertex2d(-halfside, -halfside);
	glVertex2d(-halfside, halfside);
	glEnd();
	glPopMatrix();
}

int FallenStar::GetX()
{
	return mFallenStarX;
}

int FallenStar::GetY()
{
	return mFallenStarY;
}

void FallenStar::SetPos(int arg_x, int arg_y)
{
	mFallenStarX = arg_x;
	mFallenStarY = arg_y;
}
