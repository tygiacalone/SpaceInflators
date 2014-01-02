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
	void addAliensOrStars();
	void removeDeadGameObjects();

	//Setters and Getters
	void addActor(Actor * pointer){m_actorList.push_back(pointer);} 
	vector<Actor *> getList(){return m_actorList;}
	PlayerShip * getPlayerShip(){return m_playerShip;}
	void setPlayerShip(PlayerShip * pointer) {m_playerShip = pointer;}

private:
	vector<Actor *> m_actorList;
	PlayerShip * m_playerShip;
};

#endif // _GAMEWORLD_H_
