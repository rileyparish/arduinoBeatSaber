/*
Hey there! This code was originally written for our YouTube channel, https://www.youtube.com/RKadeGaming
We're glad that you've taken an interest in our project, and we hope that you have a good time building it!
We've made this code public, and you're free to use it however you like. If you end up sharing the project with others though, 
we'd appreciate some attribution so that people know where to find more stuff like this.
Thanks, and have fun! :)
*/

// this sketch is intended for the right-hand control

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

// the time that the last swipe was sent
unsigned long latestSwipe = millis();

void setup(){
    pinMode(upperLeft, INPUT);
    pinMode(upperRight, INPUT);
    pinMode(lowerLeft, INPUT);
    pinMode(lowerRight, INPUT);
}

void loop(){
    updateResistorStates();
    executeMovement();
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

}

void swipeRight(){

}

void swipeUp(){

}

void swipeDown(){

}