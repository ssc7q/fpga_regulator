/* lib.c */


#include <ioadm.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>


/* Functions */

/*
 * Prepare I/O operations
 *
 * Out: 0  - ok
 *      -1 - unable to init any I/O operation
 *      -2 - device not opened
 */
int ioadm_lib_prepare_io( ioadm_device_t *dev )
{
    int                     ret = 0;
    char                    shm_object_name[40] = "";

    if ( dev->fd == -1 )
        return (-2);

    /* Ask for driver's mode */
    if ( devctl( dev->fd, DCMD_IOADM_GET_BUFF_MODE, NULL, 0, &ret ) != EOK )
        return (-1);

    switch ( ret & 0x00ffff ) {

        case LIBMODE_SHARED_MEMORY: {
            int             io_permissions = 0;

            /* Get shared object name */
            if ( devctl( dev->fd, DCMD_IOADM_GET_SHMOBJECT, shm_object_name, sizeof ( shm_object_name ), NULL ) != EOK )
                return (-1);

            /* Open shared object */
            if ( ret & LIBMODE_BUFPERM_W )
                dev->shm_object = shm_open( shm_object_name, O_RDWR, 0777 );
            else
                dev->shm_object = shm_open( shm_object_name, O_RDONLY, 0777 );
            if ( dev->shm_object == -1 )
                return (-1);

            /* Get shared object size */
            if ( devctl( dev->fd, DCMD_IOADM_GET_SHMOBJECT_SZ, NULL, 0, &dev->shm_object_sz ) != EOK ) {
                close( dev->shm_object );
                dev->shm_object = -1;
                return (-1);
            }

            /* mmap() shared object */
            if ( ret & LIBMODE_BUFPERM_W )
                io_permissions |= PROT_WRITE;
            if ( ret & LIBMODE_BUFPERM_R )
                io_permissions |= PROT_READ;
            dev->shm_buffer = (void *)mmap( 0, dev->shm_object_sz, io_permissions,
                                       MAP_SHARED, dev->shm_object, 0 );

            if ( dev->shm_buffer == MAP_FAILED ) {
                close( dev->shm_object );
                dev->shm_object = -1;
                dev->shm_buffer = NULL;
                return (-1);
            }

            dev->lib_mode = ret;
            break;
        }

        case LIBMODE_SIMPLE: {
            dev->lib_mode = ret;
            break;
        }

        default:
            return (-1);

    }

    return (0);
}

/*
 * Get library mode
 *
 * Out: Library mode
 */
int ioadm_lib_get_mode( ioadm_device_t *dev )
{
    return (dev->lib_mode & 0x00ffff);
}

/*
 * Get io permissions
 *
 * Out: 0 - device not opened or I/O operations not permitted
 *      1 - R
 *      2 - W
 *      3 - RW
 */
int ioadm_lib_get_io_permissions( ioadm_device_t *dev )
{
    if ( dev->lib_mode & LIBMODE_BUFPERM_R )
        return (1);

    if ( dev->lib_mode & LIBMODE_BUFPERM_W )
        return (2);

    if ( dev->lib_mode & LIBMODE_BUFPERM_W &&
         dev->lib_mode & LIBMODE_BUFPERM_R )
        return (3);

    return (0);
}

/*
 * Send custom devctl()
 *
 * In: Devctl command
 *     Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_custom_dcmd( ioadm_device_t *dev, int dcmd, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, dcmd, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Open io-adm device
 *
 * In: Device name ("l783m0")
 * Out: 0 - ok
 *      1 - open() failed
 *      2 - device already opened
 */
int ioadm_open( ioadm_device_t *dev, char *device )
{
    char                    dev_[100] = "";

    dev->fd             = -1;
    dev->shm_object     = -1;
    dev->shm_buffer     = NULL;
    dev->shm_object_sz  = 0;
    dev->lib_mode       = LIBMODE_UNLOAD;

    if ( dev->fd != -1 )
        return (2);

    sprintf( dev_, "/dev/io-adm/%s", device );
    if ( (dev->fd = open( dev_, O_RDWR )) == -1 )
        return (1);

    return (0);
}

/*
 * Open io-adm device
 *
 * In: Device name ("l783m")
 *     PCI index of the device (0)
 * Out: 0 - ok
 *      1 - open() failed
 *      2 - device already opened
 */
int ioadm_open_pci( ioadm_device_t *dev, char *device, int pci_index )
{
    char                    dev_[100] = "";

    dev->fd             = -1;
    dev->shm_object     = -1;
    dev->shm_buffer     = NULL;
    dev->shm_object_sz  = 0;
    dev->lib_mode       = LIBMODE_UNLOAD;

    if ( dev->fd != -1 )
        return (2);

    sprintf( dev_, "/dev/io-adm/%s%i", device, pci_index );
    if ( (dev->fd = open( dev_, O_RDWR )) == -1 )
        return (1);

    return (0);
}

