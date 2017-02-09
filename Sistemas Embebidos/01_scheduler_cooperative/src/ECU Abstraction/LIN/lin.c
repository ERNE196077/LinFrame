

#include "stdlib.h"
#include "board.h"
#include "usart.h"
#include "lin.h"

tUartCallbackFunction Lin_Isr (){
	/* Callback function */
}

const Pin pins[] = {PIN_USART1_RXD, PIN_USART1_TXD, PIN_USART1_CTS, PIN_USART1_RTS, PIN_USART1_SCK };
static LinConfigType_t* LinConfig;


void Lin_Init (LinConfigType_t* Config){
	
	LinConfig = Config;
  
  int i;
	for (i = 0; i < Config->LinNumberOfChannels ; i++)
	{

		PIO_Configure(pins, PIO_LISTSIZE(pins));
		
		PMC_EnablePeripheral(Config->LinChannel[i].IdUsart);
		           
		/* Reset & disable receiver and transmitter, disable interrupts */
		USART_Configure( Config->LinChannel[i].pUsart, US_MR_USART_MODE_LIN_MASTER|US_MR_USCLKS_MCK|US_MR_CHRL_8_BIT|US_MR_CHMODE_NORMAL|US_MR_PAR_NO|US_MR_NBSTOP_1_BIT, 
			(uint32_t)Config->LinChannel[i].LinChannelBaudrate, BOARD_MCK ) ;

		/* Clear pending IRQs and Set priority of IRQs */
		//NVIC_ClearPendingIRQ(Config->LinChannel[i].IrqnUsart);
		//NVIC_SetPriority(Config->LinChannel[i].IrqnUsart, 1);

		/* Enables the USART to transfer and receive data. */
		USART_SetTransmitterEnabled (Config->LinChannel[i].pUsart , 1);
		USART_SetReceiverEnabled (Config->LinChannel[i].pUsart , 1);

		//USART_EnableIt(Config->LinChannel[i].pUsart, US_IER_LINTC);  //US_IER_RXRDY |
		/* Enable interrupt  */
		//NVIC_EnableIRQ(Config->LinChannel[i].IrqnUsart);	
	}
}	


Std_ReturnType_t Lin_SendFrame (uint8_t Channel, LinPduType_t* PduInfoPtr){

	uint32_t frame_cfg = 
			(PduInfoPtr->Cs ? US_LINMR_CHKTYP : 0x0 |  0x1) |
			US_LINMR_NACT_IGNORE |//? US_LINMR_NACT_SUBSCRIBE : US_LINMR_NACT_PUBLISH |
			US_LINMR_DLC(PduInfoPtr->Drc)  ;

	USART_LinSetMode(LinConfig->LinChannel[Channel].pUsart, frame_cfg );

	while (!USART_LinTxReady(LinConfig->LinChannel[Channel].pUsart))
	{ } 
	USART_LinWriteId(LinConfig->LinChannel[Channel].pUsart,PduInfoPtr->Pid);
  printf("Fuck/n/r%08x",LinConfig->LinChannel[Channel].pUsart->US_CSR);
	return E_OK;
}

Std_ReturnType_t Lin_GetSlaveResponse (uint8_t Channel, uint8_t** LinSduPtr){


	return E_OK;
}
