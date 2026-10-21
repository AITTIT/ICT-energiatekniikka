#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  {

    //HANDLE THE COMMAND LINE PARAMETERS
    if (argc != 4) {
        printf("Usage: %s <ip address> <port> <current value (0 = static/1 = changing)>");
        return 1;
    }

    char *ip_address;
    int port, current_value;

    ip_address = argv[1];
    port = atoi(argv[2]);
    current_value = atoi(argv[3]);

    printf("%s %d %d", ip_address, port, current_value);

    return 0;
}