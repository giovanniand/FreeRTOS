#ifndef __KEY_H
#define	__KEY_H

#include "stm32f1xx.h"

//���Ŷ���
/*******************************************************/
#define KEY1_PIN                  GPIO_PIN_0                 
#define KEY1_GPIO_PORT            GPIOA                      
#define KEY1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY2_PIN                  GPIO_PIN_13                 
#define KEY2_GPIO_PORT            GPIOC                      
#define KEY2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
/*******************************************************/

 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	0
#define KEY_OFF	1


#endif /* __LED_H */

