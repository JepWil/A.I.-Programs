// ASSIGNMENT 3 - Behavior Arises... - BY JEPPE WILLATZEN - Student ID: 4487 - UPPSALA UNIVERSITY - SUPERVISOR: Jerry Jonsson 
// A Tank-Healer-Monster simulation

#include "Game.h"
#include <iostream>
#include <string>


Game::Game(void)
{
	mW = 1280.0f, mH = 720.0f;
	mMouseX = mMouseY = 0;
	mMouseButton = mMouseState = 0;
	mGlutGetModifiers = 0;
	mxGrid = std::make_shared<Grid>();

	// Grid
	mxGrid->CreateGrid(mW, mH, CELL_SIZE, this);	

	// Shared Blackboard (all actors)
	mxSharedBBoard = std::make_shared<Blackboard>();

	// Tank
	int mRandPosY = Random(1, mxGrid->Get_All_Cells().size());
	int mRandPosX = Random(1, mxGrid->Get_All_Cells()[mRandPosY].size() / TANK_X_SPAWN_FACTOR);
	mxTank = std::make_shared<Tank>(mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellX, mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellY, this, 0, CELL_SIZE);
	std::cout << mxTank->GetID() << std::endl;

	// Healer
	mRandPosY = Random(1, mxGrid->Get_All_Cells().size());
	mRandPosX = Random(1, mxGrid->Get_All_Cells()[mRandPosY].size() / HEALER_X_SPAWN_FACTOR);
	mxHealer = std::make_shared<Healer>(mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellX, mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellY, this, 1, CELL_SIZE);
	std::cout << mxHealer->GetID() << std::endl;

	// God Monster
	mRandPosY = Random(1, mxGrid->Get_All_Cells().size() - 1);
	mRandPosX = Random(mxGrid->Get_All_Cells()[mRandPosY].size() / MONSTER_X_SPAWN_FACTOR, (mxGrid->Get_All_Cells()[mRandPosY].size() - 1));
	auto xMonster = std::make_shared<Monster>(mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellX, mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellY, this, mxHealer->GetID() + 1, CELL_SIZE);
	std::cout << xMonster->GetID() << std::endl;
	mMonsters.push_back(xMonster);

	//// Minion Monsters
	for (int i = 1; i <= MAX_MONSTERS; i++)
	{
		mRandPosY = Random(1, mxGrid->Get_All_Cells().size() - 1);
		mRandPosX = Random(mxGrid->Get_All_Cells()[mRandPosY].size() / MONSTER_X_SPAWN_FACTOR, (mxGrid->Get_All_Cells()[mRandPosY].size() - 1));
		auto xMonster = std::make_shared<Monster>(mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellX, mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellY, this, mMonsters[i - 1]->GetID() + 1, CELL_SIZE);
		mMonsters.push_back(xMonster);
	}
}

Game::~Game(void){}


void Game::ChangeSize(int w, int h)
{
	glViewport(0, 0, mW, mH); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	glOrtho(0.0f, mW, mH, 0.0f, -1.0f, 1.0f); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

void Game::Draw(void)
{ 
	// Render stuff here
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw Objects
	mxGrid->DrawCell(mW / CELL_SIZE, mH / CELL_SIZE);	
	for (std::shared_ptr<Monster> m : mMonsters)
	{
		if (m != NULL)
		{
			m->Draw();
		}
	}
	mxTank->Draw();
	mxHealer->Draw();

	glutSwapBuffers();
}

void Game::Update()
{
	for (int i = 0; i < mMonsters.size(); i++)
	{
		if (mMonsters[i]->GetAlive() == false)
		{
			//*((int*)0) = 1;

			std::shared_ptr<Monster> tmp = mMonsters[i];
			mMonsters[i] = mMonsters[mMonsters.size() - 1];
			mMonsters[mMonsters.size() - 1] = tmp;
			mMonsters.pop_back();
		}
	}


	// Update stuff here
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = ((float)timeSinceStart - (float)mOldTimeSinceStart) / 1000;
	MonsterSpawnTimer += deltaTime;

	mxGrid->UpdateCellTypes(mxTank->GetX(), mxTank->GetY(), Cell::celltype::EMPTY);
	mxTank->Update(deltaTime);
	mxGrid->UpdateCellTypes(mxTank->GetX(), mxTank->GetY(), Cell::celltype::TANK);

	mxGrid->UpdateCellTypes(mxHealer->GetX(), mxHealer->GetY(), Cell::celltype::EMPTY);
	mxHealer->Update(deltaTime);
	mxGrid->UpdateCellTypes(mxHealer->GetX(), mxHealer->GetY(), Cell::celltype::HEALER);

	mxSharedBBoard->Add("AttackingMonsters", 0);

	mxSharedBBoard->Add("AttackTank", false);
	mxSharedBBoard->Add("AttackHealer", false);

	for (size_t i = 0; i < mMonsters.size(); i++)
	{
		if (mMonsters[i] != NULL) 
		{
			mxGrid->UpdateCellTypes(mMonsters[i]->GetX(), mMonsters[i]->GetY(), Cell::celltype::EMPTY);
			mMonsters[i]->Update(deltaTime);
			mxGrid->UpdateCellTypes(mMonsters[i]->GetX(), mMonsters[i]->GetY(), Cell::celltype::MONSTER);
		}
	}

	if (MonsterSpawnTimer >= 5)
	{
		int mRandPosY = Random(1, mxGrid->Get_All_Cells().size() - 1);
		int mRandPosX = Random(mxGrid->Get_All_Cells()[mRandPosY].size() / MONSTER_X_SPAWN_FACTOR, (mxGrid->Get_All_Cells()[mRandPosY].size() - 1));
		auto xMonster = std::make_shared<Monster>(mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellX, mxGrid->Get_All_Cells()[mRandPosY][mRandPosX]->mCellY, this, mMonsters[mMonsters.size()-1]->GetID() + 1, CELL_SIZE);
		mMonsters.push_back(xMonster);
		MonsterSpawnTimer = 0;
	}

	//std::cout << mxSharedBBoard->Getint("AttackingMonsters") << std::endl;
	mOldTimeSinceStart = timeSinceStart;
}



void Game::NormalKeys(unsigned char key, int x, int y)
{
}

void Game::SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) 
	{
	}
}

void Game::Mouse(int button, int state, int x, int y)
{
	mMouseButton = button;
	mMouseState = state;
	mGlutGetModifiers = glutGetModifiers();
	mMouseX = x; mMouseY = y;
}

void Game::Gfx_Text(float x, float y, const char *fmt, ...)
{
	char text[256]; va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt); vsprintf_s(text, fmt, ap); va_end(ap);
	glPushMatrix(); glLoadIdentity();
    glRasterPos2f(x,y);
    for (char *c=text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	glPopMatrix();
}

std::vector<std::shared_ptr<Monster>> Game::GetMonstersVector()
{
	return mMonsters;
}

std::shared_ptr<Blackboard> Game::GetSharedBlackboard()
{
	return mxSharedBBoard;
}

std::shared_ptr<Grid> Game::GetGrid()
{
	return mxGrid;
}



