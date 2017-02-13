

#include "stdlib.h"
#include "board.h"
#include "usart.h"
#include "lin.h"

#define MAX_USART_DEVICES (3u)
#define USART_BASE	0x40024000U
#define	USART_PAGESIZE	0x4000U 
#define USART_ADDR(US_NUM)	((Usart *)(USART_BASE + (USART_PAGESIZE * (US_NUM))))
#define USART_IDSTART	(13)
#define USART_IDN(US_NUM)	(USART_IDSTART + (US_NUM))
#define USART_IRQNSTART	(13)
#define USART_IRQN(US_NUM)	(USART_IRQNSTART + (US_NUM))

static LinConfigType_t* LinConfig;

volatile uint8_t 				*bufferPtr[MAX_USART_DEVICES];
volatile uint16_t				receiveHold[MAX_USART_DEVICES];
volatile LinTransferState_t		channelTransferState[MAX_USART_DEVICES];



void Lin_PublishCallback( Usart *pUsart )
{
	uint8_t bufferNo;

	/* Calculate channel */
	bufferNo = (uint8_t) ( (uint32_t) pUsart - USART_BASE) / USART_PAGESIZE;
	
	//if(channelTransferState[bufferNo] != LIN_TRANSFER_IDLE)
		USART_Write(pUsart, *(bufferPtr[bufferNo]++), 0);

}

void Lin_SubscribeCallback( Usart *pUsart )
{
	uint8_t bufferNo;

	/* Calculate channel */
	bufferNo = (uint8_t) ( (uint32_t) pUsart - USART_BASE) / USART_PAGESIZE;

	/* Copy received value to hold array and set 9th bit to 1 to indicate a new value has been copied */
	receiveHold[bufferNo] = USART_Read( pUsart, 0 ) | 0x0100;
}

void Lin_TransferCompleteCallback( Usart *pUsart )
{
	uint8_t bufferNo;

	bufferNo = (uint8_t) ( (uint32_t) pUsart - USART_BASE) / USART_PAGESIZE;

	USART_DisableIt( pUsart, US_IER_TXRDY | US_IER_RXRDY | US_IER_LINTC );

	channelTransferState[bufferNo] = LIN_TRANSFER_COMPLETE;
}


void Lin_Init (LinConfigType_t* Config){
	
	LinConfig = Config;
	uint8_t ChannelNumber;
  	int i;
    uint32_t usartMode = 0;
  	uint32_t linMode = 0;

  	Usart *pUsart = NULL;
  	uint32_t peripheralId;
  	uint32_t irqNumber;

	for (i = 0; i < Config->LinNumberOfChannels ; i++)
	{
		ChannelNumber = Config->LinChannel[i].LinChannelId;

		if( ChannelNumber >= MAX_USART_DEVICES )
		{
			break;
		}

		pUsart = USART_ADDR(ChannelNumber);
		peripheralId = USART_IDN(ChannelNumber);
		irqNumber = USART_IRQN(ChannelNumber);


		
		/* Enables the USART to transfer and receive data. */
		USART_SetTransmitterEnabled ( pUsart , 1);
		USART_SetReceiverEnabled ( pUsart , 1);
		
		PMC_EnablePeripheral( peripheralId );

		usartMode = US_MR_USART_MODE_LIN_MASTER|US_MR_USCLKS_MCK|US_MR_CHMODE_NORMAL;
		           
		/* Reset & disable receiver and transmitter, disable interrupts */
		USART_Configure( pUsart, usartMode, (uint32_t) Config->LinChannel[i].LinChannelBaudrate, BOARD_MCK );

		USART_PassLinCallbacks( pUsart, Lin_SubscribeCallback, Lin_PublishCallback, Lin_TransferCompleteCallback );

		
		/* Clear pending IRQs and Set priority of IRQs */
		NVIC_ClearPendingIRQ( irqNumber );
		NVIC_SetPriority( irqNumber, 1);

		channelTransferState[i] = LIN_TRANSFER_IDLE;	
	}
}	


Std_ReturnType_t Lin_SendFrame( uint8_t Channel, LinPduType_t *PduInfoPtr )
{
	uint32_t usartMode = 0;
	uint32_t linMode = 0;
	uint32_t channelStatus = 0;
	Usart *pUsart = NULL;

	pUsart = USART_ADDR(Channel);

	usartMode = USART_GetUsartMode(pUsart);

	if( !US_MR_USART_MODE_LIN_MASTER & usartMode )
	{
		return E_NOK;
	}

	if( LIN_TRANSFER_IN_PROGRESS == channelTransferState[Channel] )
	{
		return E_NOK;
	}

	/* Configure frame Checksum Type*/
	if ( LIN_CLASSIC_CS == (PduInfoPtr->Cs) )
	{
		linMode |= US_LINMR_CHKTYP;
	}
	else
	{
		/* Enhanced checksum type not used for LIN 1.3*/
		return E_NOK;
	}

	/* Configure LIN Node Action*/
	switch ( PduInfoPtr->Drc )
	{
		case LIN_MASTER_RESPONSE:
			linMode |= US_LINMR_NACT_PUBLISH;
			break;

		case LIN_SLAVE_RESPONSE:
			linMode |= US_LINMR_NACT_SUBSCRIBE;
			break;

		default:
			return E_NOK;
	}

	/* Configure Data Lenght Control */
	linMode |= US_LINMR_DLC( PduInfoPtr->Dl - 1 );
	
	/* Configure Wakeup Signal Type*/
	linMode |= US_LINMR_WKUPTYP;

	USART_LinSetMode( pUsart, linMode );



	channelTransferState[Channel] = LIN_TRANSFER_IN_PROGRESS;

	bufferPtr[Channel] = PduInfoPtr->SduPtr;

	switch ( PduInfoPtr->Drc )
	{
		case LIN_MASTER_RESPONSE:
			USART_EnableIt( pUsart, US_IER_TXRDY | US_IER_LINTC );
			break;

		case LIN_SLAVE_RESPONSE:
			USART_EnableIt( pUsart, US_IER_RXRDY | US_IER_LINTC );
			break;

		default:
			return E_NOK;
	}

		channelStatus = USART_GetChannelStatus( pUsart );

	
	if( US_CSR_TXRDY == (channelStatus & US_CSR_TXRDY) )
	{
		USART_LinWriteId( pUsart, PduInfoPtr->Pid );
		NVIC_EnableIRQ(  USART_IRQN(Channel) );
	}
	else
	{
		return E_NOK;
	}

	return E_OK;
}

Std_ReturnType_t Lin_GetSlaveResponse( uint8_t Channel, uint8_t** LinSduPtr )
{

	/* Check 9th bit for new received value flag */
	if( (receiveHold[Channel] & 0x0100) != 0 )
	{
		/* Clear flag */
		receiveHold[Channel] &= 0xFF;

		**LinSduPtr = (uint8_t) receiveHold[Channel] & 0xFF;

		return E_OK;
	}
	
	return E_NOK;
}
