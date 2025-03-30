// Create a constant macro for each LED pin
// Change the number to match the pin each LED is connected to
#define LED_1 11
#define LED_2 10
#define LED_3 9
#define LED_4 6

int brightness1 = 0;
int brightness2 = 0;
int pair = 0;
int pattern = 0;
int speed = 0;


// The code inside setup() will only run once when the Arduino first receives power or new software is flashed.
void setup() {
  Serial.begin(9600);
  // There is nothing to do here.  The pinmode and other initialization stuff will happen in the
  // initilization state for the LED control task.
}



// These are our variables and constants that control each state
#define STATE_INIT 0

#define STATE_111 111
#define STATE_112 112
#define STATE_211 211
#define STATE_212 212

#define STATE_121 121
#define STATE_122 122
#define STATE_221 221
#define STATE_222 222

unsigned long Pair1Pattern1Stage1_duration = 250;
unsigned long Pair1Pattern1Stage2_duration = 250;
unsigned long Pair2Pattern1Stage1_duration = 250;
unsigned long Pair2Pattern1Stage2_duration = 250;

unsigned long Pair1Pattern2Stage1_duration = 10;
unsigned long Pair1Pattern2Stage2_duration = 10;
unsigned long Pair2Pattern2Stage1_duration = 10;
unsigned long Pair2Pattern2Stage2_duration = 10;

// The state variable
int state_LEDPairTask_1 = STATE_INIT; // We start in the INIT state
int state_LEDPairTask_2 = STATE_INIT; // 

// We need to store what time a new pattern state was entered.
unsigned long pattern_state_enterance_time_1 = 0;
unsigned long pattern_state_enterance_time_2 = 0;

