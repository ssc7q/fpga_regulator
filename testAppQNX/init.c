/*
 * (c) CBD BC, Russia, Spb.
 *
 * Mail: support@kpda.ru
 *
 * Date: 22/11/2009
 * Dev:  A. Docuchaev
 */


/* init.c */


//#include "l783m.h"
#include "pci1721.h"


static  char                *pci1721_opts [] = {
    "verbose",              /* 0 */
    "idx",                  /* 1 */
    "ir_fixed",             /* 2 */
    NULL
};


/*
 * Called from io_adm_drv_init()
 * Out: EOK - ok
 */
int pci1721_parse_options( pci1721_t *pci1721, char *options )
{
    char                    *value, *restore, *c;
    int                     opt;
    int                     rc = 0;
    int                     err = EOK;



    if ( !options )
        return EOK;

    restore = NULL;

    while ( options && *options != '\0' ) {

        c = options;
        restore = strchr( options, ',' );
        opt = getsubopt( &options, pci1721_opts, &value );

        switch ( opt ) {
            case  0:    /* "verbose" */
                if ( value )
                    pci1721->verbose = strtoul( value, 0, 0 );
                else
                    pci1721->verbose = 1;
                break;

            case 1:     /* "idx" */
                if ( value )
                    pci1721->descriptor.device_idx = strtoul( value, 0, 0 );
                break;

            case 2:     /* "ir_fixed" */
                //pci1721->ir_fixed = 1;
                break;

            default:
                MSG( STDF, "pci1721: Undefined option\n" );
                return EINVAL;
                break;
        }   /* switch */

        if (restore != NULL)
            *restore = ',';

    }   /* while */

    errno = err;
    return (rc);
}


void * io_adm_drv_init( char *options )
{

	struct sched_param param;
    struct _clockperiod clk_per_new;
    struct _clockperiod clk_per_old;

	pci1721_t                 *pci1721 = (pci1721_t *)malloc( sizeof( pci1721_t ) );
    if ( !pci1721 ) {
        fprintf( stderr, "pci1721: Can't allocate pci1721_t\n" );
        return (NULL);
    }
    pci1721->verbose                 = 0;
    //pci1721->ir_fixed                = 0;

    /* Init drv-descriptor */
    IO_ADM_DRV_DESCRIPTOR_INIT( pci1721->descriptor )
    pci1721->descriptor.io_adm_drv_open    = io_adm_drv_open;
    pci1721->descriptor.io_adm_drv_close   = io_adm_drv_close;
    pci1721->descriptor.io_adm_drv_destroy = io_adm_drv_destroy;
    pci1721->descriptor.io_adm_drv_write   = io_adm_drv_write;
    pci1721->descriptor.io_adm_drv_read    = io_adm_drv_read;
    pci1721->descriptor.io_adm_drv_devctl  = io_adm_drv_devctl;
    pci1721->descriptor.io_adm_drv_devname = io_adm_drv_devname;
    pci1721->descriptor.io_adm_drv_devid   = io_adm_drv_devid;
    pci1721->descriptor.io_adm_drv_devstat = io_adm_drv_devstat;
    pci1721->descriptor.device_idx         = 0;
    pci1721->descriptor.caps               = 0;
    //pci1721->cyclicMode = 0;

    /* Parse options */
    if ( pci1721_parse_options( pci1721, options ) != EOK ) {
        free( pci1721 );
        return NULL;
    }

    if ( pci1721->verbose )
        MSG( STDF, "\nDriver for Advantech PCI1721 DAC started\n" );

    ThreadCtl( _NTO_TCTL_IO, 0 );

    /* Find device */
    if ( scan_pci( pci1721 ) != 0 ) {
        free( pci1721 );
        return NULL;
    }
    pci1721->descriptor.device_state |= IO_ADM_DRV_STATE_DEV_DETECTED;

//    pthread_cond_init(& pci1713->adcDataReadyCondVar, NULL);
//    pthread_mutex_init(&pci1713->adcDataReadyMutex, NULL);
    pthread_mutex_init(&(pci1721->bufMutex), NULL);
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    struct sched_param params;
    params.sched_priority = PCI1721_THREAD_PRIO;
    pthread_attr_setschedparam(&attr, &params);
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

    /* Init rbuf */
    if ( init_buffer( pci1721, &pci1721->buffer, 102400 ) != 0 ) {
        free( pci1721 );
        return NULL;
    }

    if ( pthread_create( &pci1721->dac1721FillingThread,
                                     &attr,
                                     dac1721FillingThreadFunc,
                                    pci1721 ) != EOK ) {
                    MSG( ERRF, "pci1721: Error: Can't create thread\n" );
                    free( pci1721 );
                    return NULL;
                }


//    if ( pci1721->iid < 0 ) {
//        MSG( STDF, "pci1721: InterruptAttach() failed\n" );
//        free( pci1721 );
//        return NULL;
//    }


    if ( pci1721->verbose > 2 )
        MSG( STDF, "buffer: size=0x%X addr=0x%X\n", pci1721->buffer.size,  (uint32_t)pci1721->buffer.data );
    pci1721->descriptor.device_state |= IO_ADM_DRV_STATE_DEV_MEM_ALLOCATED;

    io_adm_drv_open( (void *)pci1721 );


    pci1721->descriptor.device_state |= IO_ADM_DRV_STATE_DEV_INITED;

    pci1721->descriptor.device_state |= IO_ADM_DRV_STATE_READY;

    pci1721->descriptor.caps = IO_ADM_DRV_CAPS_DEVNAME |
                             IO_ADM_DRV_CAPS_DEVID |
                             IO_ADM_DRV_CAPS_DEVSTATS;

    // adjust clock period
    clk_per_new.fract = 0;
    clk_per_new.nsec = 10000;

    ClockPeriod(CLOCK_REALTIME, &clk_per_new, &clk_per_old, 0);

    return pci1721;
}

