//need to set up time when program a new chip

//code V1 with analog pin connected to int1

// power profile
// sleep mode 800 uMA
// active 20 mA
// Nan-Wei Gong Feb 1st 2014
//#include <stdio.h>
#include <Time.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <SD.h>
// For the SD Shield Plus
#include "Wire.h"

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
//for system time sync
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

const int chipSelect = 4;

int sleepStatus = 0;        // variable to store a request for sleep
int led =9;
int wakePin1 = 2;
int wakePin2 = 3;
int analogPin = 0;
int button1 = 0;
int button2 = 0;
// This is to talk to the real time clock
boolean FORCE_UPDATE_DATETIME= false;
//


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
byte m_second, m_minute, m_hour, m_dayOfWeek, m_dayOfMonth, m_month, m_year;

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
  
  
  
  // hand input time messsage 
  /*
  m_second = 50;
  m_minute = 10;
  m_hour = 14;
  m_dayOfWeek = 5;
  m_dayOfMonth = 29;
  m_month = 8;
  m_year = 14;
  setDateDs1307(m_second, m_minute, m_hour, m_dayOfWeek, m_dayOfMonth, m_month, m_year);
  Serial.println("date reset");
  */
  
  //check or sync Date and Time with system 
  //String Standard_datetime = "29/08/14 18:31:00";
  //06/10/11 15:10:00
  //DateUpdate(Standard_datetime);
  //String PCTIME = getPCtime();
  //Serial.println(PCTIME);
  

  if(getPCtime())
    Serial.println("CAN  GET TIME FROM PC");
  else 
    Serial.println("CANT GET TIME FROM PC");
    
  datetimeUpdate(1,1,9,14, 14,32,00);
  //dayOfWeek,i_dayOfMonth, i_month,i_year,i_hour,i_minute,i_second)


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
  // time sync
  /*
  if(getPCtime())
    Serial.println("CAN!!!!  GET TIME FROM PC");
  else 
    Serial.println("CANT     GET TIME FROM PC");
    */
//    digitalWrite(led, LOW);
  delay(50);

   if (sleepStatus == LOW) {            // start to put the device in sleep
      button1 = 0;
      button2 = 0;
      sleepNow();                      // sleep function called here
      }

}

// "29/08/14 18:31:00"
void datetimeUpdate( byte i_dayOfWeek,
                     byte i_dayOfMonth,
                     byte i_month,
                     byte i_year,
                     byte i_hour,
                     byte i_minute,
                     byte i_second)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // A few of these need masks because certain bits are control bits
  m_second     = bcdToDec(Wire.read() & 0x7F);
  m_minute     = bcdToDec(Wire.read());
  m_hour       = bcdToDec(Wire.read() & 0x3F);
  m_dayOfWeek  = bcdToDec(Wire.read());
  m_dayOfMonth = bcdToDec(Wire.read());
  m_month      = bcdToDec(Wire.read());
  m_year       = bcdToDec(Wire.read());
  
  //sync
  if((m_year == i_year) && (m_month == i_month) && !FORCE_UPDATE_DATETIME)
  {
    Serial.println(m_year);
    Serial.println(i_year);
    Serial.println(m_month);
    Serial.println(i_month);
    Serial.println(FORCE_UPDATE_DATETIME);
    Serial.println("month and year already update.");
  }
  else
  {
    
    if(FORCE_UPDATE_DATETIME)
      Serial.println("Force update.");
    else
      Serial.println("Date too old, so update.");
      
    setDateDs1307(i_second, 
                  i_minute, 
                  i_hour, 
                  i_dayOfWeek, 
                  i_dayOfMonth, 
                  i_month, 
                  i_year);
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
  m_second     = bcdToDec(Wire.read() & 0x7F);
  m_minute     = bcdToDec(Wire.read());
  m_hour       = bcdToDec(Wire.read() & 0x3F);
  m_dayOfWeek  = bcdToDec(Wire.read());
  m_dayOfMonth = bcdToDec(Wire.read());
  m_month      = bcdToDec(Wire.read());
  m_year       = bcdToDec(Wire.read());

  String dataString = "";

  dataString += Print2Digit(m_dayOfMonth);
  dataString += String("/");
  dataString += Print2Digit(bcdToDec(m_month));
  dataString += String("/"); // Y2k1 bug!
  dataString += Print2Digit(bcdToDec(m_year));
  dataString += String(" ");
  dataString += Print2Digit(m_hour);
  dataString += String(":");
  dataString += Print2Digit(m_minute);
  dataString += String(":");
  dataString += Print2Digit(m_second);

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

void setDateDs1307(byte m_second,        // 0-59
                   byte m_minute,        // 0-59
                   byte m_hour,          // 1-23
                   byte m_dayOfWeek,     // 1-7
                   byte m_dayOfMonth,    // 1-28/29/30/31
                   byte m_month,         // 1-12
                   byte m_year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(m_second));    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(m_minute));
   Wire.write(decToBcd(m_hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(decToBcd(m_dayOfWeek));
   Wire.write(decToBcd(m_dayOfMonth));
   Wire.write(decToBcd(m_month));
   Wire.write(decToBcd(m_year));
   Wire.endTransmission();
}

boolean getPCtime() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    if( Serial.read() == TIME_HEADER ) {        
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        char c= Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      //DateTime.sync(pctime);   // Sync Arduino clock to the time received on the serial port
      Serial.print("pctime: ");
      Serial.println(pctime);
      return true;   // return true if time message received on the serial port
    }  
  }
  return false;  //if no message return false
}

