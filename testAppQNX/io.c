/*
 * (c) CBD BC, Russia, Spb.
 *
 * Mail: support@kpda.ru
 *
 * Date: 23/11/2009
 * Dev:  A. Docuchaev
 */


/* io.c */


//#include "l783m.h"
#include "pci1721.h"
#include <sys/trace.h>


/* Defines */


#define DEVCTL_RET( cond, devctl_status )       \
    if ( cond ) {                               \
        msg->o.ret_val = (devctl_status);       \
        msg->o.nbytes  = 0;       				\
        break;                                  \
    }


/* Funcs */

/*
 * _IO_READ handler
 *
 * In: Device ptr
 *     Message ptr (Message size >= 4)
 *     Size to read
 * Out: n - count of readed bytes
 *      0 - error
 */

//int io_adm_drv_read( void *device, void *msg, uint32_t size )
//{
//	pci1713_t                 *pci1713   = (pci1713_t *)device;
//	pci1713_msg_t             *msg2    = (pci1713_msg_t *)msg;
//	int                     read_sz  = 0;
//	int i=0;
//	//int availSz=0;
//    /* Buffer size not enough to store shared-memory offset */
//	if ( size < 4 )
//		return (0);
//
//	/* ADC Disabled */
//	if ( !pci1713->adc_start )
//		return (0);
//
//	read_sz = size;     /* Size client ready to read */
//	if ( read_sz > 0 ) {
//
//		pthread_mutex_lock(&pci1713->adcDataReadyMutex);
//		 /* No available data - wait for it */
//		//while ( (availSz = rbuf_avail_sz(&pci1713->i_rbuf) ) < read_sz )
//		while ( (pci1713->i_rbuf.iFragment)  < read_sz )
//		{
//				pthread_cond_wait(&pci1713->adcDataReadyCondVar, &pci1713->adcDataReadyMutex);
//				i++;
//		}
//		/* Send offset */
//		*((uint32_t *)(&msg2->data)) = pci1713->i_rbuf.read_idx;
//
//		/* Move read ptr */
//		pci1713->i_rbuf.read_idx = (pci1713->i_rbuf.read_idx + read_sz) % pci1713->i_rbuf.sz;
//
//		pci1713->i_rbuf.iFragment -= read_sz;
//
//		pthread_mutex_unlock(&pci1713->adcDataReadyMutex);
//
//	}
//	return (read_sz);
//}

//
int io_adm_drv_read( void *device, void *msg, uint32_t size )
{
	return (-1);
//    l783m_t                 *l783m   = (l783m_t *)device;
//    l783m_msg_t             *msg2    = (l783m_msg_t *)msg;
//    int                     read_sz  = 0;
//
//    /* DSP BIOS is damaged */
//    if ( !l783m->bios_loaded )
//        return (0);
//
//    /* Buffer size not enough to store shared-memory offset */
//    if ( size < 4 )
//        return (0);
//
//    /* ADC Disabled */
//    if ( !l783m->adc_start )
//        return (0);
//
//    if ( l783m->ir_fixed ) {
//
//        InterruptWait( 0, NULL );
//
//        read_sz = size;     /* Size client ready to read */
//        l783m->i_rbuf.avail_sz = l783m->i_rbuf.fragmentation_sz;  /* Buffer has */
//        if ( read_sz > l783m->i_rbuf.avail_sz )
//            read_sz = l783m->i_rbuf.avail_sz;
//        if ( read_sz > l783m->i_rbuf.sz )
//            read_sz = l783m->i_rbuf.sz;
//        if ( read_sz > 0 ) {
//            /* Send offset */
//            *((uint32_t *)(&msg2->data)) = 0;
//
//            l783m->i_rbuf.read_idx = 0;
//        }
//
//    } else {
//
//        /* No available data - wait for it */
//        while ( l783m->i_rbuf.write_idx == l783m->i_rbuf.read_idx )
//            InterruptWait( 0, NULL );
//
//        read_sz = size;     /* Size client ready to read */
//        l783m->i_rbuf.avail_sz = rbuf_avail_sz( &l783m->i_rbuf ); /* Buffer has */
//        if ( read_sz > l783m->i_rbuf.avail_sz )
//            read_sz = l783m->i_rbuf.avail_sz;
//        if ( l783m->i_rbuf.read_idx + read_sz > l783m->i_rbuf.sz )
//            read_sz = l783m->i_rbuf.sz - l783m->i_rbuf.read_idx;
//        if ( read_sz > 0 ) {
//            /* Send offset */
//            *((uint32_t *)(&msg2->data)) = l783m->i_rbuf.read_idx;
//
//            /* Move read ptr */
//            l783m->i_rbuf.read_idx = (l783m->i_rbuf.read_idx + read_sz) % l783m->i_rbuf.sz;
//        }
//
//    }
//
//    l783m->descriptor.data_readed += read_sz;
//    return (read_sz);
}

