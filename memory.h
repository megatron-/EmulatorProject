#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned char BYTE; // unsigned char can be anything, better define it

const int Amount = 256; // Amount of RAM ( In bytes )

class Memory
{ 
private:
	BYTE Address[Amount]; // The RAM
public:
	Memory() {}
	~Memory() {}
	BYTE& Read(BYTE loc) { return Address[loc]; } // Read operation
	void Write(BYTE addr, BYTE val) { Address[addr] = val; } // Write operation
};

#endif
