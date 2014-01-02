#include "actor.h"
#include "StudentWorld.h"
#include <math.h>
#include <vector>
#include <iostream>
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

	m_justacted = false;
	m_world = world;
	setAlive(true);
	m_world->addActor(this);
	this->setVisible(true);
}

bool Actor::isAlive()
{
	if(getY() < 0 )
		setAlive(false);

	if(getY() > VIEW_HEIGHT-1)
		setAlive(false);

	if(getX() < 0)
		setAlive(false);

	if(getX() > VIEW_WIDTH-1)
		setAlive(false);

	if(getHealth() <= 0)
		setAlive(false);

	return m_alive;
}

bool Actor::canIAct()
{
	if (m_justacted == true)
	{
		m_justacted = false;
		return false;
	}

	if (m_justacted == false)
		m_justacted = true;
		return true;
}

////////////////////////////////////////
// Star Implementation
////////////////////////////////////////

Star::Star(StudentWorld * world)
	:Actor(world, IID_STAR, rand() % VIEW_WIDTH, VIEW_HEIGHT-1)
{
	setHealth(1);
}

void Star::doSomething()
{
	if(getY() >= 0)
		moveTo(getX(), getY()-1);
}

Star::~Star()
{}


////////////////////////////////////////
// Player Ship Implementation
////////////////////////////////////////

PlayerShip::PlayerShip(StudentWorld * world)
	:Actor(world, IID_PLAYER_SHIP, VIEW_WIDTH/2, 1)
{
	setHealth(50);
	getWorld()->setPlayerShip(this);
	setTorpedos(0);
}

PlayerShip::~PlayerShip()
{
}

int PlayerShip::collision() 
{
	
	vector<Actor *> list = getWorld()->getList();
	int collisionCounter = 0;

	for (int i = 0; i < list.size(); i++)
	{   //Dynamic Casting to Alien
		Alien *ptr = dynamic_cast<Alien *>(list[i]);

		if(!isAlive())
			return 0;

		if(ptr != NULL)
		{   // Compare for identical coordinates to detect collision.
			if( (this->getX() == list[i]->getX()) && (this->getY() ==list[i]->getY() ))
			{
				collisionCounter++;
				damage(IID_NACHLING); //I know it's going to be an alien of some type.
				list[i]->setHealth(0);
				list[i]->setAlive(false);
			}
		}
	}

	return collisionCounter;
}

//Reference Table for damage parametere "ID"
/*
const int IID_PLAYER_SHIP      = 0;
const int IID_NACHLING         = 1;
const int IID_WEALTHY_NACHLING = 2;
const int IID_SMALLBOT         = 3;
const int IID_BULLET           = 4;
const int IID_TORPEDO          = 5;
const int IID_FREE_SHIP_GOODIE = 6;
const int IID_ENERGY_GOODIE    = 7;
const int IID_TORPEDO_GOODIE   = 8;
const int IID_STAR             = 9;
*/

void PlayerShip::damage(const int ID)
{
	//Different damage and sound depending on ID of collision object.
	switch (ID)
	{
	case IID_NACHLING:
		setHealth(getHealth()-15);
		getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
		break;
	case IID_WEALTHY_NACHLING:
		setHealth(getHealth()-15);
		getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
		break;
	case IID_SMALLBOT:
		setHealth(getHealth()-15);
		getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
		break;
	case IID_BULLET:
		setHealth(getHealth()-2);
		getWorld()->playSound(SOUND_PLAYER_HIT);
		break;
	case IID_TORPEDO:
		setHealth(getHealth()-8);
		getWorld()->playSound(SOUND_PLAYER_HIT);
		break;
	}

	//Play death sound if player dies.
	//Also sets Player to dead if health is <= 0
	if (!isAlive())
		getWorld()->playSound(SOUND_PLAYER_DIE);

}