/*
 * _IO_WRITE handler
 *
 * In: Device ptr
 *     Message ptr
 *     Message size
 * Out: -1 - error
 */
int io_adm_drv_write( void *device, void *msg, uint32_t size )
{
    /*l783m_t                 *l783m = (l783m_t *)device;*/

    /* Not permitted */

    return (-1);
}

/*
 * _IO_DEVCTL handler
 *
 * In: Device ptr
 *     io_devctl_t message ptr
 *     Buffer ptr
 * Out: 0 - ok
 */


int io_adm_drv_devctl( void *device, io_devctl_t *msg, void *_data )
{
	pci1721_t	*pci1721 = (pci1721_t *)device;
	//unsigned int ttt = DCMD_IOADM_GET_SHMOBJECT;
	// msg->o.nbytes  = 0; !!!!!! We cannot zero this because devctl i and o messages share the same memory area !!!!!!!!!


// *******************************ADC PARAMETERS SETTING***********************



	   switch ( msg->i.dcmd ) {
	   	   case DCMD_IOADM_SET_CHANNEL_SETTINGS:// пол€рность, диапазон,
	   	   {
	   		   pci1721_msg_channel_settings_t *data = (pci1721_msg_channel_settings_t *)_data;
	   		   DEVCTL_RET( data->number >= DA_MAX_CHANNELS, 	DEVCTL_WRONG_CHAN_NUM );
	   		   setChannelSettins(pci1721, data);
	   		   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = 0;
	   		   break;
	   	   }
	   	   case DCMD_IOADM_SET_BUFF: //выделение шмема под буфер
	   	   {
	   		   pci1721_buff_settings_t *data = (pci1721_buff_settings_t *)_data;
	   		   DEVCTL_RET( data->buf_size < DA_FIFO_SIZE, 	DEVCTL_WRONG_FIFO_SIZE );

	   		   pthread_mutex_lock(&pci1721->bufMutex);
	   		   free_rbuf( &pci1721->buffer );
			   init_buffer( pci1721, &pci1721->buffer, data->buf_size * BYTES_IN_SAMPLE);
			   pthread_mutex_unlock(&pci1721->bufMutex);

			   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = 0;
	   		   break;
	   	   }
	   	   case DCMD_IOADM_GET_BUFF_INFO: // размер буфера, свободного места. индексы чтени€ и записи
	   	   {
	   		   int n;
	   		   pci1721_msg_buff_info_t *data = (pci1721_msg_buff_info_t *)_data;
	   		   data->bufSize =  pci1721->buffer.size / BYTES_IN_SAMPLE;
	   		   data->indexRd =  pci1721->buffer.indexRd;
	   		   data->indexWr =  pci1721->buffer.indexWr;
	   		   n =  pci1721->buffer.indexRd - pci1721->buffer.indexWr;
	   		   if (n <= 0)// ==0 - полностью весь буфер свободен
	   			   n += pci1721->buffer.size / BYTES_IN_SAMPLE;
	   		   n -= 2; // вообще то нужно -1 чтобы не заполн€лс€ ѕќЋЌќ—“№ё весь буфер никогда (а то условие Wr = Rd не будет правильно работать), но чтобы по возможности было чЄтное кол-во выбора делаю -2
	   		   if (n<0) n = 0; // защита, котора€ никогда не должна сработать, но мало ли...
	   		   data->nSamples = n;
	   		   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = sizeof(pci1721_msg_buff_info_t);
	   		   break;
	   	   }
	   	   case DCMD_IOADM_SET_TIMING:// настройка клока
	   	   {
	   		   pci1721_msg_timing_params_t *data = (pci1721_msg_timing_params_t *)_data;
	   		   DEVCTL_RET( data->channel_rate > DA_MAX_FREQ, 	DEVCTL_WRONG_SAMPLE_RATE );

	   		   SetPacer(pci1721, data->channel_rate, DA_MAX_FREQ);

	   		   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = 0;
	   		   break;
	   	   }
	   	   case DCMD_IOADM_SET_LAST_INDEX:
	   	   {
	   		   pci1721_msg_set_last_index_t *data = (pci1721_msg_set_last_index_t *)_data;
	   		   pci1721->buffer.indexWr = data->index;
	   		   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = 0;
	   		   break;
	   	   }

	   	   case DCMD_IOADM_GET_BUFF_MODE:
	   	   {
			   msg->o.ret_val = LIBMODE_SHARED_MEMORY | LIBMODE_BUFPERM_W;
			   msg->o.nbytes  = 0;
			   break;
		   }

	   	   case DCMD_IOADM_GET_SHMOBJECT:
	   	   {

			   char *name = (char *)_data;
			   strcpy( name, pci1721->buffer.shm_name );
			   msg->o.ret_val = DEVCTL_NO_ERROR;
			   msg->o.nbytes  = 40;
			   break;
		   }

		   case DCMD_IOADM_GET_SHMOBJECT_SZ:
		   {
			   msg->o.ret_val = pci1721->buffer.size;
			   break;
		   }

		   case DCMD_IOADM_START:
		   {
			   pci1721_msg_start_t *data = (pci1721_msg_start_t *)_data;
			   faoStart(pci1721, data);
			   msg->o.ret_val = DEVCTL_NO_ERROR;
			   msg->o.nbytes  = 0;
			   break;
		   }
	   	   case DCMD_IOADM_STOP:
		   {
			   faoStop(pci1721);
			   msg->o.ret_val = DEVCTL_NO_ERROR;
			   msg->o.nbytes  = 0;
			   break;
		   }
		   case DCMD_IOADM_SET_SAMPLE:
	   	   {
	   		   pci1721_msg_set_sample_t *data = (pci1721_msg_set_sample_t *)_data;
	   		   DEVCTL_RET( data->number >= DA_MAX_CHANNELS, 	DEVCTL_WRONG_CHAN_NUM );
	   		   DEVCTL_RET( data->sample >= DA_MAX_SAMPLE, 	DEVCTL_WRONG_SAMPLE_VALUE );
	   		   setRawVoltage(pci1721, data->number, data->sample);
	   		   msg->o.ret_val = DEVCTL_NO_ERROR;
	   		   msg->o.nbytes  = 0;
	   		   break;
	   	   }





//// start and stop channels setup for FAI
//		   case DCMD_IOADM_CHANNELS: {
//			   pci1713_msg_active_channels_t *bi_msg = (pci1713_msg_active_channels_t *)data;
//
//			   DEVCTL_RET( bi_msg->start_ch+bi_msg->channels  > ADC_MAX_CHANNELS, 	1 )
//			   DEVCTL_RET( bi_msg->start_ch+bi_msg->channels <= 0, 					2 )
//			   DEVCTL_RET( bi_msg->start_ch  >= ADC_MAX_CHANNELS, 					3 )
//			   DEVCTL_RET( bi_msg->start_ch  < 0, 									4 )
//
//			   pci1713->channels = bi_msg->channels;
//			   pci1713->start_ch = bi_msg->start_ch;
//
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
//// channels params setup (bipolar/uniploar, diff/singleended, gaincodes)
//		   case DCMD_IOADM_SET_CHANNEL: {
//			   int arrSz;
//			   int i;
//			   arrSz = msg->i.nbytes/sizeof(pci1713_msg_channel_settings_t);
//			   for (i=0; i<arrSz; ++i)
//			   {
//				   uint8_t x;
//				   pci1713_msg_channel_settings_t *ch_msg = ( (pci1713_msg_channel_settings_t *)data ) + i;
//				   DEVCTL_RET( ch_msg->num >= ADC_MAX_CHANNELS, 1 )
//				   DEVCTL_RET( ch_msg->num < 0,                 1
//							   )
//				   out8(pci1713->board_info.base+4, ch_msg->num); // MUX start channel
//				   out8(pci1713->board_info.base+5, ch_msg->num); // MUX stop channel
//				   x = ( (ch_msg->gain)&7 ) |
//					   ( ( (ch_msg->b_u)&1 ) << 4 ) |
//					   ( ( (ch_msg->s_d)&1 ) << 5 );
//				   out8(pci1713->board_info.base+2, x);
//
//			   }
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
//// set timing params (channel-to-channel sampling rate) for FAI
//		   case DCMD_IOADM_SET_TIMING: {
//			   pci1713_msg_timing_params_t   *tp    = (pci1713_msg_timing_params_t *)data;
//
//			   SetPacer(pci1713->board_info.base, tp->channel_rate, ADC_SRC_FREQ);
//			   pci1713->rate = tp->channel_rate;
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
//
//
//
//// ******************************* ADC FAI AND BUFFERING *******************************
//
//
//
//// start FAI
//		   case DCMD_IOADM_START: {
//
//			   DEVCTL_RET( pci1713->adc_start == 1, 1 )      /* Already started */
//
//			   pci1713->adc_start = 1;
//			   pci1713->cyclicMode = *((int*)data);
//
//			   pci1713->i_rbuf.write_idx = pci1713->i_rbuf.read_idx = 0;
//			   pci1713->i_rbuf.iFragment = 0;
//
//			   // active channels setup
//			   out8(pci1713->board_info.base+4, pci1713->start_ch); // MUX start channel
//			   out8(pci1713->board_info.base+5, pci1713->start_ch + pci1713->channels-1); // MUX stop channel
//
//			   // clear FIFO
//			   out8(pci1713->board_info.base + 9, 0);
//			   // turn adc FAI on with interrupt
//			   const uint8_t ucTrigSrc = 2;       //Pacer trigger
//			   const uint8_t ucIrqEn = 0x10;      //Enable interrupt;
//			   const uint8_t ucFh = 0x20;         //Half full interrupt
//			   out8(pci1713->board_info.base + 6, ucTrigSrc | ucIrqEn | ucFh);
//
//			   pci1713->descriptor.device_state |= IO_ADM_DRV_STATE_DEV_STARTED;
//			   pci1713->descriptor.device_state &= ~IO_ADM_DRV_STATE_DEV_DATA_OVERRUN;
//
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
////stop FAI
//		   case DCMD_IOADM_STOP: {
//			   adc1713Stop(pci1713);
//			   msg->o.ret_val = 0;
//			   msg->o.nbytes  = 0;
//			   break;
//		   }
//// change shmem buf size and fragment size
//		   case DCMD_IOADM_SET_IN_BUFF: {
//			   pci1713_buff_info_t   *bi_msg = (pci1713_buff_info_t *)data;
//
//			   DEVCTL_RET( bi_msg->buf_sz < 0,   1 )
//			   DEVCTL_RET( bi_msg->frag_sz <= 0, 1 )
//
//			   free_rbuf( &pci1713->i_rbuf );
//
//			   if ( bi_msg->buf_sz == 0 )      /* Set to default */
//				   bi_msg->buf_sz = RBUF_SZ - (RBUF_SZ % bi_msg->frag_sz);
//
//			   init_rbuf( pci1713, &pci1713->i_rbuf, bi_msg->buf_sz, bi_msg->frag_sz );
//
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
//		   case DCMD_IOADM_GET_BUFF_MODE: {
//
//			   msg->o.nbytes  = 0;
//			   msg->o.ret_val = LIBMODE_SHARED_MEMORY | LIBMODE_BUFPERM_R;
//			   break;
//		   }
//
//		   case DCMD_IOADM_GET_SHMOBJECT: {
//			   char            *name = (char *)data;
//
//			   strcpy( name, pci1713->i_rbuf.shm_name );
//
//			   msg->o.nbytes  = 40;
//			   msg->o.ret_val = 0;
//			   break;
//		   }
//
//		   case DCMD_IOADM_GET_SHMOBJECT_SZ: {
//			   msg->o.ret_val = pci1713->i_rbuf.sz;
//			   break;
//		   }
//
//		   case DCMD_IOADM_GET_SHMOBJECT_RD: {
//			   int                     sz = 0;
//
//			   sz = io_adm_drv_read( pci1713, data, pci1713->i_rbuf.fragmentation_sz );
//
//			   msg->o.nbytes  = sizeof( int );
//			   msg->o.ret_val = sz;
//			   break;
//		   }
//
//		   case DCMD_IOADM_GET_SAMPLE:
//		   {
//			   uint16_t        val     = 0;
//			   int cnt=1;
//			   uint32_t i;
//			   int nSamples;
//			   //trace_logf( _NTO_TRACE_USERFIRST + 0,  "PCI1713.DCMD_IOADM_GET_SAMPLE: enter");
//			   // active channels setup
//			   pci1713_msg_get_sample_t *bi_msg = (pci1713_msg_get_sample_t *)data;
//			   nSamples = bi_msg->nSamples;
//			   out8(pci1713->board_info.base+4, bi_msg->start_ch); // MUX start channel
//			   out8(pci1713->board_info.base+5, bi_msg->start_ch + bi_msg->channels-1); // MUX stop channel
//
//			   for (i=0; i < nSamples; ++i)
//			   {
//				   // set SW trigger
//				   const uint8_t ucTrigSrc = 1;       //s/w trigger
//				   const uint8_t ucIrqEn = 0x0;      //disable interrupt;
//				   out8(pci1713->board_info.base + 6, ucTrigSrc | ucIrqEn );
//				   // start and wait for conversion to complete
//				   out8(pci1713->board_info.base, 0);
//
//				   // wait for A/D conversion to complete
//				   while( 1&in8(pci1713->board_info.base+7) ){cnt++;} // read status reg FIFO empty bit
//				   // get ADC value
//				   val = (uint16_t)in16(pci1713->board_info.base);
//				   // val = (uint16_t)in16(pci1713->board_info.base);
//				   *( (uint16_t *)data + i) = val;
//			   }
//			   //trace_logf( _NTO_TRACE_USERFIRST + 0,  "PCI1713.DCMD_IOADM_GET_SAMPLE: val= %u", val);
//		   }
//		   break;
//
//		   case DCMD_IOADM_GET_SAMPLE:
//		   {
//
//			   int i,j;
//			   uint16_t        val     = 0;
//			   int arrSz;
//
//			   trace_logf( _NTO_TRACE_USERFIRST + 0,  "PCI1713.DCMD_IOADM_GET_SAMPLE: enter");
//
//			   //StopPacer(pci1713->board_info.base);
//			   //out8(pci1713->board_info.base+6, 0);
//			   //out8(pci1713->board_info.base+9, 0);
//
//			   arrSz = msg->i.nbytes/sizeof(pci1713_msg_get_sample_t);
//			   for (i=0; i<arrSz; ++i)
//			   {
//				   uint8_t x;
//				   pci1713_msg_get_sample_t     *ch_msg = ( (pci1713_msg_get_sample_t *)data ) + i;
//				   int cnt;
//
//				   DEVCTL_RET( ch_msg->ch_sett.num >= ADC_MAX_CHANNELS, 1 )
//				   DEVCTL_RET( ch_msg->ch_sett.num < 0,                 1
//							   )
//				   out8(pci1713->board_info.base+4, ch_msg->ch_sett.num); // MUX start channel
//				   out8(pci1713->board_info.base+5, ch_msg->ch_sett.num); // MUX stop channel
//
//				   if (ch_msg->ch_sett.b_u != -1) // need to apply channel settings
//				   {
//					   x = ( (ch_msg->ch_sett.gain)&7 ) |
//						   ( ( (ch_msg->ch_sett.b_u)&1 ) << 4 ) |
//						   ( ( (ch_msg->ch_sett.s_d)&1 ) << 5 );
//					   out8(pci1713->board_info.base+2, x);
//				   }
//				   // set SW trigger
//				   const uint8_t ucTrigSrc = 1;       //s/w trigger
//				   const uint8_t ucIrqEn = 0x0;      //disable interrupt;
//				   out8(pci1713->board_info.base + 6, ucTrigSrc | ucIrqEn );
//				   // start and wait for conversion to complete
//				   out8(pci1713->board_info.base, 0);
//				   cnt=0;
//				   // wait for A/D conversion to complete
//	   			   while( 1&in8(pci1713->board_info.base+7) ){cnt++;}; // read status reg FIFO empty bit
//				   // get ADC value
//				   val = (uint16_t)in16(pci1713->board_info.base);
//
//
//				   // val = (uint16_t)in16(pci1713->board_info.base);
//
//				   *( (uint16_t *)data + i) = val;
//				   trace_logf( _NTO_TRACE_USERFIRST + 0,  "PCI1713.DCMD_IOADM_GET_SAMPLE: val= %u", val);
//
//			   }
//
//			   msg->o.nbytes  = sizeof( uint16_t ) * arrSz;
//			   msg->o.ret_val = 0;
//			   trace_logf( _NTO_TRACE_USERFIRST + 0,  "PCI1713.DCMD_IOADM_GET_SAMPLE: exit");
//			   break;
//		   }
//		   case DCMD_IOADM_GET_STATE: {
//			   *(uint32_t *)data = pci1713->descriptor.device_state;
//
//			   msg->o.nbytes  = sizeof( uint32_t );
//			   msg->o.ret_val = 0;
//			   break;
//		   }

	       default:
	            msg->o.ret_val = 0xff;  /* Error status */
	            msg->o.nbytes  = 0;

	    }
	return (0);
}

