/*
  Dragon-radar clock
  Script written by Iver Iscariot Søbakk, for his custom PCB with an Atmega328 (open source at the github you found this file)
*/

int PWM = 9;        // the PWM pin the LED is attached to

int A = 14;         // Hour cathode 1
int B = 15;         // Hour cathode 2
int C = 16;         // Hour anode 1
int D = 17;         // Hour anode 2

int E = 18;         // minute cathode 1
int F = 19;         // minute cathode 2
int G = 6 ;         // minute anode 1
int H = 7 ;         // minute anode 2

int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by

// Define the HOUR/MIN pin states to select the correct LED from the two "matrix"es (see schematic)
const byte pinStates[12][4] = {
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

int mincounter = 1;
int houcounter = 1;

void setHour(int inputNumber) {    
    if (inputNumber < 1 || inputNumber > 12) return; // Ensure valid range
    
    digitalWrite(A, pinStates[inputNumber - 1][0]);
    digitalWrite(B, pinStates[inputNumber - 1][1]);
    digitalWrite(C, pinStates[inputNumber - 1][2]);
    digitalWrite(D, pinStates[inputNumber - 1][3]);
}

void setMinute(int inputNumber) {    
    inputNumber = inputNumber/5 + 1; // divides to the nearest 5 minutes, rounding up, so yu have more time than you think 4 -> 0+1, lighting the "5 min" led
    if (inputNumber < 1 || inputNumber > 12) return; // Ensure valid range
    
    digitalWrite(E, pinStates[inputNumber - 1][0]);
    digitalWrite(F, pinStates[inputNumber - 1][1]);
    digitalWrite(G, pinStates[inputNumber - 1][2]);
    digitalWrite(H, pinStates[inputNumber - 1][3]);
}


// the setup routine runs once when you press reset:
void setup() {
  pinMode(PWM, OUTPUT);
  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);

  
  digitalWrite(A, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(B, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(C, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(D, LOW);  // turn the LED on (HIGH is the voltage level)

  digitalWrite(E, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(F, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(G, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(H, LOW);  // turn the LED on (HIGH is the voltage level)

  // just for circle example
  analogWrite(PWM, 127);   // half brightness


  Serial.begin(9600);
}


void loop() {
  setHour(houcounter);
  setMinute(mincounter);

  delay(100);

  mincounter += 1;
  if (mincounter == 13){  // if all the way round
    mincounter = 1;      // start over
    houcounter += 1;     // increment hour
  }
  if (houcounter == 13){  // iff all the way round
    houcounter = 1;       //start over
  }

}
