/* io-adm-msg.h */


#include <stdint.h>
#include <devctl.h>


#ifndef _CBD_BC_IO_ADM_MSG_H_
#define _CBD_BC_IO_ADM_MSG_H_


/* Defs */

/* DEVCTL operation extensions */
#define DEVCTL_IO_ADM_CMD_BASE              0
#define DEVCTL_IO_ADM_CMD_RESERVED          0x80
#define DEVCTL_IO_ADM_CMD_IOT( cmd, type )  __DIOT( _DCMD_MISC,                 \
                (DEVCTL_IO_ADM_CMD_BASE + cmd) & (~DEVCTL_IO_ADM_CMD_RESERVED), \
                type )
#define DEVCTL_IO_ADM_CMD_IOF( cmd, type )  __DIOF( _DCMD_MISC,                 \
                (DEVCTL_IO_ADM_CMD_BASE + cmd) & (~DEVCTL_IO_ADM_CMD_RESERVED), \
                type )
#define DEVCTL_IO_ADM_CMD_IOTF( cmd, type )  __DIOTF( _DCMD_MISC,               \
                (DEVCTL_IO_ADM_CMD_BASE + cmd) & (~DEVCTL_IO_ADM_CMD_RESERVED), \
                type )
#define IS_DEVCTL_CMD_RESERVED( cmd )       \
                (cmd & DEVCTL_IO_ADM_CMD_RESERVED)
                
/* io-adm DEVCTL reserved operations (flag DEVCTL_IO_ADM_CMD_RESERVED is set) */
#define DEVCTL_IO_ADM_CMD_ITEST             (DEVCTL_IO_ADM_CMD_IOT( 1, int ) | DEVCTL_IO_ADM_CMD_RESERVED)
#define DEVCTL_IO_ADM_CMD_OTEST             (DEVCTL_IO_ADM_CMD_IOF( 1, int ) | DEVCTL_IO_ADM_CMD_RESERVED)
#define DEVCTL_IO_ADM_CMD_IOTEST            (DEVCTL_IO_ADM_CMD_IOTF( 1, int ) | DEVCTL_IO_ADM_CMD_RESERVED)


#endif  /* _CBD_BC_IO_ADM_MSG_H_ */
