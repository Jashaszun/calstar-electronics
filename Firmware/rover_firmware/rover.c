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

//Arduino pin assignments
#define LEFT_SONAR_PING 2
#define SOLAR_SERVO 3
#define RIGHT_SONAR_PING 4
#define L_SKID_SERVO 5
#define R_SKID_SERVO 6
#define L_MOTOR_DIR 7
#define R_MOTOR_DIR 8
#define R_MOTOR_PWM 9
#define L_MOTOR_PWM 10
#define L_ENCODER_B 12
#define L_ENCODER_A 13
#define SOLAR_POT 14
#define SOLAR_ADC 15
#define R_ENCODER_B 16
#define R_ENCODER_A 17
#define ACCEL_GYRO_SDA 18
#define ACCEL_GYRO_SCL 19

#define SERIAL_BAUD 19200

NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING, 180);
NewPing rightSonar(RIGHT_SONAR_PING, RIGHT_SONAR_PING, 180);

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

//Transition Functions

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

//Update Functions

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

void movementLoop(){
	if(checkClearance()){
		roverStateMachine.transitionTo(SolarDeployment);		
	} else {
		//YOUR CODE HERE
	}
	
}

int readSolar(){
	float solarOutput = analogRead(SOLAR_ADC);
	float solarAngle = analogRead(SOLAR_POT);
	Serial.print("Solar ADC Output: ")
	Serial.println(solarOutput);
	Serial.print("Solar Potentiometer Output: ")
	Serial.println(solarAngle);
}

//utility

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