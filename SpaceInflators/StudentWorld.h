#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <queue>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp

class Actor;
class Star;
class PlayerShip;

class StudentWorld : public GameWorld
{
public:

	StudentWorld();
	virtual ~StudentWorld(); //virtual
	virtual void init(); //virtual
	virtual int move(); //virtual
	virtual void cleanUp(); //virtual
	virtual void addAliensOrStars();
	void removeDeadGameObjects();
	double getRoundFactor();
	virtual void updateDisplayText();
	void updateEnemyProjectiles();
	void updateActiveAliens();
	void incDeadAliens(){m_deadAlienCounter++;}
	string formatGameText(int score, int round, double energyPercent, int torpedoes, int shipsLeft);

	//Setters and Getters
	void addActor(Actor * pointer){m_actorList.push_back(pointer);} 
	vector<Actor *> getList(){return m_actorList;}
	PlayerShip * getPlayerShip(){return m_playerShip;}
	void setPlayerShip(PlayerShip * pointer) {m_playerShip = pointer;}
	void setRound(int number){m_round = number;}
	int getRound(){return m_round;}
	int getDeadAlienCounter(){return m_deadAlienCounter;}
	int getNumEnemyProjectiles(){return m_numEnemyProjectiles;}
	int getNumActiveAliens(){return m_numActiveAliens;}

	

private:
	vector<Actor *> m_actorList;
	PlayerShip * m_playerShip;
	int m_round;
	int m_deadAlienCounter;
	int m_numActiveAliens;
	int m_numEnemyProjectiles;
};

#endif // _GAMEWORLD_H_
