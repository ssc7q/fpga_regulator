/* ioadm.h */


#include <io-adm-msg.h>

#ifndef _CBD_BC_IO_ADM_IOADM_H_
#define _CBD_BC_IO_ADM_IOADM_H_


    /* Defines */
	/* Driver states */
	#define IO_ADM_DRV_STATE_READY                      0x00000001ul    /* _IO_READ, _IO_WRITE, IO_DEVCTL allowed */
	#define IO_ADM_DRV_STATE_IRQ_ENABLED                0x00000002ul    /* IRQ enabled */
	#define IO_ADM_DRV_STATE_DEV_STARTED                0x00000004ul    /* Device is started */
	#define IO_ADM_DRV_STATE_DEV_DETECTED               0x00000100ul
	#define IO_ADM_DRV_STATE_DEV_INITED                 0x00000200ul
	#define IO_ADM_DRV_STATE_DEV_MEM_ALLOCATED          0x00000400ul
	#define IO_ADM_DRV_STATE_DEV_DATA_OVERRUN           0x00000800ul

		/* Standard io-adm DEVCTL operations */
    #define DCMD_IOADM_SET_IRQ_SOURCE           DEVCTL_IO_ADM_CMD_IOT(   1,  unsigned long long )
    #define DCMD_IOADM_SET_IRQ_POLARITY         DEVCTL_IO_ADM_CMD_IOT(   2,  unsigned long long )
    #define DCMD_IOADM_WAIT_IRQ_EV              DEVCTL_IO_ADM_CMD_IOT(   3,  int )
    #define DCMD_IOADM_CHANNELS                 DEVCTL_IO_ADM_CMD_IOT(   4,  int )
    #define DCMD_IOADM_SET_CHANNEL              DEVCTL_IO_ADM_CMD_IOT(   5,  int )
    #define DCMD_IOADM_SET_IN_BUFF              DEVCTL_IO_ADM_CMD_IOT(   6,  void * )
    #define DCMD_IOADM_SET_FRATE                DEVCTL_IO_ADM_CMD_IOT(   7,  int )
    #define DCMD_IOADM_SET_TIMING               DEVCTL_IO_ADM_CMD_IOT(   8,  void * )
    #define DCMD_IOADM_SET_SYNC                 DEVCTL_IO_ADM_CMD_IOT(   9,  void * )
    #define DCMD_IOADM_START                    DEVCTL_IO_ADM_CMD_IOT(  10,  int )
    #define DCMD_IOADM_STOP                     DEVCTL_IO_ADM_CMD_IOT(  11,  int )
    #define DCMD_IOADM_PAUSE                    DEVCTL_IO_ADM_CMD_IOT(  12,  int )
    #define DCMD_IOADM_RESUME                   DEVCTL_IO_ADM_CMD_IOT(  13,  int )
    #define DCMD_IOADM_GET_BUFF_MODE            DEVCTL_IO_ADM_CMD_IOTF( 14,  int )
    #define DCMD_IOADM_GET_SHMOBJECT            DEVCTL_IO_ADM_CMD_IOTF( 15,  char[40] )
    #define DCMD_IOADM_GET_SHMOBJECT_SZ         DEVCTL_IO_ADM_CMD_IOTF( 16,  int )
    #define DCMD_IOADM_GET_SHMOBJECT_RD         DEVCTL_IO_ADM_CMD_IOTF( 17,  int )
    #define DCMD_IOADM_GET_SAMPLE               DEVCTL_IO_ADM_CMD_IOTF( 18,  int )
    #define DCMD_IOADM_SET_SAMPLE               DEVCTL_IO_ADM_CMD_IOT(  19,  int )
    #define DCMD_IOADM_SET_CALIBRATION_MUL      DEVCTL_IO_ADM_CMD_IOT(  20,  void * )
    #define DCMD_IOADM_ENABLE_CALIBRATION       DEVCTL_IO_ADM_CMD_IOT(  21,  int )
    #define DCMD_IOADM_GET_SAMPLE_RANGES        DEVCTL_IO_ADM_CMD_IOTF( 22,  void * )
    #define DCMD_IOADM_GET_INT_TIME             DEVCTL_IO_ADM_CMD_IOF(  23,  double )
    #define DCMD_IOADM_GET_RD_TIME              DEVCTL_IO_ADM_CMD_IOF(  24,  double )
    #define DCMD_IOADM_GET_RDBACK_TIME          DEVCTL_IO_ADM_CMD_IOF(  25,  double )
    #define DCMD_IOADM_GET_WR_TIME              DEVCTL_IO_ADM_CMD_IOF(  26,  double )
    #define DCMD_IOADM_GET_STATE                DEVCTL_IO_ADM_CMD_IOF(  27,  unsigned int )
    #define DCMD_IOADM_GET_RECEIVED_DATA_SZ     DEVCTL_IO_ADM_CMD_IOF(  28,  unsigned long long )
    #define DCMD_IOADM_GET_READED_DATA_SZ       DEVCTL_IO_ADM_CMD_IOF(  29,  unsigned long long )
    #define DCMD_IOADM_GET_AVAIL_DATA_SZ        DEVCTL_IO_ADM_CMD_IOF(  30,  unsigned long )
    #define DCMD_IOADM_GET_LAST_RECV_TIME       DEVCTL_IO_ADM_CMD_IOF(  31,  unsigned long long )
    #define DCMD_IOADM_CLEAR_STATISTIC          DEVCTL_IO_ADM_CMD_IOT(  32,  int )
    #define DCMD_IOADM_READ_REG                 DEVCTL_IO_ADM_CMD_IOTF( 33,  void * )
    #define DCMD_IOADM_WRITE_REG                DEVCTL_IO_ADM_CMD_IOT(  34,  void * )
    #define DCMD_IOADM_READ_BACK                DEVCTL_IO_ADM_CMD_IOTF( 35,  void * )
	// ZE добавка
	#define DCMD_IOADM_SET_CHANNEL_SETTINGS 	DEVCTL_IO_ADM_CMD_IOT( 36,  void * )
	#define DCMD_IOADM_SET_BUFF             	DEVCTL_IO_ADM_CMD_IOT( 37,  void * )
	#define DCMD_IOADM_GET_BUFF_INFO        	DEVCTL_IO_ADM_CMD_IOF( 38,  void * )
	#define DCMD_IOADM_SET_LAST_INDEX        	DEVCTL_IO_ADM_CMD_IOT( 39,  void * )
	#define DCMD_IOADM_PORT_READ     			DEVCTL_IO_ADM_CMD_IOTF( 40,  void * )
	#define DCMD_IOADM_PORT_WRITE     			DEVCTL_IO_ADM_CMD_IOT( 41,  void * )

    /* Library mode (see ioadm_prepare_io()) */
    #define LIBMODE_UNLOAD                      0x000000    /* Driver unloaded */
    #define LIBMODE_SHARED_MEMORY               0x000001    /* Driver use shared memory */
    #define LIBMODE_SIMPLE                      0x000002    /* Driver require _IO_READ / _IO_WRITE operations */
    #define LIBMODE_BUFPERM_R                   0x010000    /* Read operations permitted */
    #define LIBMODE_BUFPERM_W                   0x020000    /* Write operations permitted */

    /* Device states (see ioadm_get_state()) */
    #define DEVICE_STATE_READY                  0x00000001ul    /* _IO_READ, _IO_WRITE, IO_DEVCTL allowed */
    #define DEVICE_STATE_IRQ_ENABLED            0x00000002ul    /* IRQ enabled */
    #define DEVICE_STATE_STARTED                0x00000004ul    /* Device is started */
    #define DEVICE_STATE_DETECTED               0x00000100ul    /* Device found */
    #define DEVICE_STATE_INITED                 0x00000200ul    /* The device inited */
    #define DEVICE_STATE_MEM_ALLOCATED          0x00000400ul    /* Memory allocated */

    /* Misc */
    #define IOADM_DEVICE_INIT                   { -1, -1, NULL, 0, LIBMODE_UNLOAD }


    /* Types */
    typedef struct ioadm_device {

        /* Device */
        int                 fd;             /* Device descriptor */

        /* I/O */
        int                 shm_object;     /* Shared memory object */
        void                *shm_buffer;
        int                 shm_object_sz;

        /* lib */
        int                 lib_mode;       /* Library mode */

    } ioadm_device_t;

    typedef struct ioadm_reg_desc {

        uint64_t            reg_type;
        uint64_t            reg_offset;
        uint64_t            reg_value;

    } ioadm_reg_desc_t;


    /* Functions */
