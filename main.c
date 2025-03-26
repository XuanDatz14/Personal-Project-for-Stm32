#include "main.h"
#include<stdio.h>
#include"I2C_LCD.h"

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
volatile uint32_t xung_cam_bien = 0;  // Biến đếm số xung
// Hàm xử lý ngắt khi có xung từ cảm biến
volatile uint8_t flag_led = 0; // led sangs khi co xung
//volatile double tong_luong_nuoc = 0.0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == pa0_exti0_Pin)  // Nếu tín hiệu đến từ PA0
    {
    	flag_led = 1;
    	HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 1);
        xung_cam_bien++;  // Tăng biến đếm mỗi khi nhận được xung

         }
}

int main(void)
{
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  	LCD_Init();
  	LCD_SendString("LAP TRINH NHUNG");  // In dòng "Test LCD"
  	HAL_Delay(2000);  // Đợi
    LCD_Clear();
    char buffer[64];
  /* USER CODE END 2 */
    HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 1);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
        // Tính toán lưu lượng nước (L/phút)
        double luong_nuoc = 0.0;
        luong_nuoc = (double)xung_cam_bien / 7.5;  // Giả sử 1 xung = 7.5 L

        // Hiển thị số xung trên LCD
       // sprintf(buffer, "Count: %lu pulse", pulse_count);
        //LCD_PutCursor(0, 0);  // Di chuyển con trỏ đến dòng đầu tiên
        //LCD_SendString(buffer);

        // Hiển thị lưu lượng nước (L/phút)
        if (luong_nuoc > 0) {
            sprintf(buffer, " %.2f L/m ", luong_nuoc);
        } else {
            sprintf(buffer, "No_Data");
        }

        LCD_PutCursor(0, 0);  // Move cursor to the first line (row 0, col 0)
        LCD_SendString("Flow :");
        LCD_SendString(buffer);

        LCD_PutCursor(1, 0);  // Move cursor to the first line (row 0, col 0)

        LCD_SendString("Total :");
        sprintf(buffer," %.2f L ",luong_nuoc/60);
        LCD_SendString(buffer);

        // To display the current flow on the third line (row 2)
        if (flag_led) {
                HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
                flag_led = 0;  // Reset cờ để kiểm tra lại trong chu kỳ tiếp theo
            } else {
                HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
            }

        // Đặt lại số xung cho chu kỳ đo lường tiếp theo
       // pulse_count = 0;

        HAL_Delay(500);  // Chờ 3 giây trước khi làm mới màn hình
    }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_Pin */
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : pa0_exti0_Pin */
  GPIO_InitStruct.Pin = pa0_exti0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(pa0_exti0_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
