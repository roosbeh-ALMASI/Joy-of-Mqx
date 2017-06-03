/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      FRDM-K64F
 * [Project]       Joy-of-Mqx
 * [Version]       1.00
 * [Author]        roosbeh.almasi
 * [Date]          06/02/2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

//-----------------------------------------------------------------------
// Standard C/C++ Includes
//-----------------------------------------------------------------------

#include <stdio.h>
//-----------------------------------------------------------------------
// KSDK Includes
//-----------------------------------------------------------------------
#include "main.h"
//-----------------------------------------------------------------------
// Application Includes
//-----------------------------------------------------------------------
#include "MainLoop.h"
//-----------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------


void main_task(uint32_t param);

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

/*
Task Template List

typedef struct task_template_struct
{
    _mqx_uint     TASK_TEMPLATE_INDEX;
    TASK_FPTR     TASK_ADDRESS;
    _mem_size     TASK_STACKSIZE;
    _mqx_uint     TASK_PRIORITY;
     char        *TASK_NAME;
    _mqx_uint     TASK_ATTRIBUTES;
     uint32_t     CREATION_PARAMETER;
    _mqx_uint     DEFAULT_TIME_SLICE;

} TASK_TEMPLATE_STRUCT, * TASK_TEMPLATE_STRUCT_PTR;

*/


#define MAIN_TASK              8U


const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK,     main_task,    0xC00, 20, "main_task",    MQX_AUTO_START_TASK},
   { 0L,        0L,        0L,    0L,  0L,         0L }
};


#define TASK_EXAMPLE_PRIO            6U
#define TASK_EXAMPLE_STACK_SIZE   1024U











//-----------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------

void main_task(uint32_t param)
{

    osa_status_t result = kStatus_OSA_Error;

    hardware_init();
    dbg_uart_init();
    OSA_Init();



    MainLoop();

    OSA_Start();


    while (1)
    {
       __asm("NOP");
    }


}
//-----------------------------------------------------------------------
// Task Functions
//-----------------------------------------------------------------------






////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