void PlayerShip::doSomething()
{
	if(!isAlive())
		return;

	//Detect collisions, decrement health per collision, destroy aliens that collide.
	collision();

	//Check to see if player is still alive after collisions.
	if (!isAlive())
		return;



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
		case KEY_PRESS_UP:
			if (getY() < VIEW_HEIGHT-1)
				moveTo(getX(), getY()+1);

			break;
		case KEY_PRESS_DOWN:
			if (getY() > 0)
				moveTo(getX(), getY()-1);

			break;
		case KEY_PRESS_SPACE:
			{
			if (canIAct())
			{
				//create Bullet
				if(getY() != 39)
					new Bullet(getWorld(), getX(), getY()+1, ORIGIN_PLAYERSHIP); 
			}
			break;
			}
		case KEY_PRESS_TAB:
			if (canIAct())
			{
				if (getTorpedos() > 0)
				{
					if(getY() != 39)
					{
						//create Torpedo
						new Torpedo(getWorld(), getX(), getY()+1, ORIGIN_PLAYERSHIP); 
						//decrement Torpedo count
						setTorpedos(getTorpedos()-1);
					}
				}
			}
			break;
		}
	}

	//AGAIN Detect collisions, decrement health per collision, destroy aliens that collide.
	collision();
}




////////////////////////////////////////
// Projectile Implementation
////////////////////////////////////////

Projectile::Projectile(StudentWorld * world, const int imageID, int x, int y, bool origin)
	:Actor(world, imageID, x, y)
{	
	m_origin = origin;
}

Projectile::~Projectile()
{}


	//	const bool ORIGIN_PLAYERSHIP = true;
	//	const bool ORIGIN_ALIEN = false;
void Projectile::doSomething()
{
	collision();

	if(!isAlive())
		return;

	if(getOrigin() == ORIGIN_PLAYERSHIP)
		moveTo(getX(), getY()+1);

	if(getOrigin() == ORIGIN_ALIEN)
		moveTo(getX(), getY()-1);

	collision();
}


int Projectile::collision()
{
	int collisionCounter = 0;

	PlayerShip * playership = getWorld()->getPlayerShip();

	if (m_origin == ORIGIN_ALIEN)
		if (this->getX() == playership->getX() && this->getY() == playership->getY() )
		{
			playership->damage(getID());

			//Set Bullet/Torpedo to dead
			setHealth(0);
			setAlive(false);


			return 1;
		}

	vector<Actor *> list = getWorld()->getList();

	if (m_origin == ORIGIN_PLAYERSHIP)
	{
		for (int i = 0; i < list.size(); i++)
		{   //Dynamic Casting to Alien
			Alien *ptr = dynamic_cast<Alien *>(list[i]);
			if(ptr != NULL)
			{   // Compare for identical coordinates to detect collision.
				if( (this->getX() == ptr->getX()) && (this->getY() == ptr->getY() ))
				{
					collisionCounter++;

      					ptr->damage(getID()); //I know it's going to be an alien of some type.

					//Set Bullet/Torpedo to dead
					setHealth(0);
					setAlive(false);


					return collisionCounter;
				}
			}
		}
	}

	return collisionCounter;
}


////////////////////////////////////////
// Bullet Implementation
////////////////////////////////////////

Bullet::Bullet(StudentWorld * world, int x, int y, bool origin)
	:Projectile(world, IID_BULLET, x, y, origin)
{
	setHealth(1);
	setAlive(true);
}



////////////////////////////////////////
// Torpedo Implementation
////////////////////////////////////////

Torpedo::Torpedo(StudentWorld * world, int x, int y, bool origin)
	:Projectile(world, IID_TORPEDO, x, y, origin)
{
	setHealth(1);
	setAlive(true);
}




////////////////////////////////////////
// Alien Implementation
////////////////////////////////////////

Alien::Alien(StudentWorld * world, const int ID)
	:Actor(world, ID, rand() % 30, 39)
{
	setHealth(4);
	setAlive(true);
}

void Alien::damage(const int ID)
{
	//Different damage and sound depending on ID of collision object.
	switch (ID)
	{
	case IID_PLAYER_SHIP:
		setHealth(0);
		setAlive(false);
		break; 
	case IID_BULLET:
		setHealth(getHealth()-2);
		getWorld()->playSound(SOUND_PLAYER_HIT);
		break;
	case IID_TORPEDO:
		setHealth(getHealth()-8);
		getWorld()->playSound(SOUND_PLAYER_HIT);
		break;

		
	}

}

////////////////////////////////////////
// Alien Subclass Implementation
////////////////////////////////////////

////////////////////////////////////////
// Nachlings Implementation
////////////////////////////////////////


//Starts with rand X coord between 0 and 29. Fixed Y coord of 39
Nachling::Nachling(StudentWorld * world, const int ID)
	:Alien(world, ID)
{
	setHealth(int(5 * getWorld()->getRoundFactor()));
	m_state = 0;
}

void Nachling::doSomething()
{
	if(!isAlive())
		return;

	//Can only act every other tick.
	if(!canIAct())
		return;

	doState(m_state);

	isAlive();
}

