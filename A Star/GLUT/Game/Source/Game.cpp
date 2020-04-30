// ASSIGNMENT 2 - When You Wish Upon A* - BY JEPPE WILLATZEN - Student ID: 4478 - UPPSALA UNIVERSITY - SUPERVISOR: Jerry Jonsson 
// See readme.txt and changelog.txt for instructions and notes!

#include "Game.h"
#include <iostream>
#include <string>

Game::Game(void)
{
	mW = 1280.0f, mH = 720.0f;
	mMouseX = mMouseY = 0;
	mMouseButton = mMouseState = 0;
	mGlutGetModifiers = 0;

	// Grid
	mGrid.CreateGrid(mW, mH, CELL_SIZE);
	// Spaceship
	mRandPosY = Random(1, mGrid.GetFreeCells().size() - 1);
	mRandPosX = Random(1, mGrid.GetFreeCells()[mRandPosY].size() - 1);
	mShip.CreateShip(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY, CELL_SIZE);
	mGrid.GetCellAt(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY)->mCellType = celltype::SPACESHIP;
	// Trading Post
	mRandPosY = Random(1, mGrid.GetFreeCells().size() - 1);
	mRandPosX = Random(1, mGrid.GetFreeCells()[mRandPosY].size() - 1);
	mTradingPost.CreateTradingPost(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY, CELL_SIZE);
	mGrid.GetCellAt(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY)->mCellType = celltype::TRADINGPOST;
	// Fallen Star
	mRandPosY = Random(1, mGrid.GetFreeCells().size() - 1);
	mRandPosX = Random(1, mGrid.GetFreeCells()[mRandPosY].size() - 1);
	mFallenStar.CreateFallenStar(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY, CELL_SIZE);
	mGrid.GetCellAt(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY)->mCellType = celltype::FALLENSTAR;
	// Starchaser
	mRandPosY = Random(1, mGrid.GetFreeCells().size() - 1);
	mRandPosX = Random(1, mGrid.GetFreeCells()[mRandPosY].size() - 1);
	mStarChaser = std::make_shared<StarChaser>(this);
	mStarChaser->CreateStarChaser(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY, CELL_SIZE);
	mGrid.GetCellAt(mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellX, mGrid.GetFreeCells()[mRandPosY][mRandPosX]->mCellY)->mCellType = celltype::STARCHASER;
}

Game::~Game(void){
}


void Game::ChangeSize(int w, int h)
{
	/*mW = w, mH = h;*/
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
	mGrid.DrawCell(mW / CELL_SIZE, mH / CELL_SIZE);	
	mStarChaser->DrawStarChaser();
	mShip.DrawShip();
	mTradingPost.DrawTradingPost();

	if (!mStarChaser->GetHasStar()) {
		mFallenStar.DrawFallenStar();
	}

	glutSwapBuffers();
}

void Game::Update()
{
	// Update stuff here
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = ((float)timeSinceStart - (float)mOldTimeSinceStart) / 1000;

	mStarChaser->Update(deltaTime);
	mOldTimeSinceStart = timeSinceStart;
}

void Game::NormalKeys(unsigned char key, int x, int y)
{
	if (key == '1')
	{
		mSimulationEnd = false;
	}
}



void Game::SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) 
	{
		mSelection += 1;
	}
	if (key == GLUT_KEY_DOWN) 
	{
		mSelection -= 1;
	}

	if (mSelection == 1)
	{
		std::cout << "StarChaser selected" << std::endl;
	}
	else if (mSelection == 2)
	{
		std::cout << "FallenStar selected" << std::endl;
	}
	else if (mSelection == 3)
	{
		std::cout << "Spaceship selected" << std::endl;
	}
	else if (mSelection == 4)
	{
		std::cout << "TradingPost selected" << std::endl;
	}
	else if (mSelection >= 5)
	{
		mSelection = 1;
		std::cout << "StarChaser selected" << std::endl;
	}
	else if (mSelection <= 0)
	{
		mSelection = 4;
		std::cout << "TradingPost selected" << std::endl;
	}
}

void Game::Mouse(int button, int state, int x, int y)
{
	mMouseButton = button;
	mMouseState = state;
	mGlutGetModifiers = glutGetModifiers();
	mMouseX = x; mMouseY = y;

	if (mMouseButton == GLUT_RIGHT_BUTTON && mMouseState == GLUT_DOWN)
	{

		mGrid.pressGridBlocked(mMouseX, mMouseY);
	}

	if (mMouseButton == GLUT_LEFT_BUTTON && mMouseState == GLUT_DOWN)
	{
		mGrid.pressGridEntities(mMouseX, mMouseY, mSelection);

		if (mSelection == 1) {
			mStarChaser->SetPos(mGrid.GetCellAt(mMouseX, mMouseY)->mCellX, mGrid.GetCellAt(mMouseX, mMouseY)->mCellY);
		}
		if (mSelection == 2) {
			mFallenStar.SetPos(mGrid.GetCellAt(mMouseX, mMouseY)->mCellX, mGrid.GetCellAt(mMouseX, mMouseY)->mCellY);
		}
		if (mSelection == 3) {
			mShip.SetPos(mGrid.GetCellAt(mMouseX, mMouseY)->mCellX, mGrid.GetCellAt(mMouseX, mMouseY)->mCellY);
		}
		if (mSelection == 4) {
			mTradingPost.SetPos(mGrid.GetCellAt(mMouseX, mMouseY)->mCellX, mGrid.GetCellAt(mMouseX, mMouseY)->mCellY);
		}
	}
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



