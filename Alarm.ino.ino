const int ledPin = 13;
const int snoozePin = 2;
const int offPin = 3;

int inTime[3];
int originalAlarm[] = {0, 0, 10};
int currentAlarm[] = {0, 0, 10};
bool callAlarm;

void setup() 
{
  pinMode(snoozePin, INPUT);
  pinMode(offPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(snoozePin), snooze, RISING);
  attachInterrupt(digitalPinToInterrupt(offPin), alarmOff, RISING);
  callAlarm = true;
}

void loop() 
{
  inTime[2] += 1;oo
  checkTime(inTime);
  Serial.print(inTime[0]);
  Serial.print(":");
  Serial.print(inTime[1]);
  Serial.print(":");
  Serial.print(inTime[2]);
  if(callAlarm)
  {
    checkAlarm();
  }
  else
  {
    Serial.println("");
    delay(997);
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

void checkAlarm()
{
  if((inTime[0] > currentAlarm[0]) || (inTime[1] > currentAlarm[1]) || (inTime[2] > currentAlarm[2]))
  {
    Serial.println(" ALARM");
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(697);
  }
  else
  {
    //Serial.println("");
    delay(997);
  }
}

void snooze()
{
  //Serial.print("SNOOZE ");
  currentAlarm[0] = inTime[0];
  currentAlarm[1] = inTime[1];
  currentAlarm[2] = inTime[2] + 10;

  checkTime(currentAlarm);
}

void alarmOff()
{
  detachInterrupt(digitalPinToInterrupt(offPin));
  Serial.println("");
  Serial.print("ALARM OFF ");
  Serial.print(inTime[0]);
  Serial.print(":");
  Serial.print(inTime[1]);
  Serial.print(":");
  Serial.println(inTime[2]);

  callAlarm = false;

  currentAlarm[0] = inTime[0];
  currentAlarm[1] = inTime[1];
  currentAlarm[2] = inTime[2];
}
