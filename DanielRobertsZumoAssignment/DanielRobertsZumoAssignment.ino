#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED 13
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  200 // microseconds
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     250 // 0 is stopped, 400 is full speed
#define TURN_SPEED        125
#define FORWARD_SPEED     150
#define REVERSE_DURATION  300 // ms
#define TURN_DURATION     900 // ms
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
String phase;
int hitCount;
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
   
  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);  
  delay(1000);
}
 
void setup()
{
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
   
  pinMode(LED, HIGH);
   
  waitForButtonAndCountDown();
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }
   

  sensors.read(sensor_values); //Read the current values for the sensor of the Zumo
  
  if (sensor_values[0] > QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    
    hitCount +=1; // If this  number is equal to 2(hit the same side twice) then a corner has been reached
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
   // if (hitCount>=2){
      //print("a Corner has been reached");
      //If I had the XBee's connected, this is where i would print a message
   // }
    
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED); // Go forward after turning
  }
 
  else if (sensor_values[5] > QTR_THRESHOLD) // else if the right sensor detects a line, turn to the left
  {
    
    hitCount+=1;  // If this  number is equal to 2(hit the same side twice) then a corner has been reached
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED); // Turn to the left by reversing the right tread and going forward on the left tread
    delay(TURN_DURATION);
   // if (hitCount>=2){
      //print("a Corner has been reached");
      // Here i would print the state of the Zumo, that it had reached a corner if my Xbee's were connected
   // }
  
    
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED); // go forward after turning
  }
  else
  {
    // otherwise, go straight
    hitCount=0; //reset the hitCount for detecting corners
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}
