/* -------------------------------------------------------------------------------
* Authors: Nicolas Duley, Zakkaria Hales, Scott VandenToorn, Ciana Witherell
* Class: EGR10703
* Date: xx/xx/2019
* Instructor: Professor Stevens
* Program: Robo Sockey "The Boi" Final Code
* Description: At the heart of The Boi's code is a state machine. The potential
*              states are as follows: START, RANDOM, REVERSE, GOAL_FIND, TO_GOAL, FIRE, ONE80,
*              LEFT_90, RIGHT_90, SLIGHT_L and SLIGHT_R.
*
*              The Boi drives straight until it times out (5 seconds) or detects an 
*              obstacle/other robot with its IR or microswitch sensors. Upon obstacle
*              detection, The Boi will reverse and execute a random turn.
*
*              The Boi has a "waterwheel" that continuously spins forward in hopes 
*              of gathering a ball. If The Boi detects that it has gathered a ball 
*              it will go into goal finding mode. When in goal finding mode it will 
*              continue with its normal random movement until it detects the IR beam 
*              mounted above the goal with its array of IR recievers. Once it detects 
*              the IR beam, The Boi will turn toward the goal and drive until it is
*              underneath the goalpost. Once the upright facing IR sensors detect
*              that it is under the goal, it will enter scoring mode where the 
*              waterwheel servo reverses and the wheels reverse. After it has scored
*              the ball it will go back to random movement.
* -------------------------------------------------------------------------------- */


#include "robot.h"            //include custom library
#include <Servo.h>            //include servo library

Servo servoc;                 //water wheel servo

struct Robot robot;

void setup(){
  Serial.begin(9600);

  pinMode(R_PWM, OUTPUT);       //PWM right motor
  pinMode(L_PWM, OUTPUT);       //PWM left motor
  pinMode(L_PIN1, OUTPUT);      //left motor pin 1
  pinMode(L_PIN2, OUTPUT);      //left motor pin 2
  pinMode(R_PIN1, OUTPUT);      //right motor pin 1
  pinMode(R_PIN2, OUTPUT);      //right motor pin 2
  pinMode(switchPin, INPUT);    //right microswitch pin

  servoc.attach(SERVO_PIN);     //attach continuous servo to pin 11 of Arduino

  robot.sensors.button = 0;
}

