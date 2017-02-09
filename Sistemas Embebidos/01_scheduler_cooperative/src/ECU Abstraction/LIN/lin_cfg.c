
#include "lin_cfg.h"
/* 3 USARTs */

LinConfigType_t LinConfiguration = {
	3,
	{{0u, USART0, ID_USART0, USART0_IRQn, 9600},
	{1u, USART1, ID_USART1, USART1_IRQn, 115200u},
	{2u, USART2, ID_USART2, USART2_IRQn, 115200u}}
};
