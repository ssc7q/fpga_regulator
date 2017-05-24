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
//    typedef struct port_io_msg_port_operations	// ���������� ��������� ������ ������
//    {
//    	unsigned int	sample_size_in_bytes; // ��� ������������ - out8, out16, out32
//     	unsigned int	quantity;
//     	unsigned long	duration;			// ���� ���������� ������� ������� ����������� ��������
//     	void*			adrs;             	// ������ ������ ���� ��������
//     	void*			data;            	// ��� ������ ��������
//     }
//    __attribute__ ((__packed__)) port_io_msg_port_operations_t;
    /* DCMD_IOADM_PORT_OPERATIONS message */
    typedef struct port_io_msg_port_operations	// ���������� ��������� ������ ������
    {
    	unsigned int	sample_size_in_bytes; // ��� ������������ - out8, out16, out32
     	// ��� ����� �������� �� ����� unsigned int	quantity;
     	// ���� �� �����unsigned long	duration;			// ���� ���������� ������� ������� ����������� ��������
     	unsigned int	adrs;             	// ������������� ������ ������ ���� ��������
     	unsigned int	data;            	// ��� ������ ��������
     }
    __attribute__ ((__packed__)) port_io_msg_port_operations_t;

#endif /* PORT_IO_MSG_H_ */
