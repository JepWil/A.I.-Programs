#pragma once

#include "BehaviorTree.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include "glut.h" // Always have this the furthest down of the includes

class Game;

class Monster
{
public:
	Monster(int p_x, int p_y, Game* p_game, int p_id, int p_size);
	~Monster();
	void Update(float p_deltatime);
	int GetX();
	int GetY();
	int GetID();
	void SetX(int p_value);
	void SetY(int p_value);
	void SetID(int p_id);
	void Draw();
	std::shared_ptr<BehaviorTree> mxBehaviorTree;
	bool GetAlive();
private:
	Game* mxGame;
	bool mIsAlive = true;
	int mId = 0;
	int mMonsterX = 0;
	int mMonsterY = 0;
	int mCellSize = 0;
	float TankThreat = 0;
	float HealerThreat = 0;
	int mHP = 100;
};


// Actual Nodes:
class Move : public Leaf {
public:
	~Move() override {}
	Move(std::shared_ptr<Blackboard> p_Blackboard, Game* game) : mxBlackboard(p_Blackboard) { mxGame = game; }
	Game* mxGame = NULL;
	int MoveCounter = 0;
	Status Update() override
	{
		if (MoveCounter >= 25)
		{
			MoveCounter = 0;
			if ((mxBlackboard->Getint("TargetX") + mxBlackboard->Getint("CellSize")) < mxBlackboard->Getint("X"))
			{
				mxBlackboard->Add("X", mxBlackboard->Getint("X") - mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Target Down
			else if ((mxBlackboard->Getint("TargetY") + mxBlackboard->Getint("CellSize")) < mxBlackboard->Getint("Y"))
			{
				mxBlackboard->Add("Y", mxBlackboard->Getint("Y") - mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Left
			else if ((mxBlackboard->Getint("TargetX") - mxBlackboard->Getint("CellSize")) > mxBlackboard->Getint("X"))
			{
				mxBlackboard->Add("X", mxBlackboard->Getint("X") + mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Target 
			else if ((mxBlackboard->Getint("TargetY") - mxBlackboard->Getint("CellSize")) > mxBlackboard->Getint("Y"))
			{
				mxBlackboard->Add("Y", mxBlackboard->Getint("Y") + mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
		}
		MoveCounter++;
		return Status::FAIL;
	}
protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};

class Attack : public Leaf {
public:
	~Attack() override {}
	Attack(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int AttackCounter = 0;
	Status Update() override
	{
		if (euclideandistance(mxBlackboard->Getint("X"), mxBlackboard->Getint("Y"), mxBlackboard->Getint("TargetX"), mxBlackboard->Getint("TargetY")) < 19 && AttackCounter > 50)
		{
			mxBlackboard->Add("AttackingMonsters", 1);
			mxBlackboard->Add("AttackTarget", true);
			AttackCounter = 0;
			return Status::SUCCESS;
		}
		mxBlackboard->Add("AttackingMonsters", 0);
		mxBlackboard->Add("AttackTarget", false);
		AttackCounter++;
		return Status::FAIL;
	}

protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};