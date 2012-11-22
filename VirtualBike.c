/********************************************************************
* Name : User Shortcode
* Description : Drive trough Google Earth
* Version : 1.0
* Author : Sebastian Hansack, 
*/

//widerstände 12kohm bis 30 kohm

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

// constant timeout - in millisecond
const int TIMEOUT_TIME = 2000;

// Debuging
const bool DEBUG = 0;

// globale variables
unsigned long RotationStart = 0;
float WheelDistanceInM = 0;

// forward declaration
float getSpeed(unsigned long rotation_start, unsigned long rotation_end);
void setOutput(float meter_per_second);
void timeout(unsigned long time);

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
	WheelDistanceInM =  2 * 3.14 * 2.54 * WHEEL / 100;
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
		while (digitalRead(SPEED_PIN) == LOW){
			timeout(millis());
		} 
		// after phaseshift
		setOutput(getSpeed(RotationStart, millis()));
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
	float cycle_per_second = 1 / second_per_cycle;
	
	float meter_per_second = cycle_per_second * WheelDistanceInM;
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
*			fast => 	30 to ???	8.33 to ???
*F*/
void setOutput(float meter_per_second) {
	Keyboard.releaseAll();
	
	if (DEBUG) {
		Keyboard.print("Meter per Second  ");
		Keyboard.println(meter_per_second);
	}
	
	if (meter_per_second <=  SPEED_NORMAL) {
		// slow
		if (DEBUG) {
			Keyboard.println("slow");
		} else {	
			Keyboard.write(KEY_UP_ARROW);
		}
	} else if (meter_per_second <= SPEED_FAST) {
		// normal
		if (DEBUG) {
			Keyboard.println("normal");
		} else {	
			Keyboard.press(KEY_UP_ARROW);
		}
	} else {
		// fast
		if (DEBUG) {
			Keyboard.println("fast");
		} else {	
			Mouse.move(0, 0, -10);
		}
	}
}

/*F******************************************************************
* void
* timeout()
*
* PURPOSE : clean everything
*
* RETURN :  void
*
*F*/

void timeout(unsigned long time){
	if ((time - RotationStart) > TIMEOUT_TIME) {
		Keyboard.releaseAll();
	}
}