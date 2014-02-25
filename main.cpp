#include "driver.h"
#include "driver.cpp"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
//#include <limits.h>

using namespace std;

struct driverNode
{
	driver currentDriver;
	driverNode* next;
};

void addDriver(driverNode* head, driver newGuy)
{
	driverNode* newNode = new driverNode;
	newNode->currentDriver=newGuy;
	newNode->next=NULL;

	driverNode* currentCar=head;
	while(currentCar->next)
	{
		currentCar=currentCar->next;
	}
	currentCar->next=newNode;
}

bool allDriversDone(driverNode* head, int distance)
{
	for(driverNode* currentCar=head; currentCar->next; currentCar=currentCar->next)
	{
		if(currentCar->currentDriver.getYPosition()<distance)
			return false;
	}
	return true;
}

bool couldMergeLeft(driver first, driver second, int numLanes)
{
	if( (first.getLane()+1)>=numLanes )
		return false;
	if(first.getLane()==second.getLane())
		return true;
	if(first.getYPosition()==second.getYPosition() && second.getLane()==(first.getLane()+1) )
		return false;
	return true;
}

bool couldMergeRight(driver first, driver second, int numLanes)
{
	if( (first.getLane()-1)<0 )
		return false;
	if(first.getLane()==second.getLane())
		return true;
	if(first.getYPosition()==second.getYPosition() && second.getLane()==(first.getLane()-1) )
		return false;
	return true;
}

float spaceBetween(driver first, driver second)
{
	return second.getYPosition()-first.getYPosition();
}

bool mightCollide(driver first, driver second)
{
	if(first.getLane()==second.getLane())
	{
		float distanceToMove=first.getSpeed()/60;
		float distanceBetween= second.getYPosition()-1 - first.getYPosition();
		if(distanceBetween<0)
			return false;
		if(distanceBetween<distanceToMove)
			return true;
	}
	return false;
}

int moveDriversForward(driverNode* head, int numLanes, float startPoint, float endPoint)
{
	int numTailgates=0;
	bool passRight=false;
	for(driverNode* currentCar=head; currentCar; currentCar=currentCar->next)
	{
		//if( !(currentCar->currentDriver.hasMoved()) ) {
		int timesMerged=0;
		bool merged=true;
		bool canMergeRight=false;
		bool canMergeLeft=true;

		if(!passRight)
		{
			if(currentCar->currentDriver.getLane()!=0)
				canMergeRight=true;

	                if(canMergeRight)
        	        {
				for(driverNode* mergeBackCheck=head; mergeBackCheck; mergeBackCheck=mergeBackCheck->next)
                        	{
                                	if( !(currentCar->currentDriver.canMergeRight(mergeBackCheck->currentDriver, numLanes)) )
                                        	canMergeRight=false;
	                        }
        	                if(canMergeRight)
                	        {
                        	        currentCar->currentDriver.mergeRight();
	                        }
        	        }
		}

		while(merged && timesMerged<3)
		{
			merged=false;
			canMergeLeft=true;
			for(driverNode* collisionCheck=head; collisionCheck; collisionCheck=collisionCheck->next)
			{
				if(collisionCheck!=currentCar)
				{
					if(mightCollide(currentCar->currentDriver, collisionCheck->currentDriver))
					{
						for(driverNode* mergeCheck=head; mergeCheck && canMergeLeft; mergeCheck=mergeCheck->next)
						{
							if(!couldMergeLeft(currentCar->currentDriver,mergeCheck->currentDriver, numLanes))
							{
								canMergeLeft=false;
								timesMerged=99;
							}
						}
						if(canMergeLeft)
						{
							currentCar->currentDriver.mergeLeft();
							merged=true;
						}
						else
						{
							if(!passRight)
							{
								currentCar->currentDriver.tailgate( spaceBetween(currentCar->currentDriver, collisionCheck->currentDriver ) );
								//cout<<"Distance: "<<spaceBetween(currentCar->currentDriver,collisionCheck->currentDriver)<<endl;
								merged=true;
								//cout<<currentCar<<endl;
								if(currentCar->currentDriver.getYPosition()>startPoint && currentCar->currentDriver.getYPosition()<endPoint)
									numTailgates++;
							}
							else
							{
								for(driverNode* mergeCheck=head; mergeCheck && canMergeRight; mergeCheck=mergeCheck->next)
								{
									if(!couldMergeRight(currentCar->currentDriver,mergeCheck->currentDriver,numLanes))
									{
										canMergeRight=false;
										timesMerged=99;
									}
								}
								if(canMergeRight)
								{
									currentCar->currentDriver.mergeRight();
									merged=true;
								}
								else
								{
									merged=true;
									currentCar->currentDriver.tailgate( spaceBetween(currentCar->currentDriver,collisionCheck->currentDriver) );
									if(currentCar->currentDriver.getYPosition()>startPoint && currentCar->currentDriver.getYPosition()<endPoint)
										numTailgates++;
								}
							}
						}
					}
				}
			}
		}

		if(!merged)
			currentCar->currentDriver.moveForward();
		currentCar->currentDriver.doneMoving(); //}
	}

	return numTailgates;
}

