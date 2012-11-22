/********************************************************************
* Name : User Shortcode
* Description : Drive trough Google Earth
* Version : 1.0
* Author : Sebastian Hansack, 
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
const int SPEED_PIN = 7; // Messsonde 
const int WHEEL = 27; // in Zoll

// constants speed -- opening in meter per second
const float SPEED_NORMAL = 4.16;
const float SPEED_FAST = 8.33;

// globale variables
unsigned long RotationStart = 0;
float WheelDistanceInM = 0;

// forward declaration
float getSpeed(unsigned long start, unsigned long end);

/*F******************************************************************
* void
* setup()
*
* PURPOSE : init the arduino hardware
*
* RETURN :  void
*
* NOTES :   none
*F*/
void setup() {
	// init controlls
	Mouse.begin();
	Keyboard.begin();
	
	// init pins
	pinMode(SPEED_PIN, INPUT);
	
	// init global vars
	WheelDistanceInM = 3.14 * 2.54 * WHEEL / 100;
}

/*F******************************************************************
* void
* loop()
*
* PURPOSE : lifetime
*
* RETURN :  void
*
* NOTES :   none
*F*/
void loop() {
	if (digitalRead(SPEED_PIN) == LOW) {
		while (digitalRead(SPEED_PIN) == LOW){} 
		// after phaseshift
		getSpeed(RotationStart, millis());
	}
	
	// steer the bicycle
	
}

/*F******************************************************************
* float
* getSpeed( start, end )
*
* star    unsigned long; => rotation start in milliseconds
* end     unsigned long; => rotation end in milliseconds 
*
* PURPOSE : calculating the speed of a wheel in meter per second
*
* RETURN :  float => speed in meter per second
*
* NOTES :   none
*F*/
float getSpeed(unsigned long rotation_start, unsigned long rotation_end) {
	// next rotation start is this rotation end
	RotationStart = rotation_end;
	
	float second_per_cycle = (rotation_end - rotation_start) / 1000.0f;
	float cycle_per_second = 1 / secondPerCycle;
	
	float meter_per_second = second_per_cycle * WheelDistanceInM;
	return meter_per_second;
}

/*F******************************************************************
* float
* setOutput( meter_per_second )
*
* meter_per_second      float; => speed 
*
* PURPOSE : calculating the speed of a wheel in meter per second
*
* RETURN :  void
*
* NOTES :				km/h		meter per second
*			slow =>		0 to 15		0 to 4.16
*			normal => 	15 to 30	4.16 to 8.33 
*			fast => 	0 to ???	8.33 to ???
*F*/
void setOutput(float meter_per_second) {
	if (meter_per_second <=  SPEED_NORMAL) {
		// slow
	} else if (meter_per_second <= SPEED_FAST) {
		// normal
		Keyboard.write('w');
	} else {
		// fast
		Mouse.move(0, 0, 10);
	}
}


