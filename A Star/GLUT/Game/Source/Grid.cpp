#include "Grid.h"
#include <iostream>

Grid::Grid()
{
}

void Grid::Draw(int arg_window_width, int arg_window_height, int arg_hori_dist, int arg_vert_dist)
{
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

void Grid::CreateGrid(int arg_width_, int arg_height_, int arg_cell_size)
{
	for (int i = 0; i < arg_height_ / arg_cell_size; i++)
	{
		std::vector<std::shared_ptr<Cell>> TempVec; // Inner vector
		std::vector<std::shared_ptr<Cell>> TempFreeCellsVec; // Inner vector
		for (int j = 0; j < arg_width_ / arg_cell_size; j++)
		{
			std::shared_ptr<Cell> xCell = std::make_shared<Cell>(j*arg_cell_size + GRID_OFFSET, i*arg_cell_size + GRID_OFFSET, arg_cell_size, Random(1, 3), Random(1, 3));
			TempVec.push_back(xCell);
			if (xCell->mCellAccess == FREE)
			{
				TempFreeCellsVec.push_back(xCell);
			}
		}
		mFreeCells.push_back(TempFreeCellsVec);
		mCells.push_back(TempVec);
	}
}

int Grid::GetDistance(std::shared_ptr<Cell> arg_lhs, std::shared_ptr<Cell> arg_rhs) // Manhattan & Euclidean
{
	int distanceX = (std::abs(arg_lhs->mCellX - arg_rhs->mCellX)) / CELL_SIZE;
	int distanceY = (std::abs(arg_lhs->mCellY - arg_rhs->mCellY)) / CELL_SIZE;

	if (distanceX > distanceY)
	{
		return mDIAGCOST * distanceY + mORTHOCOST * (distanceX - distanceY);
	}
	else
		return mDIAGCOST * distanceX + mORTHOCOST * (distanceY - distanceX);

	//return sqrt(pow(arg_lhs->mCellX - arg_rhs->mCellX, 2) + pow(arg_lhs->mCellY - arg_rhs->mCellY, 2));
}

std::vector<std::vector<std::shared_ptr<Cell>>> Grid::GetFreeCells()
{
	return mFreeCells;
}

std::shared_ptr<Cell> Grid::GetFreeCellAt(int arg_x, int arg_y)
{
	return mFreeCells[floor(arg_y / CELL_SIZE)][floor(arg_x / CELL_SIZE)];
}

void Grid::pressGridBlocked(int arg_mouseX, int arg_mouseY)
{
	for (int i = 0; i < (int)mCells.size(); i++)
	{
		for (int j = 0; j < (int)mCells[i].size(); j++)
		{
			// if the cell's top left x y is less than the mouse's and mouses's x y is less than bottom right
			if ((arg_mouseX > mCells[i][j]->mCellX - CELL_SIZE / 2 && arg_mouseY > mCells[i][j]->mCellY - CELL_SIZE / 2)
				&& (arg_mouseX < mCells[i][j]->mCellX + CELL_SIZE / 2 && arg_mouseY < mCells[i][j]->mCellY + CELL_SIZE / 2))
			{
				if (mCells[i][j]->mCellAccess == cellaccess::FREE)
				{
					mCells[i][j]->mCellAccess = cellaccess::BLOCKED;
				}
				else if (mCells[i][j]->mCellAccess == cellaccess::BLOCKED)
				{
					mCells[i][j]->mCellAccess = cellaccess::FREE;
				}
			}
		}
	}
}

void Grid::pressGridEntities(int arg_mouseX, int arg_mouseY, int arg_selection)
{
	for (int i = 0; i < (int)mCells.size(); i++)
	{
		for (int j = 0; j < (int)mCells[i].size(); j++)
		{
			// if the cell's top left x y is less than the mouse's and mouses's x y is less than bottom right
			if ((arg_mouseX > mCells[i][j]->mCellX - CELL_SIZE / 2 && arg_mouseY  > mCells[i][j]->mCellY - CELL_SIZE / 2)
				&& (arg_mouseX < mCells[i][j]->mCellX + CELL_SIZE / 2 && arg_mouseY < mCells[i][j]->mCellY + CELL_SIZE / 2))
			{
				if (arg_selection == 1 && mCells[i][j]->mCellAccess == cellaccess::FREE)
				{
					for (int h = 0; h < (int)mCells.size(); h++)
					{
						for (int g = 0; g< (int)mCells[i].size(); g++)
						{
							if (mCells[h][g]->mCellType = celltype::STARCHASER) {
								mCells[h][g]->mCellType = celltype::DIRT;
								break;
							}
						}
					}
					mCells[i][j]->mCellType = celltype::STARCHASER;
				}
				else if (arg_selection == 2 && mCells[i][j]->mCellAccess == cellaccess::FREE)
				{
					for (int h = 0; h < (int)mCells.size(); h++)
					{
						for (int g = 0; g < (int)mCells[i].size(); g++)
						{
							if (mCells[h][g]->mCellType = celltype::FALLENSTAR) {
								mCells[h][g]->mCellType = celltype::SPACEGRASS;
								break;
							}
						}
					}
					mCells[i][j]->mCellType = celltype::FALLENSTAR;
				}
				else if (arg_selection == 3 && mCells[i][j]->mCellAccess == cellaccess::FREE)
				{
					for (int h = 0; h < (int)mCells.size(); h++)
					{
						for (int g = 0; g < (int)mCells[i].size(); g++)
						{
							if (mCells[h][g]->mCellType = celltype::SPACESHIP) {
								mCells[h][g]->mCellType = celltype::DIRT;
								break;
							}
						}
					}
					mCells[i][j]->mCellType = celltype::SPACESHIP;
				}
				else if (arg_selection == 4 && mCells[i][j]->mCellAccess == cellaccess::FREE)
				{
					for (int h = 0; h < (int)mCells.size(); h++)
					{
						for (int g = 0; g < (int)mCells[i].size(); g++)
						{
							if (mCells[h][g]->mCellType = celltype::TRADINGPOST) {
								mCells[h][g]->mCellType = celltype::SPACEGRASS;
								break;
							}
						}
					}
					mCells[i][j]->mCellType = celltype::TRADINGPOST;
				}
			}
		}
	}
}

std::shared_ptr<Cell> Grid::GetCellAt(int arg_x, int arg_y)
{
	return mCells[floor(arg_y / CELL_SIZE)][floor(arg_x / CELL_SIZE)];
}

std::vector<std::shared_ptr<Cell>> Grid::GetNeighbourCells(std::shared_ptr<Cell> arg_cell, std::shared_ptr<Grid> arg_grid)
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
			neighbourCells.push_back(arg_grid->GetAllCells()[arg_cell->mCellY / CELL_SIZE + i][arg_cell->mCellX / CELL_SIZE + j]);
		}
	}
	return neighbourCells;
}

std::vector<std::vector<std::shared_ptr<Cell>>> Grid::GetAllCells()
{
	return mCells;
}


Grid::~Grid()
{
}