#ifdef __cplusplus
 extern "C" {
#endif

    /* Lib control */
    int ioadm_lib_prepare_io( ioadm_device_t *dev );
    int ioadm_lib_get_mode( ioadm_device_t *dev );
    int ioadm_lib_get_io_permissions( ioadm_device_t *dev );
    int ioadm_custom_dcmd( ioadm_device_t *dev, int dcmd, void *msg, int sz );

    /* Manager control */

    /* Open device */
    int ioadm_open( ioadm_device_t *dev, char *device );
    int ioadm_open_pci( ioadm_device_t *dev, char *device, int pci_index );
    int ioadm_open_pci_network( ioadm_device_t *dev, char *device, int pci_index, char *network_name);
    /* Close device */
    void ioadm_close( ioadm_device_t *dev );

    /* Device control */
    int ioadm_set_irq_sources( ioadm_device_t *dev, uint64_t irqs );
    int ioadm_set_irq_polarity( ioadm_device_t *dev, uint64_t irqp );
    int ioadm_wait_irq_event( ioadm_device_t *dev );
    int ioadm_set_channels_number( ioadm_device_t *dev, int num );
    int ioadm_set_buff( ioadm_device_t *dev, void *msg, int sz );
    int ioadm_set_frame_rate( ioadm_device_t *dev, int rate );
    int ioadm_set_timing( ioadm_device_t *dev, void *msg, int sz );
    int ioadm_set_sync( ioadm_device_t *dev, void *msg, int sz );
    int ioadm_start( ioadm_device_t *dev, int param );
    int ioadm_stop( ioadm_device_t *dev, int param );
    int ioadm_pause( ioadm_device_t *dev );
    int ioadm_resume( ioadm_device_t *dev );
    int ioadm_set_calibration_multipliers( ioadm_device_t *dev, void *msg, int sz );
    int ioadm_enable_calibration( ioadm_device_t *dev, int enable );
    int ioadm_get_sample_ranges( ioadm_device_t *dev, void *msg, int sz );

    /* Device statistic */
    double             ioadm_get_b2int_time( ioadm_device_t *dev );
    double             ioadm_get_read_time( ioadm_device_t *dev );
    double ioadm_get_write_time( ioadm_device_t *dev );
    unsigned int       ioadm_get_state( ioadm_device_t *dev );
    unsigned long long ioadm_get_received_data_sz( ioadm_device_t *dev );
    unsigned long long ioadm_get_readed_data_sz( ioadm_device_t *dev );
    unsigned int       ioadm_get_available_data_sz( ioadm_device_t *dev );
    unsigned long long ioadm_get_last_receive_time( ioadm_device_t *dev );
    int                ioadm_clear_statistic( ioadm_device_t *dev );

    /* Device I/O */
    int ioadm_read( ioadm_device_t *dev, void *buffer, int sz );
    int ioadm_write( ioadm_device_t *dev, void *buffer, int sz );
    int ioadm_read_fragment_wc( ioadm_device_t *dev, void **ptr );
    int ioadm_get_sample( ioadm_device_t *dev, void *channel, int sz );
    int ioadm_set_sample( ioadm_device_t *dev, void *channel, int sz );
    /* RD/WR registers */
    int ioadm_reg_read( ioadm_device_t *dev, ioadm_reg_desc_t *reg );
    int ioadm_reg_write( ioadm_device_t *dev, ioadm_reg_desc_t *reg );
#ifdef __cplusplus
 }
#endif


#endif  /* _CBD_BC_IO_ADM_IOADM_H_ */