void Nachling::doState(int m_state)
{
	PlayerShip * playership = getWorld()->getPlayerShip();
	int BASE_CHANCE_OF_FIRING = 10;
	int chancesOfFiring = (BASE_CHANCE_OF_FIRING / getWorld()->getRound() + 1);

	switch(m_state)
	{
		case 0:
			//If location is the same as playership and not a border.
			if((this->getX() == playership->getX()) && (getX() != 0) && (getX() != 29))
			{
			//Step b.

				setState(1);
				
				//Compute MDB
				m_minBorderDist = calcMDB(getX());

				if(m_minBorderDist > 3)
					m_horMoveDist = ((rand() % 3)+1);
				else
					m_horMoveDist = m_minBorderDist;

				setrandDir();

				m_horMoveRem = m_horMoveDist;
			}
			//1/3rd chance.
			else if ((rand() % 100) < 33)
			{
			//Step a.

				int playerDir = 0;

				if(playership->getX() > getX())
					playerDir = 1;
				else
					playerDir = -1;

				moveTo(getX()+playerDir, getY()-1);

				return;
			}

			//Step c.
			
				moveTo(getX(), getY()-1);

				//Check if it's alive, change to dead if it isn't.
				if(!isAlive())
				{
					setHealth(0);
					setAlive(false);
				}

				return;
				break;


		/*******************************************************/
		/*******************************************************/
		case 1:
				if(playership->getY() > getY())
					setState(2);

				if(m_horMoveRem == 0)
				{
					if (m_randDir == 'L')
						m_randDir = 'R';
					else
						m_randDir = 'L';

					m_horMoveRem = 2 * m_horMoveDist;
				}
				else
				{
					m_horMoveRem--;
				}

							
				if(m_randDir == 'R')
					moveTo(getX()+1, getY());
				else
					moveTo(getX()-1, getY());

				//Compute chance of firing here.
				if(((rand() % chancesOfFiring)+1) == 1) //1 in 10 chance.
				{
					StudentWorld * thisWorld = getWorld();

					if(thisWorld->getNumEnemyProjectiles() < (2 * thisWorld->getRound()))
					{
						new Bullet(thisWorld, getX(), getY()-1, ORIGIN_ALIEN);
						thisWorld->playSound(SOUND_ENEMY_FIRE);
					}
				}


				//Chance of 1 in 20 to go to state 2.
				if(((rand() % 20)+1) == 1)
					setState(2);

				return;
				break;







		/*******************************************************/
		/*******************************************************/
		case 2:
			if (getY() == VIEW_HEIGHT-1)
			{
				setState(0);
				return;
			}
			else if (getX() == 0)
				m_randDir = 'R';
			else if (getX() == VIEW_WIDTH-1)
				m_randDir = 'L';
			else 
				setrandDir();

			if (m_randDir == 'R')
				moveTo(getX()+1, getY()+1);
			else
				moveTo(getX()-1, getY()+1);

			return;
			break;
	}
}

void Nachling::setrandDir()
{
	int random = (rand() % 2);
	switch(random)
	{
		case 0:
			m_randDir = 'L';
			break;
		case 1:
			m_randDir = 'R';
			break;
	}
}

int Nachling::calcMDB(int x)
{
	int toLeft = 0;
	int toRight = 0;

	toLeft = (x - 0);
	toRight = (29 - x);

	if (toLeft < toRight)
		return toLeft;
	else
		return toRight;
}

void Nachling::damage(const int ID)
{

	Alien::damage(ID);

	if (!isAlive())
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		getWorld()->incDeadAliens();
		switch(ID)
		{
		case IID_BULLET:
			getWorld()->increaseScore(1000);
			break;
		case IID_TORPEDO:
			getWorld()->increaseScore(1000);
			break;
		}
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}


////////////////////////////////////////
// Wealthy Nachlings Implementation
////////////////////////////////////////


WealthyNachling::WealthyNachling(StudentWorld * world, const int ID)
	:Nachling(world, ID)
{
	setHealth(int(8 * getWorld()->getRoundFactor())); 
	setState(0);
}

void WealthyNachling::doSomething()
{
	if(!isAlive())
		return;

	if (checkMalfunctioning())
		return;
	else if (((rand() % 200)+1) == 1)
	{
		m_malfunctioningCount = 30;
		m_isMalfunctioning = true;
		return;
	}

	if(!canIAct())
		return;

	doState(getState());

	isAlive();

}

