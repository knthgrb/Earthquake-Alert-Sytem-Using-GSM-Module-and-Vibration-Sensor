#include <SoftwareSerial.h>

SoftwareSerial SIM900A(8,9);

const int vibrationPin = A0;
const int relayPin = 7;
int threshold;
const char* phoneNumbers[] = {"+639617802973", "+639096862170"};
String message = "Warning! Earthquake alert!!";

void setup()
{
  SIM900A.begin(9600); 
  Serial.begin(9600); 
  pinMode(vibrationPin, INPUT);
  pinMode(relayPin, OUTPUT);
  Serial.println ("Initializing...");

  //sensor calibration
  Serial.println("Calibrating... Please keep the sensor still.");

  // get average sensor values for a short duration
  int calibrationSum = 0;
  for (int i = 0; i < 50; i++)
  {
    calibrationSum += analogRead(vibrationPin);
    delay(10);
  }
  int calibrationValue = calibrationSum / 50;

  // Set the threshold based on the calibrated value
  threshold = calibrationValue + 500; //adjust threshold offset as needed

  Serial.println("Initialization complete...");
  delay(1000);
}

void loop()
{
  int vibrationValue = analogRead(vibrationPin);
  Serial.print("\nvibration value: ");
  Serial.println(vibrationValue);

  if(vibrationValue > threshold){
    Serial.println("\nvibration greater than threshold");
    sendMessage(message);
    digitalWrite(relayPin, HIGH);
    delay(10000); //time for the bell to ring

    digitalWrite(relayPin, LOW);
  }
  else {
    Serial.print("\nvibration less than threshold");
  }
  delay(1000);
}

void sendMessage(String message)
{
  for (int i = 0; i < sizeof(phoneNumbers) / sizeof(phoneNumbers[0]); i++)
  {
    Serial.println("Sending Message please wait….");
    SIM900A.println("AT+CMGF=1"); // Text Mode initialization
    delay(1000);

    SIM900A.print("AT+CMGS=\"");
    SIM900A.print(phoneNumbers[i]);
    SIM900A.println("\""); // receiver's mobile number
    delay(1000);

    Serial.println("Set SMS Content");
    SIM900A.println(message); // text message here
    delay(100);

    Serial.println("Done");
    SIM900A.println((char)26); // End of message
    delay(3000); // delay between messages to avoid issues with the GSM module

    Serial.println("Message sent successfully");
  }
}

