// Enhandsor 
// Geroge Chang 07.16.2014
// test arduino board nano (v3.1)  w/ ATmega368  
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);               // wait for a second
  Serial.println("light on  ");
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);               // wait for a second
}
