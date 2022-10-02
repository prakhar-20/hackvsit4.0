//BY basic electronics
#include <SoftwareSerial.h>
SoftwareSerial bt(5,6);
char command; 
int red = 8;
int rstate =0;
int bstate=0;
String password;
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
int irsensor = 3;
#define SS_PIN 10
#define RST_PIN 9
int micPin = A0;          // pin that the mic is attached to
int gndPin = A1;
int powerPin = A2;
int micValue1 = 0;  
int micValue2 = 0; // the Microphone value
int led1 = 4;
int doorstate = 0;
int buzzer = 2;
boolean lightOn = false;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name
int taptap =0;
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  pinMode(micPin, INPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(gndPin,LOW);
  delay(500);
  digitalWrite(powerPin,HIGH);
  pinMode(irsensor,INPUT);
  // put your setup code here, to run once:
bt.begin(9600);
Serial.begin(9600);
SPI.begin(); 
pinMode(8,OUTPUT);
mfrc522.PCD_Init();   // Initiate MFRC522
myServo.attach(7); //servo pin
myServo.write(100); //servo start position
Serial.println("Put your card to the reader...");
Serial.println();
delay(5000);
myServo.write(155);
}

void loop() {
  // put your main code here, to run repeatedly:

 if(digitalRead(irsensor)==LOW)
 {
  doorstate = 1;
 }
if(doorstate == 1 and digitalRead(irsensor)==HIGH)
{
  delay(5000);
  doorstate = 0;
  myServo.write(155);
  }
if(taptap>2)
{
  digitalWrite(led1,HIGH);
  }
else
{
digitalWrite(led1,LOW);
}

if(taptap>4)
{
  digitalWrite(buzzer,HIGH);
  }
else
{
  digitalWrite(buzzer,LOW);
  }
micValue1 = analogRead(micPin);  // read pin value
  //Serial.println(micValue1);
  delay(1);
  micValue2 = analogRead(micPin);
  //Serial.println(micValue2);
  
  if (micValue1-micValue2 > 4||micValue2-micValue1 > 4){
    Serial.println(micValue1);
    Serial.println(micValue2);
    taptap+=1;
  //lightOn = !lightOn;
  delay(100);
 // digitalWrite(led1, lightOn);
  }
 
 
if(bt.available() > 0){ 
    command = bt.read(); 
    password+=command;
    
    Serial.println(password);
 


      if(password.equals("openlock"))
    {
      digitalWrite(red,HIGH);
          myServo.write(100);
          taptap=0;
          password = "";

    }
    else if(password.indexOf("clear")>=0)

    {
      password="";
 
      }
}

    
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A6 C4 F9 F9" or content.substring(1)== "13 C1 4C 63") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    myServo.attach(7);
    myServo.write(100);
    taptap =0;

     }
     else
     {Serial.println("Unauthorised access");
     }

    }
