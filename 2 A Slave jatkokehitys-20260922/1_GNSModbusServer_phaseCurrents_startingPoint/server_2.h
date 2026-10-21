#ifndef SERVER_2_H
#define SERVER_2_H

#define DEBUG_MAIN 1
//#define DEBUG_MODBUS_MAPPING 1
#include <stdbool.h>
#include <modbus/modbus.h>

/******************** Globals *************************/
extern int socket;
extern modbus_t *ctxPtr;
extern modbus_mapping_t *mb_mapping;
extern bool staticSignal;

/******************** Globals *************************/
#endif
