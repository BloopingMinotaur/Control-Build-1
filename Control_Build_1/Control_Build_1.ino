// Right motor pin assignments
int pwmR = 3;
const int L1 = 5;
const int L2 = 6;

// Left motor pin assignments
int pwmL = 11;
const int R1 = 9;
const int R2 = 10;

//Joystick Pin assignments
#define joyX A0
#define joyY A1

// initializing the variable width and length for the rectangular path
int width_counter = 0;
int length_counter = 0;

// Variables that control the dimensions of the rectangle path
int length = 2;
int width = 2;

// Variables that control the radius of the circle.
float radius = 1.5;

// Functions to control motor output

// 1 means backward
// 0 means stop
// -1 means forward
void motorR(int speed, int direction) {
  analogWrite(pwmR, speed);

  if (direction == -1) {
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
  } else if (direction == 1) {
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
  } else if (direction == 0) {
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
}

void motorL(int speed, int direction) {
  analogWrite(pwmL, speed);

  if (direction == -1) {
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
  } else if (direction == 1) {
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
  } else if (direction == 0) {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
  }
}

void path_rectangle(int width, int length) {
  for (int i = 0; i < 5; i++) { // completes 1/2 of the rectangle six times to make three complete loops
    goforward(length);
    stop();
    turnleft();
    stop();
    goforward(width);
    stop();
    turnleft();
    stop();
  }
}

void path_circle(float radius){
  int pwmL = 150;
  int pwmR = pwmL*radius;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
}

void goforward(int distance) {
  int pwmR = 150;
  int pwmL = 150;
  motorR(pwmR, -1);
  motorL(pwmL, -1);
  delay(distance * 1000);
}

void turnleft() {
  int pwm = 150;
  motorR(pwm, -1);
  motorL(pwm, 0);
  delay(1000);
}

void stop() {
  motorR(0, 0);
  motorL(0, 0);
  delay(500);
}

void setup() {
  Serial.begin(115200);

  pinMode(pwmL, OUTPUT);  //sets the left motor pin as a PWM output
  pinMode(L1, OUTPUT);    // sets the logic pins for motor 1
  pinMode(L2, OUTPUT);

  pinMode(pwmR, OUTPUT);  //sets the right motor pin as a PWM output
  pinMode(R1, OUTPUT);    // sets the logic pins for motor 2
  pinMode(R2, OUTPUT);
}

void loop() {
  path_rectangle(width, length);
  path_circle(radius);
  //function for parellel parking
}
