/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Tau 
  \version      1.0
  \date         12/Jun/2016
   
  Program compiled with  WinIDEA Open Build 9.12.256 (54401), tested on Atmel SAMV71 Xplained Ultra
*/
/****************************************************************************************************/


/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Main group of includes for board definitions, chip definitions and type definitions */
#include    "board.h"
#include <stdbool.h>
#include <stdio.h>

/** Task scheduler definitions */
#include    "app_scheduler.h"
/** LED control definitions */ 
#include    "led_ctrl.h"
/** UART communication */
#include    "serial_ctrl.h"
/** Watchdog control function prototypes definitions */
#include    "wdt_ctrl.h"
/** Dynamic Memory allocation services */
#include    "memory_allocation.h"
/** Dynamic Memory allocation services */
#include    "lin.h"

/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint8_t sdutestvar[8] = {0x55,0x00,0x55,0x05};

LinPduType_t LinPduTest = { 0x0F,
               LIN_CLASSIC_CS,           
               LIN_MASTER_RESPONSE,
               4,            
               &sdutestvar};
       
       

extern LinConfigType_t LinConfiguration;
/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define USART0_PINS {PIN_USART0_RXD, PIN_USART0_TXD, PIN_USART0_CTS, PIN_USART0_RTS, PIN_USART0_SCK }
#define USART1_PINS {PIN_USART1_RXD, PIN_USART1_TXD, PIN_USART1_CTS, PIN_USART1_RTS, PIN_USART1_SCK }
#define USART2_PINS {PIN_USART2_RXD, PIN_USART2_TXD, PIN_USART2_CTS, PIN_USART2_RTS, PIN_USART2_SCK }

/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
	
	/* Disable watchdog */
	vfnWdtCtrl_Disable();
	/* Enable I and D cache */
	SCB_EnableICache();
	/* SCB_EnableDCache(); */
	/* Configure LEDs */
	vfnLedCtrl_Configure(); 
	/* Initialize UART communicaiton */
	vfnSerialCtrl_Init();
	/* Configure Non-preemtive scheduler */
	vfnScheduler_Init();
	/* Start scheduler */
	vfnScheduler_Start();
  /* Configure LIN interfaces */
	PIO_Configure((Pin[])USART0_PINS, PIO_LISTSIZE((Pin[])USART0_PINS));
	PIO_Configure((Pin[])USART1_PINS, PIO_LISTSIZE((Pin[])USART1_PINS));
	PIO_Configure((Pin[])USART2_PINS, PIO_LISTSIZE((Pin[])USART2_PINS));


  Lin_Init(&LinConfiguration);
  
  
  /* Transfer Test */
  Lin_SendFrame (0, &LinPduTest);
  
                     
  
	
	/* Once all the basic services have been started, go to infinite loop to serviced activated tasks */
	for(;;)
    {
 		vfnTask_Scheduler();
	}
}