//
//int io_adm_drv_devctl( void *device, io_devctl_t *msg, void *data )
//{
//    l783m_t                 *l783m = (l783m_t *)device;
//
//    switch ( msg->i.dcmd ) {
//
//        case DCMD_IOADM_CHANNELS: {
//            int             status = 0;
//
//            DEVCTL_RET( *(int *)data > ADC_MAX_CHANNELS, 1 )
//            DEVCTL_RET( *(int *)data <= 0, 4 )
//            status = dsp_adc_enable( l783m, 0, 0 );
//            DEVCTL_RET( status != 0, 5 )
//
//            l783m->channels = *(int *)data;
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_CONTROL_HEIGHT +
//                                      l783m->board_info.bi.base_dsp_dm_offset, l783m->channels );
//            status = dsp_bios_command( l783m, BIOS_COMMAND_LOAD_CONTROL_TABLE, 0 );
//            DEVCTL_RET( status != 0, 6 )
//
//            //status = dsp_adc_enable( l783m, 1, 0 );
//            //DEVCTL_RET( status != 0, 5 )
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_CHANNEL: {
//            l783m_msg_channel_t *ch_msg = (l783m_msg_channel_t *)data;
//
//            if ( !(ch_msg->mode & L783M_CHANNEL_PRESET) ) {
//                ch_msg->x  = ch_msg->input & 0x3F;
//                ch_msg->x |= (ch_msg->gain & 3) << 6;
//                if (ch_msg->mode & L783M_CHANNEL_SINGLE_ENDED)
//                    ch_msg->x |= 0x20;
//                if (ch_msg->mode & L783M_CHANNEL_CALIBRATION)
//                    ch_msg->x |= 0x10;
//            }
//
//            DEVCTL_RET( ch_msg->index >= ADC_MAX_CHANNELS, 1 )
//            DEVCTL_RET( ch_msg->index < 0,                 1 )
//
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_CONTROL_TABLE + ch_msg->index +
//                                      l783m->board_info.bi.base_dsp_dm_offset, ch_msg->x );
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_IN_BUFF: {
//            l783m_buff_info_t   *bi_msg = (l783m_buff_info_t *)data;
//            int                 status  = 0;
//
//            DEVCTL_RET( bi_msg->buf_sz < 0,   1 )
//            DEVCTL_RET( bi_msg->frag_sz <= 0, 1 )
//
//            free_rbuf( &l783m->i_rbuf );
//
//            if ( bi_msg->buf_sz == 0 )      /* Set to maximum */
//                bi_msg->buf_sz = RBUF_SZ - (RBUF_SZ % bi_msg->frag_sz);
//
//            init_rbuf( l783m, &l783m->i_rbuf, bi_msg->buf_sz, bi_msg->frag_sz );
//
//            status = dsp_adc_set_buffer( l783m, 0, bi_msg->frag_sz );
//            DEVCTL_RET( status != 0, 2 )
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_FRATE: {
//            int                 status = 0;
//            int                 cr     = 0;
//
//            plx_int_enable( l783m, 0 );
//
//            status = dsp_adc_enable( l783m, 0, 0 );
//            DEVCTL_RET( status != 0, 1 )
//
//            read_dsp_dm_word( l783m, BIOS_VARIABLE_CONTROL_HEIGHT + l783m->board_info.bi.base_dsp_dm_offset );
//
//            DEVCTL_RET( *(int *)data <= 0, 2 )
//
//            l783m->rate = *(int *)data;
//
//            status = dsp_adc_set_timing( l783m, &l783m->rate, &cr );
//            DEVCTL_RET( status != 0, 3 )
//
//            //status = dsp_adc_enable( l783m, 1, 0 );
//            //DEVCTL_RET( status != 0, 4 )
//
//            status = dsp_adc_set_buffer( l783m, 0 /* Max value */, l783m->i_rbuf.fragmentation_sz );
//            DEVCTL_RET( status != 0, 5 )
//
//            //status =  dsp_adc_int_enable( l783m, 1, 0 );
//            //DEVCTL_RET( status != 0, 6 )
//
//            plx_int_enable( l783m, 1 );
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_TIMING: {
//            l783m_msg_timing_params_t   *tp    = (l783m_msg_timing_params_t *)data;
//            int                         status = 0;
//
//            status = dsp_adc_set_timing( l783m, &tp->frame_rate, &tp->channel_rate );
//            DEVCTL_RET( status != 0, 1 )
//
//            l783m->rate = tp->frame_rate;
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_SYNC: {
//            l783m_msg_sync_t    *s     = (l783m_msg_sync_t *)data;
//            int                 status = 0;
//            int                 tmp    = -1;
//            int                 tmp2  = -1;
//
//            if ( s->mode != L783M_SYNC_MODE_NONE ) {
//                plx_int_enable( l783m, 0 );
//
//                //status = dsp_adc_int_enable( l783m, 1, 0 );
//                //DEVCTL_RET( status != 0, 1 )
//            }
//
//            if ( s->mode & L783M_SYNC_MODE_DIGITAL ) {
//                if ( s->mode & L783M_SYNC_MODE_FRAME )
//                    tmp = 0;  /* Digital Sync + Per-Frame Sync */
//                else
//                    tmp = 1;  /* Digital Sync */
//            }
//            else
//                if ( s->mode & L783M_SYNC_MODE_ANALOG )
//                    tmp = 2;  /* Analog Sync */
//
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_SYNC_TYPE +
//                                      l783m->board_info.bi.base_dsp_dm_offset, tmp );
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_SYNC_CHANNEL +
//                                      l783m->board_info.bi.base_dsp_dm_offset, s->channel );
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_SYNC_LEVEL +
//                                      l783m->board_info.bi.base_dsp_dm_offset, s->threshold );
//
//            switch ( s->mode & 0x0f ) {
//                case L783M_SYNC_MODE_HIGH:
//                    tmp = 0;
//                    tmp2 = 0;
//                    break;
//                case L783M_SYNC_MODE_LOW:
//                    tmp = 0;
//                    tmp2 = 1;
//                    break;
//                case L783M_SYNC_MODE_RISE:
//                    tmp = 1;
//                    tmp2 = 0;
//                    break;
//                case L783M_SYNC_MODE_FALL:
//                    tmp = 1;
//                    tmp2 = 1;
//                    break;
//            }
//
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_SYNC_EDGE +
//                                      l783m->board_info.bi.base_dsp_dm_offset, tmp );
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_SYNC_MODE +
//                                      l783m->board_info.bi.base_dsp_dm_offset, tmp2 );
//
//            status = dsp_bios_command( l783m, BIOS_COMMAND_SYNC_CONFIG, 0 );
//            DEVCTL_RET( status != 0, 2 )
//
//            if ( s->mode != L783M_SYNC_MODE_NONE ) {
//                plx_int_enable( l783m, 1 );
//                l783m->adc_start = 1;       /* Start ADC reading */
//            }
//
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_START: {
//            int             status = 0;
//
//            DEVCTL_RET( l783m->adc_start == 1, 1 )      /* Already started */
//
//            l783m->adc_start = 1;
//
//            status = dsp_bios_command( l783m, BIOS_COMMAND_LOAD_CONTROL_TABLE, 0 );
//            DEVCTL_RET( status != 0, 2 )
//
//            status = dsp_adc_enable( l783m, 1, 0 );
//            DEVCTL_RET( status != 0, 3 )
//
//            l783m->i_rbuf.write_idx = l783m->i_rbuf.read_idx = 0;
//
//            status = dsp_adc_int_enable( l783m, 1, 0 );    /* Enable ADC IRQ */
//            DEVCTL_RET( status != 0, 4 )
//
//            msg->o.nbytes  = 0;
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_STOP: {
//            int             status = 0;
//
//            l783m->adc_start = 0;
//
//            status = dsp_adc_enable( l783m, 0, 0 );
//            DEVCTL_RET( status != 0, 2 )
//
//            status = dsp_adc_int_enable( l783m, 0, 0 );    /* Disable ADC IRQ */
//            DEVCTL_RET( status != 0, 1 )
//
//            msg->o.nbytes  = 0;
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_BUFF_MODE: {
//            msg->o.ret_val = LIBMODE_SHARED_MEMORY | LIBMODE_BUFPERM_R;
//            break;
//        }
//
//        case DCMD_IOADM_GET_SHMOBJECT: {
//            char            *name = (char *)data;
//
//            strcpy( name, l783m->i_rbuf.shm_name );
//
//            msg->o.nbytes  = 40;
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_SHMOBJECT_SZ: {
//            msg->o.ret_val = l783m->i_rbuf.sz;
//            break;
//        }
//
//        case DCMD_IOADM_GET_SHMOBJECT_RD: {
//            int                     sz = 0;
//
//            sz = io_adm_drv_read( l783m, data, l783m->i_rbuf.fragmentation_sz );
//
//            msg->o.nbytes  = sizeof( int );
//            msg->o.ret_val = sz;
//            break;
//        }
//
//        case DCMD_IOADM_GET_SAMPLE: {
//            l783m_msg_channel_t     *ch_msg = (l783m_msg_channel_t *)data;
//            int                     status  = 0;
//            signed short int        val     = 0;
//
//            DEVCTL_RET( ch_msg->index >= ADC_MAX_CHANNELS, 1 )
//            DEVCTL_RET( ch_msg->index < 0,                 1 )
//
//            if ( !(ch_msg->mode & L783M_CHANNEL_PRESET) ) {
//                ch_msg->x  = ch_msg->input & 0x3F;
//                ch_msg->x |= (ch_msg->gain & 3) << 6;
//                if (ch_msg->mode & L783M_CHANNEL_SINGLE_ENDED)
//                    ch_msg->x |= 0x20;
//                if (ch_msg->mode & L783M_CHANNEL_CALIBRATION)
//                    ch_msg->x |= 0x10;
//            }
//
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_CHANNEL +
//                                      l783m->board_info.bi.base_dsp_dm_offset, ch_msg->x );
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_SAMPLE +
//                                      l783m->board_info.bi.base_dsp_dm_offset, 0x8000 );
//
//            status = dsp_bios_command( l783m, BIOS_COMMAND_ADC_SAMPLE, 0 );
//            DEVCTL_RET( status != 0, 2 )
//
//            val = (signed short int)(uint16_t)read_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_SAMPLE +
//                                                                l783m->board_info.bi.base_dsp_dm_offset );
//            *(signed short int *)data = val;
//
//            msg->o.nbytes  = sizeof( l783m_msg_channel_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_SET_CALIBRATION_MUL: {
//            int                     status = 0;
//            uint16_t                calibration_muls[4];
//            uint16_t                scale_muls[4];
//
//            status = eeprom_read_data( l783m, 20, calibration_muls, 4 );
//            DEVCTL_RET( status != 0, 1 )
//
//            status = eeprom_read_data( l783m, 24, scale_muls, 4 );
//            DEVCTL_RET( status != 0, 2 )
//
//            status = write_dsp_dm( l783m, BIOS_VARIABLE_ZERO + l783m->board_info.bi.base_dsp_dm_offset, calibration_muls, 4 );
//            DEVCTL_RET( status != 0, 3 )
//
//            status = write_dsp_dm( l783m, BIOS_VARIABLE_SCALE + l783m->board_info.bi.base_dsp_dm_offset, scale_muls, 4 );
//            DEVCTL_RET( status != 0, 4 )
//
//            msg->o.nbytes  = 0;
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_ENABLE_CALIBRATION: {
//            int                     enable = *(int *)data;
//
//            DEVCTL_RET( enable > 1 || enable < 0, 1 )
//
//            write_dsp_dm_word( l783m, BIOS_VARIABLE_CORRECTION_ENABLE +
//                                      l783m->board_info.bi.base_dsp_dm_offset, enable );
//
//            msg->o.nbytes  = 0;
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_SAMPLE_RANGES: {
//            l783m_msg_sample_ranges_t   *sr = (l783m_msg_sample_ranges_t *)data;
//
//            sr->sample_val_min = -2048;
//            sr->sample_val_max =  2048;
//
//            switch ( sr-> gain ) {
//
//                case 0:
//                    sr->u_min          = -5000000;
//                    sr->u_max          =  5000000;
//                    break;
//
//                case 1:
//                    sr->u_min          = -2500000;
//                    sr->u_max          =  2500000;
//                    break;
//
//                case 2:
//                    sr->u_min          = -1250000;
//                    sr->u_max          =  1250000;
//                    break;
//
//                case 3:
//                    sr->u_min          = -625000;
//                    sr->u_max          =  625000;
//                    break;
//
//                default:
//                    msg->o.ret_val = 1;     /* Error status */
//                    msg->o.nbytes  = 0;
//                    return (0);
//
//            }
//
//            msg->o.nbytes  = sizeof( l783m_msg_sample_ranges_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_INT_TIME: {
//            l783m->adc_int_time = (double)(l783m->adc_int_iv_cc) /
//                                          (SYSPAGE_ENTRY(qtime)->cycles_per_sec);
//
//            *(double *)data = l783m->adc_int_time;
//
//            msg->o.nbytes  = sizeof( double );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_RD_TIME: {
//            l783m->adc_read_time = (double)(l783m->adc_read_iv_cc) /
//                                          (SYSPAGE_ENTRY(qtime)->cycles_per_sec);
//
//            *(double *)data = l783m->adc_read_time;
//
//            msg->o.nbytes  = sizeof( double );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_STATE: {
//            *(uint32_t *)data = l783m->descriptor.device_state;
//
//            msg->o.nbytes  = sizeof( uint32_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_RECEIVED_DATA_SZ: {
//            *(uint64_t *)data = l783m->descriptor.data_received;
//
//            msg->o.nbytes  = sizeof( uint64_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_READED_DATA_SZ: {
//            *(uint64_t *)data = l783m->descriptor.data_readed;
//
//            msg->o.nbytes  = sizeof( uint64_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_AVAIL_DATA_SZ: {
//            *(uint32_t *)data = 0;
//
//            if ( l783m->ir_fixed ) {
//                if ( l783m->descriptor.device_state & IO_ADM_DRV_STATE_IRQ_ENABLED )
//                    *(uint32_t *)data = l783m->i_rbuf.fragmentation_sz;
//                else
//                    *(uint32_t *)data = 0;
//            } else
//                if ( l783m->descriptor.device_state & IO_ADM_DRV_STATE_IRQ_ENABLED )
//                    *(uint32_t *)data = rbuf_avail_sz( &l783m->i_rbuf );
//                else
//                    *(uint32_t *)data = 0;
//
//            msg->o.nbytes  = sizeof( uint32_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        case DCMD_IOADM_GET_LAST_RECV_TIME: {
//            *(uint64_t *)data = l783m->adc_int_ed_cc;
//
//            msg->o.nbytes  = sizeof( uint64_t );
//            msg->o.ret_val = 0;
//            break;
//        }
//
//        default:
//            msg->o.ret_val = 0xff;  /* Error status */
//            msg->o.nbytes  = 0;
//
//    }
//
//    return (0);
//}
