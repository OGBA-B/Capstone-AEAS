/**************************************************************************/
/*This code makes use of an RFID Scanner and FingerPrint Sensor to 
   create an access control system for students entering an examination.
   The code can be applied to other areas.*/
/*! Files Used include
    @file     readMifare.pde
    @author   Adafruit Industries
  @license  BSD (see license.txt)

   

   More info:
    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:
   
    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then read any of the
      4 blocks in that sector (though only block 4 is read here)
   
    If the card has a 7-byte UID it is probably a Mifare
    Ultralight card, and the 4 byte pages can be read directly.
    Page 4 is read by default since this is the first 'general-
    purpose' page on the tags.

/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <HashMap.h>
#include <StandardCplusplus.h>
#include <stdlib.h>  /* srand, rand */
#include <vector>
#include <Time.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
//int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
using namespace std;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
HashMap sReg;
int pos = 0;  
int capacity=128;
vector<int> seats(101);
// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)
#define CAP 5 // Number of available seats

// Use this line for a breakout with a software SPI connection (recommended):
///Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
void rando(vector<int> &v)
{ int i =0;
    int x;
   //randomSeed(analogRead(0));
   x = (rand()%CAP)+1;
    while(i<=CAP)
    {
    if(std::find(v.begin(), v.end(), x) != v.end())
    {
        x = (rand()%(CAP+1))+1; /* v contains x */
    }
    else
    {
     v.at(i)=x;       /* v does not contain x */
      i++;
    }
    }
}

void setup(void) {
 pinMode(7, OUTPUT); // red led
  pinMode(8, OUTPUT); // green led
   myservo.attach(9); 
  Serial.begin(9600);
   lcd.begin(16,2);         // initialize the lcd for 20 chars 4 lines and turn on backlight
 // Serial.println("");
  //Serial.println("EXAM NAME: CEG4912 Midterm");
 // Serial.println("");
 // Serial.println("INSTRUCTOR: Proffessor Dan Ionescu");
  lcd.setCursor(0,0);
  lcd.print("EXAM NAME:");
  lcd.setCursor(0,1);
  lcd.print("CEG4912 Final");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INSTRUCTOR:PROF.");
  lcd.setCursor(0,1);
  lcd.print("Dan Ionescu");
  delay(3000);
  lcd.clear();
  rando(seats);
   sReg.put(2187779150,"B. Okparanyote",seats.at(0));
  sReg.put(1458445383,"S.Shaikh",seats.at(1));
  sReg.put(1990073671,"O.David",seats.at(2));
  sReg.put(1156592363,"L.Card",seats.at(3));
  sReg.put(3230269286,"O.Nnaemego",seats.at(4));
  myservo.write(10);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
 /* Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);*/
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("");
  Serial.println("");
  Serial.println("fingertest");
  finger.begin(57600);
   if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
  //Serial.println("Waiting for a Student ...");
  lcd.print("Please Scan Card");
}
/*
 * Init
 */
 
/*shifts the string several times to
 the left to display entire string
 */
 
 void shift(String string) 
{
  int remainder=0;
  if(string.length() >= 16)
  {
    remainder= string.length()-16;
    for(int i=0;i<remainder;i++)
    {
      lcd.scrollDisplayLeft();
      delay(250);
    }
  }
}
String getString(uint32_t id)
 {
return sReg.get(id);
 }
 int getSeat(uint32_t id)
 {
  return sReg.getS(id);
 }

void loop(void) {
  uint32_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    digitalWrite(7, HIGH);
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  Serial.print("I am here1");
  success = nfc.readPassiveTargetID1(PN532_MIFARE_ISO14443A,0); //, uid, &uidLength);
  Serial.print("I am here2");
  //Serial.println(success,DEC);
  if (success) {
   String check= getString(success);
    if(check !=("You are Not Registered"))
    {
   //Serial.print(check);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(check);
   lcd.setCursor(0,1);
   lcd.print("Seat:");
    lcd.setCursor(5,1);
   lcd.print(getSeat(success));
  
   //Serial.println("");
  // Serial.print(uid[0],DEC);Serial.print(uid[1],DEC);Serial.print(uid[2],DEC);Serial.print(uid[3],DEC);
   // Serial.println(success,DEC);
  //  Serial.println("");
  //nfc.PrintHex(uid, uidLength);
    Serial.println("");
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'                    // waits 15ms for the servo to reach the position
  }
  delay(2000);
   for (pos = 180; pos >= 10; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
                      // waits 15ms for the servo to reach the position
  }
     digitalWrite(8, LOW);
     lcd.clear();
     lcd.print("Please Scan Card");
  }
  
  else
      {
        Serial.println("You are not Registered");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("You are Not");
       // delay(1000);  
        lcd.setCursor(0,1);
        lcd.print("Registered ");
        nfc.blinkL();
        lcd.clear();
        lcd.print("Please Scan Card");
      }
}
    else
    {
    Serial.println("Ooops ... unable to read the requested page!?");
    }
} 

//uint8_t getFingerprintID() {
//  uint8_t p = finger.getImage();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image taken");
//      break;
//    case FINGERPRINT_NOFINGER:
//      Serial.println("No finger detected");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return p;
//    case FINGERPRINT_IMAGEFAIL:
//      Serial.println("Imaging error");
//      return p;
//    default:
//      Serial.println("Unknown error");
//      return p;
//  }
//
//  // OK success!
//
//  p = finger.image2Tz();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image converted");
//      break;
//    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return p;
//    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
//      return p;
//    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
//      return p;
//    default:
//      Serial.println("Unknown error");
//      return p;
//  }
//  
//  // OK converted!
//  p = finger.fingerFastSearch();
//  if (p == FINGERPRINT_OK) {
//    Serial.println("Found a print match!");
//  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
//    return p;
//  } else if (p == FINGERPRINT_NOTFOUND) {
//    Serial.println("Did not find a match");
//    return p;
//  } else {
//    Serial.println("Unknown error");
//    return p;
//  }   
//  
//  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
//  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
//}
//
//// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); 
  //Serial.print(finger.fingerID); 
  int value= finger.fingerID;
  lcd.clear();
  if(value == 1) lcd.print("B Okparanyote");
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

