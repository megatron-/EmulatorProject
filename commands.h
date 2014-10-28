#ifndef COMMANDS_H_
#define COMMANDS_H_

//**********INCLUDES***************
#include "cpu.h"
#include "memory.h"
#include <string>


//*********using these, so we don't have to continuously type them**************
using std::string;




//*************COMMAND IDs****************
// These are really just ideas of mine. There will be more
#define CMD_RESTART         3501
#define CMD_EXIT            3502
#define CMD_TERMINATE       3503
#define CMD_ZEROMEMORY      3504




//*************ERROR IDs*******************
#define ERR_INVALIDCOMMAND  7001
#define ERR_COMMANDFAILED   7002
#define ERR_PROCESSFAILED   7003


// class "INPUT" definition:
class INPUT{
public:
    INPUT();                
  
    string WriteCommand();  // Function made to allow user to enter a command, then returns the user's input
    
    int ProcessCommand();  // Gets command from WriteCommand(), then processes it for a CPU function (think I worded that right)
                            // Returns error ID if applicable
                            
    int ProcessCommand(int CommandID);  // Overloaded function of "ProcessCommand". It processes a predetermined command
    
protected:
    int errorID;    // If a function fails, store the error ID in this integer, so if the user wants it, he can get it.
    
}

#endif
