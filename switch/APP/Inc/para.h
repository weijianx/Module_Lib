#ifndef _PARA_H
#define _PARA_H

#include "type.h"


#define RUN_ADDR_BASE				                                   0x00	                //������ʼ����־λ������ÿ���ϵ綼дEEPROMP
#define SLAVE_ADDR								(RUN_ADDR_BASE		 + 0x01)                //�ӻ���ַ
#define BAUDRATE		                        (SLAVE_ADDR	         + 0x01)                //������
#define PARITY			                        (BAUDRATE	         + 0x01)                //��żУ��λ
#define AUTO_UPLOAD_TIME						(PARITY		+ 0x01)

#define USER_DEFAULT_LEN						(AUTO_UPLOAD_TIME    + 0x01)


//---------------------------------------------------
#define COMPANY						(USER_DEFAULT_LEN       + 0x40) 
#define DEV_ENCODING				        (COMPANY		+ 0x40)
#define HWVERSION					(DEV_ENCODING		+ 0x40)
#define SFVERSION					(HWVERSION	        + 0x40)
#define DEV_ID						(SFVERSION		+ 0x40)
#define CUSTOMERCODE                                    (DEV_ID		        + 0x40)  


void ReadPara(void);


#endif


