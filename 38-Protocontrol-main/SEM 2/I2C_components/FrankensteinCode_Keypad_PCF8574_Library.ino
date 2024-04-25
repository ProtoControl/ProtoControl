// This sketch will contain code from the following 8 files:
// 1) Adafruit_Keypad.h DONE
// 2) Adafruit_Keypad.cpp DONE
// 3) Adafruit_Keypad_Ringbuffer.h DONE
// 4) PCF8574.cpp DONE
// 5) PCF8574.h DONE
// and some code from the following:
// 6) Arduino4Leds4ButtonsWithInterrupt (concerned because I don't care about interrupts) DONE
// 7) keypad_test.ino DONE
// 8) keypad_config.h DONE

//Now, all that remains is to hardcode the rows and columns to particular pins on the pcf8574 pin expander DONE
// AND add the code for the keypad to work DONE
// AND possibly correct the serial baud rate LATER
// AND test with an actual circuit
// AND check the pinmodes DONE

#include "Arduino.h"
//#include "Adafruit_Keypad.h"
#include "Wire.h" // for the PCF8574 Library
#include <math.h> // for the PCF8574 Library
#include <string.h> // for the Adafruit_Keypad.cpp

// PCF8574.h
#define DEFAULT_SDA SDA;
#define DEFAULT_SCL SCL;

// Uncomment to enable printing out nice debug messages.
//  #define PCF8574_DEBUG

// Uncomment for low memory usage this prevent use of complex DigitalInput structure and free 7byte of memory
// #define PCF8574_LOW_MEMORY

// Uncomment for low latency to get realtime data every time.
// #define PCF8574_LOW_LATENCY

//#define PCF8574_SOFT_INITIALIZATION

 #define BASIC_ENCODER_ALGORITHM

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef PCF8574_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

#ifdef PCF8574_LOW_LATENCY
	#define READ_ELAPSED_TIME 0
#else
	#define READ_ELAPSED_TIME 10
#endif

#define P0  	0
#define P1  	1
#define P2  	2
#define P3  	3
#define P4  	4
#define P5  	5
#define P6  	6
#define P7  	7

class PCF8574 {
public:

	PCF8574(uint8_t address);
	PCF8574(uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() );

#if !defined(__AVR) && !defined(ARDUINO_ARCH_SAMD) && !defined(TEENSYDUINO) && !defined(ARDUINO_ARCH_RENESAS)
	PCF8574(uint8_t address, int sda, int scl);
	PCF8574(uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)());
#endif

#if defined(ESP32) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_RENESAS)
	///// changes for second i2c bus
	PCF8574(TwoWire *pWire, uint8_t address);
	PCF8574(TwoWire *pWire, uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() );
#endif
#if defined(ESP32)
	PCF8574(TwoWire *pWire, uint8_t address, int sda, int scl);
	PCF8574(TwoWire *pWire, uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)());
#endif

	bool begin();
	bool begin(uint8_t address);
	void pinMode(uint8_t pin, uint8_t mode, uint8_t output_start = HIGH);

	void encoder(uint8_t pinA, uint8_t pinB);

	void attachInterrupt();
	void detachInterrupt();

	void readBuffer(bool force = true);
	uint8_t digitalRead(uint8_t pin, bool forceReadNow = false);
	#ifndef PCF8574_LOW_MEMORY
		struct DigitalInput {
			uint8_t p0;
			uint8_t p1;
			uint8_t p2;
			uint8_t p3;
			uint8_t p4;
			uint8_t p5;
			uint8_t p6;
			uint8_t p7;
		} digitalInput;


		DigitalInput digitalReadAll(void);

		bool digitalWriteAll(PCF8574::DigitalInput digitalInput);
	#else
		byte digitalReadAll(void);
		bool digitalWriteAll(byte digitalInput);
	#endif
	bool digitalWrite(uint8_t pin, uint8_t value);

//	bool readEncoderValueEvolved(uint8_t pinA, uint8_t pinB, volatile long *encoderValue, bool reverseRotation = false);
//	int8_t readEncoderValueEvolved(uint8_t pinA, uint8_t pinB);

#ifdef BASIC_ENCODER_ALGORITHM
	bool readEncoderValue(uint8_t pinA, uint8_t pinB, volatile long *encoderValue, bool reverseRotation = false);
	int8_t readEncoderValue(uint8_t pinA, uint8_t pinB);
#endif

	int getLatency() const {
		return latency;
	}

	void setLatency(int latency = READ_ELAPSED_TIME) {
		this->latency = latency;
	}

	uint8_t getTransmissionStatusCode() const {
		return transmissionStatus;
	}

	bool isLastTransmissionSuccess(){
		DEBUG_PRINT(F("STATUS --> "));
		DEBUG_PRINTLN(transmissionStatus);
		return transmissionStatus==0;
	}
