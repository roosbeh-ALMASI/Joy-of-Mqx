#include <MainLoop.h>


#define   currentYear     (2017)
#define   currentMonth    (6)
#define   currentDay      (4)
#define   currentHour     (21)
#define   currentMinute   (27)
#define   currentSecond   (15)

rtc_datetime_t    date ={

		currentYear,
		currentMonth,
		currentDay,
		currentHour,
		currentMinute,
		currentSecond
};


typedef enum{

	_FALSE,
	_TRUE

}_BOOL;






_BOOL  isLptmrRunning   =  _FALSE;
_BOOL  isWatchdogKicked =  _FALSE;


void RTC_init(void);
void RTC_AlarmTimeRequest(uin32_t  time_sec);
void SysWdPet(uint16_t ms);
void SysWdAdd(uint32_t ms);
void SysWdDel(void);



#define PRREG(z) PRINTF(#z" 0x%x\r\n", z);




void MainLoop(void)
{

	//  _time_get_elapsed_ticks(MQX_TICK_STRUCT_PTR);
	//	_time_get_ticks(MQX_TICK_STRUCT_PTR);

	//	_time_get_elapsed(TIME_STRUCT_PTR);
	//	_time_get(TIME_STRUCT_PTR);

/*
	MQX_TICK_STRUCT  tareq_elapsed;
	MQX_TICK_STRUCT_PTR  ptr;
	ptr  = &tareq_elapsed;


	TIME_STRUCT      TT;
	TIME_STRUCT_PTR  timy;
    timy = &TT;

*/

	uint32_t currentSecond;

	RTC_init();


	PRINTF("%s %s\r\n", __TIME__ , __DATE__ );


	SysWdPet(16);
	SysWdAdd(1500);

    PRINTF("system just entering while loop\r\n");


  while(1)
   {

	  RTC_DRV_GetDatetime(RTC_IDX, &date);

	  PRINTF(" date=  %d - %d - %d        time = %d:%d:%d \r\n" ,

			               date.year,
						   date.month,
						   date.day,
						   date.hour,
						   date.minute,
						   date.second);

      _time_delay(1000);
   }

	
}


//------------------------------------------------------------------------

void RTC_init(void)
{

  SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;  // Access & irq enabled

  RTC_DRV_Init(RTC_IDX);
  BOARD_InitRtcOsc();
  RTC_DRV_SetDatetime(RTC_IDX, &date);

 //  NVIC_EnableIRQ(RTC_IRQn);
 //  NVIC_EnableIRQ(RTC_Seconds_IRQn);

}


void RTC_AlarmTimeRequest(uin32_t  time_sec)
{

	RTC_DRV_GetDatetime(RTC_IDX, &date);
	RTC_HAL_ConvertDatetimeToSecs(&date, &currentSecond);

	currentSecond += time_sec;

	RTC_HAL_ConvertSecsToDatetime(&currentSecond, date);
	RTC_DRV_SetAlarm(RTC_IDX, &date, true);

}


void SysWdPet(uint16_t ms)
{
	ms = ms*33;

	SIM->SCGC5 |= 1UL;

	LPTMR0->CSR = 0;
	LPTMR0->CMR = ms;
	LPTMR0->PSR = 1UL<<2 | 1UL<<1;
	LPTMR0->CSR = 1UL | 1UL<<6;

	NVIC_EnableIRQ(LPTMR0_IRQn);

}


void SysWdAdd(uint32_t ms)
{

// watch dog module is using LPO clk= 1KHz, this could be changed to system bus clk
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

void SysWdDel()
{
// We Could log our Reset Control Module (RCM) register here to
// investigate the reason of the reset, or could do this right
// after up coming boot process

    __disable_irq();

    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);
    __asm("NOP");
    __asm("NOP");

    WDOG->STCTRLH = WDOG_STCTRLH_ALLOWUPDATE_MASK;

    __enable_irq();
}




//---------------------------------------------------------------------

void LPTMR0_IRQHandler(void)
{
// LPTMR is using 32.768 KHz external clk, this could be changed
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


void RTC_IRQHandler(void)
{

}

void RTC_Seconds_IRQHandler(void)
{

}

