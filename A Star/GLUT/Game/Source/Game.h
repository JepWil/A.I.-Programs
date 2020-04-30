#pragma once

class StarChaser;

#include "Grid.h"
#include "Spaceship.h"
#include "TradingPost.h"
#include "FallenStar.h"
#include "StarChaser.h"
#include <memory>
#include <Math.h>
#include <stdio.h>
#include <random>
#include <stdarg.h> // Required on some systems
#include "glut.h" // Always have this the furthest down of the includes

class Game 
{
	enum EntitySelection
	{
		SPACESHIP,
		STARCHASER,
		FALLENSTAR,
		TRADINGPOST,
	};

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
	const int CELL_SIZE = 20;
	Grid mGrid;
	Spaceship mShip;
	TradingPost mTradingPost;
	FallenStar mFallenStar;
	std::shared_ptr<StarChaser> mStarChaser;

	int Random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}

private:
	bool mSimulationEnd = false;	
	bool mActiveSpaceShip = true;
	bool mActiveFallenStar = true;
	bool mActiveStarChaser = true;
	bool mActiveTradingPost = true;	
	const int GRID_ALIGNMENT = 1;
	int mSelection;
	int mOldTimeSinceStart = 0;	
	int mRandPosX = 0;
	int mRandPosY = 0;	
};