int ioadm_open_pci_network( ioadm_device_t *dev, char *device, int pci_index, char *network_name)
{
    char                    dev_[100] = "";

    dev->fd             = -1;
    dev->shm_object     = -1;
    dev->shm_buffer     = NULL;
    dev->shm_object_sz  = 0;
    dev->lib_mode       = LIBMODE_UNLOAD;

    if ( dev->fd != -1 )
        return (2);

    sprintf( dev_, "%s/dev/io-adm/%s%i", network_name, device, pci_index );
    if ( (dev->fd = open( dev_, O_RDWR )) == -1 )
        return (1);

    return (0);
}



/*
 * Close io-adm device
 */
void ioadm_close( ioadm_device_t *dev )
{
    if ( dev->shm_buffer )
        munmap( dev->shm_buffer, dev->shm_object_sz );
    if ( dev->shm_object != -1 )
        close( dev->shm_object );

    close( dev->fd );

    dev->fd = -1;
    dev->shm_object = -1;
    dev->shm_buffer = NULL;
    dev->shm_object_sz = 0;
    dev->lib_mode = LIBMODE_UNLOAD;
}

/*
 * Set IRQ sources
 *
 * In: IRQ sources to be activated (each bit - IRQ#i source: see driver's public headers)
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -3 - Device is started
 */
int ioadm_set_irq_sources( ioadm_device_t *dev, uint64_t irqs )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_IRQ_SOURCE, &irqs, sizeof( irqs ), &ret ) != EOK )
        return (-2);

    if ( ret == 1)
        return (-3);

    return (ret);
}

/*
 * Set IRQ polarity
 *
 * In: IRQ polarity mask to be applied (each bit - IRQ#i polarity: see driver's public headers)
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -3 - Device is started
 */
int ioadm_set_irq_polarity( ioadm_device_t *dev, uint64_t irqp )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_IRQ_POLARITY, &irqp, sizeof( irqp ), &ret ) != EOK )
        return (-2);

    if ( ret == 1)
        return (-3);

    return (ret);
}

/*
 * Waiting for the IRQ event
 *
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -3 - Device is not started
 */
int ioadm_wait_irq_event( ioadm_device_t *dev )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_WAIT_IRQ_EV, &ret, sizeof( ret ), &ret ) != EOK )
        return (-2);

    if ( ret == 1)
        return (-3);

    return (ret);
}

/*
 * Set channel numbers
 *
 * In: Number of channels
 * Out: 0  - ok
 *      1  - device not opened
 *      2  - devctl() error
 *      3  - num > max_channels
 *      >3 - internal driver's status
 */
int ioadm_set_channels_number( ioadm_device_t *dev, int num )
{
    if ( dev->fd == -1 )
        return (1);

    if ( devctl( dev->fd, DCMD_IOADM_CHANNELS, &num, sizeof( num ), &num ) != EOK )
        return (2);

    if ( num == 1 )
        return (3);
    if ( num > 1 )
        return ( num );

    return (0);
}

/*
 * Set buffer
 *
 * In: Devctl command
 *     Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_set_buff( ioadm_device_t *dev, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_IN_BUFF, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Set frame rate
 *
 * In: Frame rate
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_set_frame_rate( ioadm_device_t *dev, int rate )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_FRATE, &rate, sizeof( rate ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Set timing params
 *
 * In: Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_set_timing( ioadm_device_t *dev, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_TIMING, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Set sync params
 *
 * In: Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_set_sync( ioadm_device_t *dev, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_SYNC, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Start the device (Enable device, enable IRQ, ...)
 *
 * In: Param
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_start( ioadm_device_t *dev, int param )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_START, &param, sizeof( param ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Stop the device (Disable device, disable IRQ, ...)
 *
 * In: Param
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_stop( ioadm_device_t *dev, int param )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_STOP, &param, sizeof( param ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Pause the device
 *
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_pause( ioadm_device_t *dev )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_PAUSE, &ret, sizeof( ret ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Resume the device
 *
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_resume( ioadm_device_t *dev )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_RESUME, &ret, sizeof( ret ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Set calibration multipliers
 *
 * In: Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_set_calibration_multipliers( ioadm_device_t *dev, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_CALIBRATION_MUL, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Set calibration multipliers
 *
 * In: Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_enable_calibration( ioadm_device_t *dev, int enable )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_ENABLE_CALIBRATION, &enable, sizeof( enable ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Get sample ranges
 *
 * In: Message ptr
 *     Message size
 * Out: 0  - ok
 *      -1 - device not opened
 *      -2 - devctl() error
 *      n  - devctl() status
 */