void loop(){
  updateMotors(&robot);         //drives motors
  updateSensors(&robot);        //updates sensors
  servo_forward();              //drives servo forward

  if(robot.sensors.IR_ball < 6.35){
    goalTrigger = 1;
  }
  
  currentTime = millis();

  switch(robot.state){
    case START:
    /*Serial.println("START");
    Serial.println();*/

    start_function();
    
    robot.state = RANDOM;
    
    break;
    
    case RANDOM:
      /*Serial.println("RANDOM");
      Serial.println();*/
      
      robot.motors.leftPWM = SPEED_HIGH;
      robot.motors.rightPWM = SPEED_HIGH;

      if(robot.sensors.IR_ball < 6.35){
        goalTrigger = 1;
      }
      
      if(robot.sensors.button == LOW){   //hit something? reverse
        robot.state = REVERSE;
      }
      else if(robot.sensors.IR_front < 12.6){
          robot.state = REVERSE;        //something too close? reverse
      }
      else{
        if(goalTrigger == 1){
          robot.state = GOAL_FIND;
        }
        else{
          robot.state = RANDOM;         //continue random movement
        }
      }
      
      if(millisTrigger == 0){
        random_start = millis();
        millisTrigger = 1;
      }

      if(currentTime > (random_start + TIMEOUT)){
        robot.state = REVERSE;
        millisTrigger = 0;
      }
      
      if(robot.sensors.IR_top < 9.5 && robot.sensors.IR_top > 1){     //under goal? fire
          robot.state = FIRE;
      }

      if(goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      
      break;

    case GOAL_FIND:
      /*Serial.print("GOAL_FIND");
      Serial.println();*/
      updateGoalSensors(&robot);
      

      if(robot.sensors.IR_top < 12 && robot.sensors.IR_top > 1 ){     //under goal? fire
        robot.state = FIRE;
        break;
      }

      if(robot.sensors.IR_sideR == 0){
        digitalWrite(L_PIN1, LOW);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, LOW);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 0);
        delay(9);
        
        digitalWrite(L_PIN1, LOW);
        digitalWrite(L_PIN2, HIGH); 
        digitalWrite(R_PIN1, LOW);
        digitalWrite(R_PIN2, HIGH);
        analogWrite(R_PWM, 105);
        analogWrite(L_PWM, 105);
        delay(69);
        
        digitalWrite(L_PIN1, HIGH);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, LOW);
        digitalWrite(R_PIN2, HIGH);
        analogWrite(R_PWM, 96);
        analogWrite(L_PWM, 96);
        delay(709);
      }

      if(robot.sensors.IR_sideL == 0){
        digitalWrite(L_PIN1, LOW);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, LOW);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 0);
        delay(9);
        
        digitalWrite(L_PIN1, LOW);
        digitalWrite(L_PIN2, HIGH); 
        digitalWrite(R_PIN1, LOW);
        digitalWrite(R_PIN2, HIGH);
        analogWrite(R_PWM, 105);
        analogWrite(L_PWM, 105);
        delay(69);
        
        digitalWrite(L_PIN1, LOW);
        digitalWrite(L_PIN2, HIGH); 
        digitalWrite(R_PIN1, HIGH);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 96);
        analogWrite(L_PWM, 96);
        delay(709);
      } 
      
      if((robot.sensors.IR_1 < 900) && (robot.sensors.IR_1 < robot.sensors.IR_2) && (robot.sensors.IR_1 < robot.sensors.IR_3)){
        digitalWrite(L_PIN1, HIGH);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, HIGH);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 135);
        analogWrite(L_PWM, 65);
        
        delay(100);
        
        robot.motors.rightPWM = 135; 
        robot.motors.leftPWM = 65;
        
        tTrigger = 0;
      }
      else if((robot.sensors.IR_3 < 900) && (robot.sensors.IR_3 < robot.sensors.IR_2) && (robot.sensors.IR_3 < robot.sensors.IR_1)){
        digitalWrite(L_PIN1, HIGH);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, HIGH);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 65);
        analogWrite(L_PWM, 135);
        
        delay(100);
        
        robot.motors.rightPWM = 65; 
        robot.motors.leftPWM = 135;

        tTrigger = 0;
      }
      else if((robot.sensors.IR_2 < 900) && (robot.sensors.IR_2 < robot.sensors.IR_1) && (robot.sensors.IR_2 < robot.sensors.IR_3)){
        digitalWrite(L_PIN1, HIGH);
        digitalWrite(L_PIN2, LOW); 
        digitalWrite(R_PIN1, HIGH);
        digitalWrite(R_PIN2, LOW);
        analogWrite(R_PWM, 120);
        analogWrite(L_PWM, 120);
        
        delay(100);
        
        robot.motors.rightPWM = 120; 
        robot.motors.leftPWM = 120;

        tTrigger = 0;
      }
      
      if(robot.sensors.button == LOW){              //hit something? reverse
        robot.state = REVERSE;
      }
      else if(robot.sensors.IR_front < 12.6){;      //something too close? reverse
        if(robot.sensors.IR_1 > 90 || robot.sensors.IR_2 > 110 || robot.sensors.IR_3 > 90){
          robot.state = REVERSE;
        }
      }
      else{
        robot.state = GOAL_FIND;          //continue random movement
      }
      
      if(millisTrigger == 0){
        random_start = millis();
        millisTrigger = 1;
      }
      
      if(tTrigger == 1){
        if(currentTime > (random_start + TIMEOUT)){
          robot.state = REVERSE;
          millisTrigger = 0;
        }
      }
      
      if(robot.sensors.IR_top < 12 && robot.sensors.IR_top > 1 ){  //under goal? fire
          robot.state = FIRE;
      }

      tTrigger = 1;
      
      break;
      
    case LEFT_90:
      /*Serial.println("LEFT_90");
      Serial.println();*/
  
      left();
      
      if(robot.sensors.IR_ball < 6.35  || goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      else{
        robot.state = RANDOM;
      }
      
      break;
      
    case RIGHT_90:
      /*Serial.println("RIGHT_90");
      Serial.println();*/
  
      right();
      
      if(robot.sensors.IR_ball < 6.35  || goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      else{
        robot.state = RANDOM;
      }
      
      break;
      
    case REVERSE:   //reverses robot then randomly chooses a turn action
      /*Serial.println("REVERSE");
      Serial.println();*/
      
      reverse();
      
      randomNum = random(0, 5);   //will give numbers 0 through 4, NOT 5
      
      if(randomNum == 0){
          robot.state = LEFT_90;
      }
      else if(randomNum == 1){
          robot.state = SLIGHT_L;
      }
      else if(randomNum == 2){
          robot.state = RIGHT_90;
      }
      else if(randomNum == 3){
          robot.state = SLIGHT_R;
      }
      else{
        robot.state = ONE80;
      }

      millisTrigger = 0;
      
      break;

    case ONE80:
      /*Serial.println("ONE80");
      Serial.println();*/
      
      one80();

      if(robot.sensors.IR_ball < 6.35  || goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      else{
        robot.state = RANDOM;
      }
      
      break;
      
    case SLIGHT_L:
      /*Serial.println("SLIGHT_L");
      Serial.println();*/

      slightL();
      
      if(robot.sensors.IR_ball < 6.35  || goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      else{
        robot.state = RANDOM;
      }
      
      break;
      
    case SLIGHT_R:
      /*Serial.println("SLIGHT_R");
      Serial.println();*/

      slightR();
      
      if(robot.sensors.IR_ball < 6.35  || goalTrigger == 1){
        robot.state = GOAL_FIND;
      }
      else{
        robot.state = RANDOM;
      }
      
      break;
      
    case FIRE:
      /*Serial.println("FIRE");
      Serial.println();*/
      
      stop_bot();
      delay(25);

      servo_reverse();
      delay(20);
      reverse_goal();
      delay(134);
      
      stop_bot();
      delay(30);
      
      servo_reverse();
      reverse_goal();
      delay(1050);

      goalTrigger = 0;
      millisTrigger = 0;
      
      robot.state = RIGHT_90;

      break;
      
    default:
      /*Serial.println("DEFAULT ACTIVATED!!");
      Serial.println();*/
      
      robot.state = REVERSE;    //if unknown state, set state to reverse
  }

  if(robot.sensors.IR_ball < 6.35){
    goalTrigger = 1;
  }
}

void updateMotors(struct Robot *robot){
  if(robot->motors.leftPWM > 0){    //forward left motor
    digitalWrite(L_PIN1, HIGH);
    digitalWrite(L_PIN2, LOW);
  }
  else{                             //reverse left motor
    digitalWrite(L_PIN1, LOW);
    digitalWrite(L_PIN2, HIGH);
  }

  if(robot->motors.rightPWM > 0){   //forward right motor
    digitalWrite(R_PIN1, HIGH);
    digitalWrite(R_PIN2, LOW);
  }
  else{                             //reverse right motor
    digitalWrite(R_PIN1, LOW);
    digitalWrite(R_PIN2, HIGH);
  }

  analogWrite(L_PWM, abs(robot->motors.leftPWM) + 2);           //drive left motor
  analogWrite(R_PWM, abs(robot->motors.rightPWM));      //drive right motors
}

void updateSensors(struct Robot *robot){
  robot->sensors.button = digitalRead(switchPin);           //micro switch sensor
  
  volt_b = analogRead(BALL_IRPIN) * 0.0048828125;           
  temp_b = 13*pow(volt_b, -1);
  robot->sensors.IR_ball = abs(temp_b);

  volt_t = analogRead(TOP_IRPIN) * 0.0048828125;           //top left IR sensor (4 - 30 cm)
  temp_t = 13*pow(volt_t, -1);
  robot->sensors.IR_top = abs(temp_t);

  volt_f = analogRead(FRONT_IRPIN);                        //front IR sensor (digital)
  temp_f = (6787.0 / (volt_f - 3.0)) - 4.0;
  robot->sensors.IR_front = abs(temp_f);
}

void updateGoalSensors(struct Robot *robot){
  robot->sensors.IR_1 = analogRead(IR1_GOALPIN);        //left sensor
  robot->sensors.IR_2 = analogRead(IR2_GOALPIN);        //middle sensor
  robot->sensors.IR_3 = analogRead(IR3_GOALPIN);        //right sensor
  robot->sensors.IR_sideR = digitalRead(RSIDE_IR);      //right side sensor
  robot->sensors.IR_sideL = digitalRead(LSIDE_IR);      //left side sensor
}

void reverse(){
  /*Serial.println("reverse function");
  Serial.println();*/
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, LOW);
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, 0);
  delay(15);
  
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, HIGH); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, 160);
  analogWrite(L_PWM, 160);
  
  delay(83);
}

