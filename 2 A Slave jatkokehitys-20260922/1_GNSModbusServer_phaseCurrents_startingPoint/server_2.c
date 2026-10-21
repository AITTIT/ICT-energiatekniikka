
/*We only need to support Read holding registers, all other messages return an exception*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "server_2.h"
#include "initialize.h"
#include "signalHandlers.h"
#include "createData.h"
#include <stdbool.h>

int socket;
modbus_t *ctxPtr;
modbus_mapping_t *mb_mapping;
bool staticSignal;

// Compile: gcc -Wall *.c -L /usr/lib/x86_64-linux-gnu/ -I /usr/include/modbus/ -lmodbus

// -create signals (current values) which change over time      
// -split the 32 bit values into 16 bit registers               DONE
// -command line parameters                                     DONE
// -install signal handlers for SIGINT and SIGTERM              DONE


// char *argv[] pointteritaulukko, taulukko osoitteita
int main(int argc, char *argv[])  {

    //HANDLE THE COMMAND LINE PARAMETERS
    if (argc != 4) {
        printf("Usage: %s <ip address> <port> <current value (1 = static / 0 = changing)>", argv[0]);
        return 1;
    }

    char *ip_address;
    int port;

    ip_address = argv[1];
    port = atoi(argv[2]);

    // static or alternating values (?) 
    if (1 == atoi(argv[3])) {
        staticSignal = true;
    } else {
        staticSignal = false;
    }
    
    
    int initResult,requestLength;

    #ifdef DEBUG_MAIN
    fprintf(stderr,"\nStart\n");
    #endif
    
    // INSTALL SIGNAL HANDLERS FOR SIGINT AND SIGTERM. I.E. WHEN ONE OF THESE SIGNALS ARRIVES
    // THE signalHandlers FUNCTION IS CALLED
    signal(SIGTERM, signalHandlers);
    signal(SIGINT, signalHandlers);
    

    initResult = initialize(ip_address,port);
    if(initResult ==-1)
    {
        return initResult;
    }

    #ifdef DEBUG_MAIN
    fprintf(stderr,"\n Listening\n");
    #endif

    for(;;)
    {
    
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        createData();
        requestLength = modbus_receive(ctxPtr, query);

       

        if(requestLength >0)
        {
        requestLength= modbus_reply(ctxPtr, query, requestLength, mb_mapping); //Read The Manual and Think!
        }
        if (requestLength ==-1)
        {
        fprintf(stderr,"Modbus reply reason for error: %s\n", modbus_strerror(errno));
        }
        else if (requestLength ==0)
        {
         fprintf(stderr,"Message length ZERO, errorvalue: %s\n", modbus_strerror(errno));
        }
 

        if (requestLength ==-1)
        {
         
           fprintf(stderr,"\nConnection closed unexpectedly\n");
           fprintf(stderr,"Quit the loop: %s\n", modbus_strerror(errno));
           break;
        }
    }

    /****************Free resources*******************/

    modbus_mapping_free(mb_mapping);
    if (-1 == close(socket))
    {
        fprintf(stderr,"\n Error closing socket %s",modbus_strerror(errno));
    } // here the strerror() function would probably do... why?

    modbus_free(ctxPtr);


    return 0;
}//main
