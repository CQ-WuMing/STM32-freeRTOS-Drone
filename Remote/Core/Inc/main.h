/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY3_Pin GPIO_PIN_13
#define KEY3_GPIO_Port GPIOC
#define KEY4_Pin GPIO_PIN_14
#define KEY4_GPIO_Port GPIOC
#define KEY5_Pin GPIO_PIN_0
#define KEY5_GPIO_Port GPIOB
#define KEY6_Pin GPIO_PIN_1
#define KEY6_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_12
#define CS_GPIO_Port GPIOB
#define IRQ_Pin GPIO_PIN_8
#define IRQ_GPIO_Port GPIOA
#define CE_Pin GPIO_PIN_11
#define CE_GPIO_Port GPIOA
#define BUZZ_Pin GPIO_PIN_12
#define BUZZ_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_15
#define KEY1_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
