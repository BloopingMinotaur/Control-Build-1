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
int l = 30;
int w = 15;

// Variables that control the radius of the circle.
float radius = 1.5;

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
  for (int i = 0; i < 1; i++) { // completes 1/2 of the rectangle six times to make three complete loops
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

void path_circle(float radius){ 
  int pwmL = 200;
  int pwmR = pwmL*radius;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
  //need to make this go only for a certain amount of time
}

void goforward(int distance) {
  int pwmR = 150;
  int pwmL = 150;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
  delay(distance * 50);
}

void turnleft() {
  int pwm = 150;
  motorR(pwm, 1);
  motorL(pwm, -1);
  delay(1000);
}

void stop() {
  motorR(0, 0);
  motorL(0, 0);
  delay(500);
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

  if (pushed) {
    goforward(20);
    stop();
//    turnleft();
//    stop();
  }
  

//  if (stateButton == true){  //plan on building this out to define the state of the robot. Want pathDecision to return a state from the 4 availible options (rectangle, circle, park left, park right)
//    pathCounter+=1;
//    pathDecision(pathCounter);
//  }
//
//  // switch structure that calls on the specific state as declared by pathDecision()
//  // this is where we will put our
//  switch(curr_state) {
//    case rectangle:
//      path_rectangle(w, l);
//      curr_state = still;
//      break;
//    case circle:
//      path_circle(radius);
//      curr_state = still;
//      break;
//    case parkLeft:
//
//      
//      break;
//    case parkRight:
//
//      
//      break;
//    case still:
//      stop();
//      break;
//  }
}
