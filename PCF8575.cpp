/*
 * PCF8575 GPIO Port Expand
 * https://www.mischianti.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Renzo Mischianti www.mischianti.org All right reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "PCF8575.h"
#include "Wire.h"

/**
 * Constructor
 * @param address: i2c address
 */
PCF8575::PCF8575(uint8_t address){
	_wire = &Wire;

	_address = address;
};

/**
 * Construcor
 * @param address: i2c address
 * @param interruptPin: pin to set interrupt
 * @param interruptFunction: function to call when interrupt raised
 */
PCF8575::PCF8575(uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
	_wire = &Wire;

	_address = address;
	_interruptPin = interruptPin;
	_interruptFunction = interruptFunction;
	_usingInterrupt = true;
};

#if !defined(__AVR) && !defined(ARDUINO_ARCH_SAMD) && !defined(TEENSYDUINO) && !defined(ARDUINO_ARCH_RENESAS)
	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 */
	PCF8575::PCF8575(uint8_t address, int sda, int scl){
		_wire = &Wire;

		_address = address;
		_sda = sda;
		_scl = scl;
	};

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 * @param interruptPin: pin to set interrupt
 	 * @param interruptFunction: function to call when interrupt raised
	 */
	PCF8575::PCF8575(uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = &Wire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif

#if defined(ESP32) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_STM32) ||  defined(ARDUINO_ARCH_RENESAS)
	/**
	 * Constructor
	 * @param address: i2c address
	 */
	PCF8575::PCF8575(TwoWire *pWire, uint8_t address){
		_wire = pWire;

		_address = address;
	};

	/**
	 * Construcor
	 * @param address: i2c address
	 * @param interruptPin: pin to set interrupt
	 * @param interruptFunction: function to call when interrupt raised
	 */
	PCF8575::PCF8575(TwoWire *pWire, uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = pWire;

		_address = address;
		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;
		_usingInterrupt = true;
	};
#endif
#if defined(ESP32)

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 */
	PCF8575::PCF8575(TwoWire *pWire, uint8_t address, int sda, int scl){
		_wire = pWire;

		_address = address;
		_sda = sda;
		_scl = scl;
	};

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 * @param interruptPin: pin to set interrupt
	 * @param interruptFunction: function to call when interrupt raised
	 */
	PCF8575::PCF8575(TwoWire *pWire, uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = pWire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif

bool PCF8575::begin(uint8_t address){
	_address = address;
	return PCF8575::begin();
}

void PCF8575::attachInterrupt(){
	// If using interrupt set interrupt value to pin
	if (_usingInterrupt){
//		for (int i = 0; i < 8;i++){
//			if (encoderPins[i]) PCF8574::digitalRead(i);
//		}
//			PCF8574::digitalReadAll();
//			(*_interruptFunction)();

//		DEBUG_PRINTLN("Using interrupt pin (not all pin is interrupted)");
//		::pinMode(_interruptPin, INPUT_PULLUP);
//		attachInterrupt(digitalPinToInterrupt(_interruptPin), (*_interruptFunction), FALLING );
		DEBUG_PRINTLN("Using interrupt pin (not all pin is interrupted)");
		::pinMode(_interruptPin, INPUT_PULLUP);
		::attachInterrupt(digitalPinToInterrupt(_interruptPin), (*_interruptFunction), FALLING );
	}

}
void PCF8575::detachInterrupt(){
	// If using interrupt set interrupt value to pin
	if (_usingInterrupt){
		::detachInterrupt(digitalPinToInterrupt(_interruptPin));
		DEBUG_PRINTLN("Detach interrupt pin");
	}

}


/**
 * wake up i2c controller
 */
bool PCF8575::begin(){
	this->transmissionStatus = 4;

#if !defined(__AVR)  && !defined(ARDUINO_ARCH_SAMD)  && !defined(TEENSYDUINO) && !defined(ARDUINO_ARCH_RENESAS)
	DEBUG_PRINT(F("begin(sda, scl) -> "));DEBUG_PRINT(_sda);DEBUG_PRINT(F(" "));DEBUG_PRINTLN(_scl);
//		_wire->begin(_sda, _scl);
#ifdef ARDUINO_ARCH_STM32
	_wire->begin((uint32_t)_sda, (uint32_t)_scl);
#elif defined(ARDUINO_ARCH_RP2040)
	_wire->setSCL(_scl);
	_wire->setSDA(_sda);
	_wire->begin();
#else
	_wire->begin((int)_sda, (int)_scl);
#endif
#else
//			Default pin for AVR some problem on software emulation
//			#define SCL_PIN _scl
// 			#define SDA_PIN _sda
	_wire->begin();
#endif

// Check if there are pins to set low
if (writeMode>0 || readMode>0){
	DEBUG_PRINTLN("Set write mode");
	_wire->beginTransmission(_address);


	DEBUG_PRINT("resetInitial pin ");
#ifdef PCF8575_SOFT_INITIALIZATION
	resetInitial = writeModeUp | readModePullUp;
#else
	resetInitial = writeModeUp | readMode;
#endif
	DEBUG_PRINTLN( resetInitial, BIN);

//	_wire->write(resetInitial);
	_wire->write((uint8_t) resetInitial);
	_wire->write((uint8_t) (resetInitial >> 8));

	initialBuffer = writeModeUp | readModePullUp;
	byteBuffered = initialBuffer;
	writeByteBuffered = writeModeUp;

	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");
	this->transmissionStatus = _wire->endTransmission();
}

	// If using interrupt set interrupt value to pin
//	if (_usingInterrupt){
//		DEBUG_PRINTLN("Using interrupt pin (not all pin is interrupted)");
//		::pinMode(_interruptPin, INPUT_PULLUP);
//		::attachInterrupt(digitalPinToInterrupt(_interruptPin), (*_interruptFunction), FALLING );
//	}

    PCF8575::attachInterrupt();

	// inizialize last read
	lastReadMillis = millis();

	return this->isLastTransmissionSuccess();
}

/**
 * Set if fin is OUTPUT or INPUT
 * @param pin: pin to set
 * @param mode: mode, supported only INPUT or OUTPUT (to semplify)
 */
void PCF8575::pinMode(uint8_t pin, uint8_t mode, uint8_t output_start){
	DEBUG_PRINT("Set pin ");
	DEBUG_PRINT(pin);
	DEBUG_PRINT(" as ");
	DEBUG_PRINTLN(mode);

	if (mode == OUTPUT){
		writeMode = writeMode | bit(pin);
		if (output_start==HIGH) {
			writeModeUp = writeModeUp | bit(pin);
		}

		//writeMode = writeMode | bit(pin);
		readMode =  readMode & ~bit(pin);
		readModePullDown 	=	readModePullDown 	& 	~bit(pin);
		readModePullUp 		=	readModePullUp 		& 	~bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);

	}else if (mode == INPUT){
		writeMode = writeMode & ~bit(pin);

		readMode 			=   readMode 			| bit(pin);
		readModePullDown 	=	readModePullDown 	| bit(pin);
		readModePullUp 		=	readModePullUp 		& ~bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);
	}else if (mode == INPUT_PULLUP){
		writeMode = writeMode & ~bit(pin);

		readMode 			=   readMode 			| bit(pin);
		readModePullDown 	=	readModePullDown 	& ~bit(pin);
		readModePullUp 		=	readModePullUp 		| bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);
	}
	else{
		DEBUG_PRINTLN("Mode non supported by PCF8575")
	}
	DEBUG_PRINT("Write mode: ");
	DEBUG_PRINTLN(writeMode, BIN);

};

