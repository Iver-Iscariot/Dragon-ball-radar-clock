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
int maxBright = 60;  // Maximum Brightness
float period = 0.5;       // Period of fade (in seconds)
int n = 3;            // Number of pulses




volatile bool wakeUpFlag = false;  // Flag to indicate button press




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

int mincounter = 0;
int houcounter = 1;


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
  unsigned long t0 = millis();
  while (millis() - t0 < n*period*1000){
    analogWrite(PWM, maxBright*0.5*(         cos(    2*PI*( millis()-t0 )/( 1000*period )    + PI)   + 1       )  ); 
    delay(5);  // if you want realy fast pulses for some reason, you might want to reduce this
  }
  analogWrite(PWM, 0);

}

void wakeUp() {
    delay(50);  // Simple debounce
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));  // Prevent multiple triggers
    wakeUpFlag = true;  // Set flag when button is pressed
}
void goToSleep() {
    Serial.println("EEPY!!");
    wakeUpFlag = false;  // Reset flag before sleep

    // Attach interrupt on FALLING edge (button press)
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), wakeUp, FALLING);

    // Ensure no immediate wake-up from noise
    delay(10);

    // Enter idle mode (timers keep running)
    LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON, SPI_OFF, USART0_OFF, TWI_OFF);

    // Wakes up here when button is pressed
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));  // Remove interrupt after wake-up
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

  // just for circle example
  analogWrite(PWM, 4);   // half brightness


  Serial.begin(9600);


  hourFormat12(); 
  delay(500);
  resetClock();
}

// ---------------------------------------- Loop ----------------------------------------------
void loop() {
  setHour(hour());      // Set the pins for the LED matrix
  setMinute(minute());
  showTime();           // Flash the PWM pin to reveal the set LED pins

  delay(3000);

  goToSleep();  // Call function to enter idle mode

  if (wakeUpFlag) {
      Serial.println("Woken up by button press!");
      delay(300);  // Avoid immediate re-triggering due to bouncing
  }

}
