#pragma once

#include "Grid.h"
#include "Monster.h"
#include "Healer.h"
#include "Tank.h"
#include <memory>
#include <Math.h>
#include <stdio.h>
#include <random>
#include <stdarg.h> // Required on some systems
#include "glut.h" // Always have this the furthest down of the includes

class Tank;
class Monster;
class Healer;
class Grid;

class Game 
{
public:
    Game(void);
    ~Game(void);
    void ChangeSize(int w, int h);
    void Draw();
	void Update();
    void NormalKeys(unsigned char key, int x, int y);
    void SpecialKeys(int key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Gfx_Text(float x, float y, const char *fmt, ...);
	float mW;
	float mH;
	int mMouseX;
	int mMouseY;
	int mMouseButton;
	int mMouseState;
	int mGlutGetModifiers;
	const int CELL_SIZE = 10;
	const int MAX_MONSTERS = 2;
	const int HEALER_X_SPAWN_FACTOR = 4;
	const int TANK_X_SPAWN_FACTOR = 4;
	const float MONSTER_X_SPAWN_FACTOR = 1.4f;
	std::vector<std::shared_ptr<Monster>> GetMonstersVector();
	std::shared_ptr<Blackboard> GetSharedBlackboard();
	std::shared_ptr<Grid> GetGrid();
private:
	std::shared_ptr<Grid> mxGrid = NULL;
	float MonsterSpawnTimer = 0.0f;
	std::shared_ptr<Blackboard> mxSharedBBoard;
	std::shared_ptr<Healer> mxHealer;
	std::shared_ptr<Tank> mxTank;
	std::vector<std::shared_ptr<Monster>> mMonsters;
	const int GRID_ALIGNMENT = 0;
	int mOldTimeSinceStart = 0;
	int Random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
};

