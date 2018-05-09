#include "IoBsp.h"



void MX_GPIO_Init(void)

{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB\
  |RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);

  
  /*Configure GPIO pins : SW15_Pin SW12_Pin SW11_Pin SW14_Pin 
                           SW23_Pin SW18_Pin */
  GPIO_InitStruct.GPIO_Pin = SW15_Pin|SW12_Pin|SW11_Pin|SW14_Pin 
                          |SW23_Pin|SW18_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : SW17_Pin SW10_Pin SW13_Pin SW16_Pin SW25_Pin */
  GPIO_InitStruct.GPIO_Pin = SW17_Pin|SW10_Pin|SW13_Pin|SW16_Pin 
                          |SW25_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SW19_Pin SW20_Pin PB5 PB6 PB7 PB8 */
  GPIO_InitStruct.GPIO_Pin = SW19_Pin|SW20_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin PC7 PC8 LED2_Pin PC10 PC11 */
  GPIO_InitStruct.GPIO_Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA15 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB9 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

}



u32 get_key(void)
{
	u32 key_val = 0;
	
	key_val |= (GPIO_ReadInputDataBit(SW10_GPIO_Port,SW10_Pin))|(GPIO_ReadInputDataBit(SW11_GPIO_Port,SW11_Pin)<<1)|
						(GPIO_ReadInputDataBit(SW12_GPIO_Port,SW12_Pin)<<2)|(GPIO_ReadInputDataBit(SW13_GPIO_Port,SW13_Pin)<<3);

	key_val |= ((GPIO_ReadInputDataBit(SW14_GPIO_Port,SW14_Pin))|(GPIO_ReadInputDataBit(SW15_GPIO_Port,SW15_Pin)<<1)|
						(GPIO_ReadInputDataBit(SW16_GPIO_Port,SW16_Pin)<<2)|(GPIO_ReadInputDataBit(SW17_GPIO_Port,SW17_Pin)<<3))<<4;
	
	key_val |= ((GPIO_ReadInputDataBit(SW18_GPIO_Port,SW18_Pin))|(GPIO_ReadInputDataBit(SW23_GPIO_Port,SW23_Pin)<<1)|
						(GPIO_ReadInputDataBit(SW25_GPIO_Port,SW25_Pin)<<2))<<8;

	return key_val;
}


