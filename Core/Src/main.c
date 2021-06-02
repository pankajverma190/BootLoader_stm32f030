/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "Internal_flash.h"

#define FLASH_SEC2_BASE_ADDRESS 0x08008000
#define flash_write_address 	0x08007000
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
void bootloader_jump_user_app(void);
uint8_t i;
//uint16_t buff_write[] = {0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x8888, 0x9999, 0x1234};
//uint16_t buff_write[] = {1234, 1234, 5678, 1234, 5678, 1234, 5678, 1234, 5678, 0x1234};
uint16_t buff_write[] = {1,2,3,4,5,6,7,8,9,0};
uint32_t buff_write_32[] = {0x12345678,2,0x12312312,4,0x99887766,6,0x11223344,8,0x22446688,5};
uint16_t data = 0x7777;
uint16_t buff_read_16[10];
uint32_t buff_read_32[10];
//uint8_t buff_write_8[] = {1,2,3,4,5,6,7,8,9,0};
uint8_t buff_write_8[] = "HELLOHELLO";
uint8_t buff_read_8[10];
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();



  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  uint8_t sector = 7;
  /* Initialize interrupts */
  MX_NVIC_Init();
//  MY_FLASH_EraseSector1( sector);
//  MY_FLASH_SetSectorAddrs(7, flash_write_address);
  //MY_FLASH_WriteN(0, &data, 5, DATA_TYPE_16);
  Flash_WriteData_With_Erase_Page(flash_write_address,&buff_write,sizeof(buff_write), DATA_TYPE_16);		//sizeof(buff)

  Flash_ReadData(flash_write_address,buff_read_16,sizeof(buff_read_16),DATA_TYPE_16);


 // Flash_WriteData_without_erase((0x08007000 + 0x3c),&buff_write_8,sizeof(buff_write_8), DATA_TYPE_8);

  Flash_WriteData_without_erase((0x08007000 + 0x400),&buff_write_32,sizeof(buff_write_32), DATA_TYPE_32);

  Flash_ReadData((0x08007000 + 0x400),buff_read_32,sizeof(buff_read_32),DATA_TYPE_32);

  erase_page((0x08007000 + 0x800));
  Flash_WriteData_without_erase((0x08007000 + 0x800),&buff_write_8,sizeof(buff_write_8), DATA_TYPE_8);

  Flash_ReadData((0x08007000 + 0x800),buff_read_8,sizeof(buff_read_8),DATA_TYPE_8);
//  Flash_WriteData_With_Erase_Page(flash_write_address + 0x800,&buff_write,sizeof(buff_write), DATA_TYPE_16);
  FLASH_EraseSector( sector);
  /* Infinite loop */

  while (i<=9)
   {
 	  HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
 	  HAL_Delay(1000);
   }
//   bootloader_jump_user_app();
 }


 void bootloader_jump_user_app(void)
 {
 	void (*app_reset_handler) (void);

 	uint32_t msp_value = *(volatile uint32_t *)FLASH_SEC2_BASE_ADDRESS;

 	__set_MSP(msp_value);

 //	SCB->VTOR = FLASH_SEC2_BASE_ADDRESS;

 	uint32_t resethandler_address = *(volatile uint32_t *)(FLASH_SEC2_BASE_ADDRESS + 4);

 	app_reset_handler = (void*) resethandler_address;

 	app_reset_handler();

 }
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI0_1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

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

}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