/**
 * Read value from i2c and bufferize it
 * @param force
 */
void PCF8575::readBuffer(bool force){
	if (millis() > PCF8575::lastReadMillis+latency || _usingInterrupt || force){
		_wire->requestFrom(_address,(uint8_t)2);// Begin transmission to PCF8575 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If uint16_ts are available to be recieved
		{
			uint16_t iInput = _wire->read();// Read a uint16_t
			iInput |= _wire->read() << 8;// Read a uint16_t
			if ((iInput & readModePullDown)>0 and (~iInput & readModePullUp)>0){
				byteBuffered = (byteBuffered & ~readMode) | (uint16_t)iInput;
			}
		}
	}
}

#ifndef PCF8575_LOW_MEMORY
	/**
	 * Read value of all INPUT pin
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	PCF8575::DigitalInput PCF8575::digitalReadAll(void){
		DEBUG_PRINTLN("Read from buffer");
		_wire->requestFrom(_address,(uint8_t)2);// Begin transmission to PCF8575 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If uint16_ts are available to be recieved
		{
			  DEBUG_PRINTLN("Data ready");
			  uint16_t iInput = _wire->read();// Read a uint16_t
				iInput |= _wire->read() << 8;// Read a uint16_t

				  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
					  DEBUG_PRINT(" -------- CHANGE --------- ");
					  byteBuffered = (byteBuffered & ~readMode) | (uint16_t)iInput;
				  }

//			  if ((iInput & readMode)>0){
//				  DEBUG_PRINT("Input ");
//				  DEBUG_PRINTLN((uint16_t)iInput, BIN);
//
//				  byteBuffered = byteBuffered | (uint16_t)iInput;
//				  DEBUG_PRINT("byteBuffered ");
//				  DEBUG_PRINTLN(byteBuffered, BIN);
//			  }
		}

		DEBUG_PRINT("Buffer value ");
		DEBUG_PRINTLN(byteBuffered, BIN);
#ifdef NOT_SEQUENTIAL_PINOUT
		if ((bit(0) & readMode)>0) digitalInput.p00 = ((byteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & readMode)>0) digitalInput.p01 = ((byteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & readMode)>0) digitalInput.p02 = ((byteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & readMode)>0) digitalInput.p03 = ((byteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & readMode)>0) digitalInput.p04 = ((byteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & readMode)>0) digitalInput.p05 = ((byteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & readMode)>0) digitalInput.p06 = ((byteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & readMode)>0) digitalInput.p07 = ((byteBuffered & bit(7))>0)?HIGH:LOW;
		if ((bit(8) & readMode)>0) digitalInput.p10 = ((byteBuffered & bit(8))>0)?HIGH:LOW;
		if ((bit(9) & readMode)>0) digitalInput.p11 = ((byteBuffered & bit(9))>0)?HIGH:LOW;
		if ((bit(10) & readMode)>0) digitalInput.p12 = ((byteBuffered & bit(10))>0)?HIGH:LOW;
		if ((bit(11) & readMode)>0) digitalInput.p13 = ((byteBuffered & bit(11))>0)?HIGH:LOW;
		if ((bit(12) & readMode)>0) digitalInput.p14 = ((byteBuffered & bit(12))>0)?HIGH:LOW;
		if ((bit(13) & readMode)>0) digitalInput.p15 = ((byteBuffered & bit(13))>0)?HIGH:LOW;
		if ((bit(14) & readMode)>0) digitalInput.p16 = ((byteBuffered & bit(14))>0)?HIGH:LOW;
		if ((bit(15) & readMode)>0) digitalInput.p17 = ((byteBuffered & bit(15))>0)?HIGH:LOW;

        if ((bit(0) & writeMode)>0) digitalInput.p0 = ((writeByteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & writeMode)>0) digitalInput.p1 = ((writeByteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & writeMode)>0) digitalInput.p2 = ((writeByteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & writeMode)>0) digitalInput.p3 = ((writeByteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & writeMode)>0) digitalInput.p4 = ((writeByteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & writeMode)>0) digitalInput.p5 = ((writeByteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & writeMode)>0) digitalInput.p6 = ((writeByteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & writeMode)>0) digitalInput.p7 = ((writeByteBuffered & bit(7))>0)?HIGH:LOW;
        if ((bit(8) & writeMode)>0) digitalInput.p10 = ((writeByteBuffered & bit(8))>0)?HIGH:LOW;
		if ((bit(9) & writeMode)>0) digitalInput.p11 = ((writeByteBuffered & bit(9))>0)?HIGH:LOW;
		if ((bit(10) & writeMode)>0) digitalInput.p12 = ((writeByteBuffered & bit(10))>0)?HIGH:LOW;
		if ((bit(11) & writeMode)>0) digitalInput.p13 = ((writeByteBuffered & bit(11))>0)?HIGH:LOW;
		if ((bit(12) & writeMode)>0) digitalInput.p14 = ((writeByteBuffered & bit(12))>0)?HIGH:LOW;
		if ((bit(13) & writeMode)>0) digitalInput.p15 = ((writeByteBuffered & bit(13))>0)?HIGH:LOW;
		if ((bit(14) & writeMode)>0) digitalInput.p16 = ((writeByteBuffered & bit(14))>0)?HIGH:LOW;
		if ((bit(15) & writeMode)>0) digitalInput.p17 = ((writeByteBuffered & bit(15))>0)?HIGH:LOW;

#else
		if ((bit(0) & readMode)>0) digitalInput.p0 = ((byteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & readMode)>0) digitalInput.p1 = ((byteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & readMode)>0) digitalInput.p2 = ((byteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & readMode)>0) digitalInput.p3 = ((byteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & readMode)>0) digitalInput.p4 = ((byteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & readMode)>0) digitalInput.p5 = ((byteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & readMode)>0) digitalInput.p6 = ((byteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & readMode)>0) digitalInput.p7 = ((byteBuffered & bit(7))>0)?HIGH:LOW;
		if ((bit(8) & readMode)>0) digitalInput.p8 = ((byteBuffered & bit(8))>0)?HIGH:LOW;
		if ((bit(9) & readMode)>0) digitalInput.p9 = ((byteBuffered & bit(9))>0)?HIGH:LOW;
		if ((bit(10) & readMode)>0) digitalInput.p10 = ((byteBuffered & bit(10))>0)?HIGH:LOW;
		if ((bit(11) & readMode)>0) digitalInput.p11 = ((byteBuffered & bit(11))>0)?HIGH:LOW;
		if ((bit(12) & readMode)>0) digitalInput.p12 = ((byteBuffered & bit(12))>0)?HIGH:LOW;
		if ((bit(13) & readMode)>0) digitalInput.p13 = ((byteBuffered & bit(13))>0)?HIGH:LOW;
		if ((bit(14) & readMode)>0) digitalInput.p14 = ((byteBuffered & bit(14))>0)?HIGH:LOW;
		if ((bit(15) & readMode)>0) digitalInput.p15 = ((byteBuffered & bit(15))>0)?HIGH:LOW;

        if ((bit(0) & writeMode)>0) digitalInput.p0 = ((writeByteBuffered & bit(0))>0)?HIGH:LOW;
        if ((bit(1) & writeMode)>0) digitalInput.p1 = ((writeByteBuffered & bit(1))>0)?HIGH:LOW;
        if ((bit(2) & writeMode)>0) digitalInput.p2 = ((writeByteBuffered & bit(2))>0)?HIGH:LOW;
        if ((bit(3) & writeMode)>0) digitalInput.p3 = ((writeByteBuffered & bit(3))>0)?HIGH:LOW;
        if ((bit(4) & writeMode)>0) digitalInput.p4 = ((writeByteBuffered & bit(4))>0)?HIGH:LOW;
        if ((bit(5) & writeMode)>0) digitalInput.p5 = ((writeByteBuffered & bit(5))>0)?HIGH:LOW;
        if ((bit(6) & writeMode)>0) digitalInput.p6 = ((writeByteBuffered & bit(6))>0)?HIGH:LOW;
        if ((bit(7) & writeMode)>0) digitalInput.p7 = ((writeByteBuffered & bit(7))>0)?HIGH:LOW;
        if ((bit(8) & writeMode)>0) digitalInput.p8 = ((writeByteBuffered & bit(8))>0)?HIGH:LOW;
        if ((bit(9) & writeMode)>0) digitalInput.p9 = ((writeByteBuffered & bit(9))>0)?HIGH:LOW;
        if ((bit(10) & writeMode)>0) digitalInput.p10 = ((writeByteBuffered & bit(10))>0)?HIGH:LOW;
        if ((bit(11) & writeMode)>0) digitalInput.p11 = ((writeByteBuffered & bit(11))>0)?HIGH:LOW;
        if ((bit(12) & writeMode)>0) digitalInput.p12 = ((writeByteBuffered & bit(12))>0)?HIGH:LOW;
        if ((bit(13) & writeMode)>0) digitalInput.p13 = ((writeByteBuffered & bit(13))>0)?HIGH:LOW;
        if ((bit(14) & writeMode)>0) digitalInput.p14 = ((writeByteBuffered & bit(14))>0)?HIGH:LOW;
        if ((bit(15) & writeMode)>0) digitalInput.p15 = ((writeByteBuffered & bit(15))>0)?HIGH:LOW;

#endif
//		if ((readMode & byteBuffered)>0){
//			byteBuffered = ~readMode & byteBuffered;
//			DEBUG_PRINT("Buffer hight value readed set readed ");
//			DEBUG_PRINTLN(byteBuffered, BIN);
//		}
		byteBuffered = (initialBuffer & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;

			DEBUG_PRINT("Buffer hight value readed set readed ");
			DEBUG_PRINTLN(byteBuffered, BIN);

		DEBUG_PRINT("Return value ");
		return digitalInput;
	};
#else
	/**
	 * Read value of all INPUT pin in byte format for low memory usage
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	uint16_t PCF8575::digitalReadAll(void){
		DEBUG_PRINTLN("Read from buffer");
		_wire->requestFrom(_address,(uint8_t)2);// Begin transmission to PCF8575 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If uint16_ts are available to be recieved
		{
			  DEBUG_PRINTLN("Data ready");
			  uint16_t iInput = _wire->read();// Read a uint16_t
				iInput |= _wire->read() << 8;// Read a uint16_t

//			  if ((iInput & readMode)>0){
//				  DEBUG_PRINT("Input ");
//				  DEBUG_PRINTLN((uint16_t)iInput, BIN);
//
//				  byteBuffered = byteBuffered | (uint16_t)iInput;
//				  DEBUG_PRINT("byteBuffered ");
//				  DEBUG_PRINTLN(byteBuffered, BIN);
//			  }
			  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
				  DEBUG_PRINT(" -------- CHANGE --------- ");
				  byteBuffered = (byteBuffered & ~readMode) | (uint16_t)iInput;

			  }

		}

		DEBUG_PRINT("Buffer value ");
		DEBUG_PRINTLN(byteBuffered, BIN);

//		uint16_t byteRead = byteBuffered;
//
//		if ((readMode & byteBuffered)>0){
//			byteBuffered = ~readMode & byteBuffered;
//			DEBUG_PRINT("Buffer hight value readed set readed ");
//			DEBUG_PRINTLN(byteBuffered, BIN);
//		}
		byte byteRead = byteBuffered | writeByteBuffered;

		//if ((byteBuffered & readModePullDown)>0 and (~byteBuffered & readModePullUp)>0){
//			byteBuffered = (resetInitial & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;
		byteBuffered = (initialBuffer & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;
			DEBUG_PRINT("Buffer hight value readed set readed ");
			DEBUG_PRINTLN(byteBuffered, BIN);

		DEBUG_PRINT("Return value ");
		return byteRead;
	};
#endif

/**
 * Read value of specified pin
 * Debounce read more fast than 10millis, non managed for interrupt mode
 * @param pin
 * @return
 */
	uint8_t PCF8575::digitalRead(uint8_t pin, bool forceReadNow){
		uint8_t value = (bit(pin) & readModePullUp)?HIGH:LOW;
		DEBUG_PRINT("Read pin ");
		DEBUG_PRINT (pin);
		// Check if pin already HIGH than read and prevent reread of i2c
	//	DEBUG_PRINTLN("----------------------------------")
	//	DEBUG_PRINT("readModePullUp   ");
	//	DEBUG_PRINTLN(readModePullUp, BIN);
	//	DEBUG_PRINT("readModePullDown ");
	//	DEBUG_PRINTLN(readModePullDown, BIN);
	//	DEBUG_PRINT("byteBuffered     ");
	//	DEBUG_PRINTLN(byteBuffered, BIN);


		if ((((bit(pin) & (readModePullDown & byteBuffered))>0) or (bit(pin) & (readModePullUp & ~byteBuffered))>0 )){
			DEBUG_PRINTLN(" ...Pin already set");
			  if ((bit(pin) & byteBuffered)>0){
				  value = HIGH;
			  }else{
				  value = LOW;
			  }
		 }else if (forceReadNow || (millis() > PCF8575::lastReadMillis+latency)){
			 DEBUG_PRINT(" ...Read from buffer... ");
			  _wire->requestFrom(_address,(uint8_t)2);// Begin transmission to PCF8574 with the buttons
			  lastReadMillis = millis();
			  if(_wire->available())   // If bytes are available to be recieved
			  {
				  DEBUG_PRINTLN(" Data ready");
			  uint16_t iInput = _wire->read();// Read a uint16_t
				iInput |= _wire->read() << 8;// Read a uint16_t
				  DEBUG_PRINT("Input ");
				  DEBUG_PRINTLN((uint16_t)iInput, BIN);

				  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
					  DEBUG_PRINT(" -------- CHANGE --------- ");
					  byteBuffered = (byteBuffered & ~readMode) | (uint16_t)iInput;
					  if ((bit(pin) & byteBuffered)>0){
						  value = HIGH;
					  }else{
						  value = LOW;
					  }
	//				  value = (bit(pin) & byteBuffered);
				  }
			  }
		}
		DEBUG_PRINT(" ..Buffer value ");
		DEBUG_PRINT(byteBuffered, BIN);
		// If HIGH set to low to read buffer only one time
		if ((bit(pin) & readModePullDown) and value==HIGH){
			byteBuffered = bit(pin) ^ byteBuffered;
			DEBUG_PRINT(" ...Buffer hight value readed set readed ");
			DEBUG_PRINT (byteBuffered, BIN);
		}else if ((bit(pin) & readModePullUp) and value==LOW){
			byteBuffered = bit(pin) ^ byteBuffered;
			DEBUG_PRINT(" ...Buffer low value readed set readed ");
			DEBUG_PRINT(byteBuffered, BIN);
		}else if(bit(pin) & writeByteBuffered){
			value = HIGH;
		}
		DEBUG_PRINT(" ...Return value ");
		DEBUG_PRINTLN(value);
		return value;
	};
