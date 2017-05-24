#include "PCI1713.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <hw/inout.h>
#include <sys/trace.h>

PCI1713::PCI1713(void)
{
	for(int i=0; i<32; i++)
	{
		setAB (i, -10.0, 10.0);
		settings[i].num = i;
		settings[i].b_u = 0;
		settings[i].gain = 4;
		settings[i].s_d = 0;
	}
	//ThreadCtl(_NTO_TCTL_IO, 0);
}

PCI1713::~PCI1713(void)
{
}



void PCI1713::setAB(int i, float min, float max )
{
	_a[i] = (max - min) / 4096;
	_b[i] = min;
}

int PCI1713::open(unsigned pci_index )
{
	// подключитьс€ к плате
	ioadm_open_pci( &board, "pci1713_g", pci_index );
	ioadm_lib_prepare_io( &board );
	// после открыти€ установить настройки
	for(int i=0; i<32; i++)
	{
		setChannelSettings(i);
	}
	return 0;
}

int PCI1713::open1715(unsigned pci_index)
{
	// подключитьс€ к плате
	ioadm_open_pci( &board, "pci1715_g", pci_index );
	ioadm_lib_prepare_io( &board );
	// после открыти€ установить настройки
	for(int i=0; i<32; i++)
	{
		setChannelSettings(i);
	}
	return 0;
}

double PCI1713::read( int chanNum, int avgQ /*= 1*/ )
{
	float res;
	read( chanNum, 1, &res, avgQ );
	return res;
}

void PCI1713::read( int chanStart, int chanQntty, float * result, int avgQ /*= 1*/)
{
	float resultTmp[32];
	// обнул€ю буфер
	for (int i=0; i<chanQntty; i++)
	{
		result[i] = 0;
		resultTmp[i] = 0;
	}
	// читаю с накоплением
	for (int avgN = 0; avgN < avgQ; avgN++)
	{
		pci1713_msg_get_sample_t * tmp;
		uint16_t res[32];
		int ret;

		if (chanQntty >= 32) return;

		tmp = (pci1713_msg_get_sample_t *)res;

		tmp->start_ch = chanStart;
		tmp->channels = chanQntty;
		tmp->nSamples = chanQntty;
	
		devctl( board.fd, DCMD_IOADM_GET_SAMPLE, tmp, sizeof( res ), &ret );

		for (int i=0; i<chanQntty; i++)
		{
			resultTmp[i] += toFloat(i, res[i]);
		}
	}

	// усредн€ю
	for (int i=0; i<chanQntty; i++)
	{
		result[i] = resultTmp[i] / avgQ;
	}

	//return 0;
}

int PCI1713::setChannelsForFai( int startChan, int chanQntty )
{
	pci1713_msg_active_channels_t tmp;
	int ret;
	tmp.start_ch = startChan;
	tmp.channels = chanQntty;
	devctl( board.fd, DCMD_IOADM_CHANNELS, &tmp, sizeof( tmp ), &ret );
	return 0;
}

