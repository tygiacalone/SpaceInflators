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
	virtual int numberOfCollisions() const = 0; // pure virtual
	virtual bool isAlive() = 0; // pure virtual
	bool isCollision() const; 

	//Setters and Getters
	int getHealth() {return m_health;} 
	void setHealth(int health) {m_health = health;} 
	void setAlive(bool alive) {m_alive = alive;} 
	bool getAlive() {return m_alive;}
	void setWorld(StudentWorld * world) {m_world = world;} 
	StudentWorld * getWorld(){return m_world;} 

private:
	int m_health;
	bool m_alive;
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
	virtual bool isAlive();
	virtual int numberOfCollisions() const {return 0;}

};

////////////////////////////////////////
// Playership Class
////////////////////////////////////////

class PlayerShip : public Actor
{
public:
	PlayerShip(StudentWorld * world);
	virtual ~PlayerShip();
	virtual bool isAlive();
	virtual void doSomething();
	virtual int numberOfCollisions() const;
	
};

////////////////////////////////////////
// Alien Classes
////////////////////////////////////////

//Superclass
class Alien : public Actor
{};


	//Subclasses
	class Smallbot : public Alien
	{};

	class Nachling : public Alien
	{};

		//Subclass of Nachling
		class WealthyNachling : public Nachling
		{};


////////////////////////////////////////
// Goodies Classes
////////////////////////////////////////

//Superclass
class Goodies : public Actor
{};


	//Subclasses
	class TorpedoGoodies : public Goodies
	{};

	class FreeShipGoodies : public Goodies
	{};

	class EnergyGoodies : public Goodies
	{};


////////////////////////////////////////
// Projectile Classes
////////////////////////////////////////

//Superclass
class Projectile : public Actor
{};


	//Subclasses
	class TorpedoProjectile : public Projectile
	{};

	class BulletProjectile : public Projectile
	{};


#endif // _ACTOR_H_