/********************************************************************
/********************************************************************
* Name : 			User Shortcode
* Description : 	Drive trough Google Earth
* Version : 		1.0 (Special 30c3 Release)
* Author : 			Sebastian Hansack
* Note : 			* This is a special Release intended to be shown on the 30c3
* 					* BEST VIEW: Tabsize = 4
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
*		Board		int.0	int.1	int.2	int.3	int.4
*		Leonardo	DP 3	DP 2	DP 0	DP 1	DP 7
*/

// This is the Digital Pin, not the Interrupt
const int PIN_LEFT = 0;
const int PIN_CENTER = 1;
const int PIN_RIGHT = 2;

const int PIN_SPEED = 3;
const int PIN_DEBUG = 7;

// Debuging
bool DEBUG = 0;

// constants keyboard
const int KEYBOARD_LEFT = 216;
const int KEYBOARD_RIGHT = 215;
const int KEYBOARD_SPEED = 69;

// globale variables
int ActualDirection = 0;
int NewDirection = 0;

/*F******************************************************************
* void
* setup()
*
* PURPOSE :	init the arduino hardware
*
* RETURN :	void
*F*/
void setup() {
  // init controlls
  Mouse.begin();
  Keyboard.begin();

  // init pins
	attachInterrupt(PIN_LEFT, setDirectionLeft, FALLING);
	attachInterrupt(PIN_CENTER, setDirectionCenter, FALLING);
	attachInterrupt(PIN_RIGHT, setDirectionRight, FALLING);

	attachInterrupt(PIN_SPEED, intSpeedKey, FALLING);
	attachInterrupt(PIN_DEBUG, setDebugMode, CHANGE);
}

/*F******************************************************************
* void
* loop()
*
* PURPOSE :	lifetime
*
* RETURN :	void
*
* NOTES :	none
*F*/
void loop() {

}
/*F******************************************************************
* void
* intSpeedKey()
*
* PURPOSE :	sent the special Tabbi Key for MOOOORRRREEEE Speed
*
* RETURN :	void
*
* NOTES :	cause @tabascoeye need this (30c3) only
*F*/
void setDebugMode() {
	if (DEBUG == 1) {
		Keyboard.println("DEBUG OFF");
		DEBUG = 0;
	} else if (DEBUG == 0) {
		Keyboard.println("DEBUG ON");
		DEBUG = 1;
	}
}
/*F******************************************************************
* void
* intSpeedKey()
*
* PURPOSE :	sent the special Tabbi Key for MOOOORRRREEEE Speed
*
* RETURN :	void
*
* NOTES :	cause @tabascoeye need this (30c3) only
*F*/
void intSpeedKey() {
	Keyboard.write(KEYBOARD_SPEED);
	setOutputDirection(NewDirection);
}
/*F******************************************************************
* void
* setDirectionLeft()
*
* PURPOSE :	set the Direction to Left
*
* RETURN :	void
*
* NOTES :	none
*F*/
void setDirectionLeft() {
	if ((ActualDirection == 1) && (DEBUG)) {
		Keyboard.println("DIR Center to Left");
	} else if ((ActualDirection == 2) && (DEBUG)) {
		Keyboard.println("DIR Right to Left");
	}

	NewDirection = 0;
}
/*F******************************************************************
* void
* setDirectionRight()
*
* PURPOSE :	set the Direction to Right
*
* RETURN :	void
*
* NOTES :	none
*F*/
void setDirectionRight() {
	if ((ActualDirection == 0) && (DEBUG)) {
		Keyboard.println("DIR Left to Right");
	} else if ((ActualDirection == 1) && (DEBUG)) {
		Keyboard.println("DIR Center to Right");
	}

	NewDirection = 2;
}
/*F******************************************************************
* void
* setDirectionCenter()
*
* PURPOSE :	set the Direction to Center
*
* RETURN :	void
*
* NOTES :	none
*F*/
void setDirectionCenter() {
	if ((ActualDirection == 2) && (DEBUG)) {
		Keyboard.println("DIR Right to Center");
	} else if ((ActualDirection == 0) && (DEBUG)) {
		Keyboard.println("DIR Left to Center");
	}

	NewDirection = 1;
}
/*F******************************************************************
* void
* setOutputDirection()
* 0 :	left
* 1 :	middle
* 2 :	right
*
* RETURN :	void
*F*/
void setOutputDirection(int new_direction) {
	if ((new_direction == 0) && (!DEBUG)) {
		Keyboard.write(KEYBOARD_LEFT);
	}

	if ((new_direction == 1) && (!DEBUG)) {
		// nothing
	}

	if ((new_direction == 2) && (!DEBUG)) {
		Keyboard.write(KEYBOARD_RIGHT);
	}

	ActualDirection = new_direction;
}
