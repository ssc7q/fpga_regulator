#ifndef PCI1713_H
#define PCI1713_H

#include <ioadm.h>
#include <adc1713_msg.h>


class PCI1713
{
	ioadm_device_t              board;
	pci1713_msg_channel_settings_t settings[32];

	float _a[32];
	float _b[32];


	void setAB(int i, float min, float max);
	int setChannelSettings (int chanNum);

public:
	float toFloat (int i, uint16_t x){return _a[i] * x + _b[i];}
	int setBufSize(int bufSz, int fragSz); // размеры в самплах
	enum Ranges {pm5=0, pm2_5, pm1_25, pm0_625, pm10, p10, p5, p2_5, p1_25 };
	int open(unsigned id);
	int open1715(unsigned id);
	int close();

	double read (int chanNum, int avgQ = 1);
	void read (int chanStart, int chanQntty, float * result, int avgQ = 1); // avgQ - колво выборок для усредения

	int startFai (int mode); 
	int stopFai ();
	int readFragment(void** buf);
	bool isOverrun();
	uint32_t getState ();

	int setChannelsForFai(int startChan, int chanQntty);
	int setTimingForFai (int rate);
	int setRange(int chanNum, Ranges range);
	int setConnection(int chanNum, int s_d);

	int collectData(int chan, int sampleFreq, int sampleNum, double * data);

	PCI1713(void);
	~PCI1713(void);
};
#endif //PCI1713_H

