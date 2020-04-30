#pragma once

//---------------------------------------------------------------------
#include "Grid.h"
#include "Grass.h"
#include "Sheep.h"
#include "Wolf.h"
#include <Math.h>
#include <stdio.h>
#include <random>
#include <stdarg.h> //Required on some systems
#include "glut.h" // Always have this the furthest down of the includes
//---------------------------------------------------------------------
class Grass;
class Sheep;
class Wolf;
class Game {
private:
	int oldTimeSinceStart = 0;	
	int random(int p_iMin, int p_iMax)
	{
		static std::random_device rd;
		static std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(p_iMin, p_iMax);
		return distr(eng);
	}
	const int SIZE = 20;
	const int GRASS_FACTOR = 20;	
	std::vector<std::shared_ptr<Grass>> Grasses;
	std::vector<std::shared_ptr<Sheep>> Sheepes;
	std::vector<std::shared_ptr<Wolf>> Wolveses;
	void CreateVisual(double arg_halfside);	
	void CreateAgents(std::string arg_type);
	void DrawAgents(std::string arg_type);
public:
    Game(void);
    ~Game(void);
    //-------------------------------------------------------
    //-------------------------------------------------------
    void  ChangeSize(int w, int h);
    void  Draw();
	void  Update();
    //-------------------------------------------------------
    void  NormalKeys(unsigned char key, int x, int y);
    void  SpecialKeys(int key, int x, int y);
    void  Mouse(int button, int state, int x, int y);
    //-------------------------------------------------------
    void  Gfx_Text(float x, float y, const char *fmt, ...);//to print text
    //-------------------------------------------------------
    float mW, mH;
    int mMouseX, mMouseY;
    int mMouseButton, mMouseState, mGlutGetModifiers;
    //-------------------------------------------------------
	Grid grid_;
	void AddAgent(std::string arg_type, int arg_x, int arg_y);
	void RemoveAgent(std::string arg_type, int arg_x, int arg_y);
	std::vector<std::shared_ptr<Sheep>> getSheep();
	std::vector<std::shared_ptr<Wolf>> getWolves();
	std::vector<std::shared_ptr<Grass>> getGrass();
    //-------------------------------------------------------
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
