/********************************************************************
/********************************************************************
* Name : User Shortcode
* Description : Drive trough Google Earth
* Version : 1.0 (Special 30c3 Release)
* Author : Sebastian Hansack
* Note : This is a special Release intended to be shown on the 
*				30c3
* BEST VIEW: Tabsize = 2
*/

/********************************************************************
* Copyright 2012	
*
* This work is licensed under the 
* Creative Commons Attribution-ShareAlike 3.0 Unported License. 
* To view a copy of this license, visit 
* http://creativecommons.org/licenses/by-sa/3.0/ 
*
* or send a letter to
* Creative Commons
* 444 Castro Street
* Suite 900
* Mountain View, California, 94041
* USA
*/

// constants general
/********************************************************************
* NOTES :	 
*		Interrupts:
*			Board			int.0		int.1		int.2		int.3		int.4
*			Leonardo	 DP 3		 DP 2		 DP 0		 DP 1		 DP 7
*/

const int PIN_SPEED = 1;	// This is the Digital Pin, not the Interrupt
const int PIN_LEFT = 0;	 // This is the Digital Pin, not the Interrupt
const int PIN_RIGHT = 3;	// This is the Digital Pin, not the Interrupt
const int PIN_DEBUG = 7;
const int WHEEL = 27; // in Zoll

// constants speed -- opening in meter per second
const float SPEED_NORMAL = 6.16;
const float SPEED_FAST = 12.33;

// constant timeout - in millisecond
const int TIMEOUT_TIME = 2000;

// Debuging
bool DEBUG = 0;

// constants keyboard
const int KEYBOARD_LEFT = 216;
const int KEYBOARD_RIGHT = 215;
const int KEYBOARD_UP = 218;
const int KEYBOARD_PAGEUP = 211;
const int TABBI_SPEED = 69;

// globale variables
unsigned long LastIntervalTime = 0;
unsigned long ActualIntervalTime = 0;
float WheelDistanceInM = 0;
int ActualDirection = 0;
int NewDirection = 0;
float ActualSpeed = 0;

// forward declaration
void intTabbiSpeed();
float getSpeed(unsigned long rotation_start, unsigned long rotation_end);
int getDirection();
void setSpeedTime();
void setOutputSpeed(float meter_per_second);
void setOutputDirection(int direction);
void timeoutSpeed(unsigned long time);

/*F******************************************************************
* void
* setup()
*
* PURPOSE : init the arduino hardware
*
* RETURN :	void
*
* NOTES :	 
*		Interrupts:
*			Board			int.0		int.1		int.2		int.3		int.4
*			Leonardo	 DP 3		 DP 2		 DP 0		 DP 1		 DP 7
*F*/
void setup() {
	// init controlls
	Mouse.begin();
	Keyboard.begin();

	// init pins
	attachInterrupt(PIN_SPEED, intTabbiSpeed, FALLING);
	attachInterrupt(PIN_LEFT, setDirectionLeft, FALLING);
	attachInterrupt(PIN_RIGHT, setDirectionRight, FALLING);
	
	// init global vars
	WheelDistanceInM = 3.14 * 2.54 * WHEEL / 100;
}

/*F******************************************************************
* void
* loop()
*
* PURPOSE : lifetime
*
* RETURN :	void
*
* NOTES :	 none
*F*/
void loop() {

	if (digitalRead(PIN_DEBUG) == LOW)
		DEBUG = 1;
	else
		DEBUG = 0;

	setOutputDirection(getDirection());

	/* USE ALL THE INTERRUPTS !!!
	if (ActualIntervalTime != LastIntervalTime) {
		setOutputSpeed(getSpeed(LastIntervalTime, ActualIntervalTime));
		// New Intervall starts
		LastIntervalTime = ActualIntervalTime;
	} */

	timeoutSpeed(millis());
}
/*F******************************************************************
* void
* intTabbiSpeed()
*
* PURPOSE : sent the special Tabbi Key for MOOOORRRREEEE Speed
*
* RETURN :	void
*
* NOTES :	 cause @tabascoeye need this (30c3) only
*F*/
void intTabbiSpeed() {
	Keyboard.press(TABBI_SPEED);
	Keyboard.release(TABBI_SPEED);
}