bool WealthyNachling::checkMalfunctioning()
{
	if (m_isMalfunctioning == false)
		return false;
	else if (m_malfunctioningCount <= 0)
	{
		m_isMalfunctioning = false;
		return false;
	}

		m_malfunctioningCount--;
		return true;
}

void WealthyNachling::damage(const int ID)
{

	Alien::damage(ID);

	if (!isAlive())
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		getWorld()->incDeadAliens();
		switch(ID)
		{
		case IID_BULLET:
			getWorld()->increaseScore(1200);
			if ((rand() % 100) < 33)
			{
				if ((rand() % 100) < 50) //50% chance for each Goodie.
				 new EnergyGoodies(getWorld(), getX(), getY());
				else
				 new TorpedoGoodies(getWorld(), getX(), getY());
			}
			break;
		case IID_TORPEDO:
			getWorld()->increaseScore(1200);
			if ((rand() % 100) < 33)
			{
				if ((rand() % 100) < 50) //50% chance for each Goodie.
				 new EnergyGoodies(getWorld(), getX(), getY());
				else
				 new TorpedoGoodies(getWorld(), getX(), getY());
			}
			break;
		}
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}

////////////////////////////////////////
// Smallbots Implementation
////////////////////////////////////////


Smallbot::Smallbot(StudentWorld * world)
	:Alien(world, IID_SMALLBOT)
{
	setHealth(int(12 * getWorld()->getRoundFactor())); 
	m_wasIJustHit = false;
}

void Smallbot::doSomething()
{
	if(!isAlive())
		return;

	if(!canIAct())
		return;

	if(wasIJustHit())
		if((rand() % 100) < 33)
		{
			int x = getX();

			if (x == 0)
				x++;
			else if (x == 29)
				x--;
			else
			{
				if ((rand() % 100) < 50)
					x++;
				else
					x--;
			}

			moveTo(x, getY()-1);
		}
	
	moveTo(getX(), getY()-1);

	//Add shooting logic here.
	StudentWorld * thisWorld = getWorld();
	if(getX() == thisWorld->getPlayerShip()->getX() && getY() >= thisWorld->getPlayerShip()->getY())
	{
		int q = int(100.0/thisWorld->getRound() + 1);
		
		// 1 in q chance to shoot Torpedo. 
		if (((rand() % q)+1) == 1)
			new Torpedo(thisWorld, getX(), getY()+1, ORIGIN_ALIEN);
		//If the number of Enemy Projectiles is less than twice the current round number, fire a Bullet.
		else if (thisWorld->getNumEnemyProjectiles() < (2 * thisWorld->getRound()))
			new Bullet(thisWorld, getX(), getY()+1, ORIGIN_ALIEN);
	}



	isAlive();		
}

bool Smallbot::wasIJustHit()
{
	if (m_wasIJustHit == true)
	{
		m_wasIJustHit = false;
		return true;
	}

	return false;
}	

void Smallbot::damage(const int ID)
{
	Alien::damage(ID);

	if (ID == IID_BULLET || ID == IID_TORPEDO)
		m_wasIJustHit = true;

	if (!isAlive())
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		getWorld()->incDeadAliens();
		switch(ID)
		{
		case IID_BULLET:
			getWorld()->increaseScore(1500);
			if ((rand() % 100) < 33) //Free Ship Goodie chance of 1 in 3.
			{
				new FreeShipGoodies(getWorld(), getX(), getY());
			}
			break;
		case IID_TORPEDO:
			getWorld()->increaseScore(1500);
			if ((rand() % 100) < 33) //Free Ship Goodie chance of 1 in 3.
			{
				new FreeShipGoodies(getWorld(), getX(), getY());
			}
			break;
		}
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}



////////////////////////////////////////
// Goodies Implementation
////////////////////////////////////////

Goodies::Goodies(StudentWorld * world, const int ID, int x, int y)
	:Actor(world, ID, x, y)
{
	m_goodieTickLifetime = int(100.0 / getWorld()->getRound()) + 30;
	m_ticksLeftToLive = m_goodieTickLifetime;
	setHealth(1);
	m_everyThreeTicks = 0;
}



int Goodies::collision()
{
	PlayerShip * playership = getWorld()->getPlayerShip();

	if(getX() == playership->getX() && getY() == playership->getY())
		return 1;

	else
		return 0;
}


	
bool Goodies::isTimeLeft()
{
	if (m_ticksLeftToLive <= 0)
	{
		setHealth(0);
		setAlive(false);
		return false;
	}
	else
	{
		m_ticksLeftToLive--;
		return true;
	}
}

