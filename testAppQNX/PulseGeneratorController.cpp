#include "PulseGeneratorController.h"


//void PulseGeneratorController::readParams(Initialize *initer)
//{	bool b;
//    _baseAdr = QString(initer->stringParameter("baseAddress").c_str()).toInt(&b,16);
//	if (!b) _baseAdr = 0x100;
//	_clkFreq = initer->Parameter("clockFrequency");
//	_pulseTimeEmzr = 1.0/_clkFreq;
//	_periodTimeEmzr = 1.0/_clkFreq;
//}

unsigned char PulseGeneratorController::readByte(unsigned offset)
{
	return _porRw->readPort(_baseAdr + offset);
}

void PulseGeneratorController::writeByte(unsigned offset, unsigned char val)
{
	_porRw->writePort(_baseAdr + offset, val);
}

void PulseGeneratorController::writeChanNum(unsigned n, bool uncond/* = true*/)
{
	if (uncond || n<CHAN_QNTTY)
		writeByte(OFFSET_CHAN_NUMBER, n);
}

//  ***** ÄËÈÒÅËÜÍÎÑÒÜ ÈÌÏÓËÜÑÀ  ***********

// --------  çàïèñü  ------------------

void PulseGeneratorController::writePulseTime(unsigned chan, unsigned rawCode)
{
	for(unsigned byteNum=0; byteNum<LENGTH_PULSE_TIME; byteNum++)
	{
		writePulseTime(chan, byteNum, rawCode);
		rawCode >>= 8;
	}
}

void PulseGeneratorController::writePulseTime(unsigned chan, double pulseTime)
{
	unsigned rawCode = pulseTime / PULSE_UNITS_IN_ONE_SECOND / _pulseTimeEmzr;
	writePulseTime(chan, rawCode);
}

void PulseGeneratorController::writePulseTime(unsigned chan, unsigned byteNum, unsigned char val)
{
	writeChanNum(chan, false);
	writeByte(OFFSET_PULSE_TIME_LSB + byteNum, val);
}

// ----------  ÷òåíèå  -----------------

unsigned PulseGeneratorController::readRawPulseTime(unsigned chan)
{
	unsigned rawCode = 0;

	writeChanNum(chan, false);

	for(int byteNum=LENGTH_PULSE_TIME-1; byteNum>=0; byteNum--)
	{
		rawCode <<= 8;
		rawCode |= readByte(OFFSET_PULSE_TIME_LSB + byteNum);
	}
	return rawCode;
}

double PulseGeneratorController::readPulseTime(unsigned chan)
{
	return readRawPulseTime(chan) * _pulseTimeEmzr * PULSE_UNITS_IN_ONE_SECOND;
}


// ***** ÏÅÐÈÎÄ *******

// --------  çàïèñü  ------------------
void PulseGeneratorController::writePeriod(unsigned chan, unsigned rawCode)
{
	for(unsigned byteNum=0; byteNum<LENGTH_PERIOD; byteNum++)
	{
		writePeriod(chan, byteNum, rawCode);
		rawCode >>= 8;
	}
}

void PulseGeneratorController::writePeriod(unsigned chan, double period)
{
	unsigned rawCode = period / _periodTimeEmzr / PERIOD_UNITS_IN_ONE_SECOND;
	writePeriod(chan, rawCode);
}

void PulseGeneratorController::writePeriod(unsigned chan, unsigned byteNum, unsigned char val)
{
	writeChanNum(chan, false);
	writeByte(OFFSET_PERIOD_LSB + byteNum, val);
}

// --------  ÷òåíèå  -------------------
unsigned PulseGeneratorController::readRawPeriod(unsigned chan)
{
	unsigned rawCode = 0;

	writeChanNum(chan, false);

	for(int byteNum=LENGTH_PERIOD-1; byteNum>=0; byteNum--)
	{
		rawCode <<= 8;
		rawCode |= readByte(OFFSET_PERIOD_LSB + byteNum);
	}
	return rawCode;
}

double PulseGeneratorController::readPeriod(unsigned chan)
{
	return readRawPeriod(chan) * _periodTimeEmzr * PERIOD_UNITS_IN_ONE_SECOND;
}

//*************  ÊÎË-ÂÎ ÈÌÏÓËÜÑÎÂ  ***************
// -----------------  ÷òåíèå  -------------------------

void PulseGeneratorController::writeQuantity(unsigned chan, unsigned qntty)
{
	for(unsigned byteNum=0; byteNum<LENGTH_QNTTY; byteNum++)
	{
		writeQuantity(chan, byteNum, qntty);
		qntty >>= 8;
	}
}

void PulseGeneratorController::writeQuantity(unsigned chan, unsigned byteNum, unsigned char val)
{
	writeChanNum(chan, false);
	writeByte(OFFSET_QNTTY_LSB + byteNum, val);
}

unsigned PulseGeneratorController::readQuantity(unsigned chan)
{
	unsigned qntty = 0;

	writeChanNum(chan, false);

	for(int byteNum=LENGTH_QNTTY-1; byteNum>=0; byteNum--)
	{
		qntty <<= 8;
		qntty |= readByte(OFFSET_QNTTY_LSB + byteNum);
	}
	return qntty;
}

void PulseGeneratorController::startChan(unsigned chan)
{
	writeChanNum(chan, false);
	writeByte(OFFSET_CTRL, CMD_START_ONE);
}

void PulseGeneratorController::stopChan(unsigned chan)
{
	writeChanNum(chan, false);
	writeByte(OFFSET_CTRL, CMD_STOP_ONE);
}

void PulseGeneratorController::startAll()
{
	writeByte(OFFSET_CTRL, CMD_START_ALL);
}

void PulseGeneratorController::stopAll()
{
	writeByte(OFFSET_CTRL, CMD_STOP_ALL);
}

void PulseGeneratorController::writeInversion(unsigned chan, int val)
{
	writeChanNum(chan, false);

	if (val)
		writeByte(OFFSET_CTRL, CMD_SET_INV);
	else
		writeByte(OFFSET_CTRL, CMD_CLR_INV);
}

void PulseGeneratorController::writeContinuity(unsigned chan, int val)
{
	writeChanNum(chan, false);

	if (val)
		writeByte(OFFSET_CTRL, CMD_SET_CONT);
	else
		writeByte(OFFSET_CTRL, CMD_CLR_CONT);
}

unsigned char PulseGeneratorController::readRawSts(unsigned chan)
{
	writeChanNum(chan, false);
	return readByte(OFFSET_CTRL);
}

unsigned char PulseGeneratorController::readRunning(unsigned chan)
{
	return (readRawSts(chan) & STS_MASK_RUNNING) ? 1 : 0;
}

unsigned char PulseGeneratorController::readInversion(unsigned chan)
{
	return (readRawSts(chan) & STS_MASK_INV) ? 1 : 0;
}

unsigned char PulseGeneratorController::readContinuity(unsigned chan)
{
	return (readRawSts(chan) & STS_MASK_CONT) ? 1 : 0;
}

unsigned char PulseGeneratorController::readVersion()
{
	return readByte(OFFSET_VERSION);
}

PulseGeneratorController::PulseGeneratorController()
{
    _clkFreq = 16000000;
    _pulseTimeEmzr = 1.0/_clkFreq;
    _periodTimeEmzr = 1.0/_clkFreq;
    //readParams (Factory::inst()->getInitializer());
}


















