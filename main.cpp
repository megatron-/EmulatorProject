#include <iostream>
#include <fstream>

#include "memory.h"
#include "cpu.h"

void ClearMemory(Memory* m)
{
	for(int i = 0; i < 256; i++) m->Write(i,0); // Clear memory
}

void Program_V01(Memory* m)
{
	// Setting up the stack
	m->Write(0,0x05); // Load BX with value
	m->Write(1,0xFF); // of 255
	m->Write(2,0x0A); // and set the stack pointer
	m->Write(3,0x01); // Load AX with value
	m->Write(4,0xDF); // of 223 ( 32 byte stack - 1/8th of memory )
	m->Write(5,0x0B); // and set the base pointer

	/* From this point we have a 32 byte stack - Load functions 100% */


	/***************** Testing stack dynamics ************************/

	// Push registers and flags on to the stack - (3 bytes/SP = 232)
	m->Write(6,0x11);

	// ROUTINE: Swap AX,BX
	m->Write(7,0x0D); // Push AX on the stack   - (4 bytes/SP = 231)
	m->Write(8,0x02); // Load AX with BX
	m->Write(9,0x13); // Pop stack into BX      - (3 bytes/SP = 232)

	// Pop flags and registers from the stack   - (0 bytes/SP = 255)
	m->Write(10,0x16);

	/* Stack works correctly, though no memory protection yet (75%) */

	m->Write(11,0x1C); // Jump if AX < BX
	m->Write(12,0x20); // To address

	// Check if jump worked
	m->Write(32,0x01); // Load AX
	m->Write(33,0x80); // With value 128
}

void WriteRom(Memory* m)
{
	std::ofstream ofs("ROM.bin", std::ios::binary);

	for(int i = 0; i < 256; i++) ofs << m->Read(i);
	ofs.close();
}

void LoadRom(Memory* m) // Loads Program ROM into memory
{
	std::ifstream ifs("ROM.bin", std::ios::binary);
	if(ifs)
	{
		std::streampos begin, end;
		begin = ifs.tellg();
  		ifs.seekg (0, std::ios::end);
  		end = ifs.tellg();
  		std::cout << "ROM located: " << (end-begin) << " bytes.\n";
  		ifs.seekg(0, std::ios::beg);
  		for(int i = 0; i < end-begin; i++)
  		{
  			m->Write(i,static_cast<unsigned char>(ifs.get()));
  		}
	}
	else std::cout << "Cannot find ROM.bin file\n";
	ifs.close();
}

int main()
{
	Memory RAMLEFT;

	ClearMemory(&RAMLEFT); 	// Init mem locations to 0
	//Program_V01(&RAMLEFT);// Writes instructions directly to memory
	//WriteRom(&RAMLEFT); 	// Saves memory to file
	LoadRom(&RAMLEFT);	  	// Loads file into memory
	
	Processor MyProcessor(RAMLEFT);
	while(MyProcessor.Ready())
	{
		MyProcessor.Fetch();
		MyProcessor.Decode();
	}
	std::cout << "Processor halted -\n" << MyProcessor;
	return 0;
}