private:
	uint8_t _address;

	#if !defined(DEFAULT_SDA)
	#  if defined(ARDUINO_ARCH_STM32)
	#    define DEFAULT_SDA PB7
	#  elif defined(ESP8266)
	#    define DEFAULT_SDA 4
	#  elif defined(SDA)
	#    define DEFAULT_SDA SDA
	#  else
	#    error "Error define DEFAULT_SDA, SDA not declared, if you have this error contact the mantainer"
	#  endif
	#endif
	#if !defined(DEFAULT_SCL)
	#  if defined(ARDUINO_ARCH_STM32)
	#    define DEFAULT_SCL PB6
	#  elif defined(ESP8266)
	#    define DEFAULT_SCL 5
	#  elif defined(SDA)
	#    define DEFAULT_SCL SCL
	#  else
	#    error "Error define DEFAULT_SCL, SCL not declared, if you have this error contact the mantainer"
	#  endif
	#endif

	int _sda = DEFAULT_SDA;
	int _scl = DEFAULT_SCL;

	TwoWire *_wire;

	bool _usingInterrupt = false;
	uint8_t _interruptPin = 2;
	void (*_interruptFunction)(){};

// I changed this from 0 to 1
	byte writeMode 			= 	0b00000001;
	byte writeModeUp		= 	0b00000000;
	byte readMode 			= 	0b00000000;
	byte readModePullUp 	= 	0b00000000;
	byte readModePullDown 	= 	0b00000000;
	byte byteBuffered 		= 	0b00000000;
	byte resetInitial		= 	0b00000000;
	byte initialBuffer		= 	0b00000000;
	unsigned long lastReadMillis = 0;

	byte writeByteBuffered = 0b00000000;

	volatile byte encoderValues = 0b00000000;

	uint8_t prevNextCode = 0;
	uint16_t store=0;

	int latency = READ_ELAPSED_TIME;

	bool checkProgression(byte oldValA, byte newValA, byte oldValB, byte newValB, byte validProgression);

//	byte validCW = B11100001;
//	byte validCCW = B01001011;
	byte validCW = 0b01001011;
	byte validCCW = 0b11100001;

	uint8_t transmissionStatus = 0;

	void setVal(uint8_t pin, uint8_t value);
	bool digitalWriteAllBytes(byte allpins);
};

// PCF8574.cpp
/**
 * Constructor
 * @param address: i2c address
 */
PCF8574::PCF8574(uint8_t address){
	_wire = &Wire;

	_address = address;
};

/**
 * Construcor
 * @param address: i2c address
 * @param interruptPin: pin to set interrupt
 * @param interruptFunction: function to call when interrupt raised
 */