// Behavior: creation 1 2 move 1 2 3 move .. etc
bool Goodies::everyThreeTicks()
{
	if (m_everyThreeTicks < 3)
	{
		m_everyThreeTicks++;
		return false;
	}
	else if ((m_everyThreeTicks % 3) == 0)
	{
		m_everyThreeTicks -= 3;
		return true;
	}
	else
		m_everyThreeTicks++;
}

////////////////////////////////////////
// Free Ship Goodie Implementation
////////////////////////////////////////

FreeShipGoodies::FreeShipGoodies(StudentWorld * world, int x, int y)
	:Goodies(world, IID_FREE_SHIP_GOODIE, x, y)
{
}

void FreeShipGoodies::doSomething()
{
	
	if (collision())
	{
		getWorld()->increaseScore(5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->incLives();
		setAlive(false);
		setHealth(0);

	}
	else if (!collision())
	{
		setBrightness(getLeftToLive() / getTickLifetime() + 0.2);

/*
		Diagnostic Tool:

		cerr << "brightness set!" << endl;
		cerr << "Ticks left: " << getLeftToLive() << endl;
		cerr << "Tick lifetime: " << getTickLifetime() << endl;
		cerr << "Brightness setting: " << (getLeftToLive() / getTickLifetime() + 0.2) << endl;
*/

		if(!isTimeLeft())
			return;
		else if(everyThreeTicks())
			moveTo(getX(), getY()-1);

		if(!isAlive())
			return;

		if (collision())
		{
			getWorld()->increaseScore(5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->incLives();
			setAlive(false);
			setHealth(0);
		}

		return;
	}
}


////////////////////////////////////////
// Energy Goodie Implementation
////////////////////////////////////////

EnergyGoodies::EnergyGoodies(StudentWorld * world, int x, int y)
	:Goodies(world, IID_ENERGY_GOODIE, x, y)
{
}


void EnergyGoodies::doSomething()
{
	if (collision())
	{
		getWorld()->increaseScore(5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayerShip()->setHealth(50);
		setAlive(false);
		setHealth(0);

	}
	else if (!collision())
	{
		setBrightness(getLeftToLive() / getTickLifetime() + 0.2);

/*
		Diagnostic Tool:

		cerr << "brightness set!" << endl;
		cerr << "Ticks left: " << getLeftToLive() << endl;
		cerr << "Tick lifetime: " << getTickLifetime() << endl;
		cerr << "Brightness setting: " << (getLeftToLive() / getTickLifetime() + 0.2) << endl;
*/

		if(!isTimeLeft())
			return;
		else if(everyThreeTicks())
			moveTo(getX(), getY()-1);

		if(!isAlive())
			return;

		if (collision())
		{
			getWorld()->increaseScore(5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayerShip()->setHealth(50);
			setAlive(false);
			setHealth(0);
		}

		return;
	}
}


////////////////////////////////////////
// Torpedo Goodie Implementation
////////////////////////////////////////

TorpedoGoodies::TorpedoGoodies(StudentWorld * world, int x, int y)
	:Goodies(world, IID_TORPEDO_GOODIE, x, y)
{
}


void TorpedoGoodies::doSomething()
{
	if (collision())
	{
		getWorld()->increaseScore(5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayerShip()->setTorpedos(getWorld()->getPlayerShip()->getTorpedos() + 5);
		setAlive(false);
		setHealth(0);
	}
	else if (!collision())
	{
		setBrightness(getLeftToLive() / getTickLifetime() + 0.2);

/*
		Diagnostic Tool:

		cerr << "brightness set!" << endl;
		cerr << "Ticks left: " << getLeftToLive() << endl;
		cerr << "Tick lifetime: " << getTickLifetime() << endl;
		cerr << "Brightness setting: " << (getLeftToLive() / getTickLifetime() + 0.2) << endl;
*/

		if(!isTimeLeft())
			return;
		else if(everyThreeTicks())
			moveTo(getX(), getY()-1);

		if(!isAlive())
			return;

		if (collision())
		{
			getWorld()->increaseScore(5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayerShip()->setTorpedos(getWorld()->getPlayerShip()->getTorpedos() + 5);
			setAlive(false);
			setHealth(0);
		}

		return;
	}
}