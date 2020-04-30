#include "StarChaser.h"

StarChaser::StarChaser(Game* arg_game)
{
	mGame = arg_game;
}

StarChaser::~StarChaser()
{
	mGame = NULL;
}

void StarChaser::Update(float arg_deltatime)
{
	timer += arg_deltatime;

	switch (mCurrentState)
	{
	case GATHERING:
		Move();
		break;

	case TRADING:
		Move();
		break;

	case RESTING:
		Move();
		break;
	};

	if (mCurrentState != RESTING && mFatigue >= MAXFATIGUE)
	{
		mPreviousState = mCurrentState;
		mCurrentState = RESTING;
		mAStarPath->clear();
		mMoving = false;
	}
}

void StarChaser::Move()
{
	if (mCurrentState == GATHERING && !mMoving && !mHasStar && mFatigue != MAXFATIGUE)
	{
		CreatePath("Fallen Star");
		mAStarPath->pop_back();

		mMoving = true;
	}
	else if (mCurrentState == TRADING && mHasStar && !mMoving && mFatigue != MAXFATIGUE)
	{
		CreatePath("Trading Post");
		mAStarPath->pop_back();

		mMoving = true;
	}
	else if (mCurrentState == RESTING && !mMoving && mFatigue > 0)
	{
		CreatePath("Spaceship");
		mAStarPath->pop_back();

		mMoving = true;
		index = 1;
	}

	if (timer >= 0.25f && (int)mAStar.mAStarParentNodes.size() > index && mMoving)
	{
		SetPos(mAStar.mAStarParentNodes[index]->mCell->mCellX, mAStar.mAStarParentNodes[index]->mCell->mCellY);

		std::cout << mAStarPath->size() << std::endl;
		if (mCurrentState == GATHERING && mAStarPath->size() == 1)
		{
			mHasStar = true;
			mMoving = false;
			index = 0;
			mAStarPath->pop_back();
			int mRandPosY = mGame->Random(1, mGame->mGrid.GetFreeCells().size() - 1);
			int mRandPosX = mGame->Random(1, mGame->mGrid.GetFreeCells()[mRandPosY].size() - 1);
			mGame->mFallenStar.SetPos(mGame->mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGame->mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY);
			mCurrentState = TRADING;
		}
		else if (mCurrentState == TRADING && mAStarPath->size() == 1)
		{
			mHasStar = false;
			mMoving = false;
			index = 0;
			mAStarPath->pop_back();
			mCurrentState = GATHERING;
		}
		else if (mCurrentState == RESTING && mAStarPath->size() == 1)
		{
			mMoving = false;
			index = 0;
			mFatigue = 0;
			mCurrentState = mPreviousState;
		}

		//mGrid.GetCellAt(mStarChaser.GetX(), mStarChaser.GetY())->mCellType = celltype::STARCHASER;
		if (mFatigue <= 100 && mCurrentState != RESTING)
			mFatigue += FATIGUERATE;

		if (mAStarPath->size() > 0)
			mAStarPath->pop_back();

		timer = 0;
		index++;
		std::cout << "Fatigue: " << mFatigue << std::endl;
	}
}

void StarChaser::CreatePath(std::string arg_entity_destination)
{
	if (arg_entity_destination == "Fallen Star")
	{
		mAStarPath = mAStar.CreateAStarPath(std::make_shared<Grid>(mGame->mGrid), mStarChaserX, mStarChaserY, mGame->mFallenStar.GetX(), mGame->mFallenStar.GetY(), mGame->CELL_SIZE);
	}
	else if (arg_entity_destination == "Trading Post")
	{
		mAStarPath = mAStar.CreateAStarPath(std::make_shared<Grid>(mGame->mGrid), mStarChaserX, mStarChaserY, mGame->mTradingPost.GetX(), mGame->mTradingPost.GetY(), mGame->CELL_SIZE);
	}
	else if (arg_entity_destination == "Spaceship")
	{
		mAStarPath = mAStar.CreateAStarPath(std::make_shared<Grid>(mGame->mGrid), mStarChaserX, mStarChaserY, mGame->mShip.GetX(), mGame->mShip.GetY(), mGame->CELL_SIZE);
	}
}

void StarChaser::CreateStarChaser(int arg_x, int arg_y, int arg_size)
{
	mStarChaserX = arg_x;
	mStarChaserY = arg_y;
	mStarChaserSize = arg_size;
}

void StarChaser::DrawStarChaser()
{
	mAStar.DrawAStarPath();


	double halfside = mStarChaserSize / 2;
	glPushMatrix();
	glTranslatef(mStarChaserX, mStarChaserY, 0);
	// bot
	glBegin(GL_LINES);
	glVertex3f(-halfside, halfside, 0.0);
	glVertex3f(halfside, halfside, 0.0);
	glEnd();
	// top
	glBegin(GL_LINES);
	glVertex3f(-halfside, -halfside, 0.0);
	glVertex3f(halfside, -halfside, 0.0);
	glEnd();
	// left
	glBegin(GL_LINES);
	glVertex3f(-halfside, -halfside, 0.0);
	glVertex3f(-halfside, halfside, 0.0);
	glEnd();
	// right
	glBegin(GL_LINES);
	glVertex3f(halfside, -halfside, 0.0);
	glVertex3f(halfside, halfside, 0.0);
	glEnd();
	// color
	glColor4f(0.8f, 0.5f, 0.2f, 1.0f); // Orange
	// draw quad
	glBegin(GL_QUADS);
	glVertex2d(halfside, halfside);
	glVertex2d(halfside, -halfside);
	glVertex2d(-halfside, -halfside);
	glVertex2d(-halfside, halfside);
	glEnd();
	glPopMatrix();
}

int StarChaser::GetX()
{
	return mStarChaserX;
}


int StarChaser::GetY()
{
	return mStarChaserY;
}

bool StarChaser::GetHasStar()
{
	return mHasStar;
}

void StarChaser::SetPos(int arg_x, int arg_y)
{
	mStarChaserX = arg_x;
	mStarChaserY = arg_y;
} 


