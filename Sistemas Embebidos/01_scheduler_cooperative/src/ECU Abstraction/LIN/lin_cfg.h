#ifndef _LIN_CFG_H_
#define _LIN_CFG_H_

#include <stdlib.h>
#include <chip.h>
#include <board.h>


typedef struct _LinChannelType_t_{
	uint8_t LinChannelId;
	uint32_t LinChannelBaudrate;
	Pin pins[5];
} LinChannelType_t;

typedef struct _LinConfigType_t_
{
	uint8_t LinNumberOfChannels;
	LinChannelType_t LinChannel[] ;
}LinConfigType_t;


#endif