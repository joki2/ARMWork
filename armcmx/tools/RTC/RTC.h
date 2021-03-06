/*
 DS1307.h - library for DS1307/DS3231 i2c rtc
 */

// ensure this library description is only included once
#ifndef RTC_h
#define RTC_h

#if defined (ARMCMX)
#include "armcmx.h"
#include "binary.h"
#include "I2CBus.h"

#elif defined (ARDUINO)
//#include <avr/pgmspace.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <Wire.h>

#endif

// library interface description
class RTC {
	// library-accessible "private" interface
private:
	I2CBus & wire;
	uint8 chipID;
	
	void readRegisters(byte reg, uint8_t *, byte);
	void writeRegisters(byte reg, uint8_t *, byte);

	enum {
		M41T62_HUNDREDTHSEC = 0,
		DS1307_SEC = 0,
		DS1307_MIN,
		DS1307_HR,
		DS1307_DOW,
		DS1307_DATE,
		DS1307_MTH,
		DS1307_YR
	};

	static const uint8 DS1307_CTRL_ID = B01101000; //DS1307, M41T62

	// Define register bit masks
	static const uint8 DS1307_CLOCKHALT = B10000000;

	static const uint8 BITS_SEC = B01111111;
	static const uint8 BITS_HR =  B00111111;
	static const uint8 BITS_MIN = B01111111;
	static const uint8 BITS_DOW = B00000111;
	static const uint8 BITS_DATE =B00111111;
	static const uint8 BITS_MTH = B00111111;
	static const uint8 BITS_YR =  B11111111;

	static const uint16 BASE_YEAR = 2000;

//		uint8_t bcdToDec(uint8_t b) { return (b>>4)*10 + (b&0x0f); }
//		uint8_t decToBcd(uint8_t d) { return ((d/10)<<4) + (d%10); }

	// user-accessible "public" interface

public:

#ifdef ARDUINO
  PROGMEM const static char NameOfDay[36];
  PROGMEM const static char NameOfMonth[60];
#else
  static const char NameOfDay[36];
  static const char NameOfMonth[60];
#endif

	enum DAYINDEX {
		SUN = 0, MON, TUE, WED, THU, FRI, SAT,
	};

	enum {
		MAXIM_DS1307 = 0,
    MAXIM_DS3234 = 0,
		ST_M41T62 = 1
	};

public:
	// cache
	uint32 time, date;

	RTC(I2CBus & w, uint8 chip = MAXIM_DS1307) : wire(w), chipID(chip), time(0), date(0) { }

	RTC(uint8 chip = MAXIM_DS1307) : wire(Wire), chipID(chip), time(0), date(0) {	}

	void init() {
		start();
	}
	bool begin() {
		start();
    return true;
	}
//		static int base_year() { return DS1307_BASE_YR; }

  // returns true if the value is changed from time/cal.
	boolean updateTime();
	boolean updateCalendar();
  inline boolean update() { return updateTime() && updateCalendar(); }
  
	uint8 getSeconds();
//		byte* getTimestamp(byte* );
	void setTime(const long &);
	void setCalendar(const long &);
//		long time();
//		long calendar();
//		void tweakMinutes(int);
	void writeRegister(byte, byte);
	byte readRegister(byte);

	void start(void);
	void stop(void);

  // utility functions
	byte dayOfWeek();
  
  inline long JD2000(void) {
    return this->JD2000(date);
  }
	static long JD2000(byte y, byte m, byte d);
	static long JD2000(const long & yymmdd);
  static float CalendarDate(float jd);


	static char * copyNameOfDay(char * buf, byte d) {
		strcpy(buf, NameOfDay + d * 4);
		return buf;
	}

	static char * copyNameOfMonth(char * buf, byte m) {
		strcpy(buf, NameOfMonth + ((m+11) % 12) * 4);
		return buf;
	}

};

#endif

