#include "Healer.h"
std::shared_ptr<Monster> mxNearestMonster = NULL;
Healer::Healer(int p_x, int p_y, Game* p_game, int p_id, int p_size)
{
	mHealerX = p_x;
	mHealerY = p_y;
	mCellSize = p_size;
	mId = p_id;

	// Game ref
	mxGame = p_game;

	// Tree
	mxBehaviorTree = std::make_shared<BehaviorTree>();

	// Order by which nodes are added: Bottom->Right->Left->Up
	std::shared_ptr<Selector> selector1 = std::make_shared<Selector>();
	selector1->AddChild(std::make_shared<HealerReposition>(mxBehaviorTree->getBlackboard(),5));
	selector1->AddChild(std::make_shared<HealerHeal>(mxBehaviorTree->getBlackboard()));

	std::shared_ptr<Selector> selector2 = std::make_shared<Selector>();
	selector2->AddChild(std::make_shared<HealerReposition>(mxBehaviorTree->getBlackboard(),1));
	selector2->AddChild(std::make_shared<HealerAttack>(mxBehaviorTree->getBlackboard()));

	std::shared_ptr<Selector> selector3 = std::make_shared<Selector>();
	selector3->AddChild(selector1);
	selector3->AddChild(selector2);

	mxBehaviorTree->setRoot(selector3);

	mxBehaviorTree->getBlackboard()->Add("CellSize", mCellSize);
}

Healer::~Healer()
{
}

void Healer::Update(float p_deltatime)
{
	if (mHP <= 0)
	{
		mHealerAlive = false;
		mxGame->GetSharedBlackboard()->Add("HealerAlive", false);
	}
	
	if (mHealerAlive)
	{
		mxGame->GetSharedBlackboard()->Add("HealerPosX", mHealerX);
		mxGame->GetSharedBlackboard()->Add("HealerPosY", mHealerY);

		mxBehaviorTree->getBlackboard()->Add("X", mHealerX);
		mxBehaviorTree->getBlackboard()->Add("Y", mHealerY);

		mxBehaviorTree->getBlackboard()->Add("MoveTargetX", mxGame->GetSharedBlackboard()->Getint("TankPosX"));
		mxBehaviorTree->getBlackboard()->Add("MoveTargetY", mxGame->GetSharedBlackboard()->Getint("TankPosY"));

		mxGame->GetSharedBlackboard()->Add("HealerHP", mHP);
		mxGame->GetSharedBlackboard()->Add("HealerID", mId);
		mxBehaviorTree->getBlackboard()->Add("TankHP", mxGame->GetSharedBlackboard()->Getint("TankHP"));

		mxGame->GetSharedBlackboard()->Add("HealerThreatGeneration", mxBehaviorTree->getBlackboard()->Getint("HealerThreatGeneration"));

		mxGame->GetSharedBlackboard()->Add("TankHP", mxBehaviorTree->getBlackboard()->Getint("TankHP"));
		mxGame->GetSharedBlackboard()->Add("HealerAlive", true);
		
		if (mxNearestMonster == NULL)
		{
			mxNearestMonster = mxGame->GetMonstersVector()[0];
		}

		for (int i = 0; i < mxGame->GetMonstersVector().size(); i++)
		{

			if (euclideandistance(mxNearestMonster->GetX(), mxNearestMonster->GetY(), mHealerX, mHealerY) >= euclideandistance(mxGame->GetMonstersVector()[i]->GetX(), mxGame->GetMonstersVector()[i]->GetY(), mHealerX, mHealerY) && mxGame->GetMonstersVector()[i]->GetAlive())
			{
				mxNearestMonster = mxGame->GetMonstersVector()[i];
			}
		}

		mxBehaviorTree->getBlackboard()->Add("AttackTargetX", mxNearestMonster->GetX());
		mxBehaviorTree->getBlackboard()->Add("AttackTargetY", mxNearestMonster->GetY());
	
		mxBehaviorTree->update();

		mHealerX = mxBehaviorTree->getBlackboard()->Getint("X");
		mHealerY = mxBehaviorTree->getBlackboard()->Getint("Y");
	}

	mxGame->GetSharedBlackboard()->Add(mId, -1);
	mxGame->GetSharedBlackboard()->Add("Healed", mxBehaviorTree->getBlackboard()->Getbool("Healed"));

	if (mxBehaviorTree->getBlackboard()->Getbool("Attacked") == true)
	{
		mxGame->GetSharedBlackboard()->Add(mId, mxNearestMonster->GetID());
	}
	
	// Take damage from attacking monsters
	if (mxGame->GetSharedBlackboard()->Getbool("AttackHealer"))
	{
		if (mxGame->GetSharedBlackboard()->Getbool("IsGuarded") == true)
		{
			mHP -= mxGame->GetSharedBlackboard()->Getint("AttackingMonsters") / 2;
			std::cout << "Healer taking Damage:	" << mxGame->GetSharedBlackboard()->Getint("AttackingMonsters") << "   :   " << mHP << std::endl;
		}
		else
		{
			mHP -= mxGame->GetSharedBlackboard()->Getint("AttackingMonsters");
			std::cout << "Healer taking Damage:	" << mxGame->GetSharedBlackboard()->Getint("AttackingMonsters") << "   :   " << mHP << std::endl;
		}
		
	}

	if (mxNearestMonster != NULL)
	{
		mxNearestMonster = NULL;
	}
}

int Healer::GetX()
{
	return mHealerX;
}

int Healer::GetY()
{
	return mHealerY;
}

int Healer::GetID()
{
	return mId;
}

void Healer::SetX(int p_value)
{
	mHealerX = p_value;
}

void Healer::SetY(int p_value)
{

	mHealerY = p_value;
}

void Healer::Draw()
{
	if (mHealerAlive)
	{
		const int VISUAL_OFFSET = 5;
		double halfside = (mCellSize / 2);

		glPushMatrix();
		glTranslatef(mHealerX - VISUAL_OFFSET, mHealerY - VISUAL_OFFSET, 0);

		// color selection
		glColor4f(0.2f, 0.8f, 0.1f, 1.0f); // Green

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

void Healer::SetID(int p_id)
{
	mxBehaviorTree->getBlackboard()->Add("ID", p_id);
}
