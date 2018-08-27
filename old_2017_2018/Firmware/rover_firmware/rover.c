//Access control: CalSTAR MR

/* 
TO DO:
1. Movement loop
	a. PID / other closed loop control - ENCODERS
	b. Avoidance of obstacles using sensors
	c. Navigation - determination of distance from start
2. Servo library - replace analogWrite with servo library functions and appropriate angles.
*/

#include <arduino.h>
#include <NewPing.h>
#include <FiniteStateMachine.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>
#include <stdint.h>

//Arduino pin assignments   (L = 1, R = 2)
#define LEFT_SONAR_TRIG 20
#define LEFT_SONAR_ECHO 21
#define RIGHT_SONAR_TRIG 19
#define RIGHT_SONAR_ECHO 18
#define L_SKID_SERVO 14
#define R_SKID_SERVO 13
#define SOLAR_SERVO 15
#define L_MOTOR_DIR 2  //ESC1DIR 
#define R_MOTOR_DIR 1  //ESC2DIR 
#define R_MOTOR_PWM 4  //ESC2PWM
#define L_MOTOR_PWM 3  //ESC1PWM
#define L_ENCODER_B 27 
#define L_ENCODER_A 26
#define SOLAR_POT 25 
// #define SOLAR_ADC 15    ?? idk i couldnt find it
#define R_ENCODER_B 27
#define R_ENCODER_A 26
#define ACCEL_GYRO_SDA 17
#define ACCEL_GYRO_SCL 16

#define SERIAL_BAUD 19200

//Initialize sensor pins as sensors
NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING, 180);
NewPing rightSonar(RIGHT_SONAR_PING, RIGHT_SONAR_PING, 180);

//Create FSM and 3 states
State SkidDeployment = State(movementStop(), deployScan(), movementStop());
State Movement = State(deploySkids(), movementLoop(), movementStop());
State SolarDeployment = State(deploySolar(), readSolar(), movementStop());

FSM roverStateMachine = FSM(SkidDeployment);

void setup(){	
	Serial.begin(SERIAL_BAUD);
	pinMode(MOTOR1_DIR, OUTPUT);
	pinMode(MOTOR1_PWM, OUTPUT);
	pinMode(MOTOR2_DIR, OUTPUT);
	pinMode(MOTOR2_PWM, OUTPUT)
	pinMode(SKID1_SERVO, OUTPUT);
	pinMode(SKID2_SERVO, OUTPUT);
	pinMode(SOLAR_SERVO, OUTPUT);
	digitalWrite(L_MOTOR_DIR, LOW);
	digitalWrite(R_MOTOR_DIR, HIGH);
}

void loop(){
	roverStateMachine.update();
}

//######################## Transition Functions ###############################

//Deploys the skids once orientation is determined.
void deploySkids(){
	//YOUR CODE HERE
}

//Stops movement once radius is cleared.
void movementStop(){
	analogWrite(MOTOR1_PWM, 0);
	analogWrite(MOTOR1_PWM, 0);	
}

//Deploys solar panels once radius is cleared.
void deploySolar(){
	//YOUR CODE HERE
}

//####################### Update Functions ##########################################

//Scans for ground using ultrasonics, triggers skid deployment if distance from both is less than 5 in. Otherwise, rotate body.
void deployScan(){
	float left_ping = leftSonar.ping_in();
	float right_ping = rightSonar.ping_in();
	if (left_ping < 5 && right_ping < 5) {
		roverStateMachine.transitionTo(Movement);
	} else {
		//YOUR CODE HERE
	}
}

//Handles movement control, including feedback, obstacle avoidance, and distance measurement.
void movementLoop(){
	if(checkClearance()){
		roverStateMachine.transitionTo(SolarDeployment);		
	} else {
		//YOUR CODE HERE
	}
	
}

//Prints readings from solar sensors to serial port.
int readSolar(){
	float solarOutput = analogRead(SOLAR_ADC);
	float solarAngle = analogRead(SOLAR_POT);
	Serial.print("Solar ADC Output: ")
	Serial.println(solarOutput);
	Serial.print("Solar Potentiometer Output: ")
	Serial.println(solarAngle);
}

//############################## Utility Functions ###################################

//Takes in 0 or 1, sets motors to drive forward if 0, reverse if 1.
void setMotorDir(int direction){
	if (direction == 0 || direction == 1){
		digitalWrite(L_MOTOR_DIR, direction);
		digitalWrite(R_MOTOR_DIR, 1 - direction);
	} else {
		return;
	}
}

//Return True if rover is confident in clearing the radius, False otherwise.
bool checkClearance(){
	//YOUR CODE HERE
}