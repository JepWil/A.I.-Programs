// BY JEPPE WILLATZEN 930922-4716 - STUDENT ID 4487 - for A.I. PROGRAMMING ASSIGNMENT 1 - SUPERVISOR - JERRY JONSSON
//
// ECOSYSTEM 
//

#include "Game.h"
#include <iostream>

//---------------------------------------------------------------------
//---------------------------------------------------------------------

Game::Game(void)
{
	mW = 1280.0f, mH = 720.0f;
	mMouseX = mMouseY = 0;
	mMouseButton = mMouseState = 0,
	mGlutGetModifiers = 0;
	grid_.CreateGrid(mH, mW);
	CreateAgents("Grass");
	CreateAgents("Sheep");
	CreateAgents("Wolf");
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

Game::~Game(void){
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::ChangeSize(int w, int h)
{
	//mW = w, mH = h;
	glViewport(0,0,mW,mH); 
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(0.0f,mW,mH,0.0f,-1.0f,1.0f); 
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::Draw(void)
{ 
	// Render stuff here
	glClear(GL_COLOR_BUFFER_BIT);
	grid_.DrawTile(mW / SIZE, mH / SIZE);
	DrawAgents("Grass");
	DrawAgents("Sheep");
	DrawAgents("Wolf");
	glutSwapBuffers();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::Update() 
{ 
	// Update stuff here
	int timeSinceStart = glutGet( GLUT_ELAPSED_TIME );
	float deltaTime = ( (float)timeSinceStart - (float)oldTimeSinceStart) / 1000;

	// Grass
	for (size_t i = 0; i < Grasses.size(); i++)
	{
		Grasses[i]->update(deltaTime);
	}

	for (size_t i = 0; i < Sheepes.size(); i++)
	{
		Sheepes[i]->update(deltaTime, grid_.GetTilesPtr());
	}

	for (size_t i = 0; i < Wolveses.size(); i++)
	{
		Wolveses[i]->update(deltaTime, grid_.GetTilesPtr());
	}

	oldTimeSinceStart = timeSinceStart;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::CreateAgents(std::string arg_type)
{
	if (arg_type == "Grass")
	{
		int maxgrass = (mW / SIZE * mH / SIZE) / GRASS_FACTOR; // constant for maxgrass to create
		while ((int)Grasses.size() < maxgrass)
		{
			bool taken = false;
			int rand_tile_index = random(1, grid_.GetTilesPtr()->size() - 1);

			for each (std::shared_ptr<Grass> g in Grasses)
			{
				if ((grid_.GetTilesPtr()->at(rand_tile_index)->x) == g->getx() && grid_.GetTilesPtr()->at(rand_tile_index)->y == g->gety())
				{
					taken = true;
					break;
				}
			}

			if (!taken)
			{
				std::shared_ptr<Grass> grass = std::make_shared<Grass>(this, grid_.GetTilesPtr()->at(rand_tile_index)->x, grid_.GetTilesPtr()->at(rand_tile_index)->y, SIZE);
				Grasses.push_back(grass);
				grid_.GetTilesPtr()->at(rand_tile_index)->type = Tile_Type::GRASS;	
			}
		}
	}

	else if (arg_type == "Sheep")
	{
		const int MAX_SHEEP = 10; // half a score
		while ((int)Sheepes.size() < MAX_SHEEP)
		{
			bool taken = false;
			int rand_tile_index = random(1, grid_.GetTilesPtr()->size() - 1);

			for each (std::shared_ptr<Sheep> s in Sheepes)
			{
				if ((grid_.GetTilesPtr()->at(rand_tile_index)->x) == s->getx() && grid_.GetTilesPtr()->at(rand_tile_index)->y == s->gety())
				{
					taken = true;
					break;
				}
			}

			if (!taken)
			{
				std::shared_ptr<Sheep> sheep = std::make_shared<Sheep>(this, grid_.GetTilesPtr()->at(rand_tile_index)->x, grid_.GetTilesPtr()->at(rand_tile_index)->y, SIZE);
				Sheepes.push_back(sheep);
				grid_.GetTilesPtr()->at(rand_tile_index)->animal_type = Animal_Tile_Type::SHEEP;
			}
		}
	}

	else if (arg_type == "Wolf")
	{
		const int MAX_WOLF = 4; // 10
		while ((int)Wolveses.size() < MAX_WOLF)
		{
			bool taken = false;
			int rand_tile_index = random(1, grid_.GetTilesPtr()->size() - 1);

			for each (std::shared_ptr<Wolf> w in Wolveses)
			{
				if ((grid_.GetTilesPtr()->at(rand_tile_index)->x) == w->getx() && grid_.GetTilesPtr()->at(rand_tile_index)->y == w->gety())
				{
					taken = true;
					break;
				}
			}

			if (!taken)
			{
				std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>(this, grid_.GetTilesPtr()->at(rand_tile_index)->x, grid_.GetTilesPtr()->at(rand_tile_index)->y, SIZE); // -1 to align with tile
				Wolveses.push_back(wolf);
				grid_.GetTilesPtr()->at(rand_tile_index)->animal_type = Animal_Tile_Type::WOLF;
			}
		}
	}
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::DrawAgents(std::string arg_type) // Draws all agents
{
	if (arg_type == "Grass") // Draw Grass
	{
		for (size_t i = 0; i < Grasses.size(); i++)
		{
			glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
			double halfside = (double)(Grasses[i]->getlength() - 1) / 2; // -1 to align with grid
			glPushMatrix();
			glTranslatef(Grasses[i]->getx(), Grasses[i]->gety(), 0);
			CreateVisual(halfside);
			glPopMatrix();
		}
	}
	else if (arg_type == "Sheep") // Draw Sheep
	{
		for (size_t i = 0; i < Sheepes.size(); i++)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			double halfside = (double)(Sheepes[i]->getlength() - 1)/ 2;  // -1 to align with grid
			glPushMatrix();
			glTranslatef(Sheepes[i]->getx(), Sheepes[i]->gety(), 0);
			CreateVisual(halfside);
			glPopMatrix();
		}
	}
	else if (arg_type == "Wolf") // Draw Wolf
	{
		for (size_t i = 0; i < Wolveses.size(); i++)
		{
			glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
			double halfside = (double)(Wolveses[i]->getlength() - 1) / 2;  // -1 to align with grid
			glPushMatrix();
			glTranslatef(Wolveses[i]->getx(), Wolveses[i]->gety(), 0);
			CreateVisual(halfside);
			glPopMatrix();
		}
	}
} 

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::AddAgent(std::string arg_type, int arg_x, int arg_y) // Adds agent 
{
	if (arg_type == "Grass")
	{
		grid_.setType(arg_x, arg_y, Tile_Type::GRASS); 
		grid_.setAnimalType(arg_x, arg_y, Animal_Tile_Type::NONE);
		std::shared_ptr<Grass> grass = std::make_shared<Grass>(this, arg_x, arg_y, SIZE);
		Grasses.push_back(grass);
	}
	else if (arg_type == "Sheep")
	{
		grid_.setAnimalType(arg_x, arg_y, Animal_Tile_Type::SHEEP);
		std::shared_ptr<Sheep> sheep = std::make_shared<Sheep>(this, arg_x, arg_y, SIZE);
		Sheepes.push_back(sheep);
	}
	else if (arg_type == "Wolf")
	{
		grid_.setAnimalType(arg_x, arg_y, Animal_Tile_Type::WOLF);
		std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>(this, arg_x, arg_y, SIZE); 
		Wolveses.push_back(wolf);
	}
}

void Game::RemoveAgent(std::string arg_type, int arg_x, int arg_y)
{
	// grass obj defined by arg_x & arg_y is dead, so remove it and replace tile with dirt
	if (arg_type == "Grass")
	{
		for (size_t i = 0; i <= Grasses.size(); i++)
		{
			if (arg_x == Grasses[i]->getx() && arg_y == Grasses[i]->gety())
			{
				Grasses.erase(Grasses.begin() + i);
				grid_.setType(arg_x, arg_y, Tile_Type::DIRT);
				break;
			}
		}
	}
	else if (arg_type == "Sheep")
	{
		for (size_t i = 0; i < Sheepes.size(); i++)
		{
			if (arg_x == Sheepes[i]->getx() && arg_y == Sheepes[i]->gety())
			{
				Sheepes.erase(Sheepes.begin() + i);
				AddAgent("Grass", arg_x, arg_y);
				break;
			}
		}
	}
	else if (arg_type == "Wolf")
	{
		for (size_t i = 0; i < Wolveses.size(); i++)
		{
			if (arg_x == Wolveses[i]->getx() && arg_y == Wolveses[i]->gety())
			{
				Wolveses.erase(Wolveses.begin() + i);
				AddAgent("Grass", arg_x, arg_y);
				break;
			}
		}
	}
}

std::vector<std::shared_ptr<Sheep>> Game::getSheep()
{
	return Sheepes;
}

std::vector<std::shared_ptr<Wolf>> Game::getWolves()
{
	return Wolveses;
}

std::vector<std::shared_ptr<Grass>> Game::getGrass()
{
	return Grasses;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::CreateVisual(double arg_halfside)
{
	glBegin(GL_QUADS);
	glVertex2d(arg_halfside, arg_halfside);
	glVertex2d(arg_halfside, -arg_halfside);
	glVertex2d(-arg_halfside, -arg_halfside);
	glVertex2d(-arg_halfside, arg_halfside);
	glEnd();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::Mouse(int button, int state, int x, int y)
{
	mMouseButton = button;
	mMouseState = state;
	mGlutGetModifiers = glutGetModifiers();
	mMouseX = x; mMouseY = y;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

//A simple C-string based print function
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

void Game::NormalKeys(unsigned char key, int x, int y)
{
	if (key >= '0' && key <= '9') {}
	if (key == 13) {}//Return
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Game::SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) {}
	if (key == GLUT_KEY_RIGHT) {}
	if (key == GLUT_KEY_UP) {}
	if (key == GLUT_KEY_DOWN) {}
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------