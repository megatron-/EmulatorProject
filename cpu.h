#ifndef CPU_H
#define CPU_H

#include <ostream>
#include <iomanip>

#include "memory.h"

typedef unsigned char REG; 	// 8 bit registers

enum OPCODES
{
	HALT = 0,
	LOAD_AX, LOAD_AXBX, LOAD_AXSP, LOAD_AXBP, 		// 0x01 - 0x04
	LOAD_BX, LOAD_BXAX, LOAD_BXSP, LOAD_BXBP,		// 0x05 - 0x08
	LOAD_SPAX, LOAD_SPBX, LOAD_BPAX, LOAD_BPBX,		// 0x09 - 0x0C
	PUSH_AX, PUSH_BX, PUSH_REG, PUSH_FLAG, PUSH_ALL,// 0x0D - 0x11
	POP_AX, POP_BX, POP_REG, POP_FLAG, POP_ALL,		// 0x12 - 0x16
	JUMP_ADDR, JUMP_CRRY, JUMP_EQU, JUMP_NEQU,		// 0x17 - 0x1A
	JUMP_GRTR, JUMP_LESS, JUMP_GEQU, JUMP_LEQU		// 0x1B - 0x1E
};

class Processor
{
private:
	Memory* MB; // Memory bus
	Memory* L;  // Left memory bank ( Default )
	Memory* R;  // Right memory bank

	REG IP; /* Instruction pointer */ REG IR; // Instruction register	
	REG SP; /* Stack pointer       */ REG BP; // Base pointer

	// 2 general purpose 8bit registers
	REG AX, BX;

	// 8-Bit Flag register
	REG FLAGS;
public:
	Processor(Memory& m): // Only implemented address 1 module
		MB(&m), L(&m), R(0), IP(0), IR(0), SP(0), BP(0),
		AX(0), BX(0), FLAGS(0) {}

	void Fetch() { IR = MB->Read(IP); } // Read instruction into register
	void Decode(); // Decode declaration

	bool Ready() { return static_cast<bool>(!FLAGS&0x01); }

	void Halt(); 	// 0x00

	void LD_AX();	// 0x01
	void LD_AXBX();	// 0x02
	void LD_AXSP();	// 0x03
	void LD_AXBP();	// 0x04

	void LD_BX();	// 0x05
	void LD_BXAX();	// 0x06
	void LD_BXSP();	// 0x07
	void LD_BXBP();	// 0x08

	void LD_SPAX();	// 0x09
	void LD_SPBX();	// 0x0A
	void LD_BPAX();	// 0x0B
	void LD_BPBX();	// 0x0C

	void PH_AX();	// 0x0D
	void PH_BX();	// 0x0E
	void PH_REG();	// 0x0F
	void PH_FLAG();	// 0x10
	void PH_ALL();	// 0x11

	void PP_AX();	// 0x12
	void PP_BX();	// 0x13
	void PP_REG();	// 0x14
	void PP_FLAG();	// 0x15
	void PP_ALL();	// 0x16

	void JMP_ADDR();// 0x17
	void JMP_CRRY();// 0x18
	void JMP_EQU();	// 0x19
	void JMP_NEQ();	// 0x1A
	void JMP_GRT(); // 0x1B
	void JMP_LES();	// 0x1C
	void JMP_GEQ();	// 0x1D
	void JMP_LEQ();	// 0x1E

	friend std::ostream& operator<< (std::ostream& os, Processor& p)
	{
		os <<  "[AX] " << std::dec << std::right << std::setw(3) << std::setfill(' ') << (int)p.AX
		   << " [BX] " << std::right << std::setw(3) << std::setfill(' ') << (int)p.BX
		   << " [IP] " << std::right << std::setw(3) << std::setfill(' ') << (int)p.IP
		   << " [IR] " << std::right << std::setw(4) << std::showbase << std::internal << std::hex << std::setfill('0') << (int)p.IR
		   << " [SP] " << std::dec << std::right << std::setw(3) << std::setfill(' ') << (int)p.SP
		   << " [BP] " << std::right << std::setw(3) << std::setfill(' ') << (int)p.BP << "\t"
		   << static_cast<bool>(p.FLAGS&0x80) << static_cast<bool>(p.FLAGS&0x40) // Masks to find flags
		   << static_cast<bool>(p.FLAGS&0x20) << static_cast<bool>(p.FLAGS&0x10)
		   << static_cast<bool>(p.FLAGS&0x08) << static_cast<bool>(p.FLAGS&0x04)
		   << static_cast<bool>(p.FLAGS&0x02) << static_cast<bool>(p.FLAGS&0x01)
		   << "\n";
		 return os;
	}
};

