#pragma once
#include <memory>
#include "Grid.h"
#include "Game.h"
#include <random>
class Sheep;
class Wolf
{
public:
	Wolf(Game* arg_game, int arg_x, int arg_y, int arg_length);
	~Wolf();
	void update(float arg_deltatime, std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	int getx();
	int gety();
	int getlength();

	enum Wolf_State
	{
		EAT,
		WANDER,
		HUNT,
		HUNT_RIGHT,
		HUNT_LEFT,
		HUNT_UP,
		HUNT_DOWN,
		DEAD,
		SPAWN_NEW,
	};

	Wolf_State wolfstate;

private:	
	float euclideandistance(int arg_x1, int arg_y1, int arg_x2, int arg_y2);

	bool sheeponme = false;

	Game* gameref;

	void sense(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	void decide();
	void act(float arg_timer);	
	void sensegrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	void sensesheep(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);
	void sensedirtngrass(std::shared_ptr<std::vector<std::shared_ptr<Tile>>> arg_tiles_vec);

	float hp = 0.0f;
	float timer = 0.0f;
	float actTimer = 0.0f;

	std::shared_ptr<Sheep> target = nullptr;

	int random_spawn = 0;
	int x = 0;
	int y = 0;
	int length = 0;

	const int MAX_HP = 50;
	const int MIN_HP = 1;
	const int HP_FACTOR = 10;
	const int WIDTH = 1270;
	const int HEIGHT = 710;

	std::vector<std::shared_ptr<Tile>> neighboursheep;
	std::vector<std::shared_ptr<Tile>> neighbourdirtgrass;
	std::vector<std::shared_ptr<Tile>> neighbourgrass;

	std::shared_ptr<Tile> neighsheep = nullptr;

	int random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
};