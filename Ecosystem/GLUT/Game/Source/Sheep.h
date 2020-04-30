#pragma once
#include <memory>
#include "Grid.h"
#include "Game.h"
#include <random>
#include <memory>

class Grass;
class Wolf;
class Game;
class Sheep
{	
public:
	Sheep(Game* arg_game, int arg_x, int arg_y, int arg_length);
	~Sheep();
	void update(float arg_deltatime, std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	int getx();
	int gety();
	int getlength();

private: 	
	std::shared_ptr<Wolf> target = nullptr;	
	std::shared_ptr<Grass> targetGrass = nullptr;
	std::shared_ptr<Tile> neighgrass = nullptr;
	float euclideandistance(int arg_x1, int arg_y1, int arg_x2, int arg_y2);
	void sense(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	void decide();
	void act(float arg_timer);
	void sensegrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	void sensewolves();
	void Sheep::sensedirtngrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);

	enum Sheep_State
	{
		EVADE,
		EAT,
		WANDER,
		ATTACKED,
		DEAD,
		SPAWN_NEW,
		EVADE_LEFT,
		EVADE_RIGHT,
		EVADE_DOWN,
		EVADE_UP,
		GRAZING,
		GRAZING_LEFT,
		GRAZING_RIGHT,
		GRAZING_DOWN,
		GRAZING_UP,
	};

	Sheep_State sheepstate;

	float hp = 0.0f;
	float timer = 0.0f;
	float actTimer = 0.0f;

	int x = 0;
	int y = 0;
	int length = 0;
	int random_spawn;

	const int MAX_HP = 75;
	const int MIN_HP = 1;
	const int HP_FACTOR = 10;
	const int WIDTH = 1270;
	const int HEIGHT = 710;

	Game* gameref;

	int random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}

	std::vector<std::shared_ptr<Tile>> neighbourgrass;
	std::vector<std::shared_ptr<Tile>> neighbourwolves;
	std::vector<std::shared_ptr<Tile>> neighbourdirtgrass;
};