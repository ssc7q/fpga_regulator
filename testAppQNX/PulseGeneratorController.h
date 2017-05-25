#ifndef PULSEGENERATORCONTROLLER_H
#define PULSEGENERATORCONTROLLER_H

#include "PortReaderWriter.h"

//#include "factory.h"

#define CHAN_QNTTY			18		// кол-во каналов

#define ALL_PARAMS_LENGTH_4


//#define OFFSET_CHAN_NUMBER		0x00
//#ifdef ALL_PARAMS_LENGTH_4
//	// вариант адресов для 4хбайтных параметров
//	#define OFFSET_PULSE_TIME_LSB	0x01
//	#define LENGTH_PULSE_TIME			4
//	#define OFFSET_PERIOD_LSB			0x05
//	#define LENGTH_PERIOD					4
//	#define OFFSET_QNTTY_LSB			0x09
//	#define LENGTH_QNTTY					4
//	#define OFFSET_CTRL						0x0D
//	#define OFFSET_VERSION				0x0E
//#else
//	// вариант адресов для коротких параметров
//	#define OFFSET_PULSE_TIME_LSB	0x01
//	#define LENGTH_PULSE_TIME			1
//	#define OFFSET_PERIOD_LSB			0x02
//	#define LENGTH_PERIOD					3
//	#define OFFSET_QNTTY_LSB			0x05
//	#define LENGTH_QNTTY					2
//	#define OFFSET_CTRL						0x07
//	#define OFFSET_VERSION				0x08
//#endif

#define OFFSET_CHAN_NUMBER		0x01
// вариант адресов для 3хбайтных параметров
#define OFFSET_PULSE_TIME_LSB	0x02
#define LENGTH_PULSE_TIME			3
#define OFFSET_PERIOD_LSB			0x05
#define LENGTH_PERIOD					3
#define OFFSET_QNTTY_LSB			0x08
#define LENGTH_QNTTY					3
#define OFFSET_CTRL						0x0B
#define OFFSET_VERSION				0x00

// команды управления
#define CMD_START_ONE		0x01
#define CMD_STOP_ONE		0x04
#define CMD_SET_INV			0x05
#define CMD_CLR_INV			0x06
#define CMD_SET_CONT		0x07
#define CMD_CLR_CONT		0x08
#define CMD_START_ALL		0x0A
#define CMD_STOP_ALL		0x0B

// маски для выделения бит статусов
#define STS_MASK_RUNNING	0x01
#define STS_MASK_INV			0x02
#define STS_MASK_CONT			0x04

#define PULSE_UNITS_IN_ONE_SECOND		1000000	// импульсы задаются в микросекундах
#define PERIOD_UNITS_IN_ONE_SECOND		1000	// период задаётся в милисекундах


class PulseGeneratorController
{
	unsigned _baseAdr;
	PortReaderWriter * _porRw; // работа с портами для кросплатформенности

    double _clkFreq;
    double _pulseTimeEmzr;
    double _periodTimeEmzr;
public:
    //void readParams(Initialize* initer); // читает параметры из файла

	void setPortRw(PortReaderWriter * p) { _porRw = p; }

	// парметры платы
	void setBaseAddress(unsigned ba) { _baseAdr = ba; }
	// для ручной настройки емзров, если автоматическая почему-то не нравится
	void setPulseTimeEmzr(double pulseEmzr)	{ _pulseTimeEmzr = pulseEmzr;		}
	void setPeriodEmzr(double periodEmzr)		{ _periodTimeEmzr = periodEmzr;	}


	// низкоуровневые функции ввода-вывода
	unsigned char readByte(unsigned offset);
	void writeByte(unsigned offset, unsigned char val);

	// ****** привязка к параметрам *******

	// установить номер канала
	void writeChanNum(unsigned n, bool uncond = true); //uncond - безуслвная установка номера канала

	// установить длительность импульса
	void writePulseTime(unsigned chan, unsigned rawCode); // задаётся непосредственно код
	void writePulseTime(unsigned chan, double pulseTime); // задаётся время, которое переводится в код
	void writePulseTime(unsigned chan, unsigned byteNum, unsigned char val); // запись отдельных байтов
	unsigned readRawPulseTime(unsigned chan=-1); // возвращает кодовое значение длительности импульса
	double readPulseTime(unsigned chan=-1); // в секундах

	// период
	void writePeriod(unsigned chan, unsigned rawCode);
	void writePeriod(unsigned chan, double period);
	void writePeriod(unsigned chan, unsigned byteNum, unsigned char val);
	unsigned readRawPeriod(unsigned chan=-1);
	double readPeriod(unsigned chan=-1);

	// количесвто импульсов для выдачи
	void writeQuantity(unsigned chan, unsigned qntty);
	void writeQuantity(unsigned chan, unsigned byteNum, unsigned char val);
	unsigned readQuantity(unsigned chan=-1);

	// управление
	void startChan(unsigned chan=-1);
	void stopChan(unsigned chan=-1);
	void startAll();
	void stopAll();
	void writeInversion(unsigned chan, int val);
	void writeContinuity(unsigned chan, int val);
	unsigned char readRawSts(unsigned chan=-1);
	unsigned char readRunning(unsigned chan=-1);
	unsigned char readInversion(unsigned chan=-1);
	unsigned char readContinuity(unsigned chan=-1);

	// версия
	unsigned char readVersion();

	PulseGeneratorController();
};

#endif // PULSEGENERATORCONTROLLER_H
