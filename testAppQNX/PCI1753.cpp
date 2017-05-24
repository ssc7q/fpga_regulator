#include "PCI1753.h"
#include <hw/inout.h>

#define SET_BIT_1(reg, bitNum) reg |= (1<<bitNum)
#define SET_BIT_0(reg, bitNum) reg &= ~(1<<bitNum)
#define SET_BIT(reg1, bitNum1, val) if (val) SET_BIT_1(reg1, bitNum1); else SET_BIT_0(reg1, bitNum1);

PCI1753::PCI1753(void)
{
	for (int i=0; i<= Config3; i++)
	{
		buffer [i] = 0;
	}
}

PCI1753::~PCI1753(void)
{
}

int PCI1753::open(unsigned pci_index)
{
    // подключиться к плате
    //return ioadm_open_pci( &board, "port_io_g", pci_index );
    return ioadm_open_pci( &board, "pci1753_g", pci_index );

}

int PCI1753::open(unsigned pci_index , char* network_name)
{
    // подключиться к плате
    //return ioadm_open_pci( &board, "port_io_g", pci_index );
    return ioadm_open_pci_network( &board, "pci1753_g", pci_index, network_name );

}

int PCI1753::setDirection( int port, int dir )
{
	int configAdr;
	int ret;
	port_io_msg_port_operations_t param;

	configAdr = port - port%4 + 3; // выбор каждого четвертого порта относит. базы
	switch (port % 4)
	{
		case 0: // A
			SET_BIT(buffer[configAdr], 4, dir);
			break;
		case 1: // B
			SET_BIT(buffer[configAdr], 1, dir);
			break;
		case 2: // C
			SET_BIT(buffer[configAdr], 0, dir);
			SET_BIT(buffer[configAdr], 3, dir);
			break;
	}
	param.sample_size_in_bytes = 1;
	param.adrs = configAdr;
	param.data = buffer[configAdr];
	devctl( board.fd, DCMD_IOADM_PORT_WRITE, &param, sizeof( param ), &ret );
	return ret;
}

int PCI1753::readPort( int port )
{
	int ret=0;
	port_io_msg_port_operations_t param;
	param.sample_size_in_bytes = 1;
	param.adrs = port;
	devctl( board.fd, DCMD_IOADM_PORT_READ, &param, sizeof( param ), &ret );
	return param.data;
}

int PCI1753::readPin( int port, int pinNum )
{
	int ret=0;
	port_io_msg_port_operations_t param;
	param.sample_size_in_bytes = 1;
	param.adrs = port;
	devctl( board.fd, DCMD_IOADM_PORT_READ, &param, sizeof( param ), &ret );
	return (param.data & (1<<pinNum))?1:0 ;
}

int PCI1753::readPin( int pinNum )
{
	int ret=0;
	port_io_msg_port_operations_t param;
	param.sample_size_in_bytes = 1;
	param.adrs = pinNum/8 + pinNum/8/3;
	devctl( board.fd, DCMD_IOADM_PORT_READ, &param, sizeof( param ), &ret );
	return (param.data & (1<<(pinNum%8)))?1:0 ;
}

int PCI1753::writePort( int port, unsigned char val )
{
	int ret=0;
	port_io_msg_port_operations_t param;
	//buffer[port] = val;
	buffer[port] = ~val;
	param.sample_size_in_bytes = 1;
	param.adrs = port;
	param.data = buffer[port];
	devctl( board.fd, DCMD_IOADM_PORT_WRITE, &param, sizeof( param ), &ret );
	return ret;
}

int PCI1753::writePin( int port, int pinNum, int val )
{
	int ret=0;
	if (val) val = 0; else val = 1;
	port_io_msg_port_operations_t param;
	//SET_BIT(buffer[port], pinNum, val)
	SET_BIT(buffer[port], pinNum, val)
	param.sample_size_in_bytes = 1;
	param.adrs = port;
	param.data = buffer[port];
	devctl( board.fd, DCMD_IOADM_PORT_WRITE, &param, sizeof( param ), &ret );
	return ret;
}

int PCI1753::writePin( int pinNum, int val )
{
	int ret=0;
	//if (val) val = 0; else val = 1;
	port_io_msg_port_operations_t param;
	param.sample_size_in_bytes = 1;
	param.adrs = pinNum/8 + pinNum/8/3;
	SET_BIT(buffer[param.adrs], pinNum%8, val)
	param.data = buffer[param.adrs];
	devctl( board.fd, DCMD_IOADM_PORT_WRITE, &param, sizeof( param ), &ret );
	return ret;
}

int PCI1753::writePinDirect( int pinNum, int val )
{
    int ret=0;
    //if (val) val = 0; else val = 1;
    port_io_msg_port_operations_t param;
    param.sample_size_in_bytes = 1;
    param.adrs = pinNum/8 + pinNum/8/3;
    SET_BIT(buffer[param.adrs], pinNum%8, val)
    param.data = buffer[param.adrs];
    out8(param.adrs+0x8000, param.data);
    //devctl( board.fd, DCMD_IOADM_PORT_WRITE, &param, sizeof( param ), &ret );
    return ret;
}

int PCI1753::close()
{
	ioadm_close(&board);
	return 0;
}
