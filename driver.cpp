#include "driver.h"
#include <stdlib.h>
#include <iostream>

driver::driver(float nSpeed, float nyPosition, float nLane)
{
	actualSpeed=speed=nSpeed;
	yPosition=nyPosition;
	currentLane=nLane;
	moved=false;
}

driver::driver()
{
	actualSpeed=speed=316800;
	yPosition=0;
	currentLane=0;
	moved=false;
}

driver::driver(const driver& other)
{
	actualSpeed=speed=other.speed;
	yPosition=other.yPosition;
	currentLane=other.currentLane;
	moved=other.moved;
}

driver& driver::operator=(driver &other)
{
	actualSpeed=speed=other.getSpeed();
	yPosition=other.getYPosition();
	currentLane=other.getLane();
	moved=other.hasMoved();
	driver* temp = new driver(other.getSpeed(), other.getYPosition(), other.getLane());
	return *temp;
}


float driver::getYPosition() { return yPosition; }

void driver::setYPosition(float newVal) { yPosition=newVal; }

float driver::getLane() { return currentLane; }

void driver::setLane(int newVal) { currentLane=newVal; }

float driver::getSpeed() { return speed; }

void driver::setSpeed(float newVal) { speed=newVal; }


void driver::incrementSpeed() { speed+=52800; }

void driver::decrementSpeed() { speed-=52800; }

float driver::getActualSpeed() { return actualSpeed; }

bool driver::wouldCollide(driver other)
{
	if(other.getLane()==currentLane)
	{
		float distanceToMove=speed/60;
		float distanceBetween=yPosition-other.getYPosition();
		if(distanceBetween<0)
			return false;
		if(distanceBetween < distanceToMove)
			return true;
	}
	return false;
}

float driver::distanceBetween(driver other)
{
	return abs(yPosition-other.getYPosition());
}

bool driver::canMergeLeft(driver other, int numLanes)
{
	std::cout<<"Checking merge Left"<<std::endl;
	if(currentLane+1>=numLanes)
		return false;
	std::cout<<"Not at leftmost lane"<<std::endl;
	if(other.getLane()==currentLane)
		return true;
	std::cout<<"Not in the same lane"<<std::endl;
	if(other.getLane()==currentLane+1)
	{
		if(other.getYPosition()==yPosition)
			return false;
	}
	std::cout<<"Not at the same yPosition"<<std::endl;
}

bool driver::canMergeRight(driver other, int numLanes)
{
	if(currentLane==0)
		return false;
	if(other.getLane()==currentLane)
		return true;
	if(other.getLane()==currentLane-1)
	{
		if(other.getYPosition()==yPosition)
			return false;
	}
	return true;
}

void driver::moveForward()
{
	float distanceForward=speed/60;
	yPosition+=distanceForward;
	actualSpeed=speed;
}

void driver::mergeRight() { currentLane--; speed-=52800; }

void driver::mergeLeft() { currentLane++; speed+=52800;}

void driver::tailgate(float distance)
{
	actualSpeed=(distance*0.9)*60;
	if( (distance-distance*0.9) > 1)
		yPosition+=(distance*0.9);
	else
		yPosition+=(distance*0.9)-1;
	//std::cout<<"tailgating: "<<yPosition<<" Trying to go: "<<speed<<" Actually going: "<<actualSpeed<<std::endl;
}

bool driver::hasMoved() { return moved; }

void driver::doneMoving() { moved=true; }

void driver::resetMotion() { moved=false; }
