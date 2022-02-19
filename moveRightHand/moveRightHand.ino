/*
Hey there! This code was originally written for our YouTube channel, https://www.youtube.com/RKadeGaming
We're glad that you've taken an interest in our project, and we hope that you have a good time building it!
We've made this code public, and you're free to use it however you like. If you end up sharing the project with others though, 
we'd appreciate some attribution so that people know where to find more stuff like this.
Thanks, and have fun! :)
*/

#include <Mouse.h>
// #include <Keyboard.h>

// this sketch is intended for the right-hand control
/*
Right-hand settings within driver4VR:
    Yaw: 2
    Pitch: -4
    Driver sensitivity: 200

    Euler: x: 2.10 y: 136.15 z:-7.00 (these might not be replicable across reboots...)
*/

// since this script takes control of the mouse, I want to be able to disable control. The start pin must read LOW before the Arduino can control the mouse.
const int startPin = 13;

// the target is divided into 4 regions; a box with an "x" in the middle. When a region is hit with a sufficient amount of light, it will trigger a movement
const int topRegion = A0;
const int bottomRegion = A1;
const int leftRegion = A3; 
const int rightRegion = A5;

int sendDelay = 80;     // cursor movement instructions can only be sent so fast without some information being lost. 80 ms seems to be about the minimum delay between commands to work reliably
int moveDist = 125;     // move the cursor in chunks of 125 units (Mouse.move() takes an unsigned char and the operating range is [-128, 127])

// these store the initial ambient light value minus a tunable light level (to adjust sensitivity)
int upThreshold = 0;
int downThreshold = 0;
int leftThreshold = 0;
int rightThreshold = 0;
int lightDiff = 100;     // if a photoresistor is lower than the initial ambient light by this amount (meaning light was introduced), the it's time to activate a movement

// these pins are connected to simple buttons and are used for fine-tuning the position of the virtual lightsabers
const int tuneUpPin = 2;
const int tuneDownPin = 3;
const int tuneLeftPin = 4;
const int tuneRightPin = 5;
const int tuneAmount = 5;
const int clickPin = 6;

// this relay controls the LED strips
const int relayPin = 8;

void setup(){
    Serial.begin(115200);
    Mouse.begin();
    //  Mouse.move(LEFT-/RIGHT+, UP-/DOWN+);

    pinMode(startPin, INPUT_PULLUP);
    while(digitalRead(startPin) == HIGH){
        Serial.println("Waiting for startPin before continuing...");
        delay(300);
    }
    pinMode(topRegion, INPUT);
    pinMode(bottomRegion, INPUT);
    pinMode(leftRegion, INPUT);
    pinMode(rightRegion, INPUT);

    // set the threshold values for each photoresistor relative to the ambient light. If a reading is below the threshold, trigger a movement
    upThreshold = analogRead(topRegion) - lightDiff;
    downThreshold = analogRead(bottomRegion) - lightDiff;
    leftThreshold = analogRead(leftRegion) - lightDiff;
    rightThreshold = analogRead(rightRegion) - lightDiff;

    pinMode(tuneUpPin, INPUT_PULLUP);
    pinMode(tuneDownPin, INPUT_PULLUP);
    pinMode(tuneLeftPin, INPUT_PULLUP);
    pinMode(tuneRightPin, INPUT_PULLUP);
    pinMode(clickPin, INPUT_PULLUP);

    pinMode(relayPin, OUTPUT);
}

void loop(){
    checkFineTune();       // check the pins to see if the cursor is being fine-tuned at the moment.
    executeMovement();    
}

void checkFineTune(){
    // move the cursor a small amount (up/down, left/right) for fine-tuning the position of the handle
    if(digitalRead(tuneUpPin) == LOW){
        Mouse.move(0, -tuneAmount);
        delay(200);
    }
    if(digitalRead(tuneDownPin) == LOW){
        Mouse.move(0, tuneAmount);
        delay(200);
    }
    if(digitalRead(tuneLeftPin) == LOW){
        Mouse.move(-tuneAmount, 0);
        delay(200);
    }
    if(digitalRead(tuneRightPin) == LOW){
        Mouse.move(tuneAmount, 0);
        delay(200);
    }
    if(digitalRead(clickPin) == LOW){
        Mouse.click();
        delay(200);
    }   
}

// read from each of the photoresistors to determine which movement to execute
void executeMovement(){
    evaluateUpSwipe();
    evaluateDownSwipe();
    evaluateLeftSwipe();
    evaluateRightSwipe();
}

void evaluateUpSwipe(){
    // an upward swipe will contact the bottom region first
    if(analogRead(bottomRegion) < upThreshold){
        Serial.println("Swiping up");
        digitalWrite(relayPin, HIGH);
        swipeUp();
        digitalWrite(relayPin, LOW);
    }
}
void evaluateDownSwipe(){
    if(analogRead(topRegion) < downThreshold){
        Serial.println("Swiping down");
        digitalWrite(relayPin, HIGH);
        swipeDown();
        digitalWrite(relayPin, LOW);
    }
}
void evaluateLeftSwipe(){
    if(analogRead(rightRegion) < leftThreshold){
        Serial.println("Swiping left");
        digitalWrite(relayPin, HIGH);
        swipeLeft();
        digitalWrite(relayPin, LOW);
    }
}
void evaluateRightSwipe(){
    if(analogRead(leftRegion) < rightThreshold){
        Serial.println("Swiping right");
        digitalWrite(relayPin, HIGH);
        swipeRight();
        digitalWrite(relayPin, LOW);
    }
}

// euler values within driver4VR: -2.10 75.15 9.8
// describe the actual swipes
void swipeUp(){
    // for whatever reason, 100 feels better than the full 125 that the other operations use.
    // move diagonally down
    Mouse.move(100, 100);
    delay(sendDelay);

    // move straight upwards twice
    Mouse.move(0, -100);
    delay(sendDelay);
    Mouse.move(0, -100);
    delay(sendDelay);

    // move diagonally back down to the home position
    Mouse.move(-100, 100);
    delay(sendDelay);
}

void swipeDown(){
    // move diagonally up
    Mouse.move(moveDist, -moveDist);
    delay(sendDelay);

    // move straight downwards twice
    Mouse.move(0, moveDist);
    delay(sendDelay);
    Mouse.move(0, moveDist);
    delay(sendDelay);

    // move diagonally back to the home position
    Mouse.move(-moveDist, -moveDist);
    delay(sendDelay);
}

void swipeLeft(){
    Mouse.move(moveDist, 0);
    delay(sendDelay);
    // delay for a bit so I can complete the swipe from the left direction
    delay(200);
    Mouse.move(-moveDist, 0);
    delay(sendDelay);
}

void swipeRight(){
    // swipe right and come back to the home position
    Mouse.move(moveDist, 0);
    delay(sendDelay);
    Mouse.move(-moveDist, 0);
    delay(sendDelay);
    delay(sendDelay);
}