#include "createData.h"
#include "server_2.h"
#include "initialize.h"
#include <modbus/modbus.h>
#include <stdio.h>

void createData()
{

static float phase1_current = 0;
static float phase2_current = 0;
static float phase3_current = 0; 


static unsigned int counter = 0;


#ifdef CREATEDEBUG
printf("\n CreateData One\n");
#endif
 
 
if(staticSignal == true){
     phase1_current = 0.15625;   //almost zero
     phase2_current = 300;      //typical current in 20kV operation
     phase3_current = 20000;   //shorted
}
else if (staticSignal == false){ /*create signals which change over time. */
    /*
     * The server calls createData before waiting for each request.  Advancing
     * the counter here gives a simple repeating waveform while keeping the
     * three phase values different from one another.
     */
    const float step = (float)(counter++ % 100);
    // Onko nää arvot sinne päinkään?
    phase1_current = 100.0f + step;
    phase2_current = 200.0f + (step * 0.5f);
    phase3_current = 300.0f + (step * 0.25f);
}


/* copy-paste coding follows.   */

// näihin laitetaan kahteen 16 bit osaan pilkotut phase??_current arvot
uint16_t phase1Registers[2];
uint16_t phase2Registers[2];
uint16_t phase3Registers[2];

#ifdef CREATEDEBUG
printf("\n CreateData two \n");
#endif

modbus_set_float_abcd(phase1_current, phase1Registers);
modbus_set_float_abcd(phase2_current, phase2Registers);
modbus_set_float_abcd(phase3_current, phase3Registers);

mb_mapping -> tab_registers[0] = phase1Registers[0];
mb_mapping -> tab_registers[1] = phase1Registers[1];
mb_mapping -> tab_registers[2] = phase2Registers[0];
mb_mapping -> tab_registers[3] = phase2Registers[1];
mb_mapping -> tab_registers[4] = phase3Registers[0];
mb_mapping -> tab_registers[5] = phase3Registers[1];

#ifdef CREATEDEBUG
printf("\n CreateData four\n");
#endif
 

}