char * io_adm_drv_devname( void *device )
{
     return ("Advantech PCI1721");
}

uint32_t io_adm_drv_devid( void *device )
{
    pci1721_t                 *pci1721 = (pci1721_t *)device;
    return ((pci1721->board_info.VendorID << 16) | pci1721->board_info.DeviceID);
}

void * io_adm_drv_devstat( void *device )
{
    /* Not realized yet */
    return NULL;
}

int io_adm_drv_open( void *device )
{
	pci1721_t                 *pci1721 = (pci1721_t *)device;
    //int                     data   = 0;

    pci1721->buffer.indexWr = 0;
    pci1721->buffer.indexRd  = 0;
    //pci1721->rate             = 1000;
    pci1721->fao_running        = 0;
    //pci1721->i_rbuf.iFragment = 0;
    //pci1721->i_rbuf.fragmentation_sz = pci1713->fifoSizeSamples*sizeof(uint16_t)/2;
    //pci1721->cyclicMode = 0;

    IO_ADM_DRV_DESCRIPTOR_CLEAR_STATS( pci1721->descriptor )

    //pci1721->channels = 1;
    //ResetAi(pci1713->board_info.base);

    return (0);
}

void io_adm_drv_close( void *device )
{
	pci1721_t                 *pci1721 = (pci1721_t *)device;

   faoStop(pci1721);

}

void io_adm_drv_destroy( void *device )
{
	pci1721_t                 *pci1721 = (pci1721_t *)device;

	io_adm_drv_close( device );

    if ( pci1721->pci_attach_handle != -1 )
        pci_detach( pci1721->pci_attach_handle );



    /*stop thread*/
    pthread_cancel(pci1721->dac1721FillingThread);
    pthread_join(pci1721->dac1721FillingThread, NULL);

//    /* Detach ISR */
//    InterruptDetach( pci1721->iid );
//    pci1721->iid = 0;

    free_rbuf( &pci1721->buffer );
    free( pci1721 );
}
