/*
 * main.c
 *
 *  Created on: Feb 5, 2022
 *      Author: Mahic
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include<string.h>
#include<stdio.h>

#define TRUE  1
#define FALSE 0


void UART2_Init(void);
void Error_handler(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_Init(void);
void CAN1_Init(void);
void CAN1_TX(void);
void CAN1_RX(void);
void CAN_Filter_Config(void);
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
uint32_t flatency;


int main(void)
{
	HAL_Init();//processor initialization
	SystemClock_Config_HSE(SYS_CLK_FREQ_50MHZ);
	GPIO_Init();
	UART2_Init();
	CAN1_Init();//Puts in initialization mode
	CAN_Filter_Config();//very important or else it wont work ,need to guide it to FIFO 0
	if(HAL_CAN_Start(&hcan1)!=HAL_OK)//Put in Normal operating mode
	{
		Error_handler();
	}
	CAN1_TX(); //Add message to txmailbox and sends  to bus
	CAN1_RX();



	while(1);

	return 0;
}

void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can1_filter;
	can1_filter.FilterActivation=ENABLE;
	can1_filter.FilterBank=0;
	can1_filter.FilterFIFOAssignment=CAN_RX_FIFO0;
	can1_filter.FilterIdHigh=0x0000;
	can1_filter.FilterIdLow=0x0000;
	can1_filter.FilterMaskIdHigh=0x0000;
	can1_filter.FilterMaskIdLow=0x0000;
	can1_filter.FilterMode=CAN_FILTERMODE_IDMASK;
	can1_filter.FilterScale=CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter)!=HAL_OK)
	{
		Error_handler();
	}
}
void CAN1_RX(void)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t r_msg[5];
	char msg[50];

	while(!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));
	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0,&RxHeader,r_msg)!=HAL_OK)
	{
		Error_handler();
	}

	sprintf(msg,"Message Recieved:%s\r\n",r_msg);
	HAL_UART_Transmit(&huart2,(uint8_t*) msg,strlen(msg),HAL_MAX_DELAY);
}

void CAN1_TX(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t msg[5]={'H','E','L','L','O'};
	char umsg[100];
	uint32_t TxMailbox;
	TxHeader.DLC=5;
	TxHeader.IDE=CAN_ID_STD;
	TxHeader.RTR=CAN_RTR_DATA;
	TxHeader.StdId=0x65D;

	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, msg,&TxMailbox)!=HAL_OK)
	{
		Error_handler();
	}
	while(HAL_CAN_IsTxMessagePending(&hcan1, TxMailbox));
	sprintf(umsg,"Message Transmitted\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)umsg, strlen(umsg), HAL_MAX_DELAY);
}

void CAN1_Init(void)
{
	hcan1.Instance=CAN1;
	hcan1.Init.Mode=CAN_MODE_LOOPBACK;
	hcan1.Init.AutoBusOff=DISABLE;
	hcan1.Init.AutoRetransmission=ENABLE;
	hcan1.Init.ReceiveFifoLocked=DISABLE;
	hcan1.Init.AutoWakeUp=DISABLE;
	hcan1.Init.TransmitFifoPriority=DISABLE;
	hcan1.Init.TimeTriggeredMode=DISABLE;

	//BIT TIMING PARAMETER
	hcan1.Init.Prescaler=5;
	hcan1.Init.SyncJumpWidth=CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1=CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2=CAN_BS2_1TQ;
	if(HAL_CAN_Init(&hcan1)!=HAL_OK)
	{
		Error_handler();
	}
}
void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;


	osc_init.OscillatorType=RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState=RCC_HSE_BYPASS;
	osc_init.PLL.PLLState=RCC_PLL_ON;
	osc_init.PLL.PLLSource=RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	{
	case SYS_CLK_FREQ_50MHZ:
	{
		osc_init.PLL.PLLM=8;//divide by 8 to make 1mhz
		osc_init.PLL.PLLN=100;//multiplying to make in 100mhz (100*1mhz from above)
		osc_init.PLL.PLLP=2;//dividing to make pllclk aka sysclk as 50mhz(100/2),min value is 2
		osc_init.PLL.PLLQ=2;//by default 2 not important
		osc_init.PLL.PLLR=2;//by default 2 not important

		clk_init.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider=RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider=RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider=RCC_HCLK_DIV2;
		flatency=FLASH_ACR_LATENCY_1WS;
		break;

	}
	case SYS_CLK_FREQ_84MHZ:
	{
		osc_init.PLL.PLLM=8;//divide by 8 to make 1mhz
		osc_init.PLL.PLLN=168;//range is 50<=PLLn<=432 datasheet
		osc_init.PLL.PLLP=2;
		osc_init.PLL.PLLQ=2;
		osc_init.PLL.PLLR=2;

		clk_init.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider=RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider=RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider=RCC_HCLK_DIV2;
		flatency=FLASH_ACR_LATENCY_2WS;
		break;
	}
	case SYS_CLK_FREQ_120MHZ:
	{
		osc_init.PLL.PLLM=8;//divide by 8 to make 1mhz
		osc_init.PLL.PLLN=240;//range is 50<=PLLn<=432 datasheet
		osc_init.PLL.PLLP=2;
		osc_init.PLL.PLLQ=2;
		osc_init.PLL.PLLR=2;

		clk_init.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider=RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider=RCC_HCLK_DIV4;
		clk_init.APB2CLKDivider=RCC_HCLK_DIV2;
		flatency=FLASH_ACR_LATENCY_3WS;
		break;
	}
	case SYS_CLK_FREQ_180MHZ:
		{
			//THESE EXTRA STEPS NEED TO BE DONE FOR 180MHZ AS IT IS MENTIONED IN DATASHEET
			//Enable the clk for power controller
			__HAL_RCC_PWR_CLK_ENABLE();
			//Set voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
			//turn on OVER DRIVE MODE REGULATOR
			__HAL_PWR_OVERDRIVE_ENABLE();
			osc_init.PLL.PLLM=8;//divide by 8 to make 1mhz
			osc_init.PLL.PLLN=360;//multiplying to make in 100mhz (100*1mhz from above)
			osc_init.PLL.PLLP=2;//dividing to make pllclk aka sysclk as 50mhz(100/2),min value is 2
			osc_init.PLL.PLLQ=2;//by default 2 not important
			osc_init.PLL.PLLR=2;//by default 2 not important

			clk_init.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider=RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider=RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider=RCC_HCLK_DIV2;
			flatency=FLASH_ACR_LATENCY_5WS;
			break;

		}
	default:
		return;
	}
	if(HAL_RCC_OscConfig(&osc_init)!=HAL_OK)
	{
		Error_handler();
	}
	if(HAL_RCC_ClockConfig(&clk_init, flatency)!=HAL_OK)
	{
		Error_handler();
	}
	//REDO SYSTICk
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);//dividing by 1000 for 1ms
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}



void UART2_Init()
{
	//High level initialization
	huart2.Instance=USART2; //base address of peripheral being used ,found in stm32f446xx.h
	huart2.Init.BaudRate=115200;
	huart2.Init.WordLength=UART_WORDLENGTH_8B;
	huart2.Init.StopBits=UART_STOPBITS_1;
	huart2.Init.Parity=UART_PARITY_NONE;
	huart2.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	huart2.Init.Mode=UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2)!=HAL_OK)//Initializes UART
	{
		//There is a problem
		Error_handler();
	}
}

void Error_handler(void)
{
	while(1);

}
