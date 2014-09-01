//need to set up time when program a new chip

//code V1 with analog pin connected to int1

// power profile
// sleep mode 800 uMA
// active 20 mA
// Nan-Wei Gong Feb 1st 2014


#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <SD.h>
// For the SD Shield Plus
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address

const int chipSelect = 4;

int sleepStatus = 0;        // variable to store a request for sleep
int led =9;
int wakePin1 = 2;
int wakePin2 = 3;
int analogPin = 0;
int button1 = 0;
int button2 = 0;
// This is to talk to the real time clock


byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Global Variables
int i;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

void setup()
{
  pinMode(led, OUTPUT);
  Wire.begin();

  Serial.begin(9600);
  Serial.print("Initializing SD card...\n");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  Serial.print("chipSelect set to output\n");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present\n");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.\n");


  second = 50;
  minute = 51;
  hour = 18;
  dayOfWeek = 4;
  dayOfMonth = 2;
  month = 2;
  year = 13;
 //setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  Serial.println("date reset");


// for the interrupt
  pinMode(wakePin1, INPUT);// button 1
  pinMode(wakePin2, INPUT);// button 2

  attachInterrupt(0, wakeUpNow1, RISING); // use interrupt 0 (pin 2) and run function
                                      // wakeUpNow when pin 2 gets HIGH
  attachInterrupt(1, wakeUpNow2, RISING); // use interrupt 1 (pin 2) and run function
                                      // wakeUpNow when pin 3 gets HIGH


}

void wakeUpNow1()        // here the interrupt is handled after wakeup for both button you blink that LED for a bit
{
  //execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  button1 = 1;
}


void wakeUpNow2()        // here the interrupt is handled after wakeup for both button you blink that LED for a bit
{
  //execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  button2 = 1;
}

void sleepNow()         // here we put the arduino to sleep
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();
    attachInterrupt(0,wakeUpNow1, RISING);
    attachInterrupt(1,wakeUpNow2, RISING);
//    power_adc_disable();
//    power_spi_disable();
//    power_timer0_disable();
//    power_timer1_disable();
//    power_timer2_disable();
//    power_twi_disable();

    sleep_mode();                // here the device is actually put to sleep!!
    sleep_disable();             // first thing after waking from sleep:
    detachInterrupt(0);
    detachInterrupt(1);
}


void loop()
{
  // make a string for assembling the data to log:
  String dataString = getDateDs1307();
  dataString += String(",");

  // read three sensors and append to the string:
  int sensor = analogRead(analogPin);

  if (sensor <= 100){
    sleepStatus = 0;
  } else
    sleepStatus = 1;

    dataString += String(sensor);
    dataString += String(",");
    dataString += String(button1);
    dataString += String(",");
    dataString += String(button2);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog1.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
  //  Serial.print("pressure sensor data");
  //  Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening datalog.csv");
  }
//    digitalWrite(led, LOW);
  delay(50);

   if (sleepStatus == LOW) {            // start to put the device in sleep
      button1 = 0;
      button2 = 0;
      sleepNow();                      // sleep function called here
      }

}


// Gets the date and time from the ds1307 and return
// result in a format a spreadsheet can parse: 06/10/11 15:10:00
String getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7F);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3F);
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());

  String dataString = "";

  dataString += Print2Digit(dayOfMonth);
  dataString += String("/");
  dataString += Print2Digit(bcdToDec(month));
  dataString += String("/"); // Y2k1 bug!
  dataString += Print2Digit(bcdToDec(year));
  dataString += String(" ");
  dataString += Print2Digit(hour);
  dataString += String(":");
  dataString += Print2Digit(minute);
  dataString += String(":");
  dataString += Print2Digit(second);

  return dataString;
}



String Print2Digit(byte Val)
{
  String dataString = "";
  if (Val < 10)
  {
    dataString = "0";
  }
  dataString += String(Val, DEC);
  return dataString;
}

void setDateDs1307(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}
