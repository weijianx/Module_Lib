/*************************************************************************************
							This file is the GD32F103 
						GD seiral mpu Systick source file
*************************************************************************************/
//Include files
#include "GD32E230_Systick.h"

//Variables define
static S_TMR_INFO TmrTbl[eTmr_CountId];

//Local functions declare
static void GD32E230_Systick_TimerControlInit( void );

//Functions implement
/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
void GD32E230_Systick_Config( void )
{
    if (SysTick_Config(SystemCoreClock / 1000U))	/* setup systick timer for 1000Hz interrupts */
	{
		while (1)
		{
			//Add capture error lines
		}
	}
	GD32E230_Systick_TimerControlInit( );
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
static void GD32E230_Systick_TimerControlInit( void )
{
    u32 i;
    for (i = 0; i < eTmr_CountId; i ++)
    {
        TmrTbl[i].delay = 0;
        TmrTbl[i].remain = 0;
        TmrTbl[i].mFlag = 0;
        TmrTbl[i].mState = TimerNoUse;
        TmrTbl[i].pFCB = NULL;
        TmrTbl[i].pdata = NULL;
    }
}




/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
void GD32E230_Systick_TimerProcess( void )
{
    u32 i;
    for (i = 0; i < eTmr_CountId; i ++)
    {
        if (TmrTbl[i].mFlag)
        {
            TmrTbl[i].mFlag = 0;
            
            if ((TmrTbl[i].pFCB)(TmrTbl[i].pdata))
            {
                TmrTbl[i].remain = TmrTbl[i].delay;
                TmrTbl[i].mState = TimerUsing;
            }
        }
    }
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
void GD32E230_TimerSet( u8 tmrid, TmrCallBack pcb, void* pdat, u32 dlyms )
{
    if ((tmrid >= eTmr_CountId) || (!pcb) || (!dlyms))
        return;
    
    TmrTbl[tmrid].mState = TimerNoUse;
    
    TmrTbl[tmrid].pFCB = pcb;
    TmrTbl[tmrid].pdata = pdat;
    TmrTbl[tmrid].delay = dlyms;
    TmrTbl[tmrid].remain = dlyms;
    TmrTbl[tmrid].mState = TimerUsing;
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
void GD32E230_Systick_TimerKill( u32 i )
{
	TmrTbl[i].delay = 0;
    TmrTbl[i].remain = 0;
    TmrTbl[i].mFlag = 0;
    TmrTbl[i].mState = TimerNoUse;
    TmrTbl[i].pFCB = NULL;
    TmrTbl[i].pdata = NULL;
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
void SysTick_Handler( void )
{
	//Timer
    u32 i;
    for (i = 0; i < eTmr_CountId; i ++)
    {
        if (TmrTbl[i].mState == TimerUsing)
        {
            if (TmrTbl[i].remain)
            {
                TmrTbl[i].remain --;
            }
            else
            {
                TmrTbl[i].mFlag = 1;
                TmrTbl[i].mState = TimerNoUse;
            }
        }
    }
}

