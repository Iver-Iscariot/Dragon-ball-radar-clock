/*
  Dragon-radar clock
  Script written by Iver Iscariot Søbakk, for his custom PCB with an Atmega328 (open source at the github you found this file)
*/
#include <TimeLib.h>
#include <LowPower.h>


#define BUTTON_PIN 3  // Pin for the button

int PWM = 9;             // the PWM pin the transistors are attached to

int A = 14; int B = 15;  // Minute cathode 
int C = 16; int D = 17;  // Minute anode 

int E = 18; int F = 19;  // Hour cathode 
int G = 6 ; int H = 7 ;  // Hour anode

// --- Time-showing-pulses
int maxBright = 127;  // Maximum Brightness
float period = 0.5;       // Period of fade (in seconds)
int n = 3;            // Number of pulses

// bools and timers for long/short buttonpress functionality
bool buttonActive = false;
bool longPressActive = false;
unsigned long buttonTimer = 0;
unsigned long longPressTime = 3000;


// Define the HOUR/MIN pin states to select the correct LED from the two "matrix"es (see schematic)
const byte pinStatesH[12][4] = {
    {0, 0, 1, 1}, // 1 -> 0011
    {0, 0, 1, 0}, // 2 -> 0010
    {0, 0, 0, 1}, // 3 -> 0001
    {0, 0, 0, 0}, // 4 -> 0000
    {0, 1, 1, 1}, // 5 -> 0111
    {0, 1, 1, 0}, // 6 -> 0110
    {0, 1, 0, 1}, // 7 -> 0101
    {0, 1, 0, 0}, // 8 -> 0100
    {1, 0, 1, 1}, // 9 -> 1011
    {1, 0, 1, 0}, // 10 -> 1010
    {1, 0, 0, 1}, // 11 -> 1001
    {1, 0, 0, 0}  // 12 -> 1000
};
const byte pinStates[12][4] = {
    {0, 0, 1, 1}, // 1  -> 0011
    {0, 0, 0, 1}, // 2  -> 0001
    {0, 0, 1, 0}, // 3  -> 0010
    {0, 0, 0, 0}, // 4  -> 0000
    {1, 0, 1, 1}, // 5 -> 1011
    {1, 0, 0, 1}, // 6 -> 1001
    {1, 0, 1, 0}, // 7 -> 1010
    {1, 0, 0, 0},  // 8 -> 1000
    {0, 1, 1, 1}, // 9  -> 0111
    {0, 1, 0, 1}, // 10 -> 0101
    {0, 1, 1, 0}, // 11 -> 0110
    {0, 1, 0, 0} // 12 -> 0100
};


// --------------------------- Functions to be called -----------------------------------
void setMinute(int inputNumber) {    
    inputNumber = inputNumber/5 + 1; // divides to the nearest 5 minutes, rounding up, so yu have more time than you think 4 -> 0+1, lighting the "5 min" led
    if (inputNumber < 1 || inputNumber > 12) return; // Ensure valid range
    
    digitalWrite(A, pinStates[inputNumber - 1][0]);
    digitalWrite(B, pinStates[inputNumber - 1][1]);
    digitalWrite(C, pinStates[inputNumber - 1][2]);
    digitalWrite(D, pinStates[inputNumber - 1][3]);
}

void setHour(int inputNumber) {    
    if (inputNumber < 1 || inputNumber > 12) return; // Ensure valid range
    
    digitalWrite(E, pinStatesH[inputNumber - 1][3]);
    digitalWrite(F, pinStatesH[inputNumber - 1][2]);
    digitalWrite(G, pinStatesH[inputNumber - 1][1]);
    digitalWrite(H, pinStatesH[inputNumber - 1][0]);
}

void resetClock(){
  // Set time to 12:00:00
  setTime(12, 0, 0, 1, 1, 2025); // setTime(hr, min, sec, day, month, year)
}

void showTime(){
  Serial.println("Showing time");
  setHour(hour());      // Set the pins for the LED matrix
  setMinute(minute());
  unsigned long t0 = millis();
  // PWM a sine on the pwm pin for the set amunt of periods
  while (millis() - t0 < n*period*1000){
    analogWrite(PWM, maxBright*0.5*(         cos(    2*PI*( millis()-t0 )/( 1000*period )    + PI)   + 1       )  ); 
    delay(5);  // if you want realy fast pulses for some reason, you might want to reduce this
  }
  analogWrite(PWM, 0);

}


// ------------------------------------ Setup ---------------------------------------------
void setup() {
  pinMode(PWM, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  pinMode(A, OUTPUT); pinMode(B, OUTPUT); pinMode(C, OUTPUT); pinMode(D, OUTPUT);
  pinMode(E, OUTPUT); pinMode(F, OUTPUT); pinMode(G, OUTPUT); pinMode(H, OUTPUT);

  // Dont know if this is strictly necesarry
  digitalWrite(A, LOW);  digitalWrite(B, LOW);  digitalWrite(C, LOW);  digitalWrite(D, LOW);  
  digitalWrite(E, LOW);  digitalWrite(F, LOW);  digitalWrite(G, LOW);  digitalWrite(H, LOW);  

  Serial.begin(9600);


  hourFormat12(); 
  delay(250);
  resetClock();
  delay(250);
  setHour(hour());      // Set the pins for the LED matrix
  setMinute(minute());
  delay(250);
  showTime();
  delay(250);
}

// ---------------------------------------- Loop ----------------------------------------------
void loop() {

	if (digitalRead(BUTTON_PIN) == LOW) {  // the button is active low / pulled high

		if (buttonActive == false) {

			buttonActive = true;
			buttonTimer = millis();
		}

		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)  ) {
      if (digitalRead(BUTTON_PIN) == HIGH) {
        longPressActive = true;
        resetClock();
        showTime();
      }
		}

	} else {

		if (buttonActive == true) {

			if (longPressActive == true) {

				longPressActive = false;

			} else {

				showTime();

			}

			buttonActive = false;

		}

	}
}
