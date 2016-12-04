//===========================================================================================
// PREAMBLE
//-------------------------------------------------------------------------------------------

//Logger type: Arduino Uno on ArduLogRTC shield (http://www.hobbytronics.co.uk/ardulog-rtc)
 
//Program author: Jan Knappe
//Contact: jan.knappe@tcd.ie

//===========================================================================================

//===========================================================================================
// SETUP
//-------------------------------------------------------------------------------------------

// INCLUDE LIBRARIES
//---------------------------------------------------------  
#include <Wire.h>                           //Connecting to RTC
#include <RTClib.h>                         //Using RTC for timestamp
  RTC_DS1307 RTC;                           //Define RTC object
  
#include <SD.h>  // using SD card to log data

// testing parameters
#define ECHO_TO_SERIAL 1  // echo data to terminal (on/off)
#define WAIT_TO_START 0  // wait for terminal input (on/off)

// define pins
#define LEDPin 5  // LED on external reset button
#define SwitchPin 8  // reed switch
const int SDPin = 10;  // SD connected to pin 10 of logger shield

// set switch variables
boolean SwitchEvent = false;  // default switch status is disconnected
const int SwitchIncrement = 1;  // increment for each contact/closure of switch
int SwitchSum = 0;  // sum of switch closures

// set LED variables
boolean LEDState = false;
int LEDTimeOnLong = 1000;
int LEDTimeOnShort = 500;
unsigned long LEDTimeOn = 0;  // initialize time for previous loop

// set logging variables
boolean LogState = false;  // trigger to initate logging to data file
int LogDelay = 1000;  // delay to prevent multiple logging events per seconds
unsigned long LastLogTime = 0;  // initialize time for previous logging

// set logging file
File data;  // ???


// ERRORS
//-------------------

void error(char const *str)
{
  Serial.print("error: ");
  Serial.println(str);    
  digitalWrite(LEDPin, HIGH);  // red LED constant on indicates error
  while(1);
}


// SETUP (RUN ONCE)
//-------------------

void setup(void) {
  Serial.begin(9600);
  
  // set pin modes
  pinMode(LEDPin, OUTPUT);  // red LED as output
  pinMode(SwitchPin, INPUT);  // SwitchPin as input
    digitalWrite(SwitchPin, HIGH);  // activate internal pullup resistor

  // set terminal output
  #if WAIT_TO_START
    Serial.println("Enter any character in the command line and press ENTER to start.");
    while (!Serial.available());
  #endif  // WAIT_TO_START

  // initialize SD card
  Serial.print("Initializing SD card... ");
  pinMode(SDPin, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(SDPin)) {
    error("Initializing unsuccessful.");    
  }
  Serial.println("successful.");
  
  // create a new data file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      data = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }  
  
  if (! data) {
    error("Could not create data file. Format SD card and try again.");
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    data.println("Failed to connect to RTC. Check battery.");
  #if ECHO_TO_SERIAL
    Serial.println("Failed to connect to RTC. Check battery.");
  #endif  //ECHO_TO_SERIAL
  }

  // print data table header
  data.println("timestamp, tips");    
  #if ECHO_TO_SERIAL
    Serial.println("timestamp, tips");
  #endif //ECHO_TO_SERIAL

  // indicate correct initialization and start of program with 3 x long LED blink
  digitalWrite(LEDPin, HIGH);
  delay(LEDTimeOnLong); 
  digitalWrite(LEDPin, LOW);
  delay(LEDTimeOnShort); 
  digitalWrite(LEDPin, HIGH);
  delay(LEDTimeOnLong); 
  digitalWrite(LEDPin, LOW);
  delay(LEDTimeOnShort); 
  digitalWrite(LEDPin, HIGH);
  delay(LEDTimeOnLong); 
  digitalWrite(LEDPin, LOW);

}

// LOOP (REPEAT)
//-------------------

void loop(void) {

  // set LED for switch closure feedback (i.e. blink for every detected tip)
  digitalWrite(LEDPin, LEDState);  // set LED to LEDState 

  // measure time since program start
  unsigned long MilliSecSinceStart = millis();  // unsigned long allows for maximum value of 49 days
  unsigned long SecSinceStart = MilliSecSinceStart/1000;
  
  if ((unsigned long)(MilliSecSinceStart - LEDTimeOn) >= LEDTimeOnShort) {
    LEDState = false;
  }

  // fetch time from RTC
  DateTime now;  
  now = RTC.now();

  digitalRead(SwitchPin);
  delay(100); 
  int switchReading = digitalRead(SwitchPin);  

  // switch calculator, looks for switch continuously
   // Look for low to high
   if ((SwitchEvent==false)&&(digitalRead(SwitchPin)==HIGH))
    {
    SwitchEvent=true;
  //  SwitchSum+=LowAmt;
  }
   if ((SwitchEvent==true)&&(digitalRead(SwitchPin)==LOW)) 
    {
      LEDState = true;
      LEDTimeOn = MilliSecSinceStart;
      SwitchEvent=false;
      SwitchSum+=SwitchIncrement;
    }


  if (SecSinceStart%60 == 0) {
    LogState = true;
    if ((LogState == true)&&((unsigned long)(MilliSecSinceStart - LastLogTime) <= LogDelay)) {
      LogState = false;
    }
    if (LogState == true) {
      if (now.day() < 10){   // Add a zero, if necessary
        data.print(0);
      }
      data.print(now.day(), DEC);           
      data.print("/");      
      if (now.month() < 10){   // Add a zero, if necessary
        data.print(0);
      }
      data.print(now.month(), DEC);      
      data.print("/");      
      data.print(now.year(), DEC);      
      data.print(" ");
      if (now.hour() < 10){   // Add a zero, if necessary
        data.print(0);
      }
      data.print(now.hour(), DEC);
      data.print(":");
      if (now.minute() < 10){   // Add a zero, if necessary
        data.print(0);
      }
      data.print(now.minute(), DEC);      
      Serial.print(now.minute(), DEC);
      data.print(":");
      data.print("00");
      data.print(", ");  
      Serial.print(", ");  
      data.print(SwitchSum); 
      Serial.println(SwitchSum); 
      data.println();

      LastLogTime = MilliSecSinceStart;
      LogState = false;
      SwitchSum = 0;
    }
  }

//  // print timestamp to data file
//  data.print(now.year(), DEC);  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  



  
  // blink LED to show we are syncing data to the card & updating FAT!
  //digitalWrite(LEDPin, HIGH);
  data.flush();
  //digitalWrite(LEDPin, LOW);
  
}
