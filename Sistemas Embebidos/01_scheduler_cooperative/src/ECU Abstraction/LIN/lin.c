

#include "stdlib.h"
#include "board.h"
#include "usart.h"
#include "lin.h"

#define USART_BASE	0x40024000U
#define	USART_PAGESIZE	0x4000U 
#define USART_ADDR(US_NUM)	((Usart *)(USART_BASE + (USART_PAGESIZE * (US_NUM))))
#define USART_IDSTART	(13)
#define USART_IDN(US_NUM)	(USART_IDSTART + (US_NUM))
#define USART_IRQNSTART	(13)
#define USART_IRQN(US_NUM)	(USART_IRQNSTART + (US_NUM))

static LinConfigType_t* LinConfig;


tUartCallbackFunction Lin_Isr (){
	/* Callback function */
}


void Lin_Init (LinConfigType_t* Config){
	
	LinConfig = Config;
	uint8_t ChannelNumber;
  Usart *tmp;
  int i;
	for (i = 0; i < Config->LinNumberOfChannels ; i++)
	{
		ChannelNumber = Config->LinChannel[i].LinChannelId; 

		PIO_Configure(Config->LinChannel[i].pins, PIO_LISTSIZE(Config->LinChannel[i].pins));

		/* Enables the USART to transfer and receive data. */
		USART_SetTransmitterEnabled (USART_ADDR(ChannelNumber) , 1);
		USART_SetReceiverEnabled (USART_ADDR(ChannelNumber) , 1);
		
		PMC_EnablePeripheral(USART_IDN(ChannelNumber));
		           
		/* Reset & disable receiver and transmitter, disable interrupts */
		USART_Configure( USART_ADDR(ChannelNumber), US_MR_USART_MODE_LIN_MASTER|US_MR_USCLKS_MCK|US_MR_CHMODE_NORMAL, 
			(uint32_t)Config->LinChannel[i].LinChannelBaudrate, BOARD_MCK ) ;

		/* Clear pending IRQs and Set priority of IRQs */
		NVIC_ClearPendingIRQ(USART_IRQN(ChannelNumber));
		NVIC_SetPriority(USART_IRQN(ChannelNumber), 1);

		//USART_EnableIt(USART_ADDR(ChannelNumber), US_IER_LINTC);  //US_IER_RXRDY |
		/* Enable interrupt  */
		NVIC_EnableIRQ(USART_IRQN(ChannelNumber));	
	}
}	


Std_ReturnType_t Lin_SendFrame (uint8_t Channel, LinPduType_t* PduInfoPtr){

	uint32_t frame_cfg = ((PduInfoPtr->Cs ? US_LINMR_CHKTYP : 0x0 |  0x1)<< 4) |
							(PduInfoPtr->Drc ? US_LINMR_NACT_SUBSCRIBE : US_LINMR_NACT_PUBLISH) |
							US_LINMR_DLC(PduInfoPtr->Dl-1u) ;

	USART_LinSetMode(USART_ADDR(Channel), frame_cfg );

	while (!USART_LinTxReady(USART_ADDR(Channel))) { } 

	USART_LinWriteId(USART_ADDR(Channel),PduInfoPtr->Pid);
	
	int j = 0;
	while (!(USART_ADDR(Channel)->US_CSR & US_CSR_LINTC)){
		USART_Write( USART_ADDR(Channel), *(PduInfoPtr->SduPtr++) , 0);
		j++;
	}	

	printf("Total de envios %d \n\r",j);
	return E_OK;
}

Std_ReturnType_t Lin_GetSlaveResponse (uint8_t Channel, uint8_t** LinSduPtr){


	return E_OK;
}
