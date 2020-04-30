#include "Sheep.h"
#include <iostream>

Sheep::Sheep(Game* arg_game, int arg_x, int arg_y, int arg_length)
{
	hp = random(MIN_HP, MAX_HP);
	length = arg_length;
	x = arg_x;
	y = arg_y;
	gameref = arg_game;
	sheepstate = Sheep::Sheep_State::WANDER;
	random_spawn = 0;
}

Sheep::~Sheep()
{
}

void Sheep::sense(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	// clear sense vectors
	neighbourwolves.clear();
	neighbourdirtgrass.clear();
	neighbourgrass.clear();

	sensegrass(arg_tiles_vec); 
	sensewolves(); 
	sensedirtngrass(arg_tiles_vec); 
}

void Sheep::decide()
{
	if (hp <= 0)
	{
		sheepstate = Sheep::Sheep_State::DEAD;
	}
	if (gameref->grid_.getAnimalType(x, y) == Animal_Tile_Type::WOLF)
	{
		std::cout << "Wolf on me!" << std::endl;
		sheepstate = Sheep::Sheep_State::DEAD; // Sheep dies before wolf gets to eat it
	}
	else if ((euclideandistance(target->getx(), target->gety(), x, y) < 8 * length))// dumbest sheep actually
	{
		sheepstate = Sheep::Sheep_State::EVADE;
	}
	else if (hp >= 90 && neighbourwolves.size() <= 0 && gameref->grid_.getAnimalType(x, y) != Animal_Tile_Type::WOLF && neighbourdirtgrass.size() != 0)
	{
		sheepstate = Sheep::Sheep_State::SPAWN_NEW;
	}
	else if (gameref->grid_.getType(x, y) == Tile_Type::GRASS && hp < 90) // tile sheep is on has grass?
	{
		sheepstate = Sheep::Sheep_State::EAT;
	}
	else if (neighbourgrass.size() <= 0 && neighbourwolves.size() <= 0)
	{
		sheepstate = Sheep::Sheep_State::WANDER;
	}
	else if (sheepstate != Sheep::Sheep_State::DEAD && hp < 75 && neighbourgrass.size() > 0 && neighbourwolves.size() <= 0)
	{
		for (size_t i = 0; i < neighbourgrass.size(); i++) // exceptionally dumb sheep
		{
			if (neighbourgrass[i]->x > x && gameref->grid_.getAnimalType(x + length, y) != Animal_Tile_Type::SHEEP)
			{
				neighgrass = neighbourgrass[i];
				sheepstate = Sheep::Sheep_State::GRAZING_RIGHT;
				break;
			}
			else if (neighbourgrass[i]->x < x && gameref->grid_.getAnimalType(x - length, y) != Animal_Tile_Type::SHEEP)
			{
				neighgrass = neighbourgrass[i];
				sheepstate = Sheep::Sheep_State::GRAZING_LEFT;
				break;
			}
			else if (neighbourgrass[i]->y > y && gameref->grid_.getAnimalType(x, y + length) != Animal_Tile_Type::SHEEP)
			{
				neighgrass = neighbourgrass[i];
				sheepstate = Sheep::Sheep_State::GRAZING_DOWN;
				break;
			}
			else if (neighbourgrass[i]->y < y && gameref->grid_.getAnimalType(x, y - length) != Animal_Tile_Type::SHEEP)
			{
				neighgrass = neighbourgrass[i];
				sheepstate = Sheep::Sheep_State::GRAZING_UP;
				break;
			}
		}
	}
}

