#pragma once
#include "Game.h"
#include "BehaviorTree.h"
#include <iostream>
#include "glut.h" // Always have this the furthest down of the includes

class Game;
class Monster;
class Tank
{
public:
	Tank(int p_x, int p_y, Game* p_game, int p_id, int p_size);
	~Tank();
	void Update(float p_deltatime);
	int GetX();
	int GetY();
	int GetID();
	void SetX(int p_value);
	void SetY(int p_value);
	void SetID(int p_id);
	void Draw();
private:
	Game* mxGame = NULL;
	bool TankAlive = true;
	std::shared_ptr<BehaviorTree> mxBehaviorTree = NULL;
	int mId = 0;
	int mTankX = 0;
	int mTankY = 0;
	int mCellSize = 0;
	int mHP = 100;
	float TankThreatGeneration = 0;
	std::shared_ptr<Monster> mxNearestMonster = NULL;
};

class TankTaunt : public Leaf {
public:
	~TankTaunt() override {}
	TankTaunt(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int TauntCounter = 0;
	float TauntTime = 3.0f;
	Status Update() override
	{
		if (TauntCounter >= 300)
		{
			std::cout << "Taunting!" << std::endl;
			mxBlackboard->Add("IsTaunting", true);
			TauntCounter = 0;
			TauntTime = 3.0f;
			return Status::SUCCESS;
		}
		else if (TauntTime > 0)
		{
			TauntTime -= 0.1f;
		}
		mxBlackboard->Add("IsTaunting", false);
		TauntCounter++;
		return Status::FAIL;
	}

protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};

class TankAttack : public Leaf {
public:
	~TankAttack() override {}
	TankAttack(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int AttackCount = 0;
	Status Update() override
	{
		if (AttackCount > 100 && euclideandistance(mxBlackboard->Getint("X"), mxBlackboard->Getint("Y"), mxBlackboard->Getint("TargetX"), mxBlackboard->Getint("TargetY")) < 19)
		{
			mxBlackboard->Add("Attacked", true);
			std::cout << "Tank attacked!" << std::endl;
			AttackCount = 0;
			return Status::SUCCESS;
		}
		mxBlackboard->Add("Attacked", false);
		AttackCount++;
		return Status::FAIL;
	}

protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};

class TankGuard : public Leaf {
public:
	~TankGuard() override {}
	TankGuard(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int GuardCounter = 0;
	float GuardTime = 10.0f;
	Status Update() override
	{
		if (GuardCounter >= 150)
		{
			GuardCounter = 0;
			mxBlackboard->Add("IsGuarding", true);
			std::cout << "Guarding!" << std::endl;
			GuardTime = 10;
			return Status::SUCCESS;
		}
		if (mxBlackboard->Getbool("IsGuarding") == true && GuardTime < 0)
		{
			mxBlackboard->Add("IsGuarding", false);
			std::cout << "Not Guarding!" << std::endl;
			GuardTime = 10;
		}
		else if (GuardTime > 0)
		{
			GuardTime -= 0.1f;
		}
		GuardCounter++;
		return Status::FAIL;
	}
protected:
	std::shared_ptr<Blackboard> mxBlackboard;
};

class TankMove : public Leaf {
public:
	~TankMove() override {}
	TankMove(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}
	int MoveCounter = 0;
	Status Update() override
	{
		if (MoveCounter >= 10)
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