void reverse_goal(){
  /*Serial.println("reverse goal function");
  Serial.println();*/
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, LOW);
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, 0);
  delay(10);
  
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, HIGH); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, 100);    //changed from 130
  analogWrite(L_PWM, 100);
}

void servo_reverse(){
  /*Serial.println("servo_reverse function");
  Serial.println();*/
  
  servoc.write(196);          //changed from 170
}

void servo_forward(){
  /*Serial.println("servo_forward function");
  Serial.println();*/
  
  servoc.write(55);
}

void right(){
  /*Serial.println("right function");
  Serial.println();*/
      
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, SPEED_HIGH + 5);
  analogWrite(L_PWM, SPEED_MED + 5);
  
  delay(550);
}

void one80(){
  /*Serial.println("one80 function");
  Serial.println();*/
  
  digitalWrite(L_PIN1, HIGH);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, SPEED_HIGH);
  analogWrite(L_PWM, SPEED_HIGH);
  
  delay(969);
}

void left(){
  /*Serial.println("left function");
  Serial.println();*/
      
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, HIGH); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, LOW);
  analogWrite(R_PWM, SPEED_MED + 10);
  analogWrite(L_PWM, SPEED_HIGH + 10);
  
  delay(675);
}

void stop_bot(){
  /*Serial.println("stop_bot function");
  Serial.println();*/
  
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, LOW);
  analogWrite(R_PWM, SPEED_STOP);
  analogWrite(L_PWM, SPEED_STOP);
  
  servoc.write(90);
}

void start_function(){
  /*Serial.println("start_function");
  Serial.println();*/
  
  digitalWrite(L_PIN1, HIGH);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, 120);
  analogWrite(L_PWM, 120);

  delay(270);
}

void slightL(){
  /*Serial.println("slightL function");
  Serial.println();*/
  
  digitalWrite(L_PIN1, LOW);
  digitalWrite(L_PIN2, HIGH); 
  digitalWrite(R_PIN1, HIGH);
  digitalWrite(R_PIN2, LOW);
  analogWrite(R_PWM, SPEED_HIGH);
  analogWrite(L_PWM, SPEED_HIGH);
  
  delay(250);
}

void slightR(){
  /*Serial.println("slightR function");
  Serial.println();*/
  
  digitalWrite(L_PIN1, HIGH);
  digitalWrite(L_PIN2, LOW); 
  digitalWrite(R_PIN1, LOW);
  digitalWrite(R_PIN2, HIGH);
  analogWrite(R_PWM, SPEED_HIGH);
  analogWrite(L_PWM, SPEED_HIGH);
  
  delay(380);
}
