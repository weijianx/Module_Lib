/* USER CODE BEGIN Header */
/**@file        main.c
* @brief        ����ģ��ĳ�ʼ�������ܵ�ִ��
* @author       Τ����
* @date         2021-04-07
* @version      V1.0.0
* @copyright    2021-2031,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2021/04/07  <td>1.0.0    <td>Τ����  <td>Τ����      <td>
* </table>
*
**********************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"


#include "para.h"
#include "ModBus.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
IWDG_HandleTypeDef  IWDG_HandleStructure;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
GPIO_PinState switch_status;
uint32_t sta = 0x00;

extern UserTypeDef UserPara;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void User_Iwdg_Init(void);
void User_Iwdg_Feed(void);

void Read_SwitchStatus(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//******************************************************************************
// ����         : User_Iwdg_Init()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ���Ź�����
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void User_Iwdg_Init(void)
{
    IWDG_HandleStructure.Init.Prescaler = IWDG_PRESCALER_8;                     //LSI��32.768��8��Ƶλ4.096K
    IWDG_HandleStructure.Init.Reload = 0x0FA0;                                  //��װ��ֵΪ4000��Լ1s
    IWDG_HandleStructure.Init.Window = 0x0FA0;
    IWDG_HandleStructure.Instance = IWDG;
    HAL_IWDG_Init(&IWDG_HandleStructure);
}


//******************************************************************************
// ����         : User_Iwdg_Feed()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ι��
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void User_Iwdg_Feed(void)
{
    HAL_IWDG_Refresh(&IWDG_HandleStructure);
}


/**
* @brief       		�������ŵ�����
* @param[in]   		None
* @return        	None
* - None
*/
void switch_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin : PA1*/
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
* @brief       		��ȡ�������ŵ�״̬
* @param[in]   		None
* @return        	None
* - None
*/
void Read_SwitchStatus(void)
{
	switch_status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	if(switch_status == GPIO_PIN_SET)
	{
		sta = 0x00;
	}
	else if(switch_status == GPIO_PIN_RESET)
	{
		sta = 0xFF00;
	}

}

/**
* @brief       		��ȡ�������ŵ�״̬
* @param[in]   		None
* @return        	���ؿ������ŵ�״ֵ̬
* - None
*/
uint32_t Get_SwitchSta(void)
{

	return sta;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
    __IO uint32_t               Auto_Up_Tick_Start = HAL_GetTick(); //���㳬ʱ����ʼֵ������ModBus��ʱ�Զ��ϴ�

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	ReadPara();                                                                 //��ȡEEPROM����
	Uart_Config_Init();                                                         //��������
	MX_TIM2_Init();																//��ʱ��2��ʼ��

	switch_config();															//�������ų�ʼ��
	User_Iwdg_Init();                                                           //���Ź�����
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		User_Iwdg_Feed();                                                   //ι��

		Read_SwitchStatus();
		
		if(UartRecvFrameOK == Bit_SET)
		{

			ModbusHandle();
			UartRecvFrameOK = Bit_RESET;
			//������ ModBus ��Ϣ���Զ��ϴ���ʱ�ӵ�ǰʱ�俪ʼ���¼�ʱ
            Auto_Up_Tick_Start = HAL_GetTick(); 

			
		}
		        //����Զ��ϴ�ʱ�䲻Ϊ 0 ���Զ��ϴ���ʱʱ�䵽
        else if((UserPara.AutoUpLoadTime != 0) 
			&& ((UserPara.AutoUpLoadTime * AUTOUPLOAD_CYCLE) <= (HAL_GetTick() - Auto_Up_Tick_Start)))   
        {
			
			ModbusAutoUpload();
//			//��¼��ǰʱ��Ϊ��һ���Զ��ϴ����¼�ʱ
            Auto_Up_Tick_Start = HAL_GetTick();  
		}
		
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
