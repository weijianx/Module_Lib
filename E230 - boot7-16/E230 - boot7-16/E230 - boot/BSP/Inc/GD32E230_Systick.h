/*************************************************************************************
							This file is the GD32F103 
						GD seiral mpu Systick header file
*************************************************************************************/
#ifndef __GD32E230_SYSTICK_H
#define __GD32E230_SYSTICK_H
//Includes
#include "Includes.h"

//Datatype define
enum{
    TimerNoUse = 0,
    TimerUsing
};

//This segment defined user data
typedef enum{
    eTmr_BootLoader = 0,
	
    //Todo: Add user timer id start with eTmr_
    eTmr_ModbusTimer,
    eTmr_LedBlinkTimer,
    
    //user timer id
    eTmr_CountId
}E_TMR_TAG;

typedef bool (*TmrCallBack)(void *);
typedef struct{
    u8          mState; //Current id timer statu 0:stopped  1:running
    u8          mFlag;  //Timer timeout flag 1:time out
    u32         delay;  //Delay time value
    u32         remain; //Remain delay value
    void*       pdata;  //User parameter
    TmrCallBack pFCB;   //Call back function pointer
}S_TMR_INFO, *PS_TMR_INFO;



//Functions declare
void GD32E230_Systick_Config( void );
void GD32E230_Systick_TimerProcess( void );

void GD32E230_Systick_TimerKill( u32 i );
void GD32E230_TimerSet( u8 tmrid, TmrCallBack pcb, void* pdat, u32 dlyms );
#endif


