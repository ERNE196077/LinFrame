#ifndef _LIN_H_
#define _LIN_H_

#include "lin_cfg.h"
#include "lin_types.h"

void Lin_Init (const LinConfigType_t* Config);					/*   Initializes LIN module   */
short int Lin_SendFrame (uint8_t Channel, LinPduType* PduInfoPtr);	/*   Sends LIN header and response if neccesary   */
short int Lin_GetSlaveResponse (uint8_t Channel, uint8_t** LinSduPtr);	/*   Receives LIN response from slave   */	

#endif