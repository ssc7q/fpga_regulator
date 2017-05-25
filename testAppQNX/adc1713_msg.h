/*
 * adc1713_msg.h
 *
 *  Created on: 26.11.2012
 *      Author: Dmitry
 */

#include <io-adm-msg.h>

#ifndef ADC1713_MSG_H_
#define ADC1713_MSG_H_

/* Types */

    /* Custom message */
    typedef struct pci1713_msg {

        char                    data[256];              /* Any data */

    } __attribute__ ((__packed__)) pci1713_msg_t;

    /* DCMD_IOADM_SET_CHANNEL message */

     typedef struct pci1713_msg_channel_settings{
     			 int                         num;              /* Channel number */
     			 int                         b_u;               /* Channel mode - bipolar (0) or unipolar (1) */
     			 int                         gain;               /* Gain according to gaincodes table: */
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
     			 int 						s_d;		// 0 - single-ended, 1 - differential

     }  __attribute__ ((__packed__)) pci1713_msg_channel_settings_t;

     /* DCMD_IOADM_GET_SAMPLE message */
		  typedef struct pci1713_msg_get_sample {
			 int start_ch;
			 int channels;
			 int nSamples;
		  } __attribute__ ((__packed__)) pci1713_msg_get_sample_t;

//         typedef struct pci1713_msg_get_sample {
//        	 pci1713_msg_channel_settings_t ch_sett;
//        	 uint16_t sample;
//         } __attribute__ ((__packed__)) pci1713_msg_get_sample_t;


     /* DCMD_IOADM_SET_IN_BUFF message */
     typedef struct pci1713_buff_info {
    	 //int 						 samplesPerFragment; // samples per fragment of data=number of samples acquired, after which driver generates event
         int                         frag_sz;            /* Fragmentation size */
         int                         buf_sz;             /* Buffer size */
     } __attribute__ ((__packed__)) pci1713_buff_info_t;

     /* DCMD_IOADM_SET_TIMING message */
     typedef struct pci1713_msg_timing_params {
         int                         channel_rate;       /* Channel rate */
         //int                         frame_rate;         /* Frame rate */
     } __attribute__ ((__packed__)) pci1713_msg_timing_params_t;

     /* DCMD_IOADM_CHANNELS message */
     typedef struct pci1713_msg_active_channels {
    	    int                         start_ch;           /* start number of active ADC channels */
    	    int                         channels;           /* Number of active ADC channels */
     } __attribute__ ((__packed__)) pci1713_msg_active_channels_t;


#endif /* ADC1713_MSG_H_ */
