This modbus slave should:

-create and store Current values in Holding registers 26 - 31 according to the Arcteq mapping file. Each current value is a 32 bit float mapped to two 16 bit Modbus registers.

-the slave shall have three command line parameters:

-the IP address to it listens on
-the TCP port number it listens on
-the third parameter indicates if the current values are static (1) or changing (0).

Example: ./a.out 127.0.0.1 502 1

The provided code lacks functionality and has one fundamental error.

The error: Input registers are used instead of holding registers. Fix this

Functionality to add

-create signals (current values) which change over time
-split the 32 bit values into 16 bit registers
-command line parameters
-install signal handlers for SIGINT and SIGTERM

The positions in the code where the changes need to be made are (almost) all marked with a comment. The easiest way forward is to keep the existing file structure.
