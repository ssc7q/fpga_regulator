/*
 * (c) CBD BC, Russia, Spb.
 *
 * Mail: support@kpda.ru
 *
 * Date: 26/11/2009
 * Dev:  A. Docuchaev
 */


/* event.c */


//#include "l783m.h"
#include "pci1721.h"
#include <sys/trace.h>
#include <pthread.h>

//struct sigevent             irq_notifier;


/*
 * Init ISR notify event
 */
//void init_isr( void )
//{
//    irq_notifier.sigev_notify = SIGEV_INTR;
//}

/*
 * Interrupt handler
 */

/*
const struct sigevent * irq_handler( void *area, int id )
{
	   volatile	pci1713_t                 *adc         = (pci1713_t *)area;
	   volatile unsigned char ucRegVal;
	   volatile int ret=0;
	   volatile int i;
	   volatile uint16_t sample;
	   //InterruptMask(adc.irq, adc.iid);
	   //Determined this interrupt source.
	   ucRegVal = in8( adc->board_info.base+7);
	   trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713 ");
	   if( ucRegVal & 8 )         //Is it our interrupt
	   {
		   trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: our IRQ ");
	        for (i=0; i< adc->fifoSizeSamples/2; i++)
			 {
	        	sample = in16(adc->board_info.base);
	        	if (i<3) trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: sample = %u ", sample);
	        	 *( (uint16_t *)(adc->i_rbuf.data + adc->i_rbuf.write_idx) ) = sample;
	        	adc->i_rbuf.write_idx = (adc->i_rbuf.write_idx + 1 * sizeof(uint16_t)) % adc->i_rbuf.sz;
			 }
	        trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: write_idx %u", adc->i_rbuf.write_idx);
	        adc->i_rbuf.iFragment += (adc->fifoSizeSamples)*sizeof(uint16_t)/2;
	        if (adc->i_rbuf.iFragment == adc->i_rbuf.fragmentation_sz)
	        {
	        	//trace_logf( _NTO_TRACE_USERFIRST + 0, "IRQ PCI1713: iffragend ");
	        	// disable adc trigger and interrupt
	        	out8(adc->board_info.base + 6, 0);
	        	adc->i_rbuf.iFragment = 0;
	        	ret = 1;
	        }
	        //Clear interrupt
	       	out8(adc->board_info.base+8, 0);
	   }
	  // InterruptUnmask(adc.irq, adc.iid);
	   if (ret)
		   return &irq_notifier;
	   else
		   return NULL;
}
*/

//const struct sigevent * irq_handler( void *area, int id )
//{
//	   volatile	pci1713_t                 *adc         = (pci1713_t *)area;
//	   volatile unsigned char ucRegVal;
//	   volatile int ret=0;
//
//
//	   //Determined this interrupt source.
//	   ucRegVal = in8( adc->board_info.base+7);
//	   //trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713 ");
//	   if( ucRegVal & 8 )         //Is it our interrupt
//	   {
//		   // mask IRQ
//		   InterruptMask(adc->board_info.InterruptLine, adc->iid);
//		   //trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: our IRQ ");
//		   ret = 1;
//	   }
//	   //!!!! we should NOT clear IRQ here!!  If we clear IRQ immediately after IRQ active we get double interrupt occurrence.
//	   // This is because of PCI bridge features of PCI-1713 device.
//	   if (ret)
//		   return &irq_notifier;
//	   else
//		   return NULL;
//}

