#pragma once

//#include "Tile.h"
#include <vector>
#include <memory>
#include <Math.h>
#include <stdio.h>
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes

enum Tile_Type
{
	DIRT,
	GRASS,
};

enum Animal_Tile_Type
{	
	WOLF,
	SHEEP,
	NONE,
};

struct Tile
{
	int x;
	int y;
	int length;
	Tile_Type type = DIRT;
	Animal_Tile_Type animal_type = NONE;
	Tile(int arg_x, int arg_y, int arg_length)
	{
		x = arg_x;
		y = arg_y;
		length = arg_length;
		type = Tile_Type::DIRT;
	}

	void draw()
	{
		double halfside = length / 2;
		glPushMatrix();
		glTranslatef(x, y+1, 0);
		glColor3f(0, 0, 0);
		//bot
		glBegin(GL_LINES);
		glVertex3f(-halfside, halfside, 0.0);
		glVertex3f(halfside, halfside, 0.0);
		glEnd();
		//top
		glBegin(GL_LINES);
		glVertex3f(-halfside, -halfside, 0.0);
		glVertex3f(halfside, -halfside, 0.0);
		glEnd();
		//left
		glBegin(GL_LINES);
		glVertex3f(-halfside, -halfside, 0.0);
		glVertex3f(-halfside, halfside, 0.0);
		glEnd();
		//right
		glBegin(GL_LINES);
		glVertex3f(halfside, -halfside, 0.0);
		glVertex3f(halfside, halfside, 0.0);
		glEnd();
		glColor4f(0.3f, 0.2f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		// Tiles
		glVertex2d(halfside, halfside);
		glVertex2d(halfside, - halfside);
		glVertex2d(- halfside, - halfside);
		glVertex2d(-halfside, halfside);
		glEnd();
		glPopMatrix();
	}
};

class Grid
{
public:
	Grid();
	~Grid();

	void Draw(int arg_windowWidth, int arg_windowHeight, int arg_lineHDistance, int arg_lineVDistance);
	void DrawTile(int arg_x, int arg_y);	
	void CreateGrid(int arg_width_, int arg_height_);
	void setType(int arg_i, int arg_j, Tile_Type arg_enum);
	void setAnimalType(int arg_i, int arg_j, Animal_Tile_Type arg_enum);

	std::shared_ptr<std::vector<std::shared_ptr<Tile>>> GetTilesPtr();
	std::shared_ptr<Tile> GetTile(int arg_i, int arg_j);
	Tile_Type getType(int arg_i, int arg_j);
	Animal_Tile_Type getAnimalType(int arg_i, int arg_j);
private:
	const int TILE_SIZE = 20;
	const int SIZE_PIXEL = 64;
	const int GRID_OFFSET = 10;
	std::vector<std::shared_ptr<Tile>> tiles_;
};

