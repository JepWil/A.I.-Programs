#include "Grid.h"
#include <iostream>

Grid::Grid()
{
}

void Grid::Draw(int arg_window_width, int arg_window_height, int arg_hori_dist, int arg_vert_dist)
{
	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // White (RGBA)
	for (float i = 0; i < arg_window_width; i += arg_hori_dist)
	{
		glVertex3f(i, 0.0f, 0.0f);
		glVertex3f(i, float(arg_window_height), 0.0f);
	}
	for (float j = 0; j < arg_window_height; j += arg_vert_dist)
	{
		glVertex3f(0.0f, j, 0.0f);
		glVertex3f(float(arg_window_width), j, 0.0f);
	}
	glEnd();
}

void Grid::DrawTile(int arg_x, int arg_y)
{
	for (int i = 0; i < arg_x * arg_y; i++)
	{
		tiles_[i]->draw();
	}
}

void Grid::CreateGrid(int arg_width_, int arg_height_)
{
	for (int i = 0; i < arg_width_ / TILE_SIZE; i++)
	{
		for (int j = 0; j < arg_height_ /20; j++)
		{
			std::shared_ptr<Tile> tile_ = std::make_shared<Tile>(j*TILE_SIZE + GRID_OFFSET, i*TILE_SIZE + GRID_OFFSET, TILE_SIZE); // j and i have been swapped
			tiles_.push_back(tile_);
		}
	}
}

Tile_Type Grid::getType(int arg_i, int arg_j)
{
	return tiles_[(floor(arg_i) / TILE_SIZE) + (SIZE_PIXEL * (int)(arg_j / TILE_SIZE))]->type;
}

Animal_Tile_Type Grid::getAnimalType(int arg_i, int arg_j)
{
	return tiles_[(floor(arg_i) / TILE_SIZE) + (SIZE_PIXEL * (int)(arg_j / TILE_SIZE))]->animal_type;
}

void Grid::setType(int arg_i, int arg_j, Tile_Type arg_enum)
{
	tiles_[(floor(arg_i) / TILE_SIZE) + (SIZE_PIXEL * (int)(arg_j / TILE_SIZE))]->type = arg_enum;
}

void Grid::setAnimalType(int arg_i, int arg_j, Animal_Tile_Type arg_enum)
{
	tiles_[(floor(arg_i) / TILE_SIZE) + (SIZE_PIXEL * (int)(arg_j / TILE_SIZE))]->animal_type = arg_enum;
}

std::shared_ptr<std::vector<std::shared_ptr<Tile>>> Grid::GetTilesPtr()
{
	auto tiles_ptr = std::make_shared<std::vector<std::shared_ptr<Tile>>>(tiles_);
	return tiles_ptr;
}

std::shared_ptr<Tile> Grid::GetTile(int arg_i, int arg_j)
{
	return tiles_[(floor(arg_i) / TILE_SIZE) + (SIZE_PIXEL * (int)(arg_j / TILE_SIZE))];
}

Grid::~Grid()
{
}
