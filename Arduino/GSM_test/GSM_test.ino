#include <GSM.h>

#define PINNUMBER ""

GSM gsm; // include a 'true' parameter for debug enabled
int led = 13;
void setup()
{
  // initialize serial communications
  Serial.begin(9600);
  //Serial.println("hello GSM");
  // connection state
  boolean notConnected = true;
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    Serial.println("RRRRRR  check");
    
    if(gsm.begin(PINNUMBER)==GSM_READY)
    {  
      notConnected = false;
      Serial.println("RRRRR WTF");
    }
    else
    {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(5000);               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(5000);               // wait for a second
      Serial.println("Not connected");
      delay(1000);
    }
    Serial.println("RRRRR  WDH");
  }

  Serial.println("GSM initialized");
}

void loop()
{
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);               // wait for a second
// once connected do something interesting
}
