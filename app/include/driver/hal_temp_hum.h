#ifndef _HAL_TEMP_HUM_H
#define _HAL_TEMP_HUM_H
#include "gizwits_product.h"
#include <stdio.h>
#include <c_types.h>
#include <gpio.h>
#include <eagle_soc.h>
/* 定义驱动引脚 */
#define DHT11_GPIO_PIN      2
/* 设置GPIO输入输出 */
#define DHT11_IO_IN         GPIO_DIS_OUTPUT(GPIO_ID_PIN(DHT11_GPIO_PIN))// gpio_output_set(0, 0, 0, GPIO_ID_PIN(DHT11_GPIO_PIN))不可用
#define DHT11_IO_OUT        gpio_output_set(0, 0, GPIO_ID_PIN(DHT11_GPIO_PIN), 0)
#define	DHT11_OUT_HIGH      GPIO_OUTPUT_SET(GPIO_ID_PIN(DHT11_GPIO_PIN), 1)
#define	DHT11_OUT_LOW       GPIO_OUTPUT_SET(GPIO_ID_PIN(DHT11_GPIO_PIN), 0)
#define	DHT11_IN            GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO_PIN))
#define MEAN_NUM            10
typedef struct
{
    uint8_t th_num;
    uint8_t th_amount; 
    uint8_t th_bufs[10][2];
}th_typedef_t; 
/* 函数声明 */
uint8_t dh11Read(uint8_t * temperature, uint8_t * humidity); 
uint8_t dh11Init(void); //初始化 DHT11
void dh11SensorTest(void);

#endif
/*_HAL_HEMP_HUM_H*/