int ioadm_get_sample_ranges( ioadm_device_t *dev, void *msg, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_SAMPLE_RANGES, msg, sz, &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Get time between 2 interrupts (in sec)
 *
 * Out: n    - Interrupt time (in sec)
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
double ioadm_get_b2int_time( ioadm_device_t *dev )
{
    int                     ret = 0;
    double                  tm  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_INT_TIME, &tm, sizeof( tm ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return tm;
}

/*
 * Get read time (in sec)
 *
 * Out: n    - time (in sec)
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
double ioadm_get_read_time( ioadm_device_t *dev )
{
    int                     ret = 0;
    double                  tm  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_RD_TIME, &tm, sizeof( tm ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return tm;
}

/*
 * Get read-back time (in sec)
 *
 * Out: n    - time (in sec)
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
double ioadm_get_readback_time( ioadm_device_t *dev )
{
    int                     ret = 0;
    double                  tm  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_RDBACK_TIME, &tm, sizeof( tm ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return tm;
}

/*
 * Get write time (in sec)
 *
 * Out: n    - time (in sec)
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
double ioadm_get_write_time( ioadm_device_t *dev )
{
    int                     ret = 0;
    double                  tm  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_WR_TIME, &tm, sizeof( tm ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return tm;
}

/*
 * Get device state
 *
 * Out: n    - state
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
unsigned int ioadm_get_state( ioadm_device_t *dev )
{
    int                     ret   = 0;
    unsigned int            state = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_STATE, &state, sizeof( state ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return state;
}

/*
 * Get received from device data size
 *
 * Out: n    - size in bytes
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
unsigned long long ioadm_get_received_data_sz( ioadm_device_t *dev )
{
    int                     ret = 0;
    unsigned long long      sz  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_RECEIVED_DATA_SZ, &sz, sizeof( sz ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return sz;
}

/*
 * Get readed from driver data size
 *
 * Out: n    - size in bytes
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
unsigned long long ioadm_get_readed_data_sz( ioadm_device_t *dev )
{
    int                     ret = 0;
    unsigned long long      sz  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_READED_DATA_SZ, &sz, sizeof( sz ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return sz;
}

/*
 * Get available data size
 *
 * Out: n    - size in bytes
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
unsigned int ioadm_get_available_data_sz( ioadm_device_t *dev )
{
    int                     ret = 0;
    unsigned long           sz  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_AVAIL_DATA_SZ, &sz, sizeof( sz ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return sz;
}

/*
 * Get last recive data time (in ClockCycles())
 *
 * Out: n    - size in bytes
 *      -1   - device not opened
 *      -2   - devctl() error
 *      -n-2 - -(devctl() status + 2)
 */
unsigned long long ioadm_get_last_receive_time( ioadm_device_t *dev )
{
    int                     ret = 0;
    unsigned long           sz  = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_LAST_RECV_TIME, &sz, sizeof( sz ), &ret ) != EOK )
        return (-2);

    if ( ret )
        return (-ret - 2);

    return sz;
}

/*
 * Clear driver's statistic
 *
 * Out:  0   - Success
 *      -1   - device not opened
 *      -2   - devctl() error
 */
int ioadm_clear_statistic( ioadm_device_t *dev )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_CLEAR_STATISTIC, &ret, sizeof( ret ), &ret ) != EOK )
        return (-2);

    return (ret);
}

/*
 * Read from the device
 *
 * In: Buffer ptr
 *     Buffer size (should be >= 4b if LIBMODE_SHARED_MEMORY)
 * Out: -1 - device not opened
 *      -2 - read() error
 *      -3 - permission denied
 *      n  - number of readed bytes
 */
int ioadm_read( ioadm_device_t *dev, void *buffer, int sz )
{
    if ( dev->fd == -1 )
        return (-1);

    if ( dev->lib_mode & LIBMODE_BUFPERM_R == 0 )
        return (-3);

    switch ( dev->lib_mode & 0x00ffff ) {

        case LIBMODE_SHARED_MEMORY: {
            uint32_t        *offset  = (uint32_t *)buffer;
            uint8_t         *shm_buf = (uint8_t *)dev->shm_buffer;

            if ( (sz = read( dev->fd, buffer, sz )) == -1 )
                return (-2);
            memcpy( buffer, (void *)&shm_buf[*offset], sz );
            break;
        }

        case LIBMODE_SIMPLE: {
            if ( (sz = read( dev->fd, buffer, sz )) == -1 )
                return (-2);
            break;
        }

        default:
            return (-2);

    }

    return (sz);
}

