#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;

// Students:  Add code to this file, actor.cpp, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////
// Actor Class
////////////////////////////////////////

class Actor : public GraphObject
{
public:
	Actor(StudentWorld * world, int imageID, int x, int y);
	virtual ~Actor(){}
	virtual void doSomething() = 0; // pure virtual
	virtual int collision() = 0; // pure virtual
	bool isAlive(); 
	virtual bool canIAct(); //virtual

	//Setters and Getters
	double getHealth() {return m_health;}
	void setHealth(int health) {m_health = health;}
	void setAlive(bool alive) {m_alive = alive;}
	bool getAlive() {return m_alive;}
	StudentWorld * getWorld(){return m_world;}


private:
	double m_health;
	bool m_alive;
	bool m_justacted;
	StudentWorld * m_world;
};

////////////////////////////////////////
// Star Class
////////////////////////////////////////

class Star : public Actor
{
public:
	Star(StudentWorld * world);
	virtual ~Star();
	virtual void doSomething();
	virtual int collision() {return 0;}
};

////////////////////////////////////////
// Playership Class
////////////////////////////////////////

class PlayerShip : public Actor
{
public:
	PlayerShip(StudentWorld * world);
	virtual ~PlayerShip();
	virtual void doSomething();
	virtual int collision();
	virtual void damage(const int ID);


	//Setters and Getters
	int getTorpedos(){return m_numTorpedos;}
	void setTorpedos(int num){m_numTorpedos = num;}

private:
	int m_numTorpedos;
};

////////////////////////////////////////
// Alien Classes
////////////////////////////////////////

//Superclass
class Alien : public Actor
{
public:
	Alien(StudentWorld * world, const int ID);
	virtual ~Alien() {}
	virtual void doSomething(){} 
	virtual int collision(){return 0;}
	virtual void damage(const int ID);
};


//Subclasses
class Smallbot : public Alien
{
public:
	Smallbot(StudentWorld * world);
	virtual ~Smallbot(){}
	virtual void doSomething();
	virtual int collision(){return 0;}
	virtual void damage(const int ID);
	bool wasIJustHit();

private:
	bool m_wasIJustHit;
};


class Nachling : public Alien
{
public:
	Nachling(StudentWorld * world, const int ID);
	virtual ~Nachling(){}
	virtual void doSomething();
	virtual int collision(){return 0;}
	virtual void doState(int m_state);
	virtual void damage(const int ID);
	int calcMDB(int x);
	void setrandDir();

	//Setters and Getters
	void setState(int num){m_state = num;}
	int getState(){return m_state;}


private:
	int m_state;
	int m_minBorderDist; //minimum border distance
	int m_horMoveDist; //horizontal movement distance
	int m_horMoveRem; //horizontal movement remaining
	char m_randDir; //Nachling movement direction
};


//Subclass of Nachling
class WealthyNachling : public Nachling
{
public:
	WealthyNachling(StudentWorld * world, const int ID);
	virtual ~WealthyNachling(){}
	virtual void doSomething();
	virtual int collision(){return 0;}
	virtual void damage(const int ID);
	virtual bool checkMalfunctioning();

private:
	int m_malfunctioningCount;
	bool m_isMalfunctioning;
};


////////////////////////////////////////
// Goodies Classes
////////////////////////////////////////

//Superclass
class Goodies : public Actor
{
public:
	Goodies(StudentWorld * world, const int ID, int x, int y);
	virtual ~Goodies(){}
	virtual void doSomething(){}
	virtual int collision();
	bool isTimeLeft();
	bool everyThreeTicks();

	//Setters and Getters
	double getLeftToLive(){return m_ticksLeftToLive;}
	double getTickLifetime(){return m_goodieTickLifetime;}


private:
	double m_goodieTickLifetime;
	double m_ticksLeftToLive;
	int m_everyThreeTicks;
};


	//Subclasses
	class FreeShipGoodies : public Goodies
	{
	public:
		FreeShipGoodies(StudentWorld * world, int x, int y);
		virtual ~FreeShipGoodies(){}
		virtual void doSomething();
		virtual int collision(){return Goodies::collision();}
	};

	
	class EnergyGoodies : public Goodies
	{
	public:
		EnergyGoodies(StudentWorld * world, int x, int y);
		virtual ~EnergyGoodies(){}
		virtual void doSomething();
		virtual int collision(){return Goodies::collision();}
	};


	class TorpedoGoodies : public Goodies
	{
	public:
		TorpedoGoodies(StudentWorld * world, int x, int y);
		virtual ~TorpedoGoodies(){}
		virtual void doSomething();
		virtual int collision(){return Goodies::collision();}
	};



////////////////////////////////////////
// Projectile Classes
////////////////////////////////////////

const bool ORIGIN_PLAYERSHIP = true;
const bool ORIGIN_ALIEN = false;

//Superclass
class Projectile : public Actor
{
public:
	Projectile(StudentWorld * world, const int imageID, int x, int y, bool origin);
	virtual ~Projectile();
	virtual void doSomething();
	virtual int collision();

	//Setters and Getters
	bool getOrigin(){return m_origin;}
	void setOrigin(const bool ORIGIN){m_origin = ORIGIN;}

private:
	//	const bool ORIGIN_PLAYERSHIP = true;
	//	const bool ORIGIN_ALIEN = false;
	bool m_origin;
};


	//Subclasses
	class Torpedo : public Projectile
	{
	public:
		Torpedo(StudentWorld * world, int x, int y, bool origin); 
		virtual ~Torpedo() {}
	};

	class Bullet : public Projectile
	{
	public:
		Bullet(StudentWorld * world, int x, int y, bool origin);
		virtual ~Bullet() {}
	};


#endif // _ACTOR_H_