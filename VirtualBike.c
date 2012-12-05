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
const int PIN_SPEED = 0; // Can be DigitalPin 0 to DigitalPin 3 
const int PIN_LEFT = A0;
const int PIN_RIGHT = A1;
const int WHEEL = 27; // in Zoll

// constants speed -- opening in meter per second
const float SPEED_NORMAL = 4.16;
const float SPEED_FAST = 8.33;

// constant timeout - in millisecond
const int TIMEOUT_TIME = 2000;

// constant diff Resistors
const int DIFF_RESISTOR = 25;

// Debuging
const bool DEBUG = 0;

// constants keyboard
const int KEYBOARD_LEFT = 216;
const int KEYBOARD_RIGHT = 215;
const int KEYBOARD_UP = 218;
const int KEYBOARD_PAGEUP = 211;

// globale variables
unsigned long LastIntervalTime = 0;
unsigned long ActualIntervalTime = 0;
float WheelDistanceInM = 0;
int ActualDirection = 0;
float ActualSpeed = 0;
int RightBorderValue = 0;
int LeftBorderValue = 0;

// forward declaration
float getSpeed(unsigned long rotation_start, unsigned long rotation_end);
int getDirection();
void setOutputSpeed(float meter_per_second);
void setOutputDirection(int direction);
void timeoutSpeed(unsigned long time);

