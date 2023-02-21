// Right motor pin assignments
int pwmR = 3;
const int R1 = 5;
const int R2 = 6;

// Left motor pin assignments
int pwmL = 11;
const int L1 = 9;
const int L2 = 10;

//Joystick Pin assignments
#define joyX A0
#define joyY A1
const int button = 2;

// initializing the variable width and length for the rectangular path
int width_counter = 0;
int length_counter = 0;

// Variables that control the dimensions of the rectangle path
int l = 2; //units of seconds
int w = 1;

// Variables that control the radius of the circle.
float radius = 1.5; //ratio between PWM of Left motor and Right Motor
int circle_Time = 10; //units of seconds

// Variables that control the Parellel Parking to the Left of the Robot
int SideDistanceLeft = -1; // variable that assigns how far to the left from the robot the desired parking spot is
int ForwardDistanceLeft = 1; // variable that assigns how far infront (+) or behind (-) the robot is from the spot

// Variables that control the Parellel Parking to the Right of the Robot
int SideDistanceRight = 1; // variable that assigns how far to the right from the robot the desired parking spot is
int ForwardDistanceRight = 1; // variable that assigns how far infront (+) or behind (-) the robot is from the spot

// Defines the various states of the robot
enum state {
  rectangle,
  circle,
  parkLeft,
  parkRight,
  still
};
state curr_state;
// Functions to control motor output

// 1 means backward
// 0 means stop
// -1 means forward
void motorR(int speed, int direction) {
  analogWrite(pwmR, speed);

  if (direction == 1) {
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
  } else if (direction == -1) {
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
  } else if (direction == 0) {
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
}

void motorL(int speed, int direction) {
  analogWrite(pwmL, speed);

  if (direction == 1) {
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
  } else if (direction == -1) {
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
  } else if (direction == 0) {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
  }
}

void path_rectangle(int w, int l) {
  for (int i = 0; i < 5; i++) { // completes 1/2 of the rectangle six times to make three complete loops
    goforward(l);
    stop();
    turnleft();
    stop();
    goforward(w);
    stop();
    turnleft();
    stop();
  }
}

void path_circle(float radius, int time){ 
  int pwmL = 125;
  int pwmR = pwmL*radius;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
  delay(time*1000); //Takes input as seconds and then converts to miliseconds
  
  //need to make this go only for a certain amount of time
}

void goforward(int distance) {
  int pwmR = 150;
  int pwmL = 150;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
  delay(distance * 1000);
}
void gobackwards(int distance) {
  int pwmR = 150;
  int pwmL = 150;
  motorR(pwmR, 1);
  motorL(pwmL, 1);
  delay(distance * 1000);
}

void turnleft() {
  int pwm = 250;
  motorR(pwm, 1);
  motorL(pwm, -1);
  delay(750);
}

void turnright() {
  int pwm = 250;
  motorR(pwm, -1);
  motorL(pwm, 1);
  delay(750);
}

void stop() {
  motorR(0, 0);
  motorL(0, 0);
  delay(500);
}

void path_parkLeft(int SideDistanceLeft,int ForwardDistanceLeft){
  if (ForwardDistanceLeft>0){
    goforward(ForwardDistanceLeft);
  } else {
    gobackwards(ForwardDistanceLeft*-1); //removes the negative from the distance value to feed a postive time value into the gobackwards function
  }
  stop();
  turnleft();
  stop();
  goforward(SideDistanceLeft);
  stop();
  turnright();
  stop();
}

void path_parkRight(int SideDistanceRight,int ForwardDistanceRight){
  if (ForwardDistanceRight>0){
    goforward(ForwardDistanceRight);
  } else {
    gobackwards(ForwardDistanceRight*-1); //removes the negative from the distance value to feed a postive time value into the gobackwards function
  }
  stop();
  
  turnright();  stop();
  goforward(SideDistanceRight);  stop();
  turnleft();  stop();
}

bool isPushed(int pushed){
  if(pushed == 0){
    return true;
  } else{
    return false;
  }
}

void pathDecision(int counter){
 if (counter == 1){
    curr_state = rectangle;
  }else if (counter == 2){
    curr_state = circle;
  } else if (counter == 3){
    curr_state = parkLeft;
  } else if (counter == 4){
    curr_state = parkRight;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(pwmL, OUTPUT);  //sets the left motor pin as a PWM output
  pinMode(L1, OUTPUT);    // sets the logic pins for motor 1
  pinMode(L2, OUTPUT);

  pinMode(pwmR, OUTPUT);  //sets the right motor pin as a PWM output
  pinMode(R1, OUTPUT);    // sets the logic pins for motor 2
  pinMode(R2, OUTPUT);

  pinMode(button, INPUT_PULLUP);
  curr_state = still;
}


int pathCounter = 0;
void loop() {
  int buttonVal = digitalRead(button);
  //Serial.println(buttonVal);
  bool pushed = isPushed(buttonVal); //returns the state of the button in true for pressed and false for unpressed
  Serial.println(pushed);
  // This is for debugging the use of the push button and checking that the stop function works
  // if (pushed) {
  //   goforward(20);
  //   stop();
  //   turnleft();
  //   stop();
  // }
  

   if (pushed){  //plan on building this out to define the state of the robot. Want pathDecision to return a state from the 4 availible options (rectangle, circle, park left, park right)
     pathCounter+=1;
     pathDecision(pathCounter);
    }

 // switch structure that calls on the specific state as declared by pathDecision()
 // Each Case calls the movement function coresponding to the case. After the movement function is complete, it turns the motors off then assigns the case still to ensure that it doesn't repeat the case next time through the loop.
 switch(curr_state) {
   case rectangle:
     path_rectangle(w, l);
     stop();
     curr_state = still;
     break;
   case circle:
     path_circle(radius,circle_Time);
     stop();
     curr_state = still;
     break;
   case parkLeft:
     path_parkLeft(SideDistanceLeft, ForwardDistanceLeft);
     curr_state = still;
     stop();
     break;
   case parkRight:
      path_parkRight(SideDistanceRight, ForwardDistanceRight);
     curr_state = still;
     stop();
     break;
   case still:
     stop();
     break;
  }
}
