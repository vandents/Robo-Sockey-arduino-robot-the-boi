#define SPEED_HIGH 99             //fast motor speed
#define SPEED_MED 76              //medium motor speed
#define SPEED_STOP 0              //motor speed 0

#define R_PWM 5                   //motor pins for right motor
#define R_PIN1 10
#define R_PIN2 9

#define L_PWM 6                   //motor pins for left motor
#define L_PIN1 8
#define L_PIN2 7

#define SERVO_PIN 11              //waterwheel servo pin

#define switchPin 12              //Microswitch pin (Digital)

#define TOP_IRPIN 0               //right goal IR pin (facing up) [Anal]
#define BALL_IRPIN 2              //ball IR pin (Anal)
#define FRONT_IRPIN 1             //front IR pin (Anal)

#define IR1_GOALPIN 3             //pin for goal IR 1 (Anal)
#define IR2_GOALPIN 4             //pin for goal IR 2 (Anal)
#define IR3_GOALPIN 5             //pin for goal IR 3 (Anal)
#define RSIDE_IR 2                //pin for right side goal IR (digital)
#define LSIDE_IR 4                //pin for left side goal IR (digital)

//#define BASELINE 950              //threshold for front IR sensors

#define TIMEOUT 4769              //threshold for timeout (4.769 seconds)

int i = 0, j = 0, millisTrigger = 0, randomNum, goalTrigger = 0, tTrigger = 1;
unsigned long random_start = 0, currentTime = 0;

//variables used in IR sensor calculations:  tr = top right,  tl = top left,  f = front,  b = ball
double volt_t, volt_b, volt_f, temp_t, temp_b, temp_f;

struct robotMotors{
  int leftPWM;
  int rightPWM;
};

struct robotSensors{
  double IR_top;
  double IR_ball;     //IR sensor inside the ball chamber
  int IR_front;
  int button;         //microswitch trigger
  int IR_1;           //slightly left goal IR receiver
  int IR_2;           //middle goal IR receiver
  int IR_3;           //slightly right goal IR receiver
  int IR_sideR;       //left side IR receiver (digital)
  int IR_sideL;       //right side IR receiver (digital)     
};

enum states{
  START,
  RANDOM,
  ONE80,
  GOAL_FIND,
  TO_GOAL,
  LEFT_90,
  RIGHT_90,
  REVERSE,
  FIRE,
  SLIGHT_L,
  SLIGHT_R
};

struct Robot{
  struct robotMotors motors;
  struct robotSensors sensors;
  enum states state = START;
};

//function prototypes
void updateMotors(struct Robot *robot);
void updateSensors(struct Robot *robot);
void updateGoalSensors(struct Robot *robot);
