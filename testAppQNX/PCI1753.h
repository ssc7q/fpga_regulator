#ifndef PCI1753_H
#define PCI1753_H

#include <ioadm.h>
#include <port_io_msg.h>

#define	BOARD_1753_OUTPUT_MODE	0
#define	BOARD_1753_INPUT_MODE	1
// то же самое
#define	OUTPUT_MODE_1753	0
#define	INPUT_MODE_1753		1

class PCI1753
{
	ioadm_device_t              board;
	
public:
	enum Regs {A0=0, __B0, C0, Config0, A1, B1, C1, Config1, A2, B2, C2, Config2, A3, B3, C3, Config3 };
	//unsigned char buffer[Config3+1]; // буфер для ВЫВОДА для всех регистров этой платы, чтобы не делать ридбэк
	unsigned char buffer[128];

    int open(unsigned pci_index);
    int open(unsigned pci_index, char *network_name);
    int setDirection(int port, int dir); // 0 - output, 1 - input // передавать A0, C2 ит.д. НЕ Config1
	int readPort(int port);
	int readPin(int port, int pinNum);
	int readPin(int pinNum); // 0-95 - без учета конфигурационных регистров
	int writePort(int port, unsigned char val);
	int writePin(int port, int pinNum, int val);
	int writePin(int pinNum, int val); // 0-95 - без учета конфигурационных регистров
	int close();

	PCI1753(void);
    ~PCI1753(void);
    int writePinDirect(int pinNum, int val);
};
#endif //PCI1753_H
