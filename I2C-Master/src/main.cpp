/*
  Copyright (c) 2025 BahnOtto
  2017.9.23
  PIN 4, 5 (A4,A5) for i2c bus
*/

#include "Arduino.h"
#include <Wire.h>
#include <NmraDCC.h>
#define kDCC_INTERRUPT 0 

#define DCC_PIN 2
NmraDcc DCC;

int slaveDeviceId = 1; 
byte myOutput;
byte myInput;

int r1Pin =  9;
int r2Pin = 10;
int r3Pin = 11;
int r4Pin = 12;

static unsigned long lastMillis = millis();

void send(byte x){
	Serial.print("send ");
	// Serial.println (x, BIN);
	Wire.beginTransmission(slaveDeviceId);
	Wire.write(x);              // sends one byte
	Wire.endTransmission();
}

void setOutput() {
	Serial.println (myInput, BIN);
	if (myInput & B00000001) {
		Serial.println ("bit 0 1");
		digitalWrite(r1Pin, LOW);
	} else  {
	    Serial.println ("bit 0 0");
	    digitalWrite(r1Pin, HIGH);
	}
	if (myInput & B00000010) {
		Serial.println ("bit 1 1");
		digitalWrite(r2Pin, LOW);
	} else  {
		Serial.println ("bit 1 0");
		digitalWrite(r2Pin, HIGH);
	}
	if (myInput & B00000100) {
		Serial.println ("bit 2 1");
		digitalWrite(r3Pin, LOW);
	} else {
		Serial.println ("bit 2 0");
		digitalWrite(r3Pin, HIGH);
	}
	if (myInput & B00001000) {
		Serial.println ("bit 3 1");
		digitalWrite(r4Pin, LOW);
	} else  {
		Serial.println ("bit 3 0");
		digitalWrite(r4Pin, HIGH);
	}
}

void receiveInput(){
	delay(10);                     // wegen Serial
	Wire.requestFrom(1,1);         // request 6 bytes from slave device #8
	while (Wire.available() > 0) { // slave may send less than requested
	     myInput = Wire.read();    // receive a byte as character
	     //   Serial.println(myInput, BIN);
	}
	setOutput();
}

void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data) {
	address -= 1;
	address *= 4;
	address += 1;
	address += (data & 0x06) >> 1;
	boolean enable = (data & 0x01) ? 1 : 0;
	// bit 0  Licht
	// bit 1  Gs1
	// bit 2  Gs2

	switch (address) {
	case 200: {
		if (enable) {
			bitWrite(myOutput,1,1);
			}
		else {
			bitWrite(myOutput,1,0);
			}
		send (myOutput);
		break;
	}
	case 201: {
		if (enable){
			bitWrite(myOutput,2,1);
		}
		else {
			bitWrite(myOutput,2,0);
			}
		send (myOutput);
		break;
	}
	case 202: {
		if (enable)
			bitWrite(myOutput,0,1);
		else
			bitWrite(myOutput,0,0);
		send (myOutput);
		break;
	}}
}

void setup() {
  Wire.begin();
  myOutput = B11111000;
  DCC.pin(digitalPinToInterrupt(DCC_PIN), DCC_PIN, false);
  DCC.init(MAN_ID_DIY, 1, FLAGS_DCC_ACCESSORY_DECODER | FLAGS_OUTPUT_ADDRESS_MODE, 0);
  pinMode(2,INPUT_PULLUP); //Interrupt 0 pin 2
  pinMode(r1Pin, OUTPUT);
  pinMode(r2Pin, OUTPUT);
  pinMode(r3Pin, OUTPUT);
  pinMode(r4Pin, OUTPUT);

//----------------------------------------
  Serial.begin(9600);
  Serial.println("Master gestartet");
  Serial.println("------------");
//----------------------------------------
  send(myOutput);
}

void loop() {
	DCC.process();
	if( millis()-lastMillis > 25 )
	    {
		receiveInput();
		lastMillis = millis();
	    }
}