void Sheep::act(float arg_timer)
{
	if (sheepstate == Sheep::Sheep_State::EAT)
	{
		hp += 0.4;
	}
	else if (sheepstate == Sheep::Sheep_State::SPAWN_NEW && neighbourdirtgrass.size() > 0)
	{
		random_spawn = random(0, neighbourdirtgrass.size() - 1);
		if (neighbourdirtgrass[random_spawn]->animal_type != Animal_Tile_Type::SHEEP)
		{
			random_spawn = random(0, neighbourdirtgrass.size() - 1);
		}
		gameref->AddAgent("Sheep", neighbourdirtgrass[random_spawn]->x, neighbourdirtgrass[random_spawn]->y);
		hp = hp / HP_FACTOR;
		sheepstate = Sheep::Sheep_State::WANDER;
	}

	// fleeing & grazing
	if (arg_timer > 1.25f)
	{
		if (sheepstate == Sheep::Sheep_State::EVADE)
		{
			if (euclideandistance(x + length, y, target->getx(), target->gety()) > euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x + length, y) != Animal_Tile_Type::WOLF))
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				x = x + length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
				std::cout << "go right" << std::endl;
			}
			else if (euclideandistance(x - length, y, target->getx(), target->gety()) > euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x - length, y) != Animal_Tile_Type::WOLF))
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				x = x - length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
				std::cout << "go left" << std::endl;
			}
			else if (euclideandistance(x, y + length, target->getx(), target->gety()) > euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x, y + length) != Animal_Tile_Type::WOLF))
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				y = y + length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
				std::cout << "go down" << std::endl;
			}
			else if (euclideandistance(x, y - length, target->getx(), target->gety()) > euclideandistance(x, y, target->getx(), target->gety()) && (gameref->grid_.getAnimalType(x, y - length) != Animal_Tile_Type::WOLF))
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				y = y - length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
				std::cout << "go up" << std::endl;
			}
		}
		if (sheepstate == Sheep::Sheep_State::WANDER && arg_timer > 1.25f)
		{
			int random_dir = random(1, 4);
			if (random_dir == 1 && x + length <= WIDTH && gameref->grid_.getAnimalType(x + length, y) != SHEEP)
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				x += length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
			}
			else if (random_dir == 2 && x - length >= 0 && gameref->grid_.getAnimalType(x - length, y) != SHEEP)
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				x -= length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
			}
			else if (random_dir == 3 && y + length <= 710 && gameref->grid_.getAnimalType(x, y + length) != SHEEP)
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				y += length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
			}
			else if (random_dir == 4 && y - length >= 0 && gameref->grid_.getAnimalType(x, y - length) != SHEEP)
			{
				gameref->grid_.setAnimalType(x, y, NONE);
				y -= length;
				gameref->grid_.setAnimalType(x, y, SHEEP);
			}
			actTimer = 0;
		}
		if (x + length <= WIDTH && sheepstate == Sheep::Sheep_State::GRAZING_RIGHT && gameref->grid_.getAnimalType(x + length, y) != Animal_Tile_Type::SHEEP)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = neighgrass->x;
			y = neighgrass->y;
			gameref->grid_.setAnimalType(x, y, SHEEP);
		}
		else if (x - length >= 0 && sheepstate == Sheep::Sheep_State::GRAZING_LEFT && gameref->grid_.getAnimalType(x - length, y) != Animal_Tile_Type::SHEEP)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = neighgrass->x;
			y = neighgrass->y;
			gameref->grid_.setAnimalType(x, y, SHEEP);
		}
		else if (y + length <= 710 && sheepstate == Sheep::Sheep_State::GRAZING_DOWN && gameref->grid_.getAnimalType(x, y + length) != Animal_Tile_Type::SHEEP)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = neighgrass->x;
			y = neighgrass->y;
			gameref->grid_.setAnimalType(x, y, SHEEP);
		}
		else if (y - length >= 0 && sheepstate == Sheep::Sheep_State::GRAZING_UP && gameref->grid_.getAnimalType(x, y - length) != Animal_Tile_Type::SHEEP)
		{
			gameref->grid_.setAnimalType(x, y, NONE);
			x = neighgrass->x;
			y = neighgrass->y;
			gameref->grid_.setAnimalType(x, y, SHEEP);
		}
		actTimer = 0;
	}

	// out of hp - remove
	if (sheepstate == Sheep::Sheep_State::DEAD)
	{
		std::cout << "I died" << std::endl;
		gameref->RemoveAgent("Sheep", x, y);
	}
	// if not dead, always lessen a little hp
	else
	{
		hp -= 0.0; // 0.05 
	}
}

void Sheep::sensedirtngrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	if (x + length <= WIDTH && gameref->grid_.getType(x + length, y) == Tile_Type::DIRT && gameref->grid_.getType(x + length, y) != Animal_Tile_Type::SHEEP) // +x,  
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x + length, y));
	}
	if (x - length >= 0 && gameref->grid_.getType(x - length, y) == Tile_Type::DIRT && gameref->grid_.getType(x - length, y) != Animal_Tile_Type::SHEEP) // -x, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x - length, y));
	}
	if (y + length <= HEIGHT && gameref->grid_.getType(x, y + length) == Tile_Type::DIRT && gameref->grid_.getType(x, y + length) != Animal_Tile_Type::SHEEP) // +y, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x, y + length));
	}
	if (y - length >= 0 && gameref->grid_.getType(x, y - length) == Tile_Type::DIRT && gameref->grid_.getType(x, y - length) != Animal_Tile_Type::SHEEP) // -y, 
	{
		neighbourdirtgrass.push_back(gameref->grid_.GetTile(x, y - length));
	}

	neighbourdirtgrass.insert(std::end(neighbourdirtgrass), std::begin(neighbourgrass), std::end(neighbourgrass));
}

void Sheep::sensegrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
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

void Sheep::sensewolves()
{
	std::shared_ptr<Wolf> temp = NULL;
	for (int i = 0; i < gameref->getWolves().size(); i++)
	{
		if (i == 0)
			temp = gameref->getWolves()[0];

		if ((euclideandistance(gameref->getWolves()[i]->getx(), gameref->getWolves()[i]->gety(), x, y) < euclideandistance(temp->getx(), temp->gety(), x, y)))
			temp = gameref->getWolves()[i];
	}

	target = temp;
}


float Sheep::euclideandistance(int arg_x1, int arg_y1, int arg_x2, int arg_y2)
{
	return sqrt((((arg_x2 - arg_x1)*(arg_x2 - arg_x1)) + ((arg_y2 - arg_y1)*(arg_y2 - arg_y1))));
}

void Sheep::update(float arg_deltatime, std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec)
{
	timer += arg_deltatime;
	actTimer += arg_deltatime;

	if (timer > 1.05f)
	{
		sense(arg_tiles_vec);
		decide();
		timer = 0;
	}

	act(actTimer); // every tick
}

int Sheep::getx()
{
	return x;
}

int Sheep::gety()
{
	return y;
}

int Sheep::getlength()
{
	return length;
}

