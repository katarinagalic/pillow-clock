const int ledPin = 13;
const String inStr;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("Hello World!");
}

void loop()
{
  
  if (Serial.available() > 0)
  {
    inStr = Serial.readString();

    Serial.print("I got: ");
    Serial.println(inStr);

    parseCommand(inStr);
  }
}

void parseCommand(String command)
{
  if(command.substring(0,5) == "testU")
  {
    blinkLED();
  }
}

void blinkLED()
{
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(700);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(700);
}
