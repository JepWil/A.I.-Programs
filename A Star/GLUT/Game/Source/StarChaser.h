#pragma once

class StateMachine;
class Game;

#include <stdarg.h> 
#include <iostream>
#include "AStarPath.h"
#include "Game.h"
#include "glut.h" // Always have this the furthest down of the includes

enum PlayStates
{
	GATHERING,
	TRADING,
	RESTING,
	IDLE,
};

class StarChaser
{
public:
	StarChaser(Game* arg_game);
	~StarChaser();
	void CreateStarChaser(int arg_x, int arg_y, int arg_size);
	void DrawStarChaser();
	int GetX();
	int GetY();
	bool GetHasStar();
	void SetPos(int arg_x, int arg_y);
	void Update(float arg_deltatime);
	
private:
	int mStarChaserX;
	int mStarChaserY;
	int mStarChaserSize;
	void Move();
	void CreatePath(std::string arg_entity_destination);
	float timer = 0;
	AStarPath mAStar;
	std::shared_ptr<std::vector<std::shared_ptr<AStarNode>>> mAStarPath;
	const int MAXFATIGUE = 100;
	const int FATIGUERATE = 2;
	int mFatigue = 0;
	int index = 0;
	bool mMoving = false;
	bool mHasStar = false;
	Game *mGame;
	PlayStates mCurrentState = GATHERING;
	PlayStates mPreviousState = IDLE;
};