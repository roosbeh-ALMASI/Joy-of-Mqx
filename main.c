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

void roosbeh_task(void);
void main_task(uint32_t param);
void task_example(task_param_t param);
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
#define ROOSBEH_ALMASI         9U

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK,     main_task,    0xC00, 20, "main_task",    MQX_AUTO_START_TASK},
   {ROOSBEH_ALMASI, roosbeh_task, 0xC00, 5U, "roosbeh_task", MQX_AUTO_START_TASK},
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

OSA_TASK_DEFINE(task_example, TASK_EXAMPLE_STACK_SIZE);
//-----------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------

void main_task(uint32_t param)
{

    osa_status_t result = kStatus_OSA_Error;

    hardware_init();
    dbg_uart_init();
    OSA_Init();


    result = OSA_TaskCreate(task_example, (uint8_t *)"example",
    		                TASK_EXAMPLE_STACK_SIZE,
							task_example_stack,
							TASK_EXAMPLE_PRIO,
							(task_param_t)0,
							false,
							&task_example_task_handler );

    if (result != kStatus_OSA_Success)
    {
        PRINTF("Failed to create example task\r\n");
        return;
    }


//  MainLoop();

    OSA_Start();


    while (1)
    {
       __asm("NOP");
    }


}
//-----------------------------------------------------------------------
// Task Functions
//-----------------------------------------------------------------------

void task_example(task_param_t param)
{
    PRINTF("\r\nRunning the Joy-of-Mqx project.\r\n");

    while(1)
    {
        PRINTF("Hello, it is time for creation\r\n");

    }
}


void roosbeh_task(void)
{
	  while(1)
	    {
	        PRINTF("**************************\r\n");

	    }
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