void CLITask(){
  if (Serial.available() > 0) {
    // Read the entire input line
    String input = Serial.readString();
    
    // Trim any leading or trailing whitespace
    input.trim();

    // Split the input string by spaces
    int firstSpace = input.indexOf(' ');
    int secondSpace = input.indexOf(' ', firstSpace + 1);

    // Extract the individual parameters from the input string
    if (firstSpace > 0 && secondSpace > firstSpace) {
      pair = input.substring(0, firstSpace).toInt();
      pattern = input.substring(firstSpace + 1, secondSpace).toInt();
      speed = input.substring(secondSpace + 1).toInt(); 
      }
  if (pair == 1 && pattern == 1){
       state_LEDPairTask_1 = STATE_111;
       Pair1Pattern1Stage1_duration = speed;
       Pair1Pattern1Stage2_duration = speed;
      }
  else if (pair == 1 && pattern == 2){
       state_LEDPairTask_1 = STATE_121;
       Pair1Pattern2Stage1_duration = speed;
       Pair1Pattern2Stage2_duration = speed;
      }
  else if (pair == 2 && pattern == 1){
       state_LEDPairTask_2 = STATE_211;
       Pair2Pattern1Stage1_duration = speed;
       Pair2Pattern1Stage2_duration = speed;
      }
  else if (pair == 2 && pattern == 2){
      state_LEDPairTask_2 = STATE_221;
       Pair2Pattern2Stage1_duration = speed;
       Pair2Pattern2Stage2_duration = speed;
      }
 }
}
void LEDPairTask_1(){

  // The task will start in state 0 (STATE_INIT). This will do any initialization that needs to happen.
  if(state_LEDPairTask_1 == STATE_INIT){
    // Start by setting each LED pin as OUTPUT
    // Note that all pins default to INPUT unless otherwise changed.
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);

    // Now turn the pin off (LOW), so we start running our code in a known hardware configuration and state
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);

    // We are done initializing. Set the state to PATTERN_1
    // Because we are tracking duration in the next state, we also need to save this exact time so we can
    // compare against it later.
    //state_LEDPairTask_1 = STATE_121;
    pattern_state_enterance_time_1 = millis();

    // Now we return, so the LEDPairTask_1 function exits.
    return;
  }
  
  //Pair 1 Pattern 1 Stage 1
  else if (state_LEDPairTask_1 == STATE_111){
    // Set LED 1 is ON, LED 2 is OFF
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);

    // Now we want to see if we have been in this state long enough to transition to the next state
    if(pattern_state_enterance_time_1 + Pair1Pattern1Stage1_duration <= millis()){
      // If we get here, then it's time to transition to the next state.

      // Set the state variable to the next state.
      state_LEDPairTask_1 = STATE_112;
      // Save the time that the transition happened.
      pattern_state_enterance_time_1 = millis();
    }
    // And now return so other tasks (if they exist) can run.
    return;
  }
  //Pair 1 Pattern 1 Stage 2
  else if (state_LEDPairTask_1 == STATE_112){
    // Set LED 1 is OFF, LED 2 is ON
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    // Now we want to see if we have been in this state long enough to transition to the next state
    if(pattern_state_enterance_time_1 + Pair1Pattern1Stage2_duration <= millis()){
      // If we get here, then it's time to transition to the next state.

      // Set the state variable to the next state.
      state_LEDPairTask_1 = STATE_111;
      // Save the time that the transition happened.
      pattern_state_enterance_time_1 = millis();
    }
    // And now return so other tasks (if they exist) can run.
    return;
    
  }
  //Pair 1 Pattern 2 Stage 1
  else if (state_LEDPairTask_1 == STATE_121){
      analogWrite(LED_1, brightness1); // Set the LED brightness
      analogWrite(LED_2, brightness1); // Set the LED brightness
     
    // Now we want to see if it has been long enough to increase brightness so the speed can be controlled
    if(millis() - pattern_state_enterance_time_1 >= Pair1Pattern2Stage1_duration){
      brightness1++; //increase brightness
      pattern_state_enterance_time_1 = millis(); //reset clock for next wait period.
      }
     if (brightness1 >= 255){ //If max brightness switch to fade out state
        state_LEDPairTask_1 = STATE_122;      
        }
      
    // And now return so other tasks (if they exist) can run.
    return;
  }

  //Pair 1 Pattern 2 Stage 2
  else if (state_LEDPairTask_1 == STATE_122){
      analogWrite(LED_1, brightness1); // Set the LED brightness
      analogWrite(LED_2, brightness1); // Set the LED brightness
      

    // Now we want to see if it has been long enough to increase brightness so the speed can be controlled
    if(millis() - pattern_state_enterance_time_1 >= Pair1Pattern2Stage2_duration){
      brightness1--; //decrease brightness
      pattern_state_enterance_time_1 = millis(); //reset clock for next wait period.
    }
    if (brightness1 <= 0) { //If min brightness switch to fade in state
      state_LEDPairTask_1 = STATE_121;
      // Save the time that the transition happened.
     
    }
    // And now return so other tasks (if they exist) can run.
    return;
  }

}
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
void LEDPairTask_2(){

  // The task will start in state 0 (LED_TASK_STATE_INIT). This will do any initialization that needs to happen.
  if(state_LEDPairTask_2 == STATE_INIT){
    // Start by setting each LED pin as OUTPUT
    // Note that all pins default to INPUT unless otherwise changed.
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);

    // Now turn the pin off (LOW), so we start running our code in a known hardware configuration and state
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);

    // We are done initializing. Set the state to PATTERN_1
    // Because we are tracking duration in the next state, we also need to save this exact time so we can
    // compare against it later.
    //state_LEDPairTask_2 = STATE_221;
    pattern_state_enterance_time_2 = millis();

    // Now we return, so the LEDPairTask_2 function exits.
    return;
  }
  //Pair 2 Pattern 1 Stage 1
  else if (state_LEDPairTask_2 == STATE_211){
    // Set LED 1 is ON, LED 2 is OFF
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);

    // Now we want to see if we have been in this state long enough to transition to the next state
    if(pattern_state_enterance_time_2 + Pair2Pattern1Stage1_duration <= millis()){
      // If we get here, then it's time to transition to the next state.

      // Set the state variable to the next state.
      state_LEDPairTask_2 = STATE_212;
      // Save the time that the transition happened.
      pattern_state_enterance_time_2 = millis();
    }
    // And now return so other tasks (if they exist) can run.
    return;
  }

  //Pair 2 Pattern 2 Stage 2
  else if (state_LEDPairTask_2 == STATE_212){
    // Set LED 1 is OFF, LED 2 is ON
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, HIGH);
    // Now we want to see if we have been in this state long enough to transition to the next state
    if(pattern_state_enterance_time_2 + Pair2Pattern1Stage2_duration <= millis()){
      // If we get here, then it's time to transition to the next state.

      // Set the state variable to the next state.
      state_LEDPairTask_2 = STATE_211;
      // Save the time that the transition happened.
      pattern_state_enterance_time_2 = millis();
    }
    // And now return so other tasks (if they exist) can run.
    return;
  }

  //Pair 2 Pattern 2 Stage 1
  else if (state_LEDPairTask_2 == STATE_221){
      analogWrite(LED_3, brightness2); // Set the LED brightness
      analogWrite(LED_4, brightness2); // Set the LED brightness
     
    // Now we want to see if we have been in this state long enough to transition to the next state
    if(millis() - pattern_state_enterance_time_2 >= Pair2Pattern2Stage1_duration){
      // If we get here, then it's time to transition to the next state.
      brightness2++;
      pattern_state_enterance_time_2 = millis();
      }
     if (brightness2 >= 255){
        state_LEDPairTask_2 = STATE_222; 
        }
    // And now return so other tasks (if they exist) can run.
    return;
  }

  //Pair 2 Pattern 2 Stage 2
  else if (state_LEDPairTask_2 == STATE_222){
      analogWrite(LED_3, brightness2); // Set the LED brightness
      analogWrite(LED_4, brightness2); // Set the LED brightness
      

    // Now we want to see if we have been in this state long enough to transition to the next state
    if(millis() - pattern_state_enterance_time_2 >= Pair2Pattern2Stage2_duration){
      // If we get here, then it's time to transition to the next state.
      brightness2--;
      pattern_state_enterance_time_2 = millis();
    }
    if (brightness2 <= 0) {
      // Set the state variable to the next state.
      state_LEDPairTask_2 = STATE_221;
    }
    // And now return so other tasks (if they exist) can run.
    return;
  }

}

// After running setup(), the code in loop() runs repeatedly...in a loop.
void loop() {
  // put your main code here, to run repeatedly:
  LEDPairTask_1();
  LEDPairTask_2();
  CLITask();
}