int PCI1713::setRange( int chanNum, Ranges range )
{ /* Gain according to gaincodes table: */
	// B/U N/A G2 G1 G0  Voltage range PCI-1713,
	//  0      0  0  0   -5V     to 5V
	//  0      0  0  1   -2.5V   to 2.5V
	//  0      0  1  0   -1.25V  to 1.25V
	//  0      0  1  1   -0.625V to 0.625V
	//  0      1  0  0   -10V    to 10V
	//  1      0  0  0    0V     to 10V
	//  1      0  0  1    0V     to 5V
	//  1      0  1  0    0V     to 2.5V
	//  1      0  1  1    0V     to 1.25V
	switch (range)
	{
	case pm5:
		settings[chanNum].b_u = 0;
		settings[chanNum].gain = 0;
		setAB(chanNum, -5.0, 5.0);
		break;
	case pm2_5:
		settings[chanNum].b_u = 0;
		settings[chanNum].gain = 1;
		setAB(chanNum, -2.5, 2.5);
		break;
	case pm1_25:
		settings[chanNum].b_u = 0;
		settings[chanNum].gain = 2;
		setAB(chanNum, -1.25, 1.25);
		break;
	case pm0_625:
		settings[chanNum].b_u = 0;
		settings[chanNum].gain = 3;
		setAB(chanNum, -0.625, 0.625);
		break;
	case pm10:
		settings[chanNum].b_u = 0;
		settings[chanNum].gain = 4;
		setAB(chanNum, -10.0, 10.0);
		break;
	case p10:
		settings[chanNum].b_u = 1;
		settings[chanNum].gain = 0;
		setAB(chanNum, 0.0, 10.0);
		break;
	case p5:
		settings[chanNum].b_u = 1;
		settings[chanNum].gain = 1;
		setAB(chanNum, 0.0, 5.0);
		break;
	case p2_5:
		settings[chanNum].b_u = 1;
		settings[chanNum].gain = 2;
		setAB(chanNum, 0.0, 2.5);
		break;
	case p1_25:
		settings[chanNum].b_u = 1;
		settings[chanNum].gain = 3;
		setAB(chanNum, 0.0, 1.25);
		break;
	}
	setChannelSettings(chanNum);
	return 0;
}

int PCI1713::setConnection( int chanNum, int s_d )
{
	settings[chanNum].s_d = s_d;
	setChannelSettings(chanNum);
	return 0;
}



int PCI1713::setChannelSettings( int chanNum )
{
	int ret;
	devctl( board.fd, DCMD_IOADM_SET_CHANNEL, settings + chanNum, sizeof( settings[chanNum] ), &ret );
	return 0;
}


int PCI1713::setTimingForFai( int rate )
{
	pci1713_msg_timing_params_t tmp;
	int ret;
	tmp.channel_rate = rate;
	devctl( board.fd, DCMD_IOADM_SET_TIMING, &tmp, sizeof( tmp ), &ret );
	return 0;
}


int PCI1713::startFai( int mode )
{
	int ret;
	// перед запуском переподготовитьс€
	ioadm_lib_prepare_io( &board );
	devctl( board.fd, DCMD_IOADM_START, &mode, sizeof( mode ), &ret );
	return 0;
}

int PCI1713::stopFai()
{
	int tmp;
	int ret;
	devctl( board.fd, DCMD_IOADM_STOP, &tmp, sizeof( tmp ), &ret );
	return 0;
}

int PCI1713::close()
{
	ioadm_close(&board);
	return 0;
}

int PCI1713::setBufSize( int bufSz, int fragSz )
{
	pci1713_buff_info_t tmp;
	int ret;
	tmp.frag_sz = fragSz * sizeof(uint16_t);
	tmp.buf_sz = bufSz * sizeof(uint16_t);
	devctl( board.fd, DCMD_IOADM_SET_IN_BUFF, &tmp, sizeof( tmp ), &ret );

	return 0;
}

int PCI1713::readFragment(void** buf)
{
	// блокирующа€
	ioadm_read_fragment_wc( &board, buf);
	return 0;
}


bool PCI1713::isOverrun()
{
	uint32_t state = getState();
	if (state&IO_ADM_DRV_STATE_DEV_DATA_OVERRUN)
		return true;
	else 
		return false;
}


uint32_t PCI1713::getState()
{
	return ioadm_get_state(&board);
}

int PCI1713::collectData( int chan, int sampleFreq, int sampleNum, double * data )
{
	void* buf;
	setBufSize (sampleNum,sampleNum);
	setChannelsForFai(chan, 1);
	setTimingForFai(sampleFreq);
	// перенес в старт‘аи ioadm_lib_prepare_io( &board );
	startFai(0);
	readFragment(&buf);
	for (int i=0; i<sampleNum; i++)
	{
		data[i] = toFloat(chan, ((uint16_t* )buf)[i]);
	}
	stopFai();
 return 0;
}
