#include "Tank.h"

Tank::Tank(int p_x, int p_y, Game* p_game, int p_id, int p_size)
{
	mTankX = p_x;
	mTankY = p_y;
	mCellSize = p_size;
	mId = p_id;

	// Game ref
	mxGame = p_game;

	// Tree
	mxBehaviorTree = std::make_shared<BehaviorTree>();

	std::shared_ptr<Selector> selector3 = std::make_shared<Selector>();
	selector3->AddChild(std::make_shared<TankMove>(mxBehaviorTree->getBlackboard()));
	selector3->AddChild(std::make_shared<TankAttack>(mxBehaviorTree->getBlackboard()));

	// Order by which nodes are added: Bottom->Right->Left->Up
	std::shared_ptr<Selector> selector2 = std::make_shared<Selector>();
	selector2->AddChild(std::make_shared<TankMove>(mxBehaviorTree->getBlackboard()));
	selector2->AddChild(std::make_shared<TankTaunt>(mxBehaviorTree->getBlackboard()));

	std::shared_ptr<Selector> selector = std::make_shared<Selector>();
	selector->AddChild(std::make_shared<TankMove>(mxBehaviorTree->getBlackboard()));
	selector->AddChild(std::make_shared<TankGuard>(mxBehaviorTree->getBlackboard()));

	std::shared_ptr<Selector> sq = std::make_shared<Selector>();
	sq->AddChild(selector3);
	sq->AddChild(selector);
	sq->AddChild(selector2);

	mxBehaviorTree->setRoot(sq);

	mxGame->GetSharedBlackboard()->Add("TankHP", mHP);
	mxBehaviorTree->getBlackboard()->Add("CellSize", mxGame->CELL_SIZE);
}

Tank::~Tank()
{
}

void Tank::Update(float p_deltatime)
{
	if (mHP <= 0)
	{
		TankAlive = false;
		mxGame->GetSharedBlackboard()->Add("TankAlive", false);
	}

	if (TankAlive)
	{
		mxGame->GetSharedBlackboard()->Add("TankPosX", mTankX);
		mxGame->GetSharedBlackboard()->Add("TankPosY", mTankY);

		mxBehaviorTree->getBlackboard()->Add("X", mTankX);
		mxBehaviorTree->getBlackboard()->Add("Y", mTankY);
		mxBehaviorTree->getBlackboard()->Add("IsTaunting", false);

		mxGame->GetSharedBlackboard()->Add("TankHP", mHP);
		mxGame->GetSharedBlackboard()->Add("TankID", mId);

		if (mxNearestMonster == NULL)
		{
			mxNearestMonster = mxGame->GetMonstersVector()[0];
		}

		for (int i = 0; i < mxGame->GetMonstersVector().size(); i++)
		{
			if (euclideandistance(mxNearestMonster->GetX(), mxNearestMonster->GetY(), mxBehaviorTree->getBlackboard()->Getint("X"), mxBehaviorTree->getBlackboard()->Getint("Y")) >= euclideandistance(mxGame->GetMonstersVector()[i]->GetX(), mxGame->GetMonstersVector()[i]->GetY(), mxBehaviorTree->getBlackboard()->Getint("X"), mxBehaviorTree->getBlackboard()->Getint("Y")))
			{
				mxNearestMonster = mxGame->GetMonstersVector()[i];
			}
		}
		
		mxBehaviorTree->getBlackboard()->Add("TankTarget", mxNearestMonster->GetID());
		mxBehaviorTree->getBlackboard()->Add("TargetX", mxNearestMonster->GetX());
		mxBehaviorTree->getBlackboard()->Add("TargetY", mxNearestMonster->GetY());

		mxBehaviorTree->update();

		mxGame->GetSharedBlackboard()->Add(mId, -1);
		mTankX = mxBehaviorTree->getBlackboard()->Getint("X");
		mTankY = mxBehaviorTree->getBlackboard()->Getint("Y");

		mxGame->GetSharedBlackboard()->Add("IsGuarding", mxBehaviorTree->getBlackboard()->Getbool("IsGuarding"));
		mxGame->GetSharedBlackboard()->Add("TankAlive", true);
		
		if (mxBehaviorTree->getBlackboard()->Getbool("IsTaunting") == true)
		{
			mxGame->GetSharedBlackboard()->Add("TankThreatGeneration", 100.0f);
		}
		else
		{
			mxGame->GetSharedBlackboard()->Add("TankThreatGeneration", 0.0f);
		}
	}

	if (mxBehaviorTree->getBlackboard()->Getbool("Attacked") == true)
	{
		mxGame->GetSharedBlackboard()->Add(mId, mxNearestMonster->GetID());
	}

	if (mxGame->GetSharedBlackboard()->Getbool("Healed"))
	{
		mHP += 25;
	}

	// Take damage from attacking monsters
	if (mxGame->GetSharedBlackboard()->Getbool("AttackTank"))
	{
		mHP -= mxGame->GetSharedBlackboard()->Getint("AttackingMonsters");
		std::cout << "Tank taking damage:	" << mxGame->GetSharedBlackboard()->Getint("AttackingMonsters") << "   :   " << mHP << std::endl;
	}
	if (mxNearestMonster != NULL)
	{
		mxNearestMonster = NULL;
	}
}

int Tank::GetX()
{
	return mTankX;
}

int Tank::GetY()
{
	return mTankY;
}

int Tank::GetID()
{
	return mId;
}

void Tank::SetX(int p_value)
{
	mTankX = p_value;
}

void Tank::SetY(int p_value)
{
	mTankY = p_value;
}

void Tank::SetID(int p_id)
{
	mId = p_id;
}

void Tank::Draw()
{
	if (TankAlive)
	{
		const int VISUAL_OFFSET = 5;
		double halfside = (mCellSize / 2);

		glPushMatrix();
		glTranslatef(mTankX - VISUAL_OFFSET, mTankY - VISUAL_OFFSET, 0);

		// color selection
		glColor4f(0.2f, 0.1f, 0.8f, 1.0f); // Blue

		// draw quad
		glBegin(GL_QUADS);
		glVertex2d(halfside - 1, halfside - 1.25);
		glVertex2d(halfside - 1, -halfside);
		glVertex2d(-halfside + 0.5, -halfside);
		glVertex2d(-halfside + 0.5, halfside - 1.25);
		glEnd();
		glPopMatrix();
	}
}
