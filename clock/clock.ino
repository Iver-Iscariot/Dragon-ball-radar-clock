/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
*/

int PWM = 9;         // the PWM pin the LED is attached to
int A = 14;         // the PWM pin the LED is attached to
int B = 15;         // the PWM pin the LED is attached to
int C = 16;         // the PWM pin the LED is attached to
int D = 17;         // the PWM pin the LED is attached to
int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by

int smallcounter = 0;
int bigcounter = 0;



// the setup routine runs once when you press reset:
void setup() {
  pinMode(PWM, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

  
  digitalWrite(A, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(B, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(C, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(D, LOW);  // turn the LED on (HIGH is the voltage level)

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  analogWrite(PWM, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
  smallcounter += 1;


  if (smallcounter%100 == 0){
    Serial.print("swag");
    digitalWrite(B, !digitalRead(B)); // toggle LSB
    if (digitalRead(B) == LOW){         // Hvis B ble endret til 0
      digitalWrite(A, !digitalRead(A)); // endre A også
    }
    bigcounter += 1;

    if (bigcounter == 3){
      digitalWrite(D, !digitalRead(D)); // toggle LSB
      if (digitalRead(D) == LOW){         // Hvis B ble endret til 0
        digitalWrite(C, !digitalRead(C)); // endre A også
      }
      bigcounter = 0;
    }
  }

}
