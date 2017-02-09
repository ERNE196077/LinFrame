#ifndef _LIN_H_
#define _LIN_H_

#include "lin_cfg.h"
#include "lin_types.h"

void Lin_Init (LinConfigType_t* Config);					/*   Initializes LIN module   */
Std_ReturnType_t Lin_SendFrame (uint8_t Channel, LinPduType_t* PduInfoPtr);	/*   Sends LIN header and response if neccesary   */
Std_ReturnType_t Lin_GetSlaveResponse (uint8_t Channel, uint8_t** LinSduPtr);	/*   Receives LIN response from slave   */	

#endif