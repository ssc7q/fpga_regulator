/*
 * adc1713_msg.h
 *
 */

#include <io-adm-msg.h>

#ifndef PORT_IO_MSG_H_
#define PORT_IO_MSG_H_

/* Types */

    /* Custom message */
    typedef struct port_io_msg
    {
        char                    data[256];              /* Any data */
    } __attribute__ ((__packed__)) port_io_msg_t;


//    /* DCMD_IOADM_PORT_OPERATIONS message */
//    typedef struct port_io_msg_port_operations	// установить параметры одного канала
//    {
//    	unsigned int	sample_size_in_bytes; // что использовать - out8, out16, out32
//     	unsigned int	quantity;
//     	unsigned long	duration;			// сюда возвратить сколько времени выполнялась операция
//     	void*			adrs;             	// адреса портов куда выводить
//     	void*			data;            	// что именно выводить
//     }
//    __attribute__ ((__packed__)) port_io_msg_port_operations_t;
    /* DCMD_IOADM_PORT_OPERATIONS message */
    typedef struct port_io_msg_port_operations	// установить параметры одного канала
    {
    	unsigned int	sample_size_in_bytes; // что использовать - out8, out16, out32
     	// для одной операции не нужно unsigned int	quantity;
     	// тоже не нужноunsigned long	duration;			// сюда возвратить сколько времени выполнялась операция
     	unsigned int	adrs;             	// относительный адреса портов куда выводить
     	unsigned int	data;            	// что именно выводить
     }
    __attribute__ ((__packed__)) port_io_msg_port_operations_t;

#endif /* PORT_IO_MSG_H_ */
