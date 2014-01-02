#include "actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;


GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp

StudentWorld::StudentWorld()
{
	m_playerShip = NULL;
	m_round = 1;
	m_deadAlienCounter = 0;
	m_numEnemyProjectiles = 0;
	m_numActiveAliens = 0;
}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < m_actorList.size(); i++)
		delete m_actorList[i];

	m_actorList.erase(m_actorList.begin(), m_actorList.end());
}

void StudentWorld::init()
{
	m_playerShip = new PlayerShip(this);
	m_deadAlienCounter = 0;
}


void StudentWorld::removeDeadGameObjects()
{

	for (int i = 1; i < m_actorList.size(); i++)
		{
			if (!m_actorList[i]->isAlive())
			{
				Actor * toBeDeleted;
				Actor * toReplace;

				toBeDeleted = m_actorList[i];
				toReplace = m_actorList[getList().size()-1];
				m_actorList[i] = toReplace;
				m_actorList.pop_back();
				delete toBeDeleted;
				

				i--;
			}
		}
}


// test parameter constants
//
//const int TEST_PARAM_ACTOR_INDEX      = 0;
//const int NUM_TEST_PARAMS             = 1;
//
//const int TEST_PARAM_NACHLING         = 1;
//const int TEST_PARAM_WEALTHY_NACHLING = 2;
//const int TEST_PARAM_SMALLBOT         = 3;
//const int TEST_PARAM_GOODIE_ENERGY    = 4;
//const int TEST_PARAM_GOODIE_TORPEDO   = 5;
//const int TEST_PARAM_GOODIE_FREE_SHIP = 6;

void StudentWorld::addAliensOrStars()
{
	updateActiveAliens();

	int fate = (rand() % 100);
	int maxAliens = int(2 + .5*m_round);
	
	if (testParamsProvided())
	{
		if (m_actorList.size() > 1)
			return;

		int testParam = getTestParam(TEST_PARAM_ACTOR_INDEX);

		switch(testParam)
		{
		case TEST_PARAM_NACHLING:
			new Nachling(this, IID_NACHLING);
			break;
		case TEST_PARAM_WEALTHY_NACHLING:
			new WealthyNachling(this, IID_WEALTHY_NACHLING);
			break;
		case TEST_PARAM_SMALLBOT:
			new Smallbot(this);
		case TEST_PARAM_GOODIE_ENERGY:
			new EnergyGoodies(this, 15, 39);
			break;
		case TEST_PARAM_GOODIE_TORPEDO:
			new TorpedoGoodies(this, 15, 39);
		case TEST_PARAM_GOODIE_FREE_SHIP:
			new FreeShipGoodies(this, 15, 39);
			break;
		}

		return;
	}

	//Add Aliens
	else if(m_numActiveAliens < maxAliens)
	{
		if (m_numActiveAliens < ((4 * m_round) - m_deadAlienCounter))
		{
			//Nachling
			if((rand() % 100) < 10)
			{
				if (fate < 70)
				{
					int fate2 = (rand() % 100);

					if (fate2 < 20)
					{
						new WealthyNachling(this, IID_WEALTHY_NACHLING);
					}
					else
						new Nachling(this, IID_NACHLING);
	
				}
				else
				{
					//add Smallbot
					new Smallbot(this); //Starting energy 12 * current round.
				}
			}
		}
	}

	//Stars
	if((rand() % 100) < 32)
		Star * twinkle = new Star(this);

}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actorList.size(); i++)
		delete m_actorList[i];

	m_actorList.erase(m_actorList.begin(), m_actorList.end());

}

double StudentWorld::getRoundFactor()
{
	double roundFactor = 0.9 + 0.1 * m_round;

	return roundFactor;
}

