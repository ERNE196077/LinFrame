#ifndef _LIN_TYPES_H_
#define _LIN_TYPES_H_

#include "std_types.h"

typedef uint8_t LinFramedPidType;  	/*  LIN frame identifier along with its two parity bits. Range: 0xFE */

typedef enum _LinFrameCsModelType_t_{
	LIN_ENHANCED_CS,				/*   Enhance frame Checksum, add PID to the equation   */
	LIN_CLASSIC_CS					/*   Classic frame Checksum   */
}LinFrameCsModelType_t;

typedef enum _LinFrameResponseType_t_{
	LIN_MASTER_RESPONSE,			/*   Response is generated from this node (Master)   */
	LIN_SLAVE_RESPONSE				/*   Response is generated from a remote slave mode   */
}LinFrameResponseType_t;			

typedef uint8_t LinFrameDlType;		/*   Number of SDU of data bytes to copy   */

typedef struct _LinPduType_t_{
	LinFramedPidType Pid;
	LinFrameCsModelType_t Cs;
	LinFrameResponseType_t Drc;
	LinFrameDlType Dl;
	uint8_t *SduPtr;
}LinPduType_t;

typedef enum _LinTransferState_t_{
  	LIN_TRANSFER_IDLE,
	LIN_TRANSFER_IN_PROGRESS,
	LIN_TRANSFER_COMPLETE
} LinTransferState_t;

#endif