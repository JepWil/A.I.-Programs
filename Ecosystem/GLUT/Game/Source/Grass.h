#pragma once
#include <memory>
#include "Grid.h"
#include "Game.h"
#include <random>

class Game;

class Grass
{	
public:
	Grass(Game *arg_game, int arg_x, int arg_y, int arg_length);
	~Grass();

	void update(float arg_timepassed);
	int getx();
	int gety();
	int getlength();
	Game* gameref;	
	
private:
	void sense();
	void decide();
	void act(float arg_timer);

	enum Grass_State
	{
		SPREADING,
		DAMAGED,
		DEPLETED,
		GROWING,
	};	

	Grass_State grassstate;	

	float timer = 0.0f;
	float arg_timer = 0.0f;
	float hp = 0.0f;

	int x = 0;
	int y = 0;
	int length = 0;
	int random_dir = 0;

	const int MAX_HP = 100;
	const int MIN_HP = 1;
	const int DMG = 5;
	const int HP_FACTOR = 10;
	const int WIDTH = 1270;
	const int HEIGHT = 710;
	const float GROWING_SPEED = 0.1f;

	bool AnimalOnMe = false;

	int random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
};

