#ifndef PULSEGENERATORCONTROLLER_H
#define PULSEGENERATORCONTROLLER_H

#include "PortReaderWriter.h"

//#include "factory.h"

#define CHAN_QNTTY			18		// ���-�� �������

#define ALL_PARAMS_LENGTH_4


//#define OFFSET_CHAN_NUMBER		0x00
//#ifdef ALL_PARAMS_LENGTH_4
//	// ������� ������� ��� 4�������� ����������
//	#define OFFSET_PULSE_TIME_LSB	0x01
//	#define LENGTH_PULSE_TIME			4
//	#define OFFSET_PERIOD_LSB			0x05
//	#define LENGTH_PERIOD					4
//	#define OFFSET_QNTTY_LSB			0x09
//	#define LENGTH_QNTTY					4
//	#define OFFSET_CTRL						0x0D
//	#define OFFSET_VERSION				0x0E
//#else
//	// ������� ������� ��� �������� ����������
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
// ������� ������� ��� 3�������� ����������
#define OFFSET_PULSE_TIME_LSB	0x02
#define LENGTH_PULSE_TIME			3
#define OFFSET_PERIOD_LSB			0x05
#define LENGTH_PERIOD					3
#define OFFSET_QNTTY_LSB			0x08
#define LENGTH_QNTTY					3
#define OFFSET_CTRL						0x0B
#define OFFSET_VERSION				0x00

// ������� ����������
#define CMD_START_ONE		0x01
#define CMD_STOP_ONE		0x04
#define CMD_SET_INV			0x05
#define CMD_CLR_INV			0x06
#define CMD_SET_CONT		0x07
#define CMD_CLR_CONT		0x08
#define CMD_START_ALL		0x0A
#define CMD_STOP_ALL		0x0B

// ����� ��� ��������� ��� ��������
#define STS_MASK_RUNNING	0x01
#define STS_MASK_INV			0x02
#define STS_MASK_CONT			0x04

#define PULSE_UNITS_IN_ONE_SECOND		1000000	// �������� �������� � �������������
#define PERIOD_UNITS_IN_ONE_SECOND		1000	// ������ ������� � ������������


class PulseGeneratorController
{
	unsigned _baseAdr;
	PortReaderWriter * _porRw; // ������ � ������� ��� �������������������

    double _clkFreq;
    double _pulseTimeEmzr;
    double _periodTimeEmzr;
public:
    //void readParams(Initialize* initer); // ������ ��������� �� �����

	void setPortRw(PortReaderWriter * p) { _porRw = p; }

	// �������� �����
	void setBaseAddress(unsigned ba) { _baseAdr = ba; }
	// ��� ������ ��������� ������, ���� �������������� ������-�� �� ��������
	void setPulseTimeEmzr(double pulseEmzr)	{ _pulseTimeEmzr = pulseEmzr;		}
	void setPeriodEmzr(double periodEmzr)		{ _periodTimeEmzr = periodEmzr;	}


	// �������������� ������� �����-������
	unsigned char readByte(unsigned offset);
	void writeByte(unsigned offset, unsigned char val);

	// ****** �������� � ���������� *******

	// ���������� ����� ������
	void writeChanNum(unsigned n, bool uncond = true); //uncond - ���������� ��������� ������ ������

	// ���������� ������������ ��������
	void writePulseTime(unsigned chan, unsigned rawCode); // ������� ��������������� ���
	void writePulseTime(unsigned chan, double pulseTime); // ������� �����, ������� ����������� � ���
	void writePulseTime(unsigned chan, unsigned byteNum, unsigned char val); // ������ ��������� ������
	unsigned readRawPulseTime(unsigned chan=-1); // ���������� ������� �������� ������������ ��������
	double readPulseTime(unsigned chan=-1); // � ��������

	// ������
	void writePeriod(unsigned chan, unsigned rawCode);
	void writePeriod(unsigned chan, double period);
	void writePeriod(unsigned chan, unsigned byteNum, unsigned char val);
	unsigned readRawPeriod(unsigned chan=-1);
	double readPeriod(unsigned chan=-1);

	// ���������� ��������� ��� ������
	void writeQuantity(unsigned chan, unsigned qntty);
	void writeQuantity(unsigned chan, unsigned byteNum, unsigned char val);
	unsigned readQuantity(unsigned chan=-1);

	// ����������
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

	// ������
	unsigned char readVersion();

	PulseGeneratorController();
};

#endif // PULSEGENERATORCONTROLLER_H
