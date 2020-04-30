#include "Grass.h"
#include <iostream>

Grass::Grass(Game *arg_game, int arg_x, int arg_y, int arg_length)
{
	hp = random(MIN_HP, MAX_HP);
	length = arg_length;
	x = arg_x;
	y = arg_y;
	gameref = arg_game;
	grassstate = Grass::Grass_State::GROWING;
	AnimalOnMe = false;
}

Grass::~Grass()
{
	gameref = NULL;
}

void Grass::sense()
{
	if (gameref->grid_.getAnimalType(x, y) != NONE) // if tile this grass occupies is also occupied by a sheep/wolf
	{
		AnimalOnMe = true;
	}
}

void Grass::decide()
{
	if (AnimalOnMe)
	{
		grassstate = Grass::Grass_State::DAMAGED;
	}

	if (hp >= 100 && grassstate != Grass::Grass_State::DAMAGED)
	{
		grassstate = Grass::Grass_State::SPREADING;
	}
	else if (hp <= 0)
	{
		grassstate = Grass::Grass_State::DEPLETED;
	}
	else if (hp <= 100 && grassstate != Grass::Grass_State::DAMAGED)
	{
		grassstate = Grass::Grass_State::GROWING;
	}
}

void Grass::act(float arg_timer)
{
	// get types of adjacent tiles. If dirt, create new grass. 
	if (grassstate == Grass::Grass_State::SPREADING)
	{
		random_dir = random(1, 4);
		if (x + length < WIDTH && gameref->grid_.getType(x + length, y) == Tile_Type::DIRT && random_dir == 1) // +x, right
		{
			gameref->AddAgent("Grass", x + length, y);
		}
		if (y + length < HEIGHT && gameref->grid_.getType(x, y + length) == Tile_Type::DIRT && random_dir == 2) // +y, down
		{
			gameref->AddAgent("Grass", x, y + length);
		}
		if (x - length > 0 && gameref->grid_.getType(x - length, y) == Tile_Type::DIRT && random_dir == 3) // -x, left
		{
			gameref->AddAgent("Grass", x - length, y);
		}
		if (y - length > 0 && gameref->grid_.getType(x, y - length) == Tile_Type::DIRT && random_dir == 4) // -y, up
		{
			gameref->AddAgent("Grass", x, y - length);
		}
		random_dir = 0;
		hp = 10;
		decide();
	}

	if (grassstate == Grass::Grass_State::DAMAGED)
	{
		hp -= DMG;
	}
	else if (grassstate == Grass::Grass_State::GROWING)
	{
		hp += GROWING_SPEED;
	}

	// current grass is dead so tell the game to remove it.
	if (grassstate == Grass::Grass_State::DEPLETED)
	{
		gameref->RemoveAgent("Grass", x, y); 
	}

}

void Grass::update(float arg_deltatime)
{
	timer += arg_deltatime;
	if (timer > 1.0f)
	{
		sense();
		decide();	
		timer = 0;
	}
	act(0); // every tick
}

int Grass::getx()
{
	return x;
}

int Grass::gety()
{
	return y;
}

int Grass::getlength()
{
	return length;
}


