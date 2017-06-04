#include <MainLoop.h>


typedef enum{

	_FALSE,
	_TRUE

}_BOOL;



_BOOL  isLptmrRunning   =  _FALSE;
_BOOL  isWatchdogKicked =  _FALSE;





void LptmrInit(uint16_t ms);
void wdog_init(uint32_t ms);
void wdog_disable(void);



#define PRREG(z) PRINTF(#z" 0x%x\r\n", z);




void MainLoop(void)
{


    LptmrInit(16);
    wdog_init(1500);

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




void wdog_init(uint32_t ms)
{
    __disable_irq();

    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);
    __asm("NOP");
    __asm("NOP");

    WDOG->TOVALL = (uint16_t)(ms & 0xffff);
    WDOG->TOVALH = (uint16_t)((ms>>16) & 0xffff);

    // clk is LPO

    WDOG->STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE_MASK   |   // uapdate regs En
    		              WDOG_STCTRLH_WDOGEN_MASK   |   // En
						  WDOG_STCTRLH_WAITEN_MASK   |   // En in wait
						  WDOG_STCTRLH_IRQRSTEN_MASK |   // IRQ En
						  WDOG_STCTRLH_STOPEN_MASK);     // En in Stop
    WDOG->PRESC = 0;

    NVIC_EnableIRQ(WDOG_EWM_IRQn);

    __enable_irq();


}

void wdog_disable()
{
    __disable_irq();

    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);
    __asm("NOP");
    __asm("NOP");

    WDOG->STCTRLH = WDOG_STCTRLH_ALLOWUPDATE_MASK;

    __enable_irq();
}



void LptmrInit(uint16_t ms)
{
	ms = ms*33;

	SIM->SCGC5 |= 1UL;

	LPTMR0->CSR = 0;
	LPTMR0->CMR = ms;
	LPTMR0->PSR = 1UL<<2 | 1UL<<1;
	LPTMR0->CSR = 1UL | 1UL<<6;

	NVIC_EnableIRQ(LPTMR0_IRQn);

}



void LPTMR0_IRQHandler(void)
{

	// A valid refresh is a write of 0xA602 followed by 0xB480
	// within 20 bus clock cycles to watchdog refresh register

	__disable_irq();

	WDOG->REFRESH = 0xA602;
	WDOG->REFRESH = 0xB480;

	LPTMR0->CSR |= 1UL<<7;
	isWatchdogKicked = _TRUE;

	__enable_irq();

}


void WDOG_EWM_IRQHandler(void)
{

	__disable_irq();

	WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;
    __asm("NOP");
    __asm("NOP");

    WDOG->STCTRLL |= WDOG_STCTRLL_INTFLG_MASK;  // CLEAR Interrupt flag

    __enable_irq();
}


