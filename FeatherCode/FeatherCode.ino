/*
  Code for the Adafruit Feather M0 Adalogger of the PillowTime Pillow Alarm Clock

  Components:
  * Adafruit Feather M0 Adalogger (AFMA)
  * DYIMore DS3231 AT24C32 IIC High Precision Real Time Clock Module Clock Timer Memory Module For Arduino (RTC)
  * Force Sensitive Resistor (FSR)
  * Coin vibration motor (CVM)
  * MicroSD Card (MSD)

  Pseudocode:
  
  When the FSR does not detect pressure, and the alarm has not been triggered (original alarm time was not reached), nothing happens.
  When the FSR detects pressure, and the alarm has not been triggered, the AFMA checks if the starting sleep time has been stored to the MSD. 
    If the starting sleep time has not been stored, the AFMA writes the current time to the MSD
  When the pressure sensor detects pressure, and the RTC time has reached the preset alarm time, the CVM engages until the FSR no longer detects pressure (alarm has been triggered).
  When the pressure sensor no longer detects pressure, and the alarm has been triggered, add five minutes to the alarm time (snooze has been armed)
  If the FSR detects pressure while snooze has been armed, wait until the new alarm time has been reached before triggering the alarm
  (Repeat above three lines until FSR does not detect pressure while snooze is armed)
  When FSR does not detect pressure while snooze is armed (5 continuous minutes after last alarm went off):
    Write the current time minus 5 min to the MSD (assumed to be when the user woke up)
    Set the alarm to the original alarm time, one day later  
  
  The circuit:
  * RTC VCC connected to AFMA 3V
  * RTC GND connected to AFMA GND
  * RTC SDA connected to AFMA SDA
  * RTC SCL connected to AFMA SCL
  * CVM to drain of 2N7000 MOSFET from AFMA BAT Pin
  * 2N7000 MOSFET with source to AFMA GND, gate to AFMA Pin 9, and drain to the negative lead of the CVM
  * FSR with positive lead to AFMA 3V and negative lead to AFMA A0 and pull-down resistor
  * 220Ω pull-down resistor connected to negative lead of FSR and AFMA GND

  Created 2019 June 4
  By Garrett Kamila Crayton
  garrett.crayton@gmail.com
*/

#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

/////
// Declare variables
/////

//Timing
RTC_DS1307 rtc;   //RTC Module
DateTime alarm1;  //Snooze Alarm (Arduino/Connector set)
DateTime alarm2;  //Main Alarm (user set)
File myFile;
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //Days of the week

//Pins
const int fsrPin = A0;    //Pressure Sensor Pin
const int motorPin = 9;     //Motor Pin

//Presure sensing variables
const float VCC = 4.98;
const float R_DIV = 3230.0; 

//Other important variables
bool alarmTriggered;  //To determine whether the alarm has been triggered or not
bool snoozeAdded;     //To determine whether the alarm time has been added or not
bool startStored;     //To determine whether the start time has been stored or not
bool endStored;       //To determine whether the end time has been stored or not

//Initialization
void setup() 
{
  //Pin Mode Setups
  pinMode(fsrPin, INPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);  

  //Begin Serial Communication
  Serial.begin(9600);

  //Check to make sure the RTC is connected and running.
  while(!Serial)
  {
    ;
  }
    
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
  }
  else
  {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    alarm1 = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), 0, 0, 0);
  }
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open("SLEEP.txt", FILE_WRITE);

  if(myFile)
  {
    myFile.close();
  }
  else
  {
    Serial.println("error opening 'SLEEP.txt'");
  }
}

