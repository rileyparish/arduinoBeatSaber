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

    Euler: x: 2.10 y: 136.15 z:-7.00
*/

// the target is divided into 4 regions; a box with an "x" in the middle. When a region is hit with a sufficient amount of light, it will trigger a movement
const int upRegion = A0;
const int downRegion = A1;
const int leftRegion = A2; 
const int rightRegion = A3;

const int lightThreshold = 300;     // when a resistor gets a reading that is smaller than this value, we want to record it (resistance decreases with light, I think)
const int activeDiff = 50;          // if the resistors were active within this time frame, then I want to register a swipe
const int swipeCooldown = 300;      // how long to wait between swipe commands (to prevent double-presses)

int sendDelay = 80;     // cursor movement instructions can only be sent so fast without some information being lost. 80 ms seems to be about the minimum delay between commands to work reliably
int moveDist = 125;     // move the cursor in chunks of 125 units (Mouse.move() takes an unsigned char and the operating range is [-128, 127])

// the time that the last swipe was sent
unsigned long latestSwipe = millis();

const int tempPin = 7;

void setup(){
    Serial.begin(115200);
    pinMode(upRegion, INPUT);
    pinMode(downRegion, INPUT);
    pinMode(leftRegion, INPUT);
    pinMode(rightRegion, INPUT);

    pinMode(tempPin, INPUT_PULLUP);
    Mouse.begin();
    //  Mouse.move(LEFT-/RIGHT+, UP-/DOWN+);

    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
}

void loop(){
    checkFineTune();       // check the pins to see if the cursor is being fine-tuned at the moment.
    // updateResistorStates();
    // executeMovement();

    if(digitalRead(tempPin) == 0){
        swipeUp();
        // swipeDown();
        // swipeLeft();
        // swipeRight();
        delay(400);
    }
    
}

void checkFineTune(){
    // move the cursor a small amount (up/down, left/right) for fine-tuning the position of the handle
    if(digitalRead(2) == 0){
        Mouse.move(0, -5);
        delay(200);
    }
    if(digitalRead(3) == 0){
        Mouse.move(0, 5);
        delay(200);
    }
    if(digitalRead(4) == 0){
        Mouse.move(-5, 0);
        delay(200);
    }
    if(digitalRead(5) == 0){
        Mouse.move(5, 0);
        delay(200);
    }
}

// based on the last time the photoresistors were active, determine if a 
void executeMovement(){
    // if it's been long enough since the last movement, evaluate for new movements
    if(millis() - latestSwipe < swipeCooldown){
        evaluateLeftSwipe();
        evaluateRightSwipe();
        evaluateUpSwipe();
        evaluateDownSwipe();
    }
}

// read the state of all of the resistor pins to determine if movement should be executed
void evaluateUpSwipe(){
    if(analogRead(upRegion < lightThreshold)){
        swipeUp();
    }
    // record the time that this swipe happened so we don't trigger multiple swipes unintentionally
    // it's ok to do this after the movement is complete because the swipes are "blocking" and no further reads happen while the swipe is being executed
    latestSwipe = millis();
}
void evaluateDownSwipe(){
    if(analogRead(downRegion < lightThreshold)){
        swipeDown();
    }
    latestSwipe = millis();
}
void evaluateLeftSwipe(){
    if(analogRead(leftRegion < lightThreshold)){
        swipeLeft();
    }
    latestSwipe = millis();
}
void evaluateRightSwipe(){
    // if the resistors have been activated within the threshold, 
    if(rightRegion < lightThreshold){
        swipeRight();
    }
    latestSwipe = millis();
}

// execute the actual swipes
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
}