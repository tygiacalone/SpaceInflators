#include "actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;


GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp

StudentWorld::StudentWorld()
{
	m_playerShip = NULL;
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
}



void StudentWorld::removeDeadGameObjects()
{

	for (int i = 0; i < m_actorList.size(); i++)
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

void StudentWorld::addAliensOrStars()
{
	if((rand() % 100) < 32)
		Star * twinkle = new Star(this);
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actorList.size(); i++)
		delete m_actorList[i];

	m_actorList.erase(m_actorList.begin(), m_actorList.end());
}

int StudentWorld::move() 
{ 
	// Add new Aliens or Stars 
	addAliensOrStars();    // add any Aliens/Stars to the space field as 



	// required (see below for details) 

	// Update the Game Status Line 
	//updateDisplayText();   // update the score/lives/round at screen top 


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


	// Remove newly-dead actors after each tick 

	removeDeadGameObjects(); // delete dead game objects 


	// return the proper result 
	if (!m_playerShip->isAlive()) 
		return GWSTATUS_PLAYER_DIED; 


	return GWSTATUS_CONTINUE_GAME;
}