float driversBetween(driverNode* head, float startPoint, float endPoint, float &maxSpeed, int &numCars)
{
	int numDrivers=0; int numDriversForAverage=0;
	maxSpeed=0;
	float totalDistance=0;
	for(driverNode* currentCar=head; currentCar; currentCar=currentCar->next)
	{
		if(startPoint <  currentCar->currentDriver.getYPosition() && currentCar->currentDriver.getYPosition() < endPoint)
		{
			numDrivers++;
			//maxSpeed+=currentCar->currentDriver.getActualSpeed();
			if(currentCar->currentDriver.getActualSpeed()<30)
				cout<<currentCar->currentDriver.getActualSpeed()<<", "<<currentCar->currentDriver.getYPosition()<<endl;
			if(currentCar->currentDriver.getActualSpeed() > maxSpeed){
				maxSpeed=currentCar->currentDriver.getActualSpeed();
				cout<<"***************"<<endl<<maxSpeed<<endl;}
			driverNode* closestCar=head;
			float minDistance=0;

			if(closestCar!=currentCar)
			{
				if(spaceBetween(currentCar->currentDriver, closestCar->currentDriver)>0)
					minDistance=spaceBetween(currentCar->currentDriver, closestCar->currentDriver);
			}
			else
			{
				closestCar=closestCar->next;
				if(spaceBetween(currentCar->currentDriver, closestCar->currentDriver)>0)
					minDistance=spaceBetween(currentCar->currentDriver, closestCar->currentDriver);
			}
			numDriversForAverage++;
			if(minDistance<=0)
				minDistance=endPoint-currentCar->currentDriver.getYPosition();
			for(closestCar=closestCar->next; closestCar; closestCar=closestCar->next)
			{
				if(closestCar!=currentCar)
				{
					if(spaceBetween(currentCar->currentDriver, closestCar->currentDriver) > 0)
						minDistance=spaceBetween(currentCar->currentDriver, closestCar->currentDriver);
				}
			}
			if(0 < minDistance && minDistance <= (endPoint-currentCar->currentDriver.getYPosition()) )
				{ totalDistance+=minDistance; numDriversForAverage++; }
		}
	}
	//maxSpeed/=numDrivers;
	numCars=numDrivers;
	return totalDistance/numDriversForAverage;
}

float trafficDensity(driverNode* head, int numLanes, float startPoint, float endPoint)
{
	float maxSpeed; int numCars;
	int avgDistance=driversBetween(head, startPoint, endPoint, maxSpeed, numCars);

	return maxSpeed*(1- (1/avgDistance) );
}

void resetDrivers(driverNode* head)
{
	for(driverNode* currentCar=head; currentCar; currentCar=currentCar->next)
	{
		currentCar->currentDriver.resetMotion();
	}
}


int main(int argc, char* argv[])
{
	if(argc!=5)
	{
		cout<<"Invalid number of arguments, use: ./[executable] [numDrivers>=2] [numLanes>=2] [distance>=10] [timeLimit>=1 hours]"<<endl;
		return 0;
	}
	int numDrivers=atoi(argv[1]);
	if(numDrivers<2)
	{
		cout<<"Invalid number of drivers"<<endl;
		return 0;
	}
	int numLanes=atoi(argv[2]);
	/*if(numLanes<2)
	{
		cout<<"Invalid number of lanes"<<endl;
		return 0;
	}*/
	int distance=atoi(argv[3]);
	if(distance<10)
	{
		cout<<"Invalid distance"<<endl;
		return 0;
	}
	int timeLimit=atoi(argv[4]);
	if(timeLimit<1)
	{
		cout<<"Invalid timeLimit"<<endl;
		return 0;
	}
	timeLimit*=60;

	int minutes=0;

	driverNode* head=new driverNode;
	head->next=NULL;
	for(int i=1; i<numDrivers; i++)
	{
		driver newDriver;
		newDriver.setYPosition(i);
		addDriver(head, newDriver);
	}

	srand(time(NULL));

	int numTailgates=0;
	int time=0;
	float lowerThird=distance/3;
	float upperThird=distance/3*2;
	float maxSpeed; int numCars;
	ofstream outputFile;
	outputFile.open("outfile");
	if(outputFile.fail()) { cout<<"Failed to open"<<endl;return 0; }

	outputFile<<"Time,Average Distance,Max Speed,Cars,Traffic Flow,Tailgates"<<endl;
	while(time<timeLimit)
	{
		if(!(rand()%10))
		{
			float newSpeed;
			int multiplier;
			if(rand()%2)
				multiplier=-1;
			else
				multiplier=1;
			driver newDriver( (316800+(multiplier*rand()%11)), 0, rand()%numLanes );
			addDriver(head, newDriver);
			cout<<"Added Driver"<<endl;
		}

		numTailgates=moveDriversForward(head, numLanes, lowerThird, upperThird);
		time++;
		//resetDrivers(head);

		cout<<"Average Distance: "<<driversBetween(head, lowerThird, upperThird, maxSpeed, numCars)/5280<<endl<<"Speed: "<<maxSpeed/5280<<endl<<"NumCars: "<<numCars<<endl;
		cout<<endl<<"Traffic Flow: "<<trafficDensity(head, numLanes, lowerThird, upperThird)<<endl;
		cout<<"Number of Tailgates: "<<numTailgates<<endl;

		outputFile<<time<<","
			<<driversBetween(head, lowerThird, upperThird, maxSpeed, numCars)/5280<<","
			<<maxSpeed/5280<<","
			<<numCars<<","
			<<trafficDensity(head, numLanes, lowerThird, upperThird)<<","
			<<numTailgates<<","
			<<endl;
	}

	return 0;
}