void StudentWorld::updateDisplayText()
{
	int score = getScore();
 
	int round = m_round;
 
	double energyPercent = double(m_playerShip->getHealth() / 50);
 
	int torpedoes = m_playerShip->getTorpedos(); 
 
	int shipsLeft = getLives();
 
 
	// Next, create a string from your statistics, of the form:  
	// “Score: 0000123  Round: 01  Energy: 98%  Torpedoes: 003  Ships: 03” 
	// where all numeric values except the Energy value must have leading  
	// zeros (e.g., 003 vs. 3), and each 
	// statistic should be separated from the last statistic by two  
	// spaces.  E.g., between the “0000123” of the 
	// score and the “R” in “Round” there must be two spaces. Each field  
	// must be exactly as wide as shown, 
	// e.g., the score must be exactly 7 digits long, the torpedo field  
	// must be 3 digits long, except for the  
	// Energy field, which could be between 1 and 3 digits (e.g., 5%, 89%  
	// or 100%) 
 
 
	string s = formatGameText(score, round, energyPercent, torpedoes, shipsLeft); 
 
	// Finally, update the display text at the top of the screen with your  
	// newly created stats 
 
	setGameStatText(s); 
	// calls GameWorld::setGameStatText
}

string StudentWorld::formatGameText(int score, int round, double energyPercent, int torpedoes, int shipsLeft)
{
	string s;

	ostringstream string;

	int energy = int(energyPercent * 100);

	string.setf(ios::fixed);
	string.precision(0);
	string.fill('0');

	string << "Score: " << setw(7) << score << "  ";
	string << "Round: " << setw(2) << round << "  ";                             
	string << "Energy: " <<  energy << "%" << "  ";
	cerr << "Energy level: " << energy << endl;
	string << "Torpedoes: " << setw(3) << torpedoes << "  ";
	string << "Ships: " << setw(2) << shipsLeft;

	s = string.str();

	return s;
}

void StudentWorld::updateEnemyProjectiles()
{
	for (int i = 1; i < m_actorList.size(); i++)
		{
			if (!m_actorList[i]->isAlive())
			{
				Projectile * ptr = dynamic_cast<Projectile *>(m_actorList[i]);

				if (ptr != NULL)
				{
					//If it's an Alien projctile
					if(ptr->getOrigin() == ORIGIN_ALIEN)
						m_numEnemyProjectiles++;
				}
					
			}
		}
}

void StudentWorld::updateActiveAliens()
{
	int counter = 0;
	for (int i = 1; i < m_actorList.size(); i++)
		{
			if (m_actorList[i]->isAlive())
			{
				Alien * ptr = dynamic_cast<Alien *>(m_actorList[i]);

				if (ptr != NULL)
					counter++;
			}
		}

	m_numActiveAliens = counter;
}

int StudentWorld::move() 
{ 

	if (m_deadAlienCounter == (4 * m_round))
	{
		m_round++;
		m_deadAlienCounter = 0;
	}

	// Add new Aliens or Stars 
	addAliensOrStars();    // add any Aliens/Stars to the space field as 

	

	// required (see below for details) 

	// Update the Game Status Line 
	updateDisplayText();   // update the score/lives/round at screen top 

	updateEnemyProjectiles();


	// The term “actors” refers to all Aliens, the Player’s ship, Stars, 
	// and any projectiles that are still active 

	// Give each actor a chance to do something 


	for (int i = 0; i < getList().size(); i++)
	{
		if (m_actorList[i]->isAlive()) 
		{ 
			m_actorList[i]->doSomething(); 
		}
 	}

	// For Debug Purposes:
	//cerr << m_deadAlienCounter << endl;
	//cerr << "Current Round: " << m_round << endl;
	//cerr << "Active Aliens: " << m_numActiveAliens << endl;
	// Remove newly-dead actors after each tick 

	removeDeadGameObjects(); // delete dead game objects 


	// return the proper result 
	if (!m_playerShip->isAlive()) 
	{
		decLives();
		return GWSTATUS_PLAYER_DIED; 
	}


	return GWSTATUS_CONTINUE_GAME;
}