/*
  Dragon-radar clock
  Script written by Iver Iscariot Søbakk, for his custom PCB with an Atmega328 (open source at the github you found this file)
*/
#include <TimeLib.h>


int PWM = 9;        // the PWM pin the LED is attached to

int A = 14; int B = 15;  // Minute cathode 
int C = 16; int D = 17;  // Minute anode 

int E = 18; int F = 19;  // Hour cathode 
int G = 6 ; int H = 7 ;  // Hour anode

int brightness = 0;  // Current brightness
int fadeAmount = 5;  // Fading increase amount (fadespeed)

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


// the setup routine runs once when you press reset:
void setup() {
  pinMode(PWM, OUTPUT);
  
  pinMode(A, OUTPUT); pinMode(B, OUTPUT); pinMode(C, OUTPUT); pinMode(D, OUTPUT);
  pinMode(E, OUTPUT); pinMode(F, OUTPUT); pinMode(G, OUTPUT); pinMode(H, OUTPUT);

  // Dont know if this is strictly necesarry
  digitalWrite(A, LOW);  digitalWrite(B, LOW);  digitalWrite(C, LOW);  digitalWrite(D, LOW);  
  digitalWrite(E, LOW);  digitalWrite(F, LOW);  digitalWrite(G, LOW);  digitalWrite(H, LOW);  

  // just for circle example
  analogWrite(PWM, 4);   // half brightness


  Serial.begin(9600);


  hourFormat12(); 
}


void loop() {
  setHour(houcounter);
  setMinute(mincounter);

  delay(50);

  mincounter += 1;
  if (mincounter == 60){  // if all the way round
    mincounter = 0;      // start over
    houcounter += 1;     // increment hour
  }
  if (houcounter == 13){  // iff all the way round
    houcounter = 1;       //start over
  }

}
