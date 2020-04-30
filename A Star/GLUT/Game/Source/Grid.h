#pragma once

#include <vector>
#include <memory>
#include <Math.h>
#include <stdio.h>
#include <random>
#include <stdarg.h> 
#include "glut.h" // Always have this the furthest down of the includes

enum celltype
{
	DIRT,
	SPACEGRASS,
	CRATER,
	SPACESHIP,
	TRADINGPOST,
	FALLENSTAR,
	STARCHASER,
};

enum cellaccess
{
	BLOCKED,
	FREE,
};

struct Cell
{
	int mCellX;
	int mCellY;
	int mCellLength;
	celltype mCellType;
	cellaccess mCellAccess;
	
	Cell(int arg_x, int arg_y, int arg_length, int arg_cell_type, int arg_cell_access)
	{
		mCellX = arg_x;
		mCellY = arg_y;
		mCellLength = arg_length;

		if (arg_cell_type == 1)
		{
			mCellType = DIRT;
		}
		else if (arg_cell_type == 2)
		{
			mCellType = SPACEGRASS;
		}
		else
			mCellType = CRATER;


		if (arg_cell_access == 1)
		{
			mCellAccess = BLOCKED;
		}
		else
			mCellAccess = FREE;
	}

	void SetColor(std::string arg_color)
	{
		if (arg_color == "yellow")
		{
			glColor4f(0.75, 0.9f, 0.0f, 1.0f);
		}
		else
		{
			glColor4f(0.3f, 0.2f, 0.1f, 1.0f);
		}
	}

	void Draw()
	{
		double halfside = mCellLength / 2;
		glPushMatrix();
		glTranslatef(mCellX, mCellY+1, 0);
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
		glColor4f(0.3f, 0.2f, 0.1f, 1.0f);

		if (mCellType == STARCHASER)
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			//glColor4f(0.8f, 0.5f, 0.2f, 1.0f); // Orange
		}
		else if (mCellType == FALLENSTAR)
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			//glColor4f(1.0f, 0.0f, 1.0f, 1.0f); // Purple/pink
		}
		else if (mCellType == TRADINGPOST)
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			//glColor4f(0.05f, 0.4f, 0.75f, 1.0f); // Blue
		}
		else if (mCellType == SPACESHIP)
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			//glColor4f(0.2f, 0.9f, 0.5f, 1.0f); // Green
		}

		if (mCellType == DIRT)
		{
			glColor4f(0.3f, 0.2f, 0.1f, 1.0f);
		}
		else if (mCellType == SPACEGRASS)
		{
			glColor4f(0.2f, 0.7f, 0.5f, 1.0f);
		}
		else
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		
		if (mCellAccess != FREE && (mCellType != STARCHASER && mCellType != TRADINGPOST && mCellType != SPACESHIP && mCellType != FALLENSTAR))
		{
			glColor4f(0.8f, 0.0f, 0.0f, 1.0f);
		}
			
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
	void Draw(int arg_windowWidth, int arg_windowHeight, int arg_lineHDistance, int arg_lineVDistance);
	void DrawCell(int arg_x, int arg_y);	
	void CreateGrid(int arg_width_, int arg_height_, int arg_cell_size);
	int GetDistance(std::shared_ptr<Cell> arg_lhs, std::shared_ptr<Cell> arg_rhs);
	std::vector<std::vector<std::shared_ptr<Cell>>> GetFreeCells();
	std::vector<std::shared_ptr<Cell>> GetNeighbourCells(std::shared_ptr<Cell> arg_cell, std::shared_ptr<Grid> arg_grid);
	std::vector<std::vector<std::shared_ptr<Cell>>> GetAllCells();
	std::shared_ptr<Cell> Grid::GetCellAt(int arg_x, int arg_y);	
	std::shared_ptr<Cell>GetFreeCellAt(int arg_x, int arg_y);
	void pressGridBlocked(int arg_mouseX, int arg_mouseY);
	void pressGridEntities(int arg_mouseX, int arg_mouseY, int arg_selection);
private:
	const int mDIAGCOST = 14;
	const int mORTHOCOST = 10;	
	const int SIZE_PIXEL = 64;
	const int GRID_OFFSET = 10;
	const int CELL_SIZE = 20;
	const int HEIGHT = 720;
	const int WIDTH = 1280;
	std::vector<std::vector<std::shared_ptr<Cell>>> mCells;
	std::vector<std::vector<std::shared_ptr<Cell>>> mFreeCells;
	int Random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
};

