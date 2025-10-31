/*
 * Example: generic_read_single_button
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This is a basic example to demonstrate how to read a single button connected to a PCF8575 I/O expander.
 * It's a generic example that can be used with any supported board (Arduino, ESP8266, ESP32, etc.).
 *
 * Connections:
 * PCF8575    ----- Microcontroller
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- SDA pin of your microcontroller (e.g., A4 on Arduino Uno)
 * SCL        ----- SCL pin of your microcontroller (e.g., A5 on Arduino Uno)
 *
 * P1         ----- Button (connected to GND, using INPUT_PULLUP)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"

// Set i2c address of the PCF8575
PCF8575 pcf8575(0x20);

void setup()
{
	Serial.begin(115200);

	// Set pin P1 to INPUT_PULLUP mode
	pcf8575.pinMode(P1, INPUT_PULLUP);

    // Initialize the PCF8575
	if (!pcf8575.begin()){
        Serial.println("PCF8575 not found");
        while(1);
    }
}

void loop()
{
    // Read the value of the button on pin P1
	uint8_t val = pcf8575.digitalRead(P1);

    // If the button is pressed, the input will be LOW
	if (val == LOW) {
        Serial.println("Button on P1 is pressed");
    }

	delay(50); // Small delay for debouncing
}