/**
 * Write on pin
 * @param pin
 * @param value
 */
//void PCF8575::digitalWrite(uint8_t pin, uint8_t value){
//	DEBUG_PRINTLN("Begin trasmission");
//	_wire->beginTransmission(_address);     //Begin the transmission to PCF8575
//	if (value==HIGH){
//		writeByteBuffered = writeByteBuffered | bit(pin);
//	}else{
//		writeByteBuffered = writeByteBuffered & ~bit(pin);
//	}
////	DEBUG_PRINT("Write data ");
////	DEBUG_PRINT(writeByteBuffered, BIN);
////	DEBUG_PRINT(" for pin ");
////	DEBUG_PRINT(pin);
////	DEBUG_PRINT(" bin value ");
////	DEBUG_PRINT(bit(pin), BIN);
////	DEBUG_PRINT(" value ");
////	DEBUG_PRINTLN(value);
//
////	Serial.print(" --> ");
////	Serial.println(writeByteBuffered);
////	Serial.println((uint8_t) writeByteBuffered);
////	Serial.println((uint8_t) (writeByteBuffered >> 8));
//
//	writeByteBuffered = writeByteBuffered & writeMode;
//	_wire->write((uint8_t) writeByteBuffered);
//	_wire->write((uint8_t) (writeByteBuffered >> 8));
//	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");
//
//	_wire->endTransmission();
//};

