#ifndef __MODBUS_ASC_H
#define __MODBUS_ASC_H

#include "type.h"

#define MB_ADDRESS_BROADCAST 0x00

#define MB_EX_NONE 			(0x00)
#define MB_EX_ILLEGAL_FUNCTION 		(0x01)                                  //?????
#define MB_EX_ILLEGAL_DATA_ADDRESS	(0x02)                                  //??????
#define MB_EX_ILLEGAL_DATA_VALUE	(0x03)                                  //?????
#define MB_EX_SLAVE_DEVICE_FAILURE	(0x04)                                  //Flash???
#define MB_EX_MEMORY_PARITY_ERROR	(0x05)                                  //????



#define MB_FUNC_READ_INPUT_REGISTER           (  4 )

/*03????????*/
#define MBASC_HOLDING_REG_REGION_BGEIN		        0x30
#define MBASC_HOLDING_REG_REGION_END			0x4F


/*04????????*/
/*04????????*/
#define MBASC_INPUT_REG_REGION1_BGEIN        	        0x4500
#define MBASC_INPUT_REG_REGION1_END          	        0x450E
#define MBASC_INPUT_REG_REGION2_BGEIN        	        0x4540
#define MBASC_INPUT_REG_REGION2_END          	        0x4542
#define MBASC_INPUT_REG_REGION3_BGEIN        	        0x4580
#define MBASC_INPUT_REG_REGION3_END          	        0x458C




/*10????????*/
#define MBASC_MUL_REG_REGION_BGEIN          MBASC_HOLDING_REG_REGION_BGEIN
#define MBASC_MUL_REG_REGION_END            MBASC_HOLDING_REG_REGION_END


#define MBASC_AUTO_UPLOAD_NONE              0
#define MBASC_AUTO_UPLOAD_10S               2
#define MBASC_AUTO_UPLOAD_20S               3
#define MBASC_AUTO_UPLOAD_30S               4


void MBASC_Function(void);
void MBASC_AutoUpLoadFrame(void);
void MBASC_SendMsg(uint8_t *u8Msg, uint8_t u8MsgLen);

void MBASC_AutoUpLoadFrame(void);

#endif