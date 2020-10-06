#include <IRremote.h>
#include <Servo.h>
#define ON_OFF_KEY 0xFF629D
#define CIRCLE_KEY 0xFF18E7
#define CUSTOM_FUNCTION_KEY1 0xFF22DD
#define CUSTOM_FUNCTION_KEY2 0xFF02FD

Servo myservo1;
Servo myservo2;
int pos1=90;
int pos2=90;

const int RECV_PIN = 3;
const int redPin = 7;
const int greenPin = 6;
const int bluePin = 5;
int red = 0;  // Initial intensity
int green = 0;
int blue = 0;
int currentBrightness = 0;  // Initial active color intensity
String targetColor = "RED"; // Initial active color
bool isOn = false;
IRrecv irrecv(RECV_PIN);  // Initialize IR Library
decode_results results;   // Initialize IR Library

void setup() {
  Serial.begin(9600);   // Start Serial
  irrecv.enableIRIn();  // Start the receiver
  // Set the three LED Pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  myservo1.attach(9);
  myservo2.attach(10);
}

void loop() {
Serial.print(red);
Serial.print(green);
Serial.print(blue);
Serial.print('\n');
  
  if (irrecv.decode(&results)) {
    // Serial.println(results.value, HEX);  // Prints HEX code IR receiver receives
    irrecv.resume(); // Receive the next value
    // Power switch
    if (results.value == ON_OFF_KEY) {
      if (isOn == true) {
        Serial.println("Turning Off"); powerOff();
      }
      else if (isOn == false) {
        Serial.println("Turning On"); powerOn(); showCurrentActiveColor();
      }
    }
    // Color operations
    if (isOn == true) {
      if (results.value == CIRCLE_KEY) // Circle
        circle();
      else if (results.value == CUSTOM_FUNCTION_KEY1)  // Be creative with your own code
        toggleActiveColor1();
      else if (results.value == CUSTOM_FUNCTION_KEY2) 
        toggleActiveColor2();
    }
  }
  delay(100);
}

// Print current color setting on serial monitor
void showCurrentActiveColor() {
  Serial.print("Now controlling color ");
  Serial.println(targetColor);
}

// Turn Off LED
void powerOff() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  isOn = false;
}

// Turn on LED
void powerOn() {
  updateColor();
  analogWrite(redPin, 1);
  analogWrite(greenPin, 46);
  analogWrite(bluePin, 19);
  isOn = true;
}

// Toggle active color that you are changing brightness on
void toggleActiveColor1() {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 74);
  analogWrite(bluePin, 61);

  showCurrentActiveColor();
//open--------------------------------------------------------------------------------------------------------------------------------
  for (int pos1 = 90; pos1 >= 0; pos1 -= 2.25) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
//close
  for (int pos1 = 0; pos1 <= 90; pos1 += 2.25) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void toggleActiveColor2() {
  analogWrite(redPin, 61);
  analogWrite(greenPin, 97);
  analogWrite(bluePin, 255);
  
  showCurrentActiveColor();
//open
    for (int pos2 = 90; pos2 <= 180; pos2 += 2.25) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  //close
    for (int pos2 = 180; pos2 >= 90; pos2 -= 2.25) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// Update LED color to current setting
void updateColor() {
  if (targetColor == "RED")
    red = currentBrightness;
  else if (targetColor == "GREEN")
    green = currentBrightness;
  else if (targetColor == "BLUE")
    blue = currentBrightness;
  setColor(red, green, blue);
}

// Output desired voltages to LED
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Circle through different colors
void circle() {
  red = 255; //255
  green = 127; //127
  blue = 0;
  Serial.println("Start Circle");
  while (true) {
    red = (red + 25 + 256) % 256;
    green = (green + 25 + 256) % 256;
    blue = (blue + 25 + 256) % 256;
    setColor(red, green, blue);
    delay(150);
    if (irrecv.decode(&results)) {
      Serial.println("Stop Circle");
      irrecv.resume();
      return;
    }
  }
}
