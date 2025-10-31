/*
 * Example: esp8266_interrupt_button
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates how to use the interrupt functionality of the PCF8575 I/O expander
 * with an ESP8266 (WeMos D1 Mini). When a button connected to the PCF8575 is pressed,
 * it triggers an interrupt on the ESP8266, which then reads all the input pins.
 *
 * Connections:
 * PCF8575    ----- WeMos D1 Mini
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- D2 (GPIO4) (with PullUp resistor)
 * SCL        ----- D1 (GPIO5) (with PullUp resistor)
 * INT        ----- D7 (GPIO13)
 *
 * P0-P15     ----- Buttons (connected to GND, using INPUT_PULLUP)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"  // https://github.com/xreef/PCF8575_library

// Define the ESP8266 pin connected to the PCF8575's INT pin
// For WeMos D1 mini, GPIO13 is D7
#define ESP8266_INTERRUPT_PIN D7

// I2C Address of the PCF8575
const uint8_t i2cAddress = 0x20;

// Flag to indicate that an interrupt has occurred
volatile bool interruptFlag = false;

// Interrupt Service Routine (ISR)
// This function is called when the interrupt pin goes from HIGH to LOW (FALLING edge)
// It should be as short and fast as possible.
void ICACHE_RAM_ATTR handleInterrupt() {
  // Set the flag to true, the main loop will handle the rest
  interruptFlag = true;
}

// Create an instance of the PCF8575 library
PCF8575 pcf8575(i2cAddress);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP8266 PCF8575 Interrupt Test");

  // Set the interrupt pin as an input with a pull-up resistor
  pinMode(ESP8266_INTERRUPT_PIN, INPUT_PULLUP);
  // Attach the interrupt to the pin
  attachInterrupt(digitalPinToInterrupt(ESP8266_INTERRUPT_PIN), handleInterrupt, FALLING);

  // Set all PCF8575 pins to INPUT_PULLUP mode for the buttons
  for (int i = 0; i < 16; i++) {
    pcf8575.pinMode(i, INPUT_PULLUP);
  }

  // Initialize the PCF8575
  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found");
    while (1);
  }
  Serial.println("PCF8575 initialized.");
}

void loop() {
  // Check if the interrupt flag has been set
  if (interruptFlag) {
    Serial.println("Interrupt triggered! Reading all pins...");

    // Read all the pins from the PCF8575
    PCF8575::DigitalInput values = pcf8575.digitalReadAll();

    // Check which button was pressed (input will be LOW)
    if (values.p0 == LOW) Serial.println("Button on P0 pressed");
    if (values.p1 == LOW) Serial.println("Button on P1 pressed");
    if (values.p2 == LOW) Serial.println("Button on P2 pressed");
    if (values.p3 == LOW) Serial.println("Button on P3 pressed");
    if (values.p4 == LOW) Serial.println("Button on P4 pressed");
    if (values.p5 == LOW) Serial.println("Button on P5 pressed");
    if (values.p6 == LOW) Serial.println("Button on P6 pressed");
    if (values.p7 == LOW) Serial.println("Button on P7 pressed");
    if (values.p8 == LOW) Serial.println("Button on P8 pressed");
    if (values.p9 == LOW) Serial.println("Button on P9 pressed");
    if (values.p10 == LOW) Serial.println("Button on P10 pressed");
    if (values.p11 == LOW) Serial.println("Button on P11 pressed");
    if (values.p12 == LOW) Serial.println("Button on P12 pressed");
    if (values.p13 == LOW) Serial.println("Button on P13 pressed");
    if (values.p14 == LOW) Serial.println("Button on P14 pressed");
    if (values.p15 == LOW) Serial.println("Button on P15 pressed");

    // Reset the interrupt flag
    interruptFlag = false;
  }
}
