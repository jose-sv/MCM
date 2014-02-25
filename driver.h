#ifndef DRIVER_H
#define DRIVER_H
class driver {
	public:
		driver& operator=(driver&);
                driver(float, float, float); //mph, miles, lane 0=first lane
                driver(const driver&);
		driver();

		float getYPosition();
                void setYPosition(float);
                float getLane();
                void setLane(int);
                float getSpeed();
                void setSpeed(float);
		float getActualSpeed();

                void incrementSpeed();
                void decrementSpeed();
                void moveForward();
		void mergeRight();
		void mergeLeft();
		bool hasMoved();
		void doneMoving();
		void resetMotion();

                bool wouldCollide(driver);
                float distanceBetween(driver);
                bool canMergeLeft(driver, int);
                bool canMergeRight(driver, int);
		void tailgate(float);

	private:
		float speed;
		float actualSpeed;
		float yPosition;
		float currentLane;
		bool moved;
};

#endif
