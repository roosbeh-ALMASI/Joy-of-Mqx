#include <MainLoop.h>


typedef enum{

	_FALSE,
	_TRUE

}_BOOL;

_BOOL  isLptmrRunning   =  _FALSE;
_BOOL  isWatchdogKicked =  _FALSE;

uint32_t dummyRead;


void LptmrInit(void);

void MainLoop(void)
{

	 LptmrInit();

	__disable_irq();

	WDOG->UNLOCK = 0xC520;
	WDOG->UNLOCK = 0xD928;

	WDOG->STCTRLH &= ~1UL;       // Disable the watchdog module
	dummyRead = WDOG->STCTRLH;  // previous line needs at least one bus clk cycle to take effect

	WDOG->STCTRLH = (1UL<<2 | 1UL<<6 | 1UL<<7);  // WDOG CLK =LPO, and is en in stop and wait mode, irq is it resets
    WDOG->TOVALL = 0x5E0;                        // If clk src = 1KHz, it will resets after 90s


	WDOG->STCTRLH |= 1UL;        // en able watch dog module
	dummyRead = WDOG->STCTRLH;   // previous line needs at least one bus clk cycle to take effect

	__enable_irq();


	NVIC_EnableIRQ(WDOG_EWM_IRQn);


    PRINTF("system just entering while loop\r\n");


  while(1)
   {
        if(isWatchdogKicked == _TRUE)
        {
        	PRINTF("our dude was kicked...>>>>>>>>>>>>>>>>>>>>!\r\n");
        	isWatchdogKicked = _FALSE;
        }
        else{

        	PRINTF("Jack shit...!\r\n");
        }


   }

	
}


void LptmrInit(void)
{

	SIM->SCGC5 |= 1UL;

	LPTMR0->CSR = 0;
	LPTMR0->CMR = 530;
	LPTMR0->PSR = 1UL<<2 | 1UL<<1;
	LPTMR0->CSR = 1UL | 1UL<<6;

	NVIC_EnableIRQ(LPTMR0_IRQn);

}



void LPTMR0_IRQHandler(void)
{
	LPTMR0->CSR |= 1UL<<7;

	isWatchdogKicked = _TRUE;

// A valid refresh is a write of 0xA602 followed by 0xB480
// within 20 bus clock cycles to watchdog refresh register

  WDOG->REFRESH = 0xA602;
  WDOG->REFRESH = 0xB480;

}


void WDOG_EWM_IRQHandler(void)
{

	//WDOG->UNLOCK = 0xC520;
    //WDOG->UNLOCK = 0xD928;

    //WDOG->STCTRLL |= 1UL<<15;  // CLEAR Interrupt flag

}