PCF8574::PCF8574(uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
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
	PCF8574::PCF8574(uint8_t address, int sda, int scl){
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
	PCF8574::PCF8574(uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = &Wire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif

#if defined(ESP32) || defined(ARDUINO_ARCH_SAMD)|| defined(ARDUINO_ARCH_RP2040)  || defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_RENESAS)
	/**
	 * Constructor
	 * @param address: i2c address
	 */
	PCF8574::PCF8574(TwoWire *pWire, uint8_t address){
		_wire = pWire;

		_address = address;
	};

	/**
	 * Construcor
	 * @param address: i2c address
	 * @param interruptPin: pin to set interrupt
	 * @param interruptFunction: function to call when interrupt raised
	 */
	PCF8574::PCF8574(TwoWire *pWire, uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
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
	PCF8574::PCF8574(TwoWire *pWire, uint8_t address, int sda, int scl){
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
	PCF8574::PCF8574(TwoWire *pWire, uint8_t address, int sda, int scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = pWire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif
	bool encoderPins[8];

	void PCF8574::attachInterrupt(){
		// If using interrupt set interrupt value to pin
		if (_usingInterrupt){
			for (int i = 0; i < 8;i++){
				if (encoderPins[i]) PCF8574::digitalRead(i);
			}
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
	void PCF8574::detachInterrupt(){
		// If using interrupt set interrupt value to pin
		if (_usingInterrupt){
			::detachInterrupt(digitalPinToInterrupt(_interruptPin));
			DEBUG_PRINTLN("Detach interrupt pin");
		}

	}

bool PCF8574::begin(uint8_t address){
	_address = address;
	return PCF8574::begin();
}


/**
 * wake up i2c controller
 */
 // How on earth is everything nested? 
bool PCF8574::begin(){
	this->transmissionStatus = 4;
	#if !defined(__AVR)  && !defined(ARDUINO_ARCH_SAMD)  && !defined(TEENSYDUINO) && !defined(ARDUINO_ARCH_RENESAS)
		DEBUG_PRINT(F("begin(sda, scl) -> "));DEBUG_PRINT(_sda);DEBUG_PRINT(F(" "));DEBUG_PRINTLN(_scl);
  //		_wire->begin(_sda, _scl);
    #ifdef ARDUINO_ARCH_STM32
      _wire->begin((uint32_t)_sda, (uint32_t)_scl);
      Serial.println("Arduino Arch STM32");
    #elif defined(ARDUINO_ARCH_RP2040)
      _wire->setSCL(_scl);
      _wire->setSDA(_sda);
      _wire->begin();
      Serial.println("Arduino Arch RP2040");
    #else
      _wire->begin((int)_sda, (int)_scl);
      Serial.println("other arch");
    #endif

	#else
	//			Default pin for AVR some problem on software emulation
	//			#define SCL_PIN _scl
	// 			#define SDA_PIN _sda
		_wire->begin();
    Serial.println("we have at least one of the Architectures defined in the first if statement");
    if (_wire) {
      Serial.println("Wire initialization succeeded.");
      // You can add additional setup code here if needed.
    } else {
      Serial.println("Error: Wire initialization failed. Check your connections and power.");
      // You may also consider adding additional error handling or diagnostic messages.
    }
	#endif

	// Check if there are pins to set low
  Serial.println(writeMode);
  Serial.println(readMode);
	if (writeMode>0 || readMode>0){
		DEBUG_PRINTLN("Set write mode");
		_wire->beginTransmission(_address);
    Serial.println("found point of entry 1");


		DEBUG_PRINT("resetInitial pin ");
    #ifdef PCF8574_SOFT_INITIALIZATION
      resetInitial = writeModeUp | readModePullUp;
      Serial.println("soft initialization 1");
    #else
      resetInitial = writeModeUp | readMode;
      Serial.println("soft initialization 2");
    #endif

    DEBUG_PRINTLN( resetInitial, BIN);
    Serial.println(resetInitial);

    _wire->write(resetInitial);

    initialBuffer = writeModeUp | readModePullUp;
    byteBuffered = initialBuffer;
    writeByteBuffered = writeModeUp;

    DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");
    this->transmissionStatus = _wire->endTransmission();
  }

	PCF8574::attachInterrupt();

	// inizialize last read
	lastReadMillis = millis();

	return this->isLastTransmissionSuccess();
}

/**
 * Set if fin is OUTPUT or INPUT
 * @param pin: pin to set
 * @param mode: mode, supported only INPUT or OUTPUT (to simplify)
 * @param output_start: output_start, for OUTPUT we can set initial value
 */
void PCF8574::pinMode(uint8_t pin, uint8_t mode, uint8_t output_start){
	DEBUG_PRINT("Set pin ");
	DEBUG_PRINT(pin);
	DEBUG_PRINT(" as ");
	DEBUG_PRINTLN(mode);

	if (mode == OUTPUT){
		writeMode = writeMode | bit(pin);
		if (output_start==HIGH) {
			writeModeUp = writeModeUp | bit(pin);
		}

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
		DEBUG_PRINTLN("Mode non supported by PCF8574")
	}
};


void PCF8574::encoder(uint8_t pinA, uint8_t pinB){
	PCF8574::pinMode(pinA, INPUT_PULLUP);
	PCF8574::pinMode(pinB, INPUT_PULLUP);

	encoderPins[pinA] = true;
	encoderPins[pinB] = true;
}

byte getBit(byte n, byte position)
{
   return (n >> position) & 1;
}

bool PCF8574::checkProgression(byte oldValA, byte oldValB, byte newValA, byte newValB, byte validProgression){
	bool findOldVal = false;
	int posFinded = 0;
	for (int pos = 0; pos<8; pos = pos + 2){
		if ((oldValB == ((validProgression & bit(pos+1))>0?HIGH:LOW)) && (oldValA == ((validProgression & bit(pos+0))>0?HIGH:LOW)) ){
			findOldVal = true;
			posFinded = pos;
		}
	}
	if (!findOldVal) return false;

	posFinded = posFinded + 2;
	if (posFinded>8) posFinded = 0;

	return ((newValB == ((validProgression & bit(posFinded+1))>0?HIGH:LOW)) && (newValA == ((validProgression & bit(posFinded+0))>0?HIGH:LOW)) );
}

#ifdef BASIC_ENCODER_ALGORITHM
	bool PCF8574::readEncoderValue(uint8_t pinA, uint8_t pinB, volatile long *encoderValue, bool reverseRotation){
		PCF8574::detachInterrupt();

		  bool changed = false;

		  byte na = PCF8574::digitalRead(pinA, true);
		  byte nb = PCF8574::digitalRead(pinB, true);

		  byte encoderPinALast = (this->encoderValues & bit(pinA))>0?HIGH:LOW;
		  byte encoderPinBLast = (this->encoderValues & bit(pinB))>0?HIGH:LOW;

		  DEBUG_PRINT(pinA);
		  DEBUG_PRINT(" TO --> ");
		  DEBUG_PRINT(encoderPinALast);
		  DEBUG_PRINT(encoderPinBLast);
		  DEBUG_PRINT(" - ");
		  DEBUG_PRINT(na);
		  DEBUG_PRINT(nb);
		  DEBUG_PRINTLN();

		  if ((encoderPinALast!=na || encoderPinBLast!=nb) && (encoderPinALast == LOW) && (na == HIGH)) {
//			  bool vCW = checkProgression(encoderPinALast, encoderPinBLast, na, nb, validCW);
//			  bool vCCW = checkProgression(encoderPinALast, encoderPinBLast, na, nb, validCCW);

				if (nb == LOW) {
					*encoderValue = *encoderValue + (!reverseRotation?+1:-1);
					changed = true;
				} else {
					*encoderValue = *encoderValue + (!reverseRotation?-1:+1);
					changed = true;
				}

	//			if (nb == LOW && vCW) {
	//	//			checkCW(encoderPinALast, encoderPinBLast, na, nb);
	//				*encoderValue = *encoderValue - 1;
	//				changed = true;
	//			} else if (vCCW) {
	//				*encoderValue = *encoderValue + 1;
	//				changed = true;
	//			}

		  }

		  this->encoderValues = (encoderPinALast!=na)?this->encoderValues ^ bit(pinA):this->encoderValues;
		  this->encoderValues = (encoderPinBLast!=nb)?this->encoderValues ^ bit(pinB):this->encoderValues;
		  PCF8574::attachInterrupt();

			return changed;
	}
	int8_t PCF8574::readEncoderValue(uint8_t pinA, uint8_t pinB) {
		volatile long encoderValue = 0;
		PCF8574::readEncoderValue(pinA, pinB, &encoderValue);
		return encoderValue;
	}

#endif

/**
 * Read value from i2c and bufferize it
 * @param force
 */
void PCF8574::readBuffer(bool force){
	if (millis() > PCF8574::lastReadMillis+latency || _usingInterrupt || force){
		_wire->requestFrom(_address,(uint8_t)1);// Begin transmission to PCF8574 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If bytes are available to be recieved
		{
			byte iInput = _wire->read();// Read a byte
			  if ((iInput & readModePullDown)>0 and (~iInput & readModePullUp)>0){
//			  if ((iInput & readMode)>0){
				byteBuffered = (byteBuffered & ~readMode) | (byte)iInput;
			}
		}
	}
}

#ifndef PCF8574_LOW_MEMORY
	/**
	 * Read value of all INPUT pin
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	PCF8574::DigitalInput PCF8574::digitalReadAll(void){
		DEBUG_PRINTLN("Read from buffer");
		_wire->requestFrom(_address,(uint8_t)1);// Begin transmission to PCF8574 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If bytes are available to be recieved
		{
			  DEBUG_PRINTLN("Data ready");
			  byte iInput = _wire->read();// Read a byte

			  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
				  DEBUG_PRINT(" -------- CHANGE --------- ");
				  byteBuffered = (byteBuffered & ~readMode) | (byte)iInput;
			  }
		}

		DEBUG_PRINT("Buffer value ");
		DEBUG_PRINTLN(byteBuffered, BIN);

		if ((bit(0) & readMode)>0) digitalInput.p0 = ((byteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & readMode)>0) digitalInput.p1 = ((byteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & readMode)>0) digitalInput.p2 = ((byteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & readMode)>0) digitalInput.p3 = ((byteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & readMode)>0) digitalInput.p4 = ((byteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & readMode)>0) digitalInput.p5 = ((byteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & readMode)>0) digitalInput.p6 = ((byteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & readMode)>0) digitalInput.p7 = ((byteBuffered & bit(7))>0)?HIGH:LOW;

		if ((bit(0) & writeMode)>0) digitalInput.p0 = ((writeByteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & writeMode)>0) digitalInput.p1 = ((writeByteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & writeMode)>0) digitalInput.p2 = ((writeByteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & writeMode)>0) digitalInput.p3 = ((writeByteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & writeMode)>0) digitalInput.p4 = ((writeByteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & writeMode)>0) digitalInput.p5 = ((writeByteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & writeMode)>0) digitalInput.p6 = ((writeByteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & writeMode)>0) digitalInput.p7 = ((writeByteBuffered & bit(7))>0)?HIGH:LOW;

		//if ((byteBuffered & readModePullDown)>0 and (~byteBuffered & readModePullUp)>0){

//		byteBuffered = (resetInitial & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;

		byteBuffered = (initialBuffer & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;

			DEBUG_PRINT("Buffer hight value readed set readed ");
			DEBUG_PRINTLN(byteBuffered, BIN);
		//}
		DEBUG_PRINT("Return value ");
		return digitalInput;
	};
#else
	/**
	 * Read value of all INPUT pin in byte format for low memory usage
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	byte PCF8574::digitalReadAll(void){
		DEBUG_PRINTLN("Read from buffer");
		_wire->requestFrom(_address,(uint8_t)1);// Begin transmission to PCF8574 with the buttons
		lastReadMillis = millis();
		if(_wire->available())   // If bytes are available to be recieved
		{
			  DEBUG_PRINTLN("Data ready");
			  byte iInput = _wire->read();// Read a byte

			  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
				  DEBUG_PRINT(" -------- CHANGE --------- ");
				  byteBuffered = (byteBuffered & ~readMode) | (byte)iInput;

			  }
		}

		DEBUG_PRINT("Buffer value ");
		DEBUG_PRINTLN(byteBuffered, BIN);

		byte byteRead = byteBuffered | writeByteBuffered;

		//if ((byteBuffered & readModePullDown)>0 and (~byteBuffered & readModePullUp)>0){
//			byteBuffered = (resetInitial & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;
		byteBuffered = (initialBuffer & readMode) | (byteBuffered  & ~readMode); //~readMode & byteBuffered;
			DEBUG_PRINT("Buffer hight value readed set readed ");
			DEBUG_PRINTLN(byteBuffered, BIN);
		//}
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
uint8_t PCF8574::digitalRead(uint8_t pin, bool forceReadNow){
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
	 }else if (forceReadNow || (millis() > PCF8574::lastReadMillis+latency)){
		 DEBUG_PRINT(" ...Read from buffer... ");
		  _wire->requestFrom(_address,(uint8_t)1);// Begin transmission to PCF8574 with the buttons
		  lastReadMillis = millis();
		  if(_wire->available())   // If bytes are available to be recieved
		  {
			  DEBUG_PRINTLN(" Data ready");
			  byte iInput = _wire->read();// Read a byte
			  DEBUG_PRINT("Input ");
			  DEBUG_PRINT((byte)iInput, BIN);

			  if ((readModePullDown & iInput)>0 or (readModePullUp & ~iInput)>0){
				  DEBUG_PRINT(" -------- CHANGE --------- ");
				  byteBuffered = (byteBuffered & ~readMode) | (byte)iInput;
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
bool PCF8574::digitalWrite(uint8_t pin, uint8_t value){
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

	_wire->write(byteBuffered);

	byteBuffered = (writeByteBuffered & writeMode) | (initialBuffer & readMode);

//	byteBuffered = (writeByteBuffered & writeMode) & (byteBuffered & readMode);
	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");

	this->transmissionStatus = _wire->endTransmission();

	return this->isLastTransmissionSuccess();
};

#ifndef PCF8574_LOW_MEMORY
	/**
	 * Read value of all INPUT pin
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	void PCF8574::setVal(uint8_t pin, uint8_t value){
		if (value==HIGH){
			writeByteBuffered = writeByteBuffered | bit(pin);
			byteBuffered  = writeByteBuffered | bit(pin);
		}else{
			writeByteBuffered = writeByteBuffered & ~bit(pin);
			byteBuffered  = writeByteBuffered & ~bit(pin);
		}

	}
	bool PCF8574::digitalWriteAll(PCF8574::DigitalInput digitalInput){

		setVal(P0, digitalInput.p0);
		setVal(P1, digitalInput.p1);
		setVal(P2, digitalInput.p2);
		setVal(P3, digitalInput.p3);
		setVal(P4, digitalInput.p4);
		setVal(P5, digitalInput.p5);
		setVal(P6, digitalInput.p6);
		setVal(P7, digitalInput.p7);

		return digitalWriteAllBytes(writeByteBuffered);
	}
#else
	bool PCF8574::digitalWriteAll(byte digitalInput){
		return digitalWriteAllBytes(digitalInput);
	}
#endif


bool PCF8574::digitalWriteAllBytes(byte allpins){
	_wire->beginTransmission(_address);     //Begin the transmission to PCF8574

	// writeByteBuffered = writeByteBuffered & (~writeMode & byteBuffered);
	writeByteBuffered = allpins;
	byteBuffered = (writeByteBuffered & writeMode) | (resetInitial & readMode);

	// byteBuffered = (writeByteBuffered & writeMode) | (byteBuffered & readMode);
	DEBUG_PRINT(" byteBuffered ");
	DEBUG_PRINTLN(byteBuffered, BIN);

	DEBUG_PRINT("Going to write data ");
	DEBUG_PRINTLN(writeByteBuffered, BIN);

	_wire->write(byteBuffered);

	byteBuffered = (writeByteBuffered & writeMode) | (initialBuffer & readMode);

//	byteBuffered = (writeByteBuffered & writeMode) & (byteBuffered & readMode);
	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");

	this->transmissionStatus = _wire->endTransmission();

	return this->isLastTransmissionSuccess();

}

// Adafruit_Keypad_Ringbuffer.h
#include <stdint.h>
#include <string.h>

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 256
#endif

template <int N> class Adafruit_Keypad_RingbufferN {
public:
  uint8_t _aucBuffer[N];
  volatile int _iHead;
  volatile int _iTail;

public:
  Adafruit_Keypad_RingbufferN(void);
  void store_char(uint8_t c);
  void clear();
  int read_char();
  int available();
  int availableForStore();
  int peek();
  bool isFull();

private:
  int nextIndex(int index);
};

typedef Adafruit_Keypad_RingbufferN<SERIAL_BUFFER_SIZE>
    Adafruit_Keypad_Ringbuffer;

template <int N>
Adafruit_Keypad_RingbufferN<N>::Adafruit_Keypad_RingbufferN(void) {
  memset(_aucBuffer, 0, N);
  clear();
}

template <int N> void Adafruit_Keypad_RingbufferN<N>::store_char(uint8_t c) {
  int i = nextIndex(_iHead);

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != _iTail) {
    _aucBuffer[_iHead] = c;
    _iHead = i;
  }
}

template <int N> void Adafruit_Keypad_RingbufferN<N>::clear() {
  _iHead = 0;
  _iTail = 0;
}

template <int N> int Adafruit_Keypad_RingbufferN<N>::read_char() {
  if (_iTail == _iHead)
    return -1;

  uint8_t value = _aucBuffer[_iTail];
  _iTail = nextIndex(_iTail);

  return value;
}

template <int N> int Adafruit_Keypad_RingbufferN<N>::available() {
  int delta = _iHead - _iTail;

  if (delta < 0)
    return N + delta;
  else
    return delta;
}

template <int N> int Adafruit_Keypad_RingbufferN<N>::availableForStore() {
  if (_iHead >= _iTail)
    return N - 1 - _iHead + _iTail;
  else
    return _iTail - _iHead - 1;
}

template <int N> int Adafruit_Keypad_RingbufferN<N>::peek() {
  if (_iTail == _iHead)
    return -1;

  return _aucBuffer[_iTail];
}

template <int N> int Adafruit_Keypad_RingbufferN<N>::nextIndex(int index) {
  return (uint32_t)(index + 1) % N;
}

template <int N> bool Adafruit_Keypad_RingbufferN<N>::isFull() {
  return (nextIndex(_iHead) == _iTail);
}

// from Arduino4Leds4ButtonsWithInterrupt
// For arduino uno only pin 1 and 2 are interrupted
#define ARDUINO_UNO_INTERRUPTED_PIN 2
// Function interrupt
void keyPressedOnPCF8574();
// Set i2c address
PCF8574 pcf8574(0x20, ARDUINO_UNO_INTERRUPTED_PIN, keyPressedOnPCF8574);

// Adafruit_Keypad.h
#define makeKeymap(x) ((byte *)x) ///< cast the passed key characters to bytes

#define KEY_JUST_RELEASED (0) ///< key has been released
#define KEY_JUST_PRESSED (1)  ///< key has been pressed

/**************************************************************************/
/*!
    @brief  key event structure
*/
/**************************************************************************/
union keypadEvent {
  struct {
    uint8_t KEY : 8;   ///< the keycode
    uint8_t EVENT : 8; ///< the edge
    uint8_t ROW : 8;   ///< the row number
    uint8_t COL : 8;   ///< the col number
  } bit;               ///< bitfield format
  uint32_t reg;        ///< register format
};

/**************************************************************************/
/*!
    @brief  Class for interfacing GPIO with a diode-multiplexed keypad
*/
/**************************************************************************/
class Adafruit_Keypad {
public:
  Adafruit_Keypad(byte *userKeymap, byte *row, byte *col, int numRows,
                  int numCols);
  ~Adafruit_Keypad();
  void begin();

  void tick();

  bool justPressed(byte key, bool clear = true);
  bool justReleased(byte key);
  bool isPressed(byte key);
  bool isReleased(byte key);
  int available();
  keypadEvent read();
  void clear();

private:
  byte *_userKeymap;
  byte *_row;
  byte *_col;
  volatile byte *_keystates;
  Adafruit_Keypad_Ringbuffer _eventbuf;

  int _numRows;
  int _numCols;

  volatile byte *getKeyState(byte key);
};

// Adafruit_Keypad.cpp
#define _KEY_PRESSED_POS (1)
#define _KEY_PRESSED (1UL << _KEY_PRESSED_POS)

#define _JUST_PRESSED_POS (2)
#define _JUST_PRESSED (1UL << _JUST_PRESSED_POS)

#define _JUST_RELEASED_POS (3)
#define _JUST_RELEASED (1UL << _JUST_RELEASED_POS)

#define _KEYPAD_SETTLING_DELAY 20



/**************************************************************************/
/*!
    @brief  default constructor
    @param  userKeymap a multidimensional array of key characters
    @param  row an array of GPIO pins that are connected to each row of the
   keypad
    @param  col an array of GPIO pins that are connected to each column of the
   keypad
    @param  numRows the number of rows on the keypad
    @param  numCols the number of columns on the keypad
*/
/**************************************************************************/
Adafruit_Keypad::Adafruit_Keypad(byte *userKeymap, byte *row, byte *col,
                                 int numRows, int numCols) {
  _userKeymap = userKeymap;
  _row = row;
  _col = col;
  _numRows = numRows;
  _numCols = numCols;

  _keystates = NULL;
}

/**************************************************************************/
/*!
    @brief  default destructor
*/
/**************************************************************************/
Adafruit_Keypad::~Adafruit_Keypad() {
  if (_keystates != NULL) {
    free((void *)_keystates);
  }
}

/**************************************************************************/
/*!
    @brief  get the state of a key with the given name
    @param  key the name of the key to be checked
*/
/**************************************************************************/
volatile byte *Adafruit_Keypad::getKeyState(byte key) {
  for (int i = 0; i < _numRows * _numCols; i++) {
    if (_userKeymap[i] == key) {
      return _keystates + i;
    }
  }
  return NULL;
}

/**************************************************************************/
/*!
    @brief  read the array of switches and place any events in the buffer.
*/
/**************************************************************************/
void Adafruit_Keypad::tick() {
  uint8_t evt;
  for (int i = 0; i < _numCols; i++) {
      pcf8574.digitalWrite(_col[i], HIGH);
  }

  int i = 0;
  for (int c = 0; c < _numCols; c++) {
    pcf8574.digitalWrite(_col[c], LOW);
    delayMicroseconds(_KEYPAD_SETTLING_DELAY);
    for (int r = 0; r < _numRows; r++) {
      i = r * _numCols + c;
      bool pressed = !pcf8574.digitalRead(_row[r]);
      Serial.print("c: ");
      Serial.println(c);
      Serial.print("value of pressed: ");
      Serial.println(pressed);
      volatile byte *state = _keystates + i;
      byte currentState = *state;
      Serial.print("current state: ");
      Serial.println(currentState);
      // _KEY_PRESSED = 10
      if (pressed && !(currentState & _KEY_PRESSED)) {
        Serial.println("event occured");
        // _JUST_PRESSED = 100
        currentState |= (_JUST_PRESSED | _KEY_PRESSED);
        evt = KEY_JUST_PRESSED;
        _eventbuf.store_char(evt);
        _eventbuf.store_char(*(_userKeymap + i));
        _eventbuf.store_char(r);
        _eventbuf.store_char(c);
      } else if (!pressed && (currentState & _KEY_PRESSED)) {
        Serial.println("event occured");
        currentState |= _JUST_RELEASED;
        currentState &= ~(_KEY_PRESSED);
        evt = KEY_JUST_RELEASED;
        _eventbuf.store_char(evt);
        _eventbuf.store_char(*(_userKeymap + i));
        _eventbuf.store_char(r);
        _eventbuf.store_char(c);
      }
      *state = currentState;
    }
    // Serial.println("");
      pcf8574.digitalWrite(_col[c], HIGH);
  }
}

/**************************************************************************/
/*!
    @brief  set all the pin modes and set up variables.
*/
/**************************************************************************/
void Adafruit_Keypad::begin() {
  _keystates = (volatile byte *)malloc(_numRows * _numCols);
  memset((void *)_keystates, 0, _numRows * _numCols);

  for (int i = 0; i < _numCols; i++) {
    pcf8574.pinMode(_col[i], OUTPUT);
    pcf8574.digitalWrite(_col[i], HIGH);
  }

  for (int i = 0; i < _numRows; i++) {
      pcf8574.pinMode(_row[i], INPUT_PULLUP);
  }
}

/**************************************************************************/
/*!
    @brief  check if the given key has just been pressed since the last tick.
    @param  key the name of the key to be checked
    @param  clear whether to reset the state (default yes) post-check
    @returns    true if it has been pressed, false otherwise.
*/
/**************************************************************************/
bool Adafruit_Keypad::justPressed(byte key, bool clear) {
  volatile byte *state = getKeyState(key);
  bool val = (*state & _JUST_PRESSED) != 0;

  if (clear)
    *state &= ~(_JUST_PRESSED);

  return val;
}

/**************************************************************************/
/*!
    @brief  check if the given key has just been released since the last tick.
    @param  key the name of the key to be checked
    @returns    true if it has been released, false otherwise.
*/
/**************************************************************************/
bool Adafruit_Keypad::justReleased(byte key) {
  volatile byte *state = getKeyState(key);
  bool val = (*state & _JUST_RELEASED) != 0;

  *state &= ~(_JUST_RELEASED);

  return val;
}

/**************************************************************************/
/*!
    @brief  check if the given key is currently pressed
    @param  key the name of the key to be checked
    @returns    true if it is currently pressed, false otherwise.
*/
/**************************************************************************/
bool Adafruit_Keypad::isPressed(byte key) {
  return (*getKeyState(key) & _KEY_PRESSED) != 0;
}

/**************************************************************************/
/*!
    @brief  check if the given key is currently released
    @param  key the name of the key to be checked
    @returns    true if it is currently released, false otherwise.
*/
/**************************************************************************/
bool Adafruit_Keypad::isReleased(byte key) {
  return (*getKeyState(key) & _KEY_PRESSED) == 0;
}

/**************************************************************************/
/*!
    @brief  check how many events are in the keypads buffer
    @returns    the number of events currently in the buffer
*/
/**************************************************************************/
int Adafruit_Keypad::available() {
  return (_eventbuf.available() / sizeof(keypadEvent));
}

/**************************************************************************/
/*!
    @brief  pop the next event off of the FIFO
    @returns    the next event in the FIFO
*/
/**************************************************************************/
keypadEvent Adafruit_Keypad::read() {
  keypadEvent k;
  k.bit.EVENT = _eventbuf.read_char();
  k.bit.KEY = _eventbuf.read_char();
  k.bit.ROW = _eventbuf.read_char();
  k.bit.COL = _eventbuf.read_char();

  return k;
}
// I think _eventbuf.read_char() works by seeing if there is a change in the adjacent values?
/**************************************************************************/
/*!
    @brief Clear out the event buffer and all the key states
*/
/**************************************************************************/
void Adafruit_Keypad::clear() {
  _eventbuf.clear();
  for (int i = 0; i < _numRows * _numCols; i++)
    *(_keystates + i) = 0;
}

/*#define R1    2
#define R2    3
#define R3    4
#define R4    5
#define C1    8
#define C2    9
#define C3    10
#define C4    11*/

const byte ROWS = 4; // rows
const byte COLS = 3; // columns
// define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
    {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte rowPins[ROWS] = {P0, P1, P2,
                      P3};         // connect to the row pinouts of the keypad
byte colPins[COLS] = {P4, P5, P6}; // connect to the column pinouts of the
                                   // keypad

Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  delay(2000);
  Serial.begin(115200);

	/*pcf8574.pinMode(P0, INPUT_PULLUP);
	pcf8574.pinMode(P1, INPUT_PULLUP);
	pcf8574.pinMode(P2, INPUT);
	pcf8574.pinMode(P3, INPUT);

	pcf8574.pinMode(P7, OUTPUT);
	pcf8574.pinMode(P6, OUTPUT, HIGH);
	pcf8574.pinMode(P5, OUTPUT);
	pcf8574.pinMode(P4, OUTPUT, LOW);*/

	Serial.print("Init pcf8574...");
	if (pcf8574.begin(0x20)){
		Serial.println("OK");
	}else{
		Serial.println("KO");
	}

  customKeypad.begin();

}

bool keyPressed = false;
void loop() {
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }
  //delay(1000);
  //Serial.println(customKeypad.available());
}

void keyPressedOnPCF8574(){
	// Interrupt called (No Serial no read no wire in this function, and DEBUG disabled on PCF library)
	 keyPressed = true;
   Serial.println("interrupt_triggered");

}



