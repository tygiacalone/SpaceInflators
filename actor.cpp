#include "actor.h"
#include "StudentWorld.h"
#include <math.h>
using namespace std;


// Students:  Add code to this file (if you wish), actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////
// Actor Implementation
////////////////////////////////////////

Actor::Actor(StudentWorld * world, int imageID, int x, int y)
	:GraphObject(imageID, x, y)
{
	if (x < 0 || x > VIEW_WIDTH-1)
		return;
	if (y < 0 || y > VIEW_HEIGHT-1)
		return;

	
	setWorld(world);
	m_world->addActor(this);
	this->setVisible(true);
}

bool Actor::isCollision() const
{
	
	vector<Actor *> list = m_world->getList();

	for (int i = 0; i < list.size(); i++)
	{
		if(this != list[i]) //Don't detect collision with itself.
		{   // Compare for identical coordinates to detect collision.
			if( (this->getX() == list[i]->getX()) && (this->getY() ==list[i]->getY() ))
				return true;
		}
	}
	
	return false;
}

////////////////////////////////////////
// Star Implementation
////////////////////////////////////////

Star::Star(StudentWorld * world)
	:Actor(world, IID_STAR, rand() % VIEW_WIDTH, VIEW_HEIGHT-1)
{
}

void Star::doSomething()
{
	if(getY() >= 0)
		moveTo(getX(), getY()-1);
}

Star::~Star()
{}

bool Star::isAlive()
{
	if(getY() < 0 )
		setAlive(false);

	return getAlive();
}


////////////////////////////////////////
// Player Ship Implementation
////////////////////////////////////////

PlayerShip::PlayerShip(StudentWorld * world)
	:Actor(world, IID_PLAYER_SHIP, VIEW_WIDTH/2, 1)
{
	setHealth(50);
	setAlive(true);
	getWorld()->setPlayerShip(this);
	getWorld()->addActor(this);
}

PlayerShip::~PlayerShip()
{
}

int PlayerShip::numberOfCollisions() const
{
	return 0;
}

void PlayerShip::doSomething()
{
	if(isCollision())
	{
		return;
	}

	int dir; //holds direction of keypress.
	
	if (getWorld()->getKey(dir))
	{
		//user hit key

		switch (dir)
		{
		case KEY_PRESS_LEFT:
			if (getX() > 0)
				moveTo(getX()-1, getY());

			break;
		case KEY_PRESS_RIGHT:
			if (getX() < VIEW_WIDTH-1)
				moveTo(getX()+1, getY());

			break;
		}
	}

}

bool PlayerShip::isAlive()
{
	if(getHealth() <= 0 )
		setAlive(false);

	return getAlive();
}