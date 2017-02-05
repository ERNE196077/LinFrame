

#include "stdlib.h"
#include "uart.h"
#include "lin.h"

tUartCallbackFunction Lin_Isr (){
	/* Callback function */
}

static LinConfigType_t* LinConfig;


void Lin_Init (const LinConfigType_t* Config){
	
	LinConfig = Config;

	for (int i = 0; i < Config->LinNumberOfChannels ; i++)
	{
		PMC_EnablePeripheral(Config->LinChannel[i]->IdUsart);
	
		/* Reset & disable receiver and transmitter, disable interrupts */
		USART_Configure( Config[i]->pUsart, US_MR_USART_MODE_LIN_MASTER, 
			Config->LinChannel[i]->LinChannelBaudrate, BOARD_MCK ) ;

		/* Clear pending IRQs and Set priority of IRQs */
		NVIC_ClearPendingIRQ(Config->LinChannel[i]->IrqnUsart);
		NVIC_SetPriority(Config->LinChannel[i]->IrqnUsart, 1);

		/* Enables the USART to transfer and receive data. */
		USART_SetTransmitterEnabled (Config->LinChannel[i]->IdUsart , 1);
		USART_SetReceiverEnabled (Config->LinChannel[i]->IdUsart , 1);

		USART_EnableIt(Config->LinChannel[i]->IdUsart, US_IER_RXRDY);
		/* Enable interrupt  */
		NVIC_EnableIRQ(Config->LinChannel[i]->IrqnUsart);	
	}
}	


short int Lin_SendFrame (uint8_t Channel, LinPduType* PduInfoPtr){

	frame_cfg = US_LINMR_NACT_SUBSCRIBE |
			PduInfoPtr->Cs ? US_LINMR_CHKTYP : 0x0 |
			PduInfoPtr->Drc  = US_LINMR_NACT_IGNORE |//? US_LINMR_NACT_SUBSCRIBE : US_LINMR_NACT_PUBLISH |
			US_LINMR_DLC(PduInfoPtr->Drc) ;

	USART_LinSetMode(LinConfig->LinChannel[Chanel]->pUsart, frame_cfg );

	while (!USART_TxReady)
	{ } 
	USART_WriteId(LinConfig->LinChannel[Chanel]->pUsart,PduInfoPtr->Pid);

}

short int Lin_GetSlaveResponse (uint8_t Channel, uint8_t** LinSduPtr){


}
