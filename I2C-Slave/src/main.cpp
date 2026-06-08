/*
  Copyright () 2025 BahnOtto
  2017.9.23
  PIN 4, 5 (A4,A5) for i2c bus
*/

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>

const byte slaveDeviceId = 1;
byte myOutput = 0;
byte myInput  = 0;
byte i;
byte mask = 1;

int lightPin =  8;
int GS1rtPin =  9;
int GS1wsPin = 10;
int GS2rtPin = 11;
int GS2wsPin = 12;

int min = 100; // gemessene Werte 94, 86, 87, 91
int max = 750; // gemessene Werte 890 971 776 951 960

unsigned int Impulsbreite = 1000;
unsigned long t1;
unsigned long t2;
unsigned long t3;
unsigned long t4;

int r1Pin = A0; // Pin14
int r2Pin = A1; // Pin15
int r3Pin = A2; // Pin16
int r4Pin = A3; // Pin17

int r1Value = 0;
int r2Value = 0;
int r3Value = 0;
int r4Value = 0;

void setOutputPins() {
	if (myOutput & B00000001)
		digitalWrite(lightPin, LOW);
	else
		digitalWrite(lightPin, HIGH);

	if (myOutput & B00000010 )  {
		digitalWrite(GS1rtPin, LOW);
		digitalWrite(GS1wsPin, HIGH);}
	else  {
		digitalWrite(GS1rtPin, HIGH);
		digitalWrite(GS1wsPin, LOW);}

	if (myOutput & B0000100 )  {
		digitalWrite(GS2rtPin, LOW);
		digitalWrite(GS2wsPin, HIGH);}
	else  {
		digitalWrite(GS2rtPin, HIGH);
		digitalWrite(GS2wsPin, LOW);}
}

void readInput(){
	//liest Rückmelder in myInput
    r1Value = analogRead(r1Pin);
	Serial.println(r1Value);
	if (r1Value > max) {
		 t1 = millis();
		 digitalWrite(13, HIGH);
		 bitWrite(myInput,0,1);
	 }
	 if (r1Value < min && millis()-t1 > Impulsbreite) {
	      digitalWrite(13, LOW);
		  bitWrite(myInput,0,0);
	  }
	//------------------------------------
	r2Value = analogRead(r2Pin);
	//	 Serial.println(r2Value);
	if (r2Value > max) {
	     t2 = millis();
		 digitalWrite(13, HIGH);
		 bitWrite(myInput,1,1);
	}
	if (r2Value < min && millis()-t2 > Impulsbreite) {
		  digitalWrite(13, LOW);
		  bitWrite(myInput,1,0);
	
		}
	//------------------------------------
	  r3Value = analogRead(r3Pin);
	//  Serial.println(r3Value);
	  if (r3Value > max) {
		  t3 = millis();
		  digitalWrite(13, HIGH);
		  bitWrite(myInput,2,1);
	   }
	  if (r3Value < min && millis()-t3 > Impulsbreite) {
	      digitalWrite(13, LOW);
	      bitWrite(myInput,2,0);
	   }
	//------------------------------------
	  r4Value = analogRead(r4Pin);
	 // Serial.println(r4Value);
	  if (r4Value > max) {
		  t4 = millis();
		  digitalWrite(13, HIGH);
		  bitWrite(myInput,3,1);
	  }
	  if (r4Value < min && millis()-t4 > Impulsbreite) {
		  digitalWrite(13, LOW);
		  bitWrite(myInput,3,0);
	  }
}


void receiveEvent(int n) {
	myOutput = Wire.read();  // receive byte as an integer
	setOutputPins();
	Serial.println(myOutput);
}

void requestEvent() {
	readInput();
    Wire.write(myInput); //
}

void setup() {
  pinMode(lightPin, OUTPUT);  // bit 0 (01) Licht
  pinMode(GS1wsPin, OUTPUT);  // bit 1 (02) Gs1 weiss
  pinMode(GS1rtPin, OUTPUT);  // bit 2 (04) Gs1 rot
  pinMode(GS2wsPin, OUTPUT);  // bit 3 (08) Gs2 weiss
  pinMode(GS2rtPin, OUTPUT);  // bit 4 (16) Gs2 rot
  // Es sind nur 3 bit ! vgl. Master !

  pinMode(r1Pin, INPUT_PULLUP);
  pinMode(r2Pin, INPUT_PULLUP);
  pinMode(r3Pin, INPUT_PULLUP);
  pinMode(r4Pin, INPUT_PULLUP); 

  Wire.begin(slaveDeviceId);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent); 
  // myOutput = B00000111; // zum testen!
  setOutputPins();

  Serial.begin(9600);
  Serial.println("setUp Slave");
//  Serial.println(r1Pin);
//  Serial.println(r2Pin);
//  Serial.println(r3Pin);
//  Serial.println(r4Pin);
}

void loop() {
    // alles in dir Interruptroutine 
}
