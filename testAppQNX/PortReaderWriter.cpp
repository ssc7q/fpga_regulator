#include "PortReaderWriter.h"

#ifdef WIN
#include "portdrv.h"
#endif

#ifdef QNX
#include <hw/inout.h>
#include <sys/neutrino.h>
#endif



#ifdef WIN
unsigned char PortReaderWriter::readPort(unsigned adr)
{
    return portdrv_inp(adr);
}

void PortReaderWriter::writePort(unsigned adr, unsigned char val)
{
    portdrv_outp(adr, val);
}

PortReaderWriter::PortReaderWriter()
{
    portdrv_StartWork();
}

PortReaderWriter::~PortReaderWriter()
{
    portdrv_CompleteWork();

}
#endif

#ifdef QNX

unsigned char PortReaderWriter::readPort(unsigned adr)
{
	return in8(adr);
}

void PortReaderWriter::writePort(unsigned adr, unsigned char val)
{
	out8(adr, val);
}

PortReaderWriter::PortReaderWriter()
{
    ThreadCtl(_NTO_TCTL_IO, 0);
}

PortReaderWriter::~PortReaderWriter()
{
}

#endif