void* dac1721FillingThreadFunc(void* _board)
{
	pci1721_t       *board         = (pci1721_t *)_board;
	int sts, i;
	word data1, data2;
//	volatile    uint16_t 		sample;
//	volatile    int 			i;
	ThreadCtl(_NTO_TCTL_IO, 0);         // Request I/O privity
    /* Init IRQ */
//    init_isr();
    //!!!plx_int_enable( pci1713, 0 );
//    adc->iid = InterruptAttach( (int)adc->board_info.InterruptLine,
//                                  irq_handler,
//                                  (const void *)adc,
//                                  sizeof( pci1713_t * ),
//                                  _NTO_INTR_FLAGS_TRK_MSK );

	while (1) {
		usleep(board->period);
		//usleep(1000);
		sts = getFifoStatus(board);
		//if ((sts == DA_FIFO_EMPTY) || (sts == DA_FIFO_HALF_FULL))
		if (sts != DA_FIFO_HALF_FULL)
		{
			//trace_logf( _NTO_TRACE_USERFIRST + 1,  "io_adm_cl:begin");
			pthread_mutex_lock(&board->bufMutex);
			// пишу не более половины буфера
			for(i=0; i<DA_FIFO_SIZE/2; i+=2)
			//while (getFifoStatus(board) != DA_FIFO_HALF_FULL)
			{
				// пишу по два 16 битных слова - так есть в описании регистров - пусть так и будет
				if (board->buffer.indexRd != board->buffer.indexWr)// когда Рд и Вр сравняются - значит программный буфер пуст
				{
					board->buffer.indexRd++;
					if (board->buffer.indexRd >= board->buffer.size / BYTES_IN_SAMPLE)
						board->buffer.indexRd = 0;
					data1 = ((word*) (board->buffer.data))[board->buffer.indexRd];
					if (board->buffer.indexRd != board->buffer.indexWr) // пара для первого существует
					{
						board->buffer.indexRd++;
						if (board->buffer.indexRd >= board->buffer.size / BYTES_IN_SAMPLE)// где-то раньше нарушилась парность и сейчас получилось что первый в конце, а второй в начале буфера
							board->buffer.indexRd = 0;
						data2 = ((word*) (board->buffer.data))[board->buffer.indexRd];
					}
					else // нарушилась парность и кончились данные
					{
						data2 = data1; // повторяю то же самое значение
					}
				}
				else // если сразу ясно что данные кончились - ничего не вывожу
					break;
				writeFifoData(board, data1, data2);

			}
			pthread_mutex_unlock(&board->bufMutex);
		}



//	    InterruptWait (NULL, NULL);
	    // increment available data counter and detect possible overrun
//	    pthread_mutex_lock((pthread_cond_t*)&adc->adcDataReadyMutex);
//	    adc->i_rbuf.iFragment += (adc->fifoSizeSamples)*sizeof(uint16_t)/2;
//	    if (adc->i_rbuf.iFragment > adc->i_rbuf.sz)
//	    	{
//	    		adc->descriptor.device_state |= IO_ADM_DRV_STATE_DEV_DATA_OVERRUN;
//	    	}
//	    pthread_mutex_unlock((pthread_cond_t*)&adc->adcDataReadyMutex);
//
//	    for (i=0; i< adc->fifoSizeSamples/2; i++)
//	     {
//	    	sample = in16(adc->board_info.base);
//	    	if (i<3) trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: sample = %u ", sample);
//	    	 *( (uint16_t *)(adc->i_rbuf.data + adc->i_rbuf.write_idx) ) = sample;
//	    	adc->i_rbuf.write_idx = (adc->i_rbuf.write_idx + 1 * sizeof(uint16_t)) % adc->i_rbuf.sz;
//	     }
//	    trace_logf( _NTO_TRACE_USERFIRST + 0,  "IRQ PCI1713: write_idx %u", adc->i_rbuf.write_idx);
//
//	    // mutex to protect iFragment
//	    pthread_mutex_lock((pthread_cond_t*)&adc->adcDataReadyMutex);
//
//	    if (adc->i_rbuf.iFragment == adc->i_rbuf.fragmentation_sz)
//	    {
//	    	//trace_logf( _NTO_TRACE_USERFIRST + 0, "IRQ PCI1713: iffragend ");
//	    	if (!(adc->cyclicMode)) // if non-cyclic mode then stop FAI
//	    	{
//	    		adc1713Stop(adc);
//	    	}
//	    	pthread_cond_signal((pthread_cond_t*)&(adc->adcDataReadyCondVar));
//
//	    }
//	    pthread_mutex_unlock((pthread_cond_t*)&adc->adcDataReadyMutex);
//
//	    //Clear interrupt (!!!! we should clear IRQ not immediately after IRQ active in order to avoid double interrupt occurrence )
//	    // This is because of PCI bridge features of PCI-1713 device.
//	    out8(adc->board_info.base+8, 0);
//	    InterruptUnmask(adc->board_info.InterruptLine, adc->iid);
	}

}
//
//const struct sigevent * irq_handler( void *area, int id )
//{
//    l783m_t                 *l783m         = (l783m_t *)area;
//    register int            prev_idma_addr = 0;
//    register int            adc_fifo_ptr   = 0,
//                            adc_fifo_start = 0,
//                            adc_fifo_end   = 0,
//                            adc_fifo_size  = 0,
//                            wd             = 0,
//                            tmp            = 0;
//    register int            sz             = 0;     /* Count of received bytes */
//    register int            reply          = 0;
//    register uint64_t       adc_read_st_cc = 0;
//
//    InterruptMask( (int)l783m->board_info.InterruptLine, l783m->iid );
//
//    /* Save IDMA addr */
//    prev_idma_addr  = l783m->idma_addr;
//
//    /* Is this our IRQ */
//    if ( !( in32( l783m->board_info.bi.ioplx + PLX_REG_INTCSR ) & PLX_REG_INTCSR_LI1S ) ) {
//        InterruptUnmask( (int)l783m->board_info.InterruptLine, l783m->iid );
//        return (NULL);
//    }
//
//    /* Process interrupt */
//    if ( l783m->adc_start ) {
//
//        /* Get read time */
//        adc_read_st_cc = ClockCycles();
//
//        adc_fifo_ptr = read_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_FIFO_PTR +
//                                         l783m->board_info.bi.base_dsp_dm_offset );
//        adc_fifo_start = read_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_FIFO_BASE +
//                                           l783m->board_info.bi.base_dsp_dm_offset );
//        adc_fifo_size = read_dsp_dm_word( l783m, BIOS_VARIABLE_ADC_FIFO_LENGTH +
//                                          l783m->board_info.bi.base_dsp_dm_offset );
//        adc_fifo_end = adc_fifo_start + adc_fifo_size;
//        if ( l783m->i_rbuf_index == 0 )
//            l783m->i_rbuf_index = adc_fifo_ptr;
//
//        while ( l783m->i_rbuf_index != adc_fifo_ptr ) {
//
//            if ( adc_fifo_ptr < l783m->i_rbuf_index )
//                wd = adc_fifo_end - l783m->i_rbuf_index;
//            else
//                wd = adc_fifo_ptr - l783m->i_rbuf_index;
//
//            tmp = (l783m->i_rbuf.sz - l783m->i_rbuf.write_idx) / sizeof(uint16_t);
//            if ( wd > tmp )
//                wd = tmp;
//            sz = wd * sizeof(uint16_t);
//
//            read_dsp_dm( l783m, l783m->i_rbuf_index, (uint16_t *)(l783m->i_rbuf.data + l783m->i_rbuf.write_idx), wd );
//            l783m->descriptor.data_received += sz;
//
//            l783m->i_rbuf.write_idx = (l783m->i_rbuf.write_idx + sz) % l783m->i_rbuf.sz;
//
//            l783m->i_rbuf_index += wd;
//            if ( l783m->i_rbuf_index >= adc_fifo_end )
//                l783m->i_rbuf_index = adc_fifo_start;
//
//            reply = 1;
//
//        }
//
//        if ( l783m->ir_fixed )
//            l783m->i_rbuf.write_idx = 0;
//
//        /* Get read time */
//        l783m->adc_read_iv_cc = ClockCycles() - adc_read_st_cc;
//
//        /* Get time between 2 interrupts */
//        if ( l783m->adc_int_st_cc == 0 )
//            l783m->adc_int_st_cc = ClockCycles();
//        else {
//            l783m->adc_int_ed_cc = ClockCycles();
//            l783m->adc_int_iv_cc = l783m->adc_int_ed_cc - l783m->adc_int_st_cc;
//            l783m->adc_int_st_cc = l783m->adc_int_ed_cc;
//        }
//
//    }
//
//    /* Clear IRQ source */
//    out16( l783m->board_info.bi.base + DSP_RESET_ADDRESS, 0 );
//
//    /* Restore IDMA address */
//    l783m->idma_addr = prev_idma_addr;
//    if ( prev_idma_addr >= 0 )
//        out16( l783m->board_info.bi.base + IDMA_ADDR, prev_idma_addr);
//
//    InterruptUnmask( (int)l783m->board_info.InterruptLine, l783m->iid );
//
//    if ( reply )
//        return ( &irq_notifier );
//    else
//        return (NULL);
//}
