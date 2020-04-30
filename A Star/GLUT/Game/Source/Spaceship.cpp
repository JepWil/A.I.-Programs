#include "Spaceship.h"

Spaceship::Spaceship()
{

}

Spaceship::~Spaceship()
{
}

void Spaceship::CreateShip(int arg_x, int arg_y, int arg_size)
{
	mSpaceshipX = arg_x;
	mSpaceshipY = arg_y;
	mSpaceshipSize = arg_size;
}

int Spaceship::GetY()
{
	return mSpaceshipY;
}

void Spaceship::SetPos(int arg_x, int arg_y)
{
	mSpaceshipX = arg_x;
	mSpaceshipY = arg_y;
}

int Spaceship::GetX()
{
	return mSpaceshipX;
}

void Spaceship::DrawShip()
{
	double halfside = mSpaceshipSize / 2;
	glPushMatrix();
	glTranslatef(mSpaceshipX, mSpaceshipY, 0);
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
	glColor4f(0.2f, 0.9f, 0.5f, 1.0f); // Green
	// draw quad
	glBegin(GL_QUADS);
	glVertex2d(halfside, halfside);
	glVertex2d(halfside, -halfside);
	glVertex2d(-halfside, -halfside);
	glVertex2d(-halfside, halfside);
	glEnd();
	glPopMatrix();
}
