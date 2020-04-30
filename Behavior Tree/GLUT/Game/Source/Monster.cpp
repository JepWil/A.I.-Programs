#include "Monster.h"
#include <string>

Monster::Monster(int p_x, int p_y, Game* p_game, int p_id, int p_size)
{
	mMonsterX = p_x;
	mMonsterY = p_y;
	mCellSize = p_size;
	mId = p_id;

	// Game ref
	mxGame = p_game;

	// Tree
	mxBehaviorTree = std::make_shared<BehaviorTree>();

	// Order by which nodes are added: Bottom->Right->Left->Up
	std::shared_ptr< Sequence> selector2 = std::make_shared<Sequence>();
	std::shared_ptr<Inverter> inv = std::make_shared<Inverter>();
	inv->SetChild(std::make_shared<Move>(mxBehaviorTree->getBlackboard(), mxGame));
	selector2->AddChild(inv);
	selector2->AddChild(std::make_shared<Attack>(mxBehaviorTree->getBlackboard()));

	mxBehaviorTree->setRoot(selector2);
	mxBehaviorTree->getBlackboard()->Add("CellSize", mCellSize);
}

Monster::~Monster()
{

}

void Monster::Update(float p_deltatime)
{
	if (mHP <= 0)
	{
		mIsAlive = false;
	}

	if (mIsAlive)
	{
		mxBehaviorTree->getBlackboard()->DamagingMonsters = 0;
		float TankDist = euclideandistance(mxGame->GetSharedBlackboard()->Getint("TankPosX"), mxGame->GetSharedBlackboard()->Getint("TankPosY"), mMonsterX, mMonsterY);
		float HealerDist = euclideandistance(mxGame->GetSharedBlackboard()->Getint("HealerPosX"), mxGame->GetSharedBlackboard()->Getint("HealerPosY"), mMonsterX, mMonsterY);
		TankThreat = (1000 / TankDist) + mxGame->GetSharedBlackboard()->Getfloat("TankThreatGeneration");
		HealerThreat = (1000 / HealerDist) + mxGame->GetSharedBlackboard()->Getfloat("HealerThreatGeneration");
	
		if (!mxGame->GetSharedBlackboard()->Getbool("HealerAlive"))
		{
			HealerThreat = 0;
		}
		else if (!mxGame->GetSharedBlackboard()->Getbool("TankAlive"))
		{
			TankThreat = 0;
		}

		if (HealerThreat > TankThreat)
		{
			mxBehaviorTree->getBlackboard()->Add("TargetX", mxGame->GetSharedBlackboard()->Getint("HealerPosX"));
			mxBehaviorTree->getBlackboard()->Add("TargetY", mxGame->GetSharedBlackboard()->Getint("HealerPosY"));
			mxBehaviorTree->getBlackboard()->Add(mId, mxGame->GetSharedBlackboard()->Getint("HealerID"));

		}

		else
		{
			mxBehaviorTree->getBlackboard()->Add("TargetX", mxGame->GetSharedBlackboard()->Getint("TankPosX"));
			mxBehaviorTree->getBlackboard()->Add("TargetY", mxGame->GetSharedBlackboard()->Getint("TankPosY"));
			mxBehaviorTree->getBlackboard()->Add(mId, mxGame->GetSharedBlackboard()->Getint("TankID"));
		}

		mxBehaviorTree->getBlackboard()->Add("X", mMonsterX);
		mxBehaviorTree->getBlackboard()->Add("Y", mMonsterY);
		mxBehaviorTree->getBlackboard()->Add("HP", mHP);

		mxGame->GetSharedBlackboard();

		mxBehaviorTree->update();

		mMonsterX = mxBehaviorTree->getBlackboard()->Getint("X");
		mMonsterY = mxBehaviorTree->getBlackboard()->Getint("Y");
		mHP = mxBehaviorTree->getBlackboard()->Getint("HP");

		mxGame->GetSharedBlackboard()->Add("AttackingMonsters", mxBehaviorTree->getBlackboard()->Getint("AttackingMonsters") + mxGame->GetSharedBlackboard()->Getint("AttackingMonsters"));

		if (mxBehaviorTree->getBlackboard()->GetAttackBBoard(mId) == mxGame->GetSharedBlackboard()->Getint("HealerID") && mxBehaviorTree->getBlackboard()->Getbool("AttackTarget"))
		{
			mxGame->GetSharedBlackboard()->Add("AttackHealer", mxBehaviorTree->getBlackboard()->Getbool("AttackTarget"));
		}
		if (mxBehaviorTree->getBlackboard()->GetAttackBBoard(mId) == mxGame->GetSharedBlackboard()->Getint("TankID") && mxBehaviorTree->getBlackboard()->Getbool("AttackTarget"))
		{
			mxGame->GetSharedBlackboard()->Add("AttackTank", mxBehaviorTree->getBlackboard()->Getbool("AttackTarget"));
		}

		// Take damage from attacking Healer / Tank
		if (mxGame->GetSharedBlackboard()->GetAttackBBoard(mxGame->GetSharedBlackboard()->Getint("HealerID")) == mId)
		{
			mHP -= 25;
			std::cout << "Monster taking Damage:	" << mHP << std::endl;
		}
		else if (mxGame->GetSharedBlackboard()->GetAttackBBoard(mxGame->GetSharedBlackboard()->Getint("TankID")) == mId)
		{
			mHP -= 50;
			std::cout << "Monster taking Damage:	" << mHP << std::endl;
		}
	}
}

int Monster::GetX()
{
	return mMonsterX;
}

int Monster::GetY()
{
	return mMonsterY;
}

int Monster::GetID()
{
	return mId;
}

void Monster::SetX(int p_value)
{
	mMonsterX = p_value;
}

void Monster::SetY(int p_value)
{
	mMonsterY = p_value;
}

void Monster::SetID(int p_id)
{
	mxBehaviorTree->getBlackboard()->Add("ID", p_id);
	mId = p_id;
}

void Monster::Draw()
{
	if (true)
	{
		const int VISUAL_OFFSET = 5;
		double halfside = (mCellSize / 2);

		glPushMatrix();
		glTranslatef(mMonsterX - VISUAL_OFFSET, mMonsterY - VISUAL_OFFSET, 0);

		// color selection
		glColor4f(0.8f, 0.2f, 0.1f, 1.0f); // Red

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

bool Monster::GetAlive()
{
	return mIsAlive;
}