/*F******************************************************************
* void
* setSpeedTime()
*
* PURPOSE : set the ActualIntervalTime
*
* RETURN :	void
*
* NOTES :	 none
*F*/
void setSpeedTime() {
	if(DEBUG)
		Keyboard.println("INTERRUPT - SPEED");
	
	ActualIntervalTime = millis();
}
/*F******************************************************************
* void
* setDirectionLeft()
*
* PURPOSE : set the Direction to Left
*
* RETURN :	void
*
* NOTES :	 none
*F*/
void setDirectionLeft() {	
	// If we are on the left Side right now, we go to the middle (1)
	if (ActualDirection == 0) {
		if (DEBUG)
			Keyboard.println("INTERRUPT - DIR FROM LEFT TO MID");
		
		NewDirection = 1;
	} else {
		if (DEBUG)
			Keyboard.println("INTERRUPT - DIR TO LEFT");
		
		NewDirection = 0;
	}
}
/*F******************************************************************
* void
* setDirectionRight()
*
* PURPOSE : set the Direction to Right
*
* RETURN :	void
*
* NOTES :	 none
*F*/
void setDirectionRight() {
	// If we are on the right Side right now, we go to the middle (1)
	if (ActualDirection == 2) {
		if (DEBUG)
			Keyboard.println("INTERRUPT - DIR FROM RIGHT TO MID");
		
		NewDirection = 1;
	} else {
		if (DEBUG)
			Keyboard.println("INTERRUPT - DIR TO RIGHT");
		
		NewDirection = 2;
	}
}

/*F******************************************************************
* float
* getSpeed( start, end )
*
* star		unsigned long; => rotation start in milliseconds
* end		 unsigned long; => rotation end in milliseconds 
*
* PURPOSE : calculating the speed of a wheel in meter per second
*
* RETURN :	float => speed in meter per second
*
* NOTES :	 none
*F*/
float getSpeed(unsigned long start_time, unsigned long end_time) {
	float second_per_cycle = (end_time - start_time) / 1000.0f;
	float cycle_per_second = 1 / second_per_cycle;

	float meter_per_second = cycle_per_second * WheelDistanceInM;
	return meter_per_second;
}

/*F******************************************************************
* int
* getDirection()
*
* PURPOSE : clean everything
*
* RETURN :	void
*
*F*/
int getDirection(){

	if (DEBUG) {
		delay(2000);
		Keyboard.print("Direction: ");
		Keyboard.println(NewDirection);
	}

	return NewDirection;
}

/*F******************************************************************
* void
* setOutputDirection()
* 0: left
* 1: middle
* 2: right
*
* RETURN :	void
*
*F*/
void setOutputDirection(int new_direction) {
	// From:		MID
	// to:			LEFT
	if ((ActualDirection == 1) && (new_direction == 0)) {
		// now we drive left
		if(!DEBUG)
				Keyboard.press(KEYBOARD_LEFT);
	}
	
	// From:		MID
	// to:			RIGHT
	if ((ActualDirection == 1) && (new_direction == 2)) {
		// now we drive right
		if(!DEBUG)
				Keyboard.press(KEYBOARD_RIGHT);
	}

	// From:		LEFT 
	// to:			RIGHT
	if ((ActualDirection == 0) && (new_direction == 2)) {
		// Release Left
		if(!DEBUG)
			Keyboard.release(KEYBOARD_LEFT);
		
		// now we drive right
		if(!DEBUG)
			Keyboard.press(KEYBOARD_RIGHT);
	}
	
	// From:		LEFT 
	// to:			MID
	if ((ActualDirection == 0) && (new_direction == 1)) {
		// Release Right
		if(!DEBUG)
			Keyboard.release(KEYBOARD_LEFT);
	}

	// From:		RIGHT
	// to:			LEFT
	if ((ActualDirection == 2) && (new_direction == 0)) {
		// Release Right
		if(!DEBUG)
			Keyboard.release(KEYBOARD_RIGHT);
		
		// now we drive left
		if(!DEBUG)
			Keyboard.press(KEYBOARD_LEFT);
	}
	
	// From:		RIGHT
	// to:			MID
	if ((ActualDirection == 2) && (new_direction == 1)) {
		// Release Right
		if(!DEBUG)
			Keyboard.release(KEYBOARD_RIGHT);
	}

	ActualDirection = new_direction;
}