bool PCF8575::digitalWrite(uint8_t pin, uint8_t value){
	DEBUG_PRINTLN("Begin trasmission");
	_wire->beginTransmission(_address);     //Begin the transmission to PCF8574
	DEBUG_PRINT("Value ");
	DEBUG_PRINT(value);
	DEBUG_PRINT(" Write data pre ");
	DEBUG_PRINT(writeByteBuffered, BIN);

	if (value==HIGH){
		writeByteBuffered = writeByteBuffered | bit(pin);
		byteBuffered  = writeByteBuffered | bit(pin);
	}else{
		writeByteBuffered = writeByteBuffered & ~bit(pin);
		byteBuffered  = writeByteBuffered & ~bit(pin);
	}
	DEBUG_PRINT("Write data ");
	DEBUG_PRINT(writeByteBuffered, BIN);
	DEBUG_PRINT(" for pin ");
	DEBUG_PRINT(pin);
	DEBUG_PRINT(" bin value ");
	DEBUG_PRINT(bit(pin), BIN);
	DEBUG_PRINT(" value ");
	DEBUG_PRINT(value);

	// writeByteBuffered = writeByteBuffered & (~writeMode & byteBuffered);
	byteBuffered = (writeByteBuffered & writeMode) | (resetInitial & readMode);

	// byteBuffered = (writeByteBuffered & writeMode) | (byteBuffered & readMode);
	DEBUG_PRINT(" byteBuffered ");
	DEBUG_PRINTLN(byteBuffered, BIN);

	DEBUG_PRINT("Going to write data ");
	DEBUG_PRINTLN(writeByteBuffered, BIN);

	_wire->write((uint8_t) byteBuffered);
	_wire->write((uint8_t) (byteBuffered >> 8));

//	_wire->write(byteBuffered);

	byteBuffered = (writeByteBuffered & writeMode) | (initialBuffer & readMode);

//	byteBuffered = (writeByteBuffered & writeMode) & (byteBuffered & readMode);
	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");

//	detachInterrupt();
	this->transmissionStatus = _wire->endTransmission();
//	attachInterrupt();

	return this->isLastTransmissionSuccess();
};


