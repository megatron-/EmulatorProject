#include <iostream>

#include "memory.h"
#include "cpu.h"

int main()
{
	bool TripleFault = false;
	int TripleCount = 0;
	Memory MemoryBank;
	Processor CPU(MemoryBank);
	while(CPU.Ready() && !TripleFault)
	{
		try
		{
			CPU.Fetch();
			CPU.Decode();
		}
		catch(const char* e)
		{
			std::cout << "Error: " << e << "\n";
			TripleCount++;
			if(TripleCount >= 3) TripleFault = true;
		}
	}
	if(TripleFault)
		std::cout << "Processor experienced a triple fault - CPU halted\n";
	return 0;
}
