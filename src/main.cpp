#include "Arduino.h"

#include "DFRobot_ESP_EC.h"   //https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK.git
#include "Adafruit_ADS1015.h" //https://github.com/GreenPonik/Adafruit_ADS1X15.git

#include "OneWire.h"
#include "DallasTemperature.h"

#include "EEPROM.h"

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

float voltage, ecValue, temperature = 25;

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
	sensors.requestTemperatures();
	return sensors.getTempCByIndex(0);
}

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(32); //needed EEPROM.begin to store calibration k in eeprom
	ec.begin();		  //by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
	sensors.begin();
	ads.setGain(GAIN_ONE);
	ads.begin();
}

void loop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{

		timepoint = millis();
		/**
		 * index 0 for adc's pin A0
 		 * index 1 for adc's pin A1
		 * index 2 for adc's pin A2
		 * index 3 for adc's pin A3
		*/
		voltage = ads.readADC_SingleEnded(0) / 10;
		Serial.print("voltage:");
		Serial.println(voltage, 4);

		//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
		Serial.print("temperature:");
		Serial.print(temperature, 1);
		Serial.println("^C");

		ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
		Serial.print("EC:");
		Serial.print(ecValue, 4);
		Serial.println("ms/cm");
	}
	ec.calibration(voltage, temperature); // calibration process by Serail CMD
}
