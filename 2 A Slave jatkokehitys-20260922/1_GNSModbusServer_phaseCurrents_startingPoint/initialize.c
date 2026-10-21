
#include <modbus/modbus.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "initialize.h"
#include "createData.h"
#include "server_2.h"

int initialize(const char *addressPtr, int portNumber)
{
  
  int result;


 #ifdef DEBUG_INIT
     fprintf(stderr,"\nInitialize One\n");
    #endif

  mb_mapping = modbus_mapping_new_start_address(
        0,COILS,
        0,DISCRETE_INPUTS,
        26,HOLDING_REGISTERS,
        0,INPUT_REGISTERS
    );


  if (mb_mapping == NULL) {
      fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
      return -1;
  }

  createData();

    #ifdef DEBUG_INIT
     fprintf(stderr,"\nLocal Data ok\n");
    #endif
    ctxPtr = modbus_new_tcp(addressPtr, portNumber);  //should be taken from command line parameter
    if (ctxPtr == NULL)
    {
        fprintf(stderr,"Unable to allocate libmodbus context,%s\n",modbus_strerror(errno));
        return -1;
    }
#ifdef DEBUG_INIT
     fprintf(stderr,"\nContext ok\n");
#endif
    socket = modbus_tcp_listen(ctxPtr,1);
    if(socket ==-1)
    {
        fprintf(stderr,"Unable to open socket: %s",modbus_strerror(errno));
        modbus_free(ctxPtr);
        return -1;
    }
#ifdef DEBUG_INIT
    fprintf(stderr,"\nSocket ok\n");
#endif
    result = modbus_tcp_accept(ctxPtr, &socket);
    if (result == -1)
    {
        fprintf(stderr,"Could not accept connection: %s\n",modbus_strerror(errno));
        close(socket);
        modbus_free(ctxPtr);
        return -1;
    }
#ifdef DEBUG_INIT
    fprintf(stderr,"\nTCP-handshake complete\n");
#endif

return 0;  //all ok if we got this far
}
