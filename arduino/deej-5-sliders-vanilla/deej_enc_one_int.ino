#include "PinChangeInterrupt.h"   // NicoHood's library

#define USE_BUTTONS 1

#define ENC_BUTTON_DEBOUNCE 50


// Adjusted for use with limited PCINT devices, such as Pro Micro
const int NUM_ENCODERS = 5;
const int encoderPinsA[NUM_ENCODERS] = {8, 9, 10, 14, 15};  // may also use 16 - any PCINT pin
const int encoderPinsB[NUM_ENCODERS] = {2, 3, 4, 5, 6};     // arbitrary - pick any IO

#ifdef USE_BUTTONS
  const int encoderButtons[NUM_ENCODERS] = {A0, A1, A2, A3, 7};  // can use analog pins, they will act as digital pins for this
#endif

int encoderValues[NUM_ENCODERS] = {512, 512, 512, 512, 512};    // set these to the initial values your deej will start with (0-1023)
int encoderMute[NUM_ENCODERS] = {0, 0, 0, 0, 0};                // unused unless using buttons as mute
const int enc_inc = 1023/100;                                   // set this to the step size each 'click' will increment/decrement
                                                                // e.g. 1023/100 means roughly 100 'clicks' between 0% and 100% volume (moving 10 each click, between 0 and 1023)
void setup() {   
  for(int i = 0; i < NUM_ENCODERS; i++){
    pinMode(encoderPinsA[i], INPUT_PULLUP);     // change to INPUT if using external pullup resistors
    pinMode(encoderPinsB[i], INPUT_PULLUP);     // change to INPUT if using external pullup resistors   
    #ifdef USE_BUTTONS
      pinMode(encoderButtons[i], INPUT_PULLUP);
    #endif  
  }

  // A new set of ISRs will need to be added for each encoder, see bottom of sketch
  attachPCINT(digitalPinToPCINT(encoderPinsA[0]), EN0_A_ISR, CHANGE);
  attachPCINT(digitalPinToPCINT(encoderPinsA[1]), EN1_A_ISR, CHANGE);
  attachPCINT(digitalPinToPCINT(encoderPinsA[2]), EN2_A_ISR, CHANGE);
  attachPCINT(digitalPinToPCINT(encoderPinsA[3]), EN3_A_ISR, CHANGE);
  attachPCINT(digitalPinToPCINT(encoderPinsA[4]), EN4_A_ISR, CHANGE);
 
  Serial.begin(9600);
}

void loop() {
  sendEncoderValues(); // Actually send data (all the time)
  // printEncoderValues(); // For debug
  delay(10);
  #ifdef USE_BUTTONS
    for(int i = 0; i < NUM_ENCODERS; i++){
      if(digitalRead(encoderButtons[i])){
        encoderMute[i] ^= 1;                   // ^= operator toggles between 1 and 0
        delay(ENC_BUTTON_DEBOUNCE);            // wait for encoder bounce (halts serial data, but presumably you aren't turning a knob while also holding mute)
        while(!digitalRead(encoderButtons[i])); // after bounce, wait for button to be released
        delay(ENC_BUTTON_DEBOUNCE);            // wait for the release encoder bounce
      }    
    }
  #endif
}


void sendEncoderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_ENCODERS; i++) {
    builtString += String((int)(encoderValues[i] - (encoderValues[i]*encoderMute[i]))) ;  // the -encoderValue*encoderMute will either subtract the value to 0 (muted) or do nothing to the value (unmuted)

    if (i < NUM_ENCODERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

/******************/
/* ENCODER 0 ISRs */
/******************/
void EN0_A_ISR(){
  if(digitalRead(encoderPinsB[0])){
    encoderValues[0] += enc_inc;
  } else{
    encoderValues[0] -= enc_inc;
  }
  encoderValues[0] = constrain(encoderValues[0], 0, 1023);
}

/******************/
/******************/


/******************/
/* ENCODER 1 ISRs */
/******************/
void EN1_A_ISR(){
  if(digitalRead(encoderPinsB[1])){
    encoderValues[1] += enc_inc;
  } else{
    encoderValues[1] -= enc_inc;
  }
  encoderValues[1] = constrain(encoderValues[1], 0, 1023);
}

/******************/
/******************/


/******************/
/* ENCODER 2 ISRs */
/******************/
void EN2_A_ISR(){
  if(digitalRead(encoderPinsB[2])){
    encoderValues[2] += enc_inc;
  } else{
    encoderValues[2] -= enc_inc;
  }
  encoderValues[2] = constrain(encoderValues[2], 0, 1023);
}

/******************/
/******************/


/******************/
/* ENCODER 3 ISRs */
/******************/
void EN3_A_ISR(){
  if(digitalRead(encoderPinsB[3])){
    encoderValues[3] += enc_inc;
  } else{
    encoderValues[3] -= enc_inc;
  }
  encoderValues[3] = constrain(encoderValues[3], 0, 1023);
}

/******************/
/******************/


/******************/
/* ENCODER 4 ISRs */
/******************/
void EN4_A_ISR(){
  if(digitalRead(encoderPinsB[4])){
    encoderValues[4] += enc_inc;
  } else{
    encoderValues[4] -= enc_inc;
  }
  encoderValues[4] = constrain(encoderValues[4], 0, 1023);
}

/******************/
/******************/

void printEncoderValues() {
  for (int i = 0; i < NUM_ENCODERS; i++) {
    String printedString = String("Encoder #") + String(i + 1) + String(": ") + String(encoderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_ENCODERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
