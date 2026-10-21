#include "signalHandlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>

void signalHandlers(int signum)
{
extern modbus_t *ctxPtr;
if (ctxPtr != NULL) {
        modbus_close(ctxPtr);
        modbus_free(ctxPtr);
    }
    printf("\nExiting due to signal %d\n", signum);
    exit(0);
}