/*F******************************************************************
* void
* setOutputSpeed()
*
* meter_per_second			float; => speed
*
* RETURN :	void
*
* NOTES :				 km/h				meter per second
*			slow =>		 0 to 15			 0 to 4.16
*			normal =>	15 to 30		4.16 to 8.33 
*			fast =>		30 to ???	 8.33 to ???
*			
*			speed				actual speed
*			----------------------------------
*			Slow		<=	Slow		==	nothing
*			Slow		<=	Normal	==	Release: Up
*			Slow		<=	Fast		==	Release: PageUp
*			Normal	<=	Slow		==	Press: Up
*			Normal	<=	Normal	==	nothing
*			Normal	<=	Fast		==	Release: PageUp; Press PageUp
*			Fast		<=	Slow		==	Press: PageUp
*			Fast		<=	Normal	==	Release: Up, Press PageUp
*			Fast		<=	Fast		==	nothing
*			
*F*/
void setOutputSpeed(float speed) {
	if (DEBUG) {
		Keyboard.print("Speed ");
		Keyboard.println(speed);
	}

	// SPEED : SLOW
	if (speed <= SPEED_NORMAL)	{
		if (DEBUG)
			Keyboard.println("Speed Slow");

		if (ActualSpeed <= SPEED_NORMAL) {
			// Slow		<=	Slow		==	nothing

		} else if (ActualSpeed <= SPEED_FAST) {
			// Slow		<=	Normal	==	Release: Up
			if(!DEBUG)
				Keyboard.release(KEYBOARD_UP);
		} else {
			// Slow		<=	Fast		==	Release: PageUp
			if(!DEBUG)
				Keyboard.release(KEYBOARD_PAGEUP);
		}

	// SPEED : NORMAL
	} else if (speed <= SPEED_FAST) {
		if (DEBUG)
			Keyboard.println("Speed Normal");

		if (ActualSpeed <= SPEED_NORMAL) {
			// Normal	<=	Slow		==	Press: Up
			if(!DEBUG)
				Keyboard.press(KEYBOARD_UP);
		} else if (ActualSpeed <= SPEED_FAST) {
			// Normal	<=	Normal	==	nothing

		} else {
			// Normal	<=	Fast		==	Release: PageUp; Press Up
			if(!DEBUG){
				Keyboard.release(KEYBOARD_PAGEUP);
				Keyboard.press(KEYBOARD_UP);
			}
		}

	// SPEED : FAST
	} else {
		if (DEBUG)
			Keyboard.println("Speed Fast");

		if (ActualSpeed <= SPEED_NORMAL) {
			// Fast		<=	Slow		==	Press: PageUp
			if(!DEBUG)
				Keyboard.press(KEYBOARD_PAGEUP);
		} else if (ActualSpeed <= SPEED_FAST) {
			// Fast		<=	Normal	==	Release: Up, Press PageUp
			if(!DEBUG){
				Keyboard.release(KEYBOARD_UP);
				Keyboard.press(KEYBOARD_PAGEUP);
			}
		} else {
			// Fast		<=	Fast		==	nothing

		}
	}

	ActualSpeed = speed;
}

/*F******************************************************************
* void
* timeout()
*
* PURPOSE : clean Speed
*
* RETURN :	void
*
*F*/
void timeoutSpeed(unsigned long timeout){
	if ((timeout - LastIntervalTime) > TIMEOUT_TIME) {
		if(!DEBUG){
			Keyboard.release(KEYBOARD_PAGEUP);
			Keyboard.release(KEYBOARD_UP);
		}
	}
}