/*
 * Read data back from the device (Read previously written data)
 *
 * In: Register
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -n - -devctl() status - 3
 */
int ioadm_readback( ioadm_device_t *dev, void *buffer, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_READ_BACK, buffer, sz, &ret ) != EOK )
        return (-2);

    if ( ret < 0 )
        return ( -ret - 3 );

    return (0);
}

/*
 * Write to the device
 *
 * In: Buffer ptr
 *     Buffer size
 * Out: -1 - device not opened
 *      -2 - read() error
 *      -3 - permission denied
 *      n  - number of written bytes
 */
int ioadm_write( ioadm_device_t *dev, void *buffer, int sz )
{
    if ( dev->fd == -1 )
        return (-1);

    if ( dev->lib_mode & LIBMODE_BUFPERM_R == 0 )
        return (-3);

    switch ( dev->lib_mode & 0x00ffff ) {

        case LIBMODE_SHARED_MEMORY: {
            return (-2);
        }

        case LIBMODE_SIMPLE: {
            if ( (sz = write( dev->fd, buffer, sz )) == -1 )
                return (-2);
            break;
        }

        default:
            return (-2);

    }

    return (sz);
}

/*
 * Read 1 fragment from device via shared memory
 *
 * In: Pointer-to-pointer
 * Out: -1 - device not opened
 *      -2 - read() error
 *      -3 - permission denied
 *      -4 - operation not supported (use ioadm_read())
 *      n  - number of readed bytes
 */
int ioadm_read_fragment_wc( ioadm_device_t *dev, void **ptr )
{
    uint32_t                offset;
    uint8_t                 *shm_buf = (uint8_t *)dev->shm_buffer;
    int                     sz;

    if ( dev->fd == -1 )
        return (-1);

    if ( dev->lib_mode & LIBMODE_BUFPERM_R == 0 )
        return (-3);

    switch ( dev->lib_mode & 0x00ffff ) {

        case LIBMODE_SHARED_MEMORY: {
            if ( devctl( dev->fd, DCMD_IOADM_GET_SHMOBJECT_RD, &offset, sizeof( offset ), &sz ) != EOK )
                return (-2);

            if ( offset + sz > dev->shm_object_sz )
                sz = ( dev->shm_object_sz - offset );

            *ptr = (uint32_t *)&shm_buf[offset];
            break;
        }

        case LIBMODE_SIMPLE: {
            return (-4);
            break;
        }

        default:
            return (-4);

    }

    return (sz);
}

/*
 * Get channel sample
 *
 * In: Channel
 * Out: n  - sample
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -3 - num > max_channels
 *      -n - -devctl() status
 */
int ioadm_get_sample( ioadm_device_t *dev, void *channel, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_GET_SAMPLE, channel, sz, &ret ) != EOK )
        return (-2);

    if ( ret == 1 )
        return (-3);
    if ( ret > 1 )
        return ( -ret );

    if ( sz >= sizeof( int ) )
        return (*(int *)channel);
    else {
        int     tmp = 0;

        if ( sz == 1 )
            tmp = (int)*(uint8_t *)channel;
        else if ( sz == 2 )
            tmp = (int)*(uint16_t *)channel;
        else
            return (*(int *)channel);
        return tmp;
    }
}

/*
 * Set channel sample
 *
 * In: Channel
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -3 - num > max_channels
 *      -n - -devctl() status
 */
int ioadm_set_sample( ioadm_device_t *dev, void *channel, int sz )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_SET_SAMPLE, channel, sz, &ret ) != EOK )
        return (-2);

    if ( ret == 1 )
        return (-3);
    if ( ret > 1 )
        return ( -ret );

    return (0);
}

/*
 * Read register
 *
 * In: Register
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -n - -devctl() status - 3
 */
int ioadm_reg_read( ioadm_device_t *dev, ioadm_reg_desc_t *reg )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_READ_REG, reg, sizeof( ioadm_reg_desc_t ), &ret ) != EOK )
        return (-2);

    if ( ret < 0 )
        return ( -ret - 3 );

    return (0);
}

/*
 * Write register
 *
 * In: Register
 * Out:  0 - success
 *      -1 - device not opened
 *      -2 - devctl() error
 *      -n - -devctl() status - 3
 */
int ioadm_reg_write( ioadm_device_t *dev, ioadm_reg_desc_t *reg )
{
    int                     ret = 0;

    if ( dev->fd == -1 )
        return (-1);

    if ( devctl( dev->fd, DCMD_IOADM_WRITE_REG, reg, sizeof( ioadm_reg_desc_t ), &ret ) != EOK )
        return (-2);

    if ( ret < 0 )
        return ( -ret - 3 );

    return (0);
}