/*F******************************************************************
* void
* setup()
*
* PURPOSE : init the arduino hardware
*
* RETURN :  void
*
* NOTES :   
*    Interrupts:
*      Board      int.0    int.1    int.2    int.3
*      Leonardo   DP 3     DP 2     DP 0     DP 1
*F*/
void setup() {
  // init controlls
  Mouse.begin();
  Keyboard.begin();
  
  // init pins
  int interrupt_pin;
  if (PIN_SPEED == 0) {
    interrupt_pin = 2;
  } else if (PIN_SPEED == 1) {
    interrupt_pin = 3;
  } else if (PIN_SPEED == 2) {
    interrupt_pin = 1;
  } else {
    interrupt_pin = 0;
  }
  
  attachInterrupt(interrupt_pin, setSpeedTime, FALLING);
  
  // init global vars
  WheelDistanceInM =  2 * 3.14 * 2.54 * WHEEL / 100;
  RightBorderValue = DIFF_RESISTOR;
  LeftBorderValue = 0 - DIFF_RESISTOR;
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
  setOutputDirection(getDirection());
  
  if (ActualIntervalTime != LastIntervalTime) {
    setOutputSpeed(getSpeed(LastIntervalTime, ActualIntervalTime);
    // New Intervall starts
    LastIntervalTime = ActualIntervalTime;
  }
  
  timeout(millis());
}

/*F******************************************************************
* void
* setSpeedTime()
*
* PURPOSE : set the ActualIntervalTime
*
* RETURN :  void
*
* NOTES :   none
*F*/
void setSpeedTime() {
  ActualIntervalTime = millis();
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
* RETURN :  void
*
*F*/
int getDirection(){
  
  if (DEBUG) {
    int debug_left = analogRead(PIN_LEFT);
    int debug_right = analogRead(PIN_RIGHT);
    delay(1000);
    Keyboard.print("Left ");
    Keyboard.println(debug_left);
    Keyboard.print("Right ");
    Keyboard.println(debug_right);
    Keyboard.print("Diff ")
    Keyboard.println(debug_left - debug_right);
  }
  
  return analogRead(PIN_LEFT) - analogRead(PIN_RIGHT);
}

/*F******************************************************************
* void
* setOutputDirection()
*
* RETURN :  void
*
*F*/
void setOutputDirection(int direction) {
  
  // if direction is really right and the actual direction is none or left
  if ((direction >= RightBorderValue) && (ActualDirection < RightBorderValue) {
    if (DEBUG) 
      Keyboard.println("Direction Right");
      
    // postiv  => right
    
    // actual  =>  dir
    // ---------------
    // Left    =>  Right
    // None    =>  Right
    
    // if actual dir is left
    if (ActualDirection <= LeftBorderValue)
      Keyboard.release(KEYBOARD_LEFT);
    
    // now we drive right
    Keyboard.press(KEYBOARD_RIGHT);
    
  } else if ((direction <= LeftBorderValue) && (ActualDirection > LeftBorderValue)){
    if (DEBUG) 
    Keyboard.println("Direction Left");
    
    // negativ => left
    
    // actual  =>  dir
    // ---------------
    // Right   =>  Left
    // None    =>  Left
    
    // if actual dir is right
    if (ActualDirection >= RightBorderValue)
      Keyboard.release(KEYBOARD_RIGHT);
    
    // now we drive left
    Keyboard.press(KEYBOARD_LEFT);
  
  } else {
    if (DEBUG) 
    Keyboard.println("Direction None");
    // no direction
    
    // actual  =>  dir
    // ---------------
    // Right   =>  None
    if (ActualDirection >= RightBorderValue)
      Keyboard.release(KEYBOARD_RIGHT);
    
    // actual  =>  dir
    // ---------------
    // Left    =>  None
    if (ActualDirection <= LeftBorderValue)
      Keyboard.release(KEYBOARD_LEFT);
      
    // actual  =>  dir
    // ---------------
    // Left    =>  Left
    // Right   =>  Right
    // None    =>  None
    
  }
  
  ActualDirection = direction;
}

/*F******************************************************************
* void
* setOutputSpeed()
*
* meter_per_second      float; => speed
*
* RETURN :  void
*
* NOTES :         km/h        meter per second
*      slow =>     0 to 15       0 to 4.16
*      normal =>  15 to 30    4.16 to 8.33 
*      fast =>    30 to ???   8.33 to ???
*      
*      speed        actual speed
*      ----------------------------------
*      Slow    <=  Slow    ==  nothing
*      Slow    <=  Normal  ==  Release: Up
*      Slow    <=  Fast    ==  Release: PageUp
*      Normal  <=  Slow    ==  Press: Up
*      Normal  <=  Normal  ==  nothing
*      Normal  <=  Fast    ==  Release: PageUp; Press PageUp
*      Fast    <=  Slow    ==  Press: PageUp
*      Fast    <=  Normal  ==  Release: Up, Press PageUp
*      Fast    <=  Fast    ==  nothing
*      
*F*/
void setOutputSpeed(float speed) {
  if (DEBUG) {
    Keyboard.print("Speed ");
    Keyboard.println(speed);
  }
  
  // SPEED : SLOW
  if (speed <= SPEED_NORMAL)  {
    if (DEBUG)
      Keyboard.println("Speed Slow");
    
    if (ActualSpeed <= SPEED_NORMAL) {
      // Slow    <=  Slow    ==  nothing
      
    } else if (ActualSpeed <= SPEED_FAST) {
      // Slow    <=  Normal  ==  Release: Up
      Keyboard.release(KEYBOARD_UP);
    } else {
      // Slow    <=  Fast    ==  Release: PageUp
      Keyboard.release(KEYBOARD_PAGEUP);
    }
  
  // SPEED : NORMAL
  } else if (speed <= SPEED_FAST) {
    if (DEBUG)
      Keyboard.println("Speed Normal");
    
    if (ActualSpeed <= SPEED_NORMAL) {
      // Normal  <=  Slow    ==  Press: Up
      Keyboard.press(KEYBOARD_UP);
    } else if (ActualSpeed <= SPEED_FAST) {
      // Normal  <=  Normal  ==  nothing
      
    } else {
      // Normal  <=  Fast    ==  Release: PageUp; Press Up
      Keyboard.release(KEYBOARD_PAGEUP);
      Keyboard.press(KEYBOARD_UP);
    }
  
  // SPEED : FAST
  } else {
    if (DEBUG)
      Keyboard.println("Speed Fast");
    
    if (ActualSpeed <= SPEED_NORMAL) {
      // Fast    <=  Slow    ==  Press: PageUp
      Keyboard.press(KEYBOARD_PAGEUP);
    } else if (ActualSpeed <= SPEED_FAST) {
      // Fast    <=  Normal  ==  Release: Up, Press PageUp
      Keyboard.release(KEYBOARD_UP);
      Keyboard.press(KEYBOARD_PAGEUP);
    } else {
      // Fast    <=  Fast    ==  nothing
      
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
* RETURN :  void
*
*F*/
void timeoutSpeed(unsigned long timeout){
  if ((timeout - RotationStart) > TIMEOUT_TIME) {
    Keyboard.release(KEYBOARD_PAGEUP);
    Keyboard.release(KEYBOARD_UP);
  }
}