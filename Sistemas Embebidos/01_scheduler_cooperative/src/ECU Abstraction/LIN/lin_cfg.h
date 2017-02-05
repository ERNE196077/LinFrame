#ifndef _LIN_CFG_H_
#define _LIN_CFG_H_

#include <stdlib.h>


typedef struct _LinChannelType_t_{
	uint8_t LinChannelId;
	uint16_t LinChannelBaudrate
} LinChannelType_t;

typedef struct _LinConfigType_t_
{
	LinChannelType_t LinChannel[] ;
	uint8_t LinNumberOfChannels;
}LinConfigType_t;


#endif