#include <MainLoop.h>


typedef enum{

	_FALSE,
	_TRUE

}_BOOL;

_BOOL  isLptmrRunning   =  _FALSE;
_BOOL  isWatchdogKicked =  _FALSE;




void LptmrInit(void);
void wdog_init(uint32_t ms);
void wdog_disable(void);
void wdog_kick(void);


#define PRREG(z) PRINTF(#z" 0x%x\r\n", z);




void MainLoop(void)
{


    LptmrInit();
    wdog_init(20);

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



/*

	int i,fbus,flashbus;

	    fbus = SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1);
	    flashbus = SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1);

	    printf("\nSystemCoreClock %d fbus %d flash %d %s %s\n",  SystemCoreClock,  fbus,  flashbus,  __TIME__,  __DATE__);


	    PRREG(RCM_SRS0);            // reset reason
	    PRREG(RCM_SRS1);

	    PRREG(WDOG->STCTRLH);
	    PRREG(WDOG->PRESC);
	    PRREG(WDOG->TOVALL);
	    PRREG(WDOG->TOVALH);
	    PRREG(WDOG->RSTCNT);

	    wdog_init(4000);            // 4 sec timeout

	    PRREG(WDOG->STCTRLH);
	    PRREG(WDOG->PRESC);
	    PRREG(WDOG->TOVALL);
	    PRREG(WDOG->TOVALH);

	    _time_delay(100);

	    wdog_kick();

	    for (i=1;i<=5; i++)
	        {
	            printf("running %d\n",i);
	            _time_delay(1);
	             wdog_kick();
	        }

	    printf("long wait reset\n");
	    _time_delay(1);
	    printf("never happen\n");


	    */

	
}



//-------------------------------------------------------------------------------------



void wdog_init(uint32_t ms)
{
    __disable_irq();

    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);
    __asm("NOP");
    __asm("NOP");

    WDOG->TOVALL = (uint16_t)(ms & 0xffff);
    WDOG->TOVALH = (uint16_t)((ms>>16) & 0xffff);

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

void wdog_kick()
{
     __disable_irq();
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
    __enable_irq();
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

	// A valid refresh is a write of 0xA602 followed by 0xB480
	// within 20 bus clock cycles to watchdog refresh register

	WDOG->REFRESH = 0xA602;
	WDOG->REFRESH = 0xB480;

	LPTMR0->CSR |= 1UL<<7;
	isWatchdogKicked = _TRUE;

}


void WDOG_EWM_IRQHandler(void)
{

	//WDOG->UNLOCK = 0xC520;
    //WDOG->UNLOCK = 0xD928;

    //WDOG->STCTRLL |= 1UL<<15;  // CLEAR Interrupt flag

}



