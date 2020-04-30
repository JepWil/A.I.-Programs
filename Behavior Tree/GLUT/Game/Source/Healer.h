#pragma once
#include "BehaviorTree.h"
#include "Game.h"
#include <iostream>
#include "glut.h" // Always have this the furthest down of the includes

class Game;

class Healer
{
public:
	Healer(int p_x, int p_y, Game* p_game, int p_id, int p_size);
	~Healer();
	void Update(float p_deltatime);
	int GetX();
	int GetY();
	int GetID();
	void SetX(int p_value);
	void SetY(int p_value);
	void Draw();
	void SetID(int p_id);
private:
	Game* mxGame;
	std::shared_ptr<BehaviorTree> mxBehaviorTree;
	int mId = 0;
	int mHealerX = 0;
	int mHealerY = 0;
	int mCellSize = 0;
	int mHP = 100;
	bool mHealerAlive = true;
};

// Actual Nodes:
class HealerReposition : public Leaf {
public:
	~HealerReposition() override {}
	HealerReposition(std::shared_ptr<Blackboard> p_Blackboard, int range) : mxBlackboard(p_Blackboard) { HealerRange = range; }
	int MoveCounter = 0;
	int HealerRange = 5;
	Status Update() override
	{
		if (MoveCounter >= 25)
		{
			MoveCounter = 0;
			if ((mxBlackboard->Getint("MoveTargetX") + mxBlackboard->Getint("CellSize") * HealerRange) < mxBlackboard->Getint("X"))
			{
				mxBlackboard->Add("X", mxBlackboard->Getint("X") - mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Target Down
			else if ((mxBlackboard->Getint("MoveTargetY") + (mxBlackboard->Getint("CellSize") * HealerRange)) < mxBlackboard->Getint("Y"))
			{
				mxBlackboard->Add("Y", mxBlackboard->Getint("Y") - mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Left
			else if ((mxBlackboard->Getint("MoveTargetX") - (mxBlackboard->Getint("CellSize") * HealerRange)) > mxBlackboard->Getint("X"))
			{
				mxBlackboard->Add("X", mxBlackboard->Getint("X") + mxBlackboard->Getint("CellSize"));
				return Status::SUCCESS;
			}
			// Check Target 
			else if ((mxBlackboard->Getint("MoveTargetY") - (mxBlackboard->Getint("CellSize") * HealerRange)) > mxBlackboard->Getint("Y"))
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

class HealerHeal : public Leaf {
public:
	~HealerHeal() override {}
	HealerHeal(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int HealCount = 0;
	Status Update() override
	{
		if (mxBlackboard->Getint("TankHP") <= 100 && HealCount > 200)
		{
			mxBlackboard->Add("Healed", true);
			HealCount = 0;
			std::cout << "Tank healed!" << std::endl;
			return Status::SUCCESS;
		}
		HealCount++;
		mxBlackboard->Add("Healed", false);
		return Status::FAIL;
	}

protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};

class HealerAttack : public Leaf {
public:
	~HealerAttack() override {}
	HealerAttack(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int AttackCount = 0;
	//int AttackRange = 5;
	Status Update() override
	{
		if (AttackCount > 300 && euclideandistance(mxBlackboard->Getint("X"), mxBlackboard->Getint("Y"), mxBlackboard->Getint("AttackTargetX"), mxBlackboard->Getint("AttackTargetY")) < 19)
		{
			mxBlackboard->Add("Attacked", true);
			AttackCount = 0;
			std::cout << "Healer Attacked!" << std::endl;
			return Status::SUCCESS;
		}
		AttackCount++;
		mxBlackboard->Add("Attacked", false);
		return Status::FAIL;
	}
protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};