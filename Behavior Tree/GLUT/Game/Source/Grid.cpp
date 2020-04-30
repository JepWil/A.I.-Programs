#include "Grid.h"
#include <iostream>

Grid::Grid()
{
}

void Grid::UpdateCellTypes(int arg_x, int arg_y, Cell::celltype type)
{
	mCells[arg_y / CELL_SIZE][arg_x / CELL_SIZE]->mCELLTYPE = type;
}

void Grid::DrawCell(int arg_x, int arg_y)
{
	for (int i = 0; i < arg_y; i++)
	{
		for (int j = 0; j < arg_x; j++)
		{
			mCells[i][j]->Draw();
		}
	}
}

void Grid::CreateGrid(int arg_width_, int arg_height_, int arg_cell_size, Game* p_xGame)
{
	mxGame = p_xGame;

	for (int i = 0; i < arg_height_ / arg_cell_size; i++)
	{
		std::vector<std::shared_ptr<Cell>> TempVec; // Inner vector
		for (int j = 0; j < arg_width_ / arg_cell_size; j++)
		{
			std::shared_ptr<Cell> xCell = std::make_shared<Cell>(j*arg_cell_size + GRID_OFFSET, i*arg_cell_size + GRID_OFFSET, arg_cell_size);
			TempVec.push_back(xCell);
		}
		mCells.push_back(TempVec);
	}
}

std::shared_ptr<Cell> Grid::GetCellAt(int arg_x, int arg_y)
{
	return mCells[arg_y / CELL_SIZE][arg_x / CELL_SIZE];
}

std::vector<std::shared_ptr<Cell>> Grid::Get_Neighbour_Cells(std::shared_ptr<Cell> arg_cell, std::shared_ptr<Grid> arg_grid)
{
	std::vector<std::shared_ptr<Cell>> neighbourCells;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (arg_cell->mCellX + j * CELL_SIZE <= 0 || arg_cell->mCellX + j * CELL_SIZE > WIDTH) {
				continue;
			}
			if (arg_cell->mCellY + i * CELL_SIZE <= 0 || arg_cell->mCellY + i * CELL_SIZE > HEIGHT) {
				continue;
			}
			if (i == 0 && j == 0) {
				continue;
			}
			neighbourCells.push_back(arg_grid->Get_All_Cells()[arg_cell->mCellY / CELL_SIZE + i][arg_cell->mCellX / CELL_SIZE + j]);
		}
	}
	return neighbourCells;
}

std::vector<std::vector<std::shared_ptr<Cell>>> Grid::Get_All_Cells()
{
	return mCells;
}


Grid::~Grid()
{}
