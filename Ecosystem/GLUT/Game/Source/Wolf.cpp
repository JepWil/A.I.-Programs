#include "Wolf.h"
#include "Sheep.h"
#include <iostream>
#include <math.h>

Wolf::Wolf(Game* arg_game, int arg_x, int arg_y, int arg_length)
{
	hp = random(MIN_HP, MAX_HP);
	length = arg_length;
	x = arg_x;
	y = arg_y;
	gameref = arg_game;
	wolfstate = Wolf::Wolf_State::WANDER;
	random_spawn = 0;
}

Wolf::~Wolf()
{
}

void Wolf::sense(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	// clear sense vectors
	neighboursheep.clear();
	neighbourdirtgrass.clear();
	neighbourgrass.clear();

	sensegrass(arg_tiles_vec);
	sensedirtngrass(arg_tiles_vec);
	sensesheep(arg_tiles_vec);
}

void Wolf::decide()
{	
	if (hp <= 0)
	{
		wolfstate = Wolf::Wolf_State::DEAD;
	}
	else if (hp >= 90 && gameref->grid_.getAnimalType(x, y) != Animal_Tile_Type::SHEEP && neighbourdirtgrass.size() != 0)
	{
		wolfstate = Wolf::Wolf_State::SPAWN_NEW;
	}
	else if (sheeponme && wolfstate == Wolf::Wolf_State::HUNT && hp < 90)
	{
		wolfstate = Wolf::Wolf_State::EAT;
		sheeponme = false;
	}
	else if (euclideandistance(target->getx(), target->gety(), x, y) < 8 * length) 
	{
		wolfstate = Wolf::Wolf_State::HUNT;
	}
	else if (neighboursheep.size() <= 0)
	{
		wolfstate = Wolf::Wolf_State::WANDER;
	}
}

void Wolf::act(float arg_timer)
{
	if (wolfstate == Wolf::Wolf_State::EAT)
	{
		hp += 0.35f;
		if (gameref->grid_.getAnimalType(x, y) != Animal_Tile_Type::SHEEP)
		{
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
	}
	else if (wolfstate == Wolf::Wolf_State::WANDER && arg_timer > 1.0f) // movement
	{
		int random_dir = random(1, 4);
		if (random_dir == 1 && x + length <= WIDTH && gameref->grid_.getAnimalType(x + length, y) != WOLF)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x += length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (random_dir == 2 && x - length >= 0 && gameref->grid_.getAnimalType(x - length, y) != WOLF)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x -= length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (random_dir == 3 && y + length <= HEIGHT && gameref->grid_.getAnimalType(x, y + length) != WOLF)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			y += length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (random_dir == 4 && y - length >= 0 && gameref->grid_.getAnimalType(x, y - length) != WOLF)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			y -= length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		actTimer = 0;
	}
	
	else if (wolfstate == Wolf::Wolf_State::SPAWN_NEW && neighbourdirtgrass.size() > 0)
	{
		wolfstate = Wolf::Wolf_State::WANDER;
		random_spawn = random(0, neighbourdirtgrass.size() - 1);
		if (neighbourdirtgrass[random_spawn]->animal_type != Animal_Tile_Type::WOLF)
		{
			random_spawn = random(0, neighbourdirtgrass.size() - 1);
		}
		gameref->AddAgent("Wolf", neighbourdirtgrass[random_spawn]->x, neighbourdirtgrass[random_spawn]->y);
		hp = hp / HP_FACTOR;
	}
	if (wolfstate == HUNT && actTimer > 0.7f)
	{
		if (euclideandistance(x + length, y, target->getx(), target->gety()) < euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x + length, y) != Animal_Tile_Type::WOLF) && x <= WIDTH)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = x + length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (euclideandistance(x - length, y, target->getx(), target->gety()) < euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x - length, y) != Animal_Tile_Type::WOLF) && x >= 0)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = x - length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (euclideandistance(x, y + length, target->getx(), target->gety()) < euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x, y + length) != Animal_Tile_Type::WOLF) && y <= HEIGHT)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			y = y + length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}
		else if (euclideandistance(x, y - length, target->getx(), target->gety()) < euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x, y - length) != Animal_Tile_Type::WOLF) && y >= 0)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			y = y - length;
			gameref->grid_.setAnimalType(x, y, WOLF);
		}

		if (target->getx() == x && target->gety() == y)
		{
			sheeponme = true;
		}
		actTimer = 0;
	}

	// out of hp - remove
	if (wolfstate == Wolf::Wolf_State::DEAD)
	{
		gameref->RemoveAgent("Wolf", x, y);
	}
	// if not dead, always lessen a little hp
	else
	{
		hp -= 0.005; 
	}
}

float Wolf::euclideandistance(int arg_x1, int arg_y1, int arg_x2, int arg_y2)
{
	return sqrt((((arg_x2 - arg_x1)*(arg_x2 - arg_x1)) + ((arg_y2 - arg_y1)*(arg_y2 - arg_y1))));
}

void Wolf::sensesheep(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	std::shared_ptr<Sheep> temp = NULL;
	for (size_t i = 0; i < gameref->getSheep().size(); i++)
	{
		if (i == 0)
			temp = gameref->getSheep()[0];

		if ((euclideandistance(gameref->getSheep()[i]->getx(), gameref->getSheep()[i]->gety(), x, y) < euclideandistance(temp->getx(), temp->gety(), x, y)))
			temp = gameref->getSheep()[i];
	}

	target = temp;
}

void Wolf::sensedirtngrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	if (x + length <= WIDTH && gameref->grid_.getType(x + length, y) == Tile_Type::DIRT ) // +x,  
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x + length, y));
	}
	if (x - length >= 0 && gameref->grid_.getType(x - length, y) == Tile_Type::DIRT) // -x, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x - length, y));
	}
	if (y + length <= HEIGHT && gameref->grid_.getType(x, y + length) == Tile_Type::DIRT) // +y, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x, y + length));
	}
	if (y - length >= 0 && gameref->grid_.getType(x, y - length) == Tile_Type::DIRT) // -y, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x, y - length));
	}

	neighbourdirtgrass.insert(std::end(neighbourdirtgrass), std::begin(neighbourgrass), std::end(neighbourgrass));
}

void Wolf::sensegrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	if (x + length <= WIDTH && gameref->grid_.getType(x + length, y) == Tile_Type::GRASS) // +x,  
	{
		neighbourgrass.push_back(gameref->grid_.GetTile(x + length, y));
	}
	if (x - length >= 0 && gameref->grid_.getType(x - length, y) == Tile_Type::GRASS) // -x, 
	{
		neighbourgrass.push_back(gameref->grid_.GetTile(x - length, y));
	}
	if (y + length <= HEIGHT && gameref->grid_.getType(x, y + length) == Tile_Type::GRASS) // +y, 
	{
		neighbourgrass.push_back(gameref->grid_.GetTile(x, y + length));
	}
	if (y - length >= 0 && gameref->grid_.getType(x, y - length) == Tile_Type::GRASS) // -y, 
	{
		neighbourgrass.push_back(gameref->grid_.GetTile(x, y - length));
	}
}

void Wolf::update(float arg_deltatime, std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	timer += arg_deltatime;
	actTimer += arg_deltatime;

	if (timer > 0.5f)
	{
		sense(arg_tiles_vec);
		decide();	
		timer = 0;
	}

	act(actTimer); // every tick
}

int Wolf::getx()
{
	return x;
}

int Wolf::gety()
{
	return y;
}

int Wolf::getlength()
{
	return length;
}