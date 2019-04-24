bool callAlarm;
bool alarmEnabled;
int timeRemaining;
String commandBuffer;
int sleepTimes[10] = {0,0,0,0,0,0,0,0,0,0};
int sleeps = 0;
void setup() 
{
  pinMode(snoozePin, INPUT);
  pinMode(offPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(snoozePin), snooze, RISING);
  attachInterrupt(digitalPinToInterrupt(offPin), alarmOff, RISING);
  callAlarm = false;
  
  timeRemaining = 0;
}

void loop() 
{
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        commandBuffer += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("Arduino Received: ");
            Serial.print(commandBuffer);
            parseCommand(commandBuffer);
            commandBuffer = ""; // Clear recieved buffer
            
        }
    }
    Serial.print("Remaining: ");
    Serial.println(timeRemaining);
    if(timeRemaining > 0){
      timeRemaining--;
    } else {
        if(alarmEnabled == true){
            sleepTimes[sleeps%10] = millis();
            callAlarm = true;
            alarmEnabled = false;
        }
        
    }
}

void parseCommand(String command){
    // set alarm command: SETALARM <seconds>"
    if(command.substring(0,8) == "SETALARM"){
           String remainingString = command.substring(9);
           timeRemaining = remainingString.toInt();
    } else if(command.substring(0,8) == "GETSLEEPS"){
           for(int i = 0; i < 10; i++){
               Serial.print(sleepTimes[i]);
               Serial.print(",");
           }
           Serial.println();
    }
}

void checkTime(int timeArr[])
{
  if(timeArr[2] >= 60)
  
  
  {
    timeArr[1] += 1;
    timeArr[2] %= 60;
  }
  if(timeArr[1] >= 60)
  {
    timeArr[0] += 1;
    timeArr[1] %= 60;
  }
  if(timeArr[0] >= 24)
  {
    timeArr[0] %= 24;
  }
}
