# PCF8575 I2C 16-bit Digital I/O Expander Library

![PCF8575 Library Logo](resources/pcf8575_library_logo.png)

[![arduino-library-badge](https://www.ardu-badge.com/badge/PCF8575%20library.svg?)](https://www.ardu-badge.com/PCF8575%20library)
[![Platform: Arduino|SAMD|ESP32|ESP8266|RP2040|STM32](https://img.shields.io/badge/Platform-Arduino%20%7C%20SAMD%20%7C%20ESP32%20%7C%20ESP8266%20%7C%20RP2040%20%7C%20STM32-green.svg)]()
[![](https://img.shields.io/badge/License-MIT-lightgrey.svg)](LICENSE)

A simple and efficient library to use the PCF8575 I2C 16-bit digital I/O expander with Arduino, ESP8266, ESP32, RP2040 and other platforms.

**Author:** Renzo Mischianti

**Website / Documentation:** https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/

**GitHub:** https://github.com/xreef/PCF8575_library

---

## 📚 Documentation & Articles

Full documentation, tutorials and examples are available on the author site:

- 🌐 **PCF8575 main article**: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
- 🛠️ **I2C Scanner**: https://playground.arduino.cc/Main/I2cScanner
- 💬 **Support forum (English)**: https://www.mischianti.org/forums/forum/mischiantis-libraries/pcf8575-16bits-i2c-digital-i-o-expander/
- 💬 **Forum di supporto (Italiano)**: https://www.mischianti.org/it/forums/forum/le-librerie-di-mischianti/pcf8575-expander-digitale-i-o-i2c-a-16bits/

---

## 📋 Table of Contents

- [Features](#-features)
- [Supported Platforms](#-supported-platforms)
- [Installation](#-installation)
- [API Overview](#-api-overview)
- [Basic Usage](#-basic-usage)
- [Interrupts](#-interrupts)
- [Rotary Encoder Support](#-rotary-encoder-support)
- [Low Memory Mode](#-low-memory-mode)
- [Example: HC-SR04 Ultrasonic Sensor](#example-hc-sr04-ultrasonic-sensor)
- [Changelog](#-changelog)
- [License](#-license)
- [Contributing](#-contributing)
- [Support & Contact](#-support--contact)

## ✨ Features

- **16 Digital I/O Pins**: Expand your microcontroller's I/O over I2C (two bytes).
- **Input & Output Modes**: Each pin can be individually configured as `INPUT`, `INPUT_PULLUP`, or `OUTPUT`.
- **Interrupt Support**: Leverage the PCF8575 interrupt pin to detect input changes without continuous polling.
- **Rotary Encoder Support**: Helpers for reading rotary encoders (optional compile-time support).
- **Flexible I2C configuration**:
  - Standard PCF8575 addresses (0x20..0x27) depending on A0/A1 pins.
  - Custom SDA/SCL pins for platforms that support it (ESP8266/ESP32/RP2040).
- **Efficient reading**: Read all 16 pins in a single I2C transaction with `digitalReadAll()`.
- **Low Memory Mode**: Optional mode to reduce RAM usage on constrained boards.
- **Pulse measurement helpers**: `pulseIn()` and `pulseInPoll()` to measure pulse widths on expander pins.

## 🎯 Supported Platforms

- ESP32 — ✅ Full support
- ESP8266 — ✅ Full support
- Raspberry Pi Pico (RP2040) — ✅ Full support
- Arduino AVR (Uno, Mega, ..) — ✅ Full support
- Arduino SAMD (Nano 33 IoT, etc.) — ✅ Full support
- STM32 — ✅ Full support

## 📦 Installation

### Arduino IDE (Library Manager)

1. Open Arduino IDE.
2. Go to Sketch > Include Library > Manage Libraries...
3. Search for "PCF8575_library" by Renzo Mischianti and install.

### PlatformIO

Add to `platformio.ini`:

```ini
lib_deps = xreef/PCF8575_library
```

### Manual installation

1. Download the latest release from GitHub: https://github.com/xreef/PCF8575_library/releases
2. Unzip and rename the folder to `PCF8575_library`.
3. Move it into your Arduino `libraries` folder.
4. Restart the Arduino IDE.

---

## 🚀 API Overview

### Constructors

```cpp
// Basic constructor with I2C address
PCF8575(uint8_t address);

// For boards with custom TwoWire instance
PCF8575(TwoWire *pWire, uint8_t address);

// For ESP32/ESP8266/RP2040: specify SDA/SCL
PCF8575(uint8_t address, int sda, int scl);

// With interrupt pin and ISR
PCF8575(uint8_t address, uint8_t interruptPin, void (*interruptFunction)());
```

### Main methods

```cpp
// Initialize the driver (must be called in setup)
bool begin();

// Set a pin mode (INPUT, INPUT_PULLUP, OUTPUT)
void pinMode(uint8_t pin, uint8_t mode, uint8_t output_start = HIGH);

// Read single pin (debounced / buffered)
uint8_t digitalRead(uint8_t pin, bool forceReadNow = false);

// Read all pins at once
#ifndef PCF8575_LOW_MEMORY
  PCF8575::DigitalInput digitalReadAll(); // returns struct with p0..p15 or p00..p17 if NOT_SEQUENTIAL_PINOUT
#else
  uint16_t digitalReadAll(); // returns 16-bit value
#endif

// Write a pin value
bool digitalWrite(uint8_t pin, uint8_t value);

// Pulse measurement helpers
unsigned long pulseIn(uint8_t pin, uint8_t value, unsigned long timeout);
unsigned long pulseInPoll(uint8_t pin, uint8_t value, unsigned long timeout);
```

See `PCF8575.h` for full API and compile-time options.

## 💡 Basic Usage

Simple example: toggle an LED and read a button.

```cpp
#include <Wire.h>
#include <PCF8575.h>

// PCF8575 address example
PCF8575 pcf8575(0x20);

const uint8_t LED_PIN = P0;       // use P0..P15 (or P00..P17 for NOT_SEQUENTIAL_PINOUT)
const uint8_t BUTTON_PIN = P1;

void setup() {
  Serial.begin(115200);

  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found");
    while (1) delay(1000);
  }

  pcf8575.pinMode(LED_PIN, OUTPUT, LOW);
  pcf8575.pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  uint8_t button = pcf8575.digitalRead(BUTTON_PIN);
  if (button == LOW) {
    pcf8575.digitalWrite(LED_PIN, HIGH);
  } else {
    pcf8575.digitalWrite(LED_PIN, LOW);
  }
  delay(100);
}
```

## ⚡ Interrupts

To use interrupts, connect the PCF8575 INT pin to an interrupt-capable MCU pin and provide an ISR. Keep ISR short — set a flag and handle logic in loop().

```cpp
volatile bool keyPressed = false;
void keyPressedOnPCF8575() { keyPressed = true; }

PCF8575 pcf8575(0x20, 2, keyPressedOnPCF8575); // address, interruptPin, ISR

void setup() {
  pcf8575.begin();
}

void loop() {
  if (keyPressed) {
    keyPressed = false;
    auto vals = pcf8575.digitalReadAll();
    // handle values
  }
}
```

## 🔄 Rotary Encoder Support

Optional encoder support can be enabled in `PCF8575.h` via compile-time defines. See comments in the header for available implementations and usage examples.

## 🧠 Low Memory Mode

If you enable `#define PCF8575_LOW_MEMORY` in `PCF8575.h`, `digitalReadAll()` returns a single 16-bit value instead of a struct, saving a few bytes of RAM.

Example (low memory):

```cpp
uint16_t values = pcf8575.digitalReadAll();
bool p0 = (values & bit(0)) > 0;
bool p8 = (values & bit(8)) > 0; // upper byte bit
```

## Example: HC-SR04 Ultrasonic Sensor

Using `pulseIn()` helper on the expander pin is possible but less accurate than a direct MCU pin due to I2C latency. Prefer connecting the ECHO pin to the MCU for precise timing.

```cpp
#include <PCF8575.h>
PCF8575 pcf(0x20);
const uint8_t ECHO_PIN = P0; // on expander
const int TRIG_PIN = 9;     // on MCU

void setup() {
  Serial.begin(115200);
  pcf.begin();
  pcf.pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pcf.pulseIn(ECHO_PIN, HIGH, 30000UL);
  unsigned long distanceCm = duration / 29 / 2;
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  delay(500);
}
```

## 📝 Changelog

- 31/10/2025: v2.0.1
  - Fix: `digitalReadAll()` in low memory mode returns full 16-bit value (no overflow).
  - Fix: corrected pin mapping for `NOT_SEQUENTIAL_PINOUT` (PCF8575.cpp).
  - Fix: in `PCF8575_LOW_MEMORY`, `digitalReadAll()` now returns bits 8-15 correctly by using a 16-bit return value.

- 30/04/2024: v2.0.0
  - Manage INPUT_PULLUP, LOW latency mode, support Arduino UNO R4 and initial OUTPUT value

- 01/02/2024: v1.1.2
  - Add the possibility to provide the address to `begin()` and return the status of begin operation

- 10/07/2023: v1.1.1
  - Add support for Arduino UNO R4

- 16/02/2023: v1.1.0
  - Fix STM32 support and add support for Raspberry Pi Pico and other RP2040 boards
  - Add support for custom SERCOM interface of Arduino SAMD devices
  - Force SDA/SCL to use GPIO numeration for STM32 (see: https://www.mischianti.org/forums/topic/compatible-with-stm32duino/)
  - Force SDA/SCL to use GPIO numeration for other boards (see: https://www.mischianti.org/forums/topic/cannot-set-sda-clk-on-esp8266/)
  - Fix the SDA/SCL type issue (#58) and add basic SAMD support

- 06/04/2022: v1.0.3
  - Fix package size

## 📄 License

This library is released under the MIT License. See the `LICENSE` file for details.

Copyright (c) 2017-2025 Renzo Mischianti

## 🤝 Contributing

Contributions are welcome. Please fork the repository, create a feature branch and submit a pull request.

## 📞 Support & Contact

- Documentation: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
- GitHub Issues: https://github.com/xreef/PCF8575_library/issues
- Author: Renzo Mischianti (@xreef)

---

If this library helped your project, please give it a star on GitHub!
