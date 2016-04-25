#include <Wire.h>
#define SLAVE_ADDRESS 0x04
const int LED = 13;
const int buzzer = 2;     // Buzzer Pin
const int buttonPin = 4;  // Open Door Button
const int doorBell = 3;   // DoorBell
const int redButton = 5;  // red Button(random 2nd button)
const unsigned long slowpPokeTime = 120000; // 120 000 = 2min//milliseconds the buzzer gets activated again after rining and openoing 
const int normalTime = 12500; // time in ms the buzzer is activated a 2nd time 
int buttonState = 0; // saves Door Button state
int redButtonFlag = 0;
int receiveDataFlag = 0;
int prevButton = 0;
unsigned long passedTime = 0;
unsigned long savedTime = 0;
unsigned long currenttime = 0;

void setup() 
{
  //Set Buzzer Pin & Led Pin to OUTPUT:
  pinMode(buzzer, OUTPUT); 
  pinMode(LED, OUTPUT);   
 // der buttonPin-Port wird als Eingang festgelegt:
  pinMode(buttonPin, INPUT);
  pinMode(doorBell, INPUT);
  pinMode(redButton, INPUT); 
  Serial.begin(9600);      // open the serial port at 9600 bps:
  Wire.begin(SLAVE_ADDRESS);  // initialize i2c as slave
  Wire.onReceive(receiveData); // define callbacks for i2c communication
  Wire.onRequest(sendData); 
}
  
  
void AktivateDoorBuzzer()
{
  Serial.print("Open Door\n");
  digitalWrite(buzzer, HIGH);    // DoorBuzzer on
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);   //DoorBuzzer off
  digitalWrite(LED, LOW);

}

int SlowpPokeTimePassed() 
{
  // Calculate if door should still be opend 
  passedTime = millis() - savedTime;
  Serial.print(passedTime);
  Serial.print("passedTime\n");

  if (passedTime <= slowpPokeTime)
  {
    Serial.print("true");
    return 1;
  }

  Serial.print("else\n");
  return 0;
}

void loop(){
  buttonState = digitalRead(buttonPin); // Der Zustand des Schalters wird ausgelesen und gespeichert:
  if (buttonState == HIGH || receiveDataFlag == 2) // Prüfe, ob der Schalter(oder I²C) gedrückt ist. 
  {
    receiveDataFlag = 0;
    Serial.print("Button1 at: ");
    savedTime = millis(); // save current time
    Serial.print(savedTime);
    AktivateDoorBuzzer();
    delay(normalTime);
    AktivateDoorBuzzer();
  }

  buttonState = digitalRead(doorBell);
  if (buttonState == LOW && SlowpPokeTimePassed() == 1)
  {
    Serial.print("DoorBell case\n");
    delay(10);
    AktivateDoorBuzzer();
    delay(4000);
  }
  buttonState = digitalRead(redButton);
  if(prevButton != 1 && buttonState == 1)
  {
    redButtonFlag = 1;
  }
  prevButton = buttonState;
}

// callback for sending data
void sendData()
{
  Wire.write(redButtonFlag);
 // Serial.print(redButtonFlag);
  redButtonFlag = 0;
}

void receiveData(int byteCount)
{
  while(Wire.available()) 
  {
    receiveDataFlag = Wire.read();
  }
}

