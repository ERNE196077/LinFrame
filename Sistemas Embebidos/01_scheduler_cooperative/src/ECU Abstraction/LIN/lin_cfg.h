#ifndef _LIN_CFG_H_
#define _LIN_CFG_H_

#include <stdlib.h>
#include <chip.h>


typedef struct _LinChannelType_t_{
	uint8_t LinChannelId;
	Usart *pUsart;
	uint32_t IdUsart;
	IRQn_Type IrqnUsart;
	uint32_t LinChannelBaudrate;
} LinChannelType_t;

typedef struct _LinConfigType_t_
{
	uint8_t LinNumberOfChannels;
	LinChannelType_t LinChannel[] ;
}LinConfigType_t;


#endif