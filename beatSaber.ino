/*
Hey there! This code was originally written for our YouTube channel, https://www.youtube.com/RKadeGaming
We're glad that you've taken an interest in our project, and we hope that you have a good time building it!
We've made this code public, and you're free to use it however you like. If you end up sharing the project with others though, 
we'd appreciate some attribution so that people know where to find more stuff like this.
Thanks, and have fun! :)
*/

// this sketch is intended for the right-hand control
/*
Right-hand settings within driver4VR:
    Yaw: 2
    Pitch: -4
    Driver sensitivity: 200

    Euler: x: 2.10 y: 136.15 z:-7.00
*/

const int upperLeft = A0;
const int upperRight = A1;
const int lowerLeft = A2; 
const int lowerRight = A3;

const int lightThreshold = 300;     // when a resistor gets a reading that is smaller than this value, we want to record it (resistance decreases with light, I think)
const int activeDiff = 50;          // if the resistors were active within this time frame, then I want to register a swipe
const int swipeCooldown = 500;      // how long to wait between swipe commands (to prevent double-presses)

unsigned long upperLeftLastActive = millis();
unsigned long upperRightLastActive = millis();
unsigned long lowerLeftLastActive = millis();
unsigned long lowerRightLastActive = millis();

int sendDelay = 80;     // cursor movement instructions can only be sent so fast without some information being lost. 80 ms seems to be about the minimum delay between commands to work reliably
int moveDist = 125;     // move the cursor in chunks of 125 units (Mouse.move() takes an unsigned char and the operating range is [-128, 127] which is not large enough to register full directional swipes)

// the time that the last swipe was sent
unsigned long latestSwipe = millis();

const int tempPin = 7;

void setup(){
    Serial.begin(115200);
    pinMode(upperLeft, INPUT);
    pinMode(upperRight, INPUT);
    pinMode(lowerLeft, INPUT);
    pinMode(lowerRight, INPUT);

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

// read from all the resistors and update the history based on the readings
void updateResistorStates(){
    if(analogRead(upperLeft) < lightThreshold){
        upperLeftLastActive = millis();
    }
    if(analogRead(upperRight) < lightThreshold){
        upperRightLastActive = millis();
    }
    if(analogRead(lowerLeft) < lightThreshold){
        lowerLeftLastActive = millis();
    }
    if(analogRead(lowerRight) < lightThreshold){
        lowerRightLastActive = millis();
    }
}

// based on the resistor histories, should a left swipe be acivated?
bool evaluateLeftSwipe(){
    if(upperLeftLastActive - upperRightLastActive < activeDiff || lowerLeftLastActive - lowerRightLastActive < activeDiff){
        swipeLeft();
    }
    latestSwipe = millis();
}
bool evaluateRightSwipe(){
    // if the resistors have been activated within the threshold, 
    if(upperRightLastActive - upperLeftLastActive < activeDiff || lowerRightLastActive - lowerLeftLastActive < activeDiff){
        swipeRight();
    }
    latestSwipe = millis();
}
bool evaluateUpSwipe(){
    if(upperLeftLastActive - lowerLeftLastActive < activeDiff || upperRightLastActive - lowerRightLastActive < activeDiff){
        swipeUp();
    }
    latestSwipe = millis();
}
bool evaluateDownSwipe(){
    if(lowerLeftLastActive - upperLeftLastActive < activeDiff || lowerRightLastActive - upperRightLastActive < activeDiff){
        swipeDown();
    }
    latestSwipe = millis();
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