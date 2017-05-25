#ifndef PORTREADERWRITER_H
#define PORTREADERWRITER_H

class PortReaderWriter
{
public:
	unsigned char readPort(unsigned adr);
	void writePort(unsigned adr, unsigned char val);
	PortReaderWriter();
	~PortReaderWriter();
};

#endif // PORTREADERWRITER_H
