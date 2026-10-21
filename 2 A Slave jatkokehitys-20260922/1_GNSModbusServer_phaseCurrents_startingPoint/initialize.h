#ifndef INITIALIZE_H
#define INITIALIZE_H

#define DEBUG_INIT 1

#define DISCRETE_INPUTS  5 //Read-only
#define COILS 10            //Read-Write
#define INPUT_REGISTERS 15   //Read-only
#define HOLDING_REGISTERS 6 //Read-Write

int initialize(const char *addressPtr, int portNumber);

#endif
