#pragma once

#include <vector>
#include <memory>
#include <Math.h>
#include <stdio.h>
#include <random>
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes
#include "Game.h"

class Game;

struct Cell
{
	enum celltype
	{
		MONSTER,
		HEALER,
		TANK,
		EMPTY,
	};

	celltype mCELLTYPE = EMPTY;
	int mCellX;
	int mCellY;
	int mCellSize;
	
	Cell(int p_x, int p_y, int p_length)
	{
		mCellX = p_x;
		mCellY = p_y;
		mCellSize = p_length;
	}

	void Draw()
	{
		const int VISUAL_OFFSET = 5;
		double halfside = mCellSize / 2;

		glPushMatrix();
		glTranslatef(mCellX - VISUAL_OFFSET, mCellY - VISUAL_OFFSET, 0);
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

		// color selection
		glColor4f(0.3f, 0.2f, 0.1f, 1.0f); // Brown
			
		// draw quad
		glBegin(GL_QUADS);
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
	void DrawCell(int p_x, int p_y);	
	void CreateGrid(int p_w, int p_h, int p_cell_size, Game* p_xGame);
	void UpdateCellTypes(int x, int y, Cell::celltype type);
	std::vector<std::shared_ptr<Cell>> Get_Neighbour_Cells(std::shared_ptr<Cell> p_xCell, std::shared_ptr<Grid> p_xGrid);
	std::vector<std::vector<std::shared_ptr<Cell>>> Get_All_Cells();
	std::shared_ptr<Cell> Grid::GetCellAt(int p_x, int p_y);	
private:
	const int SIZE_PIXEL = 64;
	const int GRID_OFFSET = 10;
	const int CELL_SIZE = 20;
	const int HEIGHT = 720;
	const int WIDTH = 1280;
	std::vector<std::vector<std::shared_ptr<Cell>>> mCells;
	Game* mxGame;
	int Random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
};