//Main Loop
void loop() 
{
  DateTime now = rtc.now();

  
  //If the serial port has a command, read the command as a string and send the string to "parseCommand"
  if (Serial.available() > 0)
  {
    String inStr = Serial.readString();
    parseCommand(inStr);
  }

  //If the pressure sensor detects pressure, check the alarm time
  if(pressSensor())
  {
    //If the alarm time has been reached, engage the pressure sensor
    if((now.year() >= alarm1.year()) && (now.month() >=  alarm1.month()) && (now.day() >= alarm1.day()) && (now.hour() >= alarm1.hour()) && (now.minute() >= alarm1.minute()) && (now.second() > alarm1.second()))
    {
      alarmTriggered = true;  //Will come in handy later
      snoozeAdded = false;    //Will come in handy later
      digitalWrite(motorPin, HIGH);
      delay(1000);
      digitalWrite(motorPin, LOW);
      delay(1000);
    }
    //If the alarm time has not been reached, check to see if the snooze function has already been called
    else
    {
      //If the snooze function has not already been called, check to see if the starting sleep time has been stored
      if(!alarmTriggered)
      {
        //If the starting sleep time has not already been stored, store the time
        if(!startStored)
        {
          File myFile = SD.open("SLEEP.txt", FILE_WRITE);
          myFile.print('[');
          
          myFile.print(now.year(), DEC);
          myFile.print('/');
          if(int(now.month() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.month(), DEC);
          myFile.print('/');
          if(int(now.day() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.day(), DEC);
          myFile.print(' ');

          if(int(now.hour() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.hour(), DEC);
          myFile.print(':');
          if(int(now.minute()) < 10)
          {
            myFile.print('0');
          }
          myFile.print((now.minute() - 1), DEC);
          myFile.print(':');
          if(int(now.second()) < 10)
          {
            myFile.print('0');
          }
          myFile.print(now.second(), DEC);
          myFile.print(", ");
          
          myFile.close();
          
          startStored = true;
          endStored = false;
        }
      }
    }
  }
  
  //If the pressure sensor does not detect pressure
  else
  {
    //If the original alarm time has ellapsed
    if(alarmTriggered)
    {
      //Check to see if the snooze time has been added to alarm1
      if(snoozeAdded)
      {
        //Check to see if alarm1 has reached. If so, store the end sleep time to (alarm1 - 5 minutes), reset alarm1 and alarm2, set snooze to false
        if((now.year() >= alarm1.year()) && (now.month() >=  alarm1.month()) && (now.day() >= alarm1.day()) && (now.hour() >= alarm1.hour()) && (now.minute() >= alarm1.minute()) && (now.second() > alarm1.second()))
        {
          //Check to see if there was a start time stored earlier (i.e. the user woke up from taking a nap)
          if(startStored)
          {
            //Check to see if the end time was stored. If so (which theoretically should be completely impossible, but still could), set startStored to false
            if(endStored)
            {
              startStored = false;
            }
            //If the end time was not stored (which should be always in this case), store the current time as the end time, and flip the values of startStored and endStored
            else
            {
              File myFile = SD.open("SLEEP.txt", FILE_WRITE);
              
              myFile.print(now.year(), DEC);
              myFile.print('/');
              if(int(now.month() < 10))
              {
                myFile.print('0');
              }
              myFile.print(now.month(), DEC);
              myFile.print('/');
              if(int(now.day() < 10))
              {
                myFile.print('0');
              }
              myFile.print(now.day(), DEC);
              myFile.print(' ');
    
              if(int(now.day() < 10))
              {
                myFile.print('0');
              }
              myFile.print(now.hour(), DEC);
              myFile.print(':');
              if(int(now.minute()) < 10)
              {
                myFile.print('0');
              }
              myFile.print((now.minute() - 1), DEC);
              myFile.print(':');
              if(int(now.second()) < 10)
              {
                myFile.print('0');
              }
              myFile.print(now.second(), DEC);
              myFile.println(']');
              
              myFile.close();
              startStored = false;
              endStored = true;
            }
          }
          alarm1 = DateTime(alarm2.year(), alarm2.month(), (alarm2.day() + 1), alarm2.hour(), alarm2.minute(), alarm2.second());
          alarm2 = alarm1;
          alarmTriggered = false;
        }
      }
      //If the snooze time has not been added, add to the time
      else
      {
        alarm1 = DateTime(alarm1.year(), alarm1.month(), alarm1.day(), alarm1.hour(), (alarm1.minute() + 5), alarm1.second());
        snoozeAdded = true;
      } 
    }
    //If the original alarm time has ellapsed, check to see if there was a start time stored (i.e. nap)
    else
    {
      //Check to see if there was a start time stored earlier (i.e. the user woke up from taking a nap)
      if(startStored)
      {
        //Check to see if the end time was stored. If so (which theoretically should be completely impossible, but still could), set startStored to false
        if(endStored)
        {
          startStored = false;
        }
        //If the end time was not stored (which should be always in this case), store the current time as the end time, and flip the values of startStored and endStored
        else
        {
          File myFile = SD.open("SLEEP.txt", FILE_WRITE);
          
          myFile.print(now.year(), DEC);
          myFile.print('/');
          if(int(now.month() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.month(), DEC);
          myFile.print('/');
          if(int(now.day() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.day(), DEC);
          myFile.print(' ');

          if(int(now.day() < 10))
          {
            myFile.print('0');
          }
          myFile.print(now.hour(), DEC);
          myFile.print(':');
          if(int(now.minute()) < 10)
          {
            myFile.print('0');
          }
          myFile.print((now.minute() - 1), DEC);
          myFile.print(':');
          
          if(int(now.second()) < 10)
          {
            myFile.print('0');
          }
          
          myFile.print(now.second(), DEC);
          myFile.println(']');

          myFile.close();
          
          startStored = false;
          endStored = true;
        }
      }
    }
  }
}

//Checks if the force resistor has detected a change in force, return true if it does, false otherwise
bool pressSensor()
{
  int fsrADC = analogRead(fsrPin);
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);

    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600)
    { 
      force = (fsrG - 0.00075) / 0.00000032639;
    }
    else
    {
      force =  fsrG / 0.000000642857;
    }


    if(force>20)
    {
      return true;
    }
    else
    {
      return false;
    }

  }
  else
  {
    return false;
  }
}

//Interpret commands
void parseCommand(String command)
{
  //Serial recieves "setAlarm(...)" (Sets alarm1 and alarm2 to the given value)
  if(command.substring(0, 8) == "setAlarm")
  {
    setAlarm(command.substring(9, -1));
  }
  //Serial recieves "setTime(...)" (Adjusts the RTC's time to the given time)
  else if(command.substring(0, 7) == "setTime")
  {
    setTime(command.substring(8, command.length()-1));
  }
  
  //Serial recieves "getData()" (Prints the contents of SD card file "SLEEP.txt")
  else if(command.substring(0, 7) == "getData")
  {
    getData();
  }
  //Serial recieves an unknown command
  else
  {
    Serial.println("Unknown Command: '" + command + "'\n");
  }
}

//Set the alarm time
void setAlarm(String newAlarm)
{
  alarm1 = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), newAlarm.substring(0, 2).toInt(), newAlarm.substring(3, 5).toInt(), newAlarm.substring(6).toInt()); 
  alarm2 = alarm1;
}

//Set the RTC time
void setTime(String dateTimeStr)
{
  int timeArr[] = {0, 0, 0, 0, 0, 0};
  
  int i = 0;
  int timeIndex = 0;
  for(int j = 0; j < dateTimeStr.length() - 1; j++)
  {
    if((isPunct(dateTimeStr[j])) || (isSpace(dateTimeStr[j])))
    {
      timeArr[timeIndex] = dateTimeStr.substring(i, j).toInt();
      timeIndex++;
      i = j + 1;
    }
  }
  timeArr[5] = dateTimeStr.substring(i, dateTimeStr.length()).toInt();
  
  rtc.adjust(DateTime(timeArr[0], timeArr[1], timeArr[2], timeArr[3], timeArr[4], timeArr[5]));
}

//Read contents of file to Serial
void getData()
{
  myFile = SD.open("SLEEP.txt");

  while(myFile.available())
  {
    Serial.write(myFile.read());
  }
  myFile.close();

  SD.remove("SLEEP.txt");
}
