
#include "lin_cfg.h"
/* 3 USARTs */


	LinChannelType_t usart_channels[] =  {{0u,115200},
										{1u, 115200u},
										{2u, 115200u}};

LinConfigType_t LinConfiguration = {
	3,
	&usart_channels
};