void Processor::Decode()
{
	switch(IR)
	{
	case HALT: 		Halt(); 	break;
	case LOAD_AX:	LD_AX(); 	break;
	case LOAD_AXBX:	LD_AXBX(); 	break;
	case LOAD_AXSP:	LD_AXSP(); 	break;
	case LOAD_AXBP:	LD_AXBP(); 	break;
	case LOAD_BX:	LD_BX();	break;
	case LOAD_BXAX: LD_BXAX();	break;
	case LOAD_BXSP:	LD_BXSP();	break;
	case LOAD_BXBP: LD_BXBP();	break;
	case LOAD_SPAX:	LD_SPAX();	break;
	case LOAD_SPBX: LD_SPBX();	break;
	case LOAD_BPAX:	LD_BPAX();	break;
	case LOAD_BPBX: LD_BPBX();	break;
	case PUSH_AX:   PH_AX();	break;
	case PUSH_BX:   PH_BX();	break;
	case PUSH_REG:  PH_REG();	break;
	case PUSH_FLAG: PH_FLAG();	break;
	case PUSH_ALL:	PH_ALL();	break;
	case POP_AX:	PP_AX();	break;
	case POP_BX:	PP_BX();	break;
	case POP_REG:	PP_REG();	break;
	case POP_FLAG:	PP_FLAG();	break;
	case POP_ALL:	PP_ALL();	break;
	case JUMP_ADDR:	JMP_ADDR();	break;
	case JUMP_CRRY: JMP_CRRY();	break;
	case JUMP_EQU:  JMP_EQU();	break;
	case JUMP_NEQU: JMP_NEQ();	break;
	case JUMP_GRTR: JMP_GRT();	break;
	case JUMP_LESS: JMP_LES();	break;
	case JUMP_GEQU: JMP_GEQ();	break;
	case JUMP_LEQU:	JMP_NEQ();	break;
	default: Halt(); break;
	}
	if(Ready()) IP++; // Point to next instruction
}

void Processor::Halt() 	  { FLAGS |= 0x01; } 		// Set Halt flag
void Processor::LD_AX()   { AX = MB->Read(++IP); } // Reads value after instruction
void Processor::LD_AXBX() { AX = BX; } 			  // Simple
void Processor::LD_AXSP() { AX = SP; }
void Processor::LD_AXBP() { AX = BP; }
void Processor::LD_BX()   { BX = MB->Read(++IP); }
void Processor::LD_BXAX() { BX = AX; }
void Processor::LD_BXSP() { BX = SP; }
void Processor::LD_BXBP() { BX = BP; }
void Processor::LD_SPAX() { SP = AX; }
void Processor::LD_SPBX() { SP = BX; }
void Processor::LD_BPAX() { BP = AX; }
void Processor::LD_BPBX() { BP = BX; }
void Processor::PH_AX()	  { MB->Write(SP--,AX);		} // Stack grows downwards	
void Processor::PH_BX()	  { MB->Write(SP--,BX); 	} // Decrement after value
void Processor::PH_REG()  { PH_AX(); PH_BX(); 		} // has been loaded
void Processor::PH_FLAG() { MB->Write(SP--,FLAGS); 	}
void Processor::PH_ALL()  { PH_REG(); PH_FLAG(); 	}
void Processor::PP_AX()   { AX = MB->Read(++SP); 	} // Popping is reverse
void Processor::PP_BX()   { BX = MB->Read(++SP); 	} // Increment before
void Processor::PP_REG()  { PP_BX(); PP_AX(); 		}
void Processor::PP_FLAG() { FLAGS = MB->Read(++SP); }
void Processor::PP_ALL()  { PP_FLAG(); PP_REG(); 	} // And in reverse order
void Processor::JMP_ADDR(){ IP = (MB->Read(++IP)-1);} // JUMP to ADDR
void Processor::JMP_CRRY(){ if(static_cast<bool>(FLAGS&0x10)) IP = (MB->Read(++IP)-1); else ++IP; }	// JUMP if CF
void Processor::JMP_EQU() { if(AX == BX) JMP_ADDR(); else ++IP; }	// JUMP if AX == BX
void Processor::JMP_NEQ() { if(AX != BX) JMP_ADDR(); else ++IP; }	// JUMP if AX != BX
void Processor::JMP_GRT() { if(AX > BX)  JMP_ADDR(); else ++IP; } 	// JUMP if AX > BX
void Processor::JMP_LES() { if(AX < BX)  JMP_ADDR(); else ++IP; }	// JUMP if AX < BX
void Processor::JMP_GEQ() { if(AX >= BX) JMP_ADDR(); else ++IP; }	// JUMP if AX >= BX
void Processor::JMP_LEQ() { if(AX <= BX) JMP_ADDR(); else ++IP; }	// JUMP if AX <= BX

#endif
