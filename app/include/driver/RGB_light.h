#ifndef __RGB_LIGHT_H__
#define __RGB_LIGHT_H__

/*pwm.h: PWM API的功能和宏定义、驱动程序级别 */
/*RGB_light.h: user interface for light API, user level*/

#include "pwm.h"
#include "c_types.h"
#include "eagle_soc.h"
#define et_uint32 uint32_t
#define et_uint8  uint8_t
 /*NOTE!!  : DO NOT CHANGE THIS FILE*/
/*Definition of GPIO PIN params, for GPIO initialization*/
//red
#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12
//green
#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_1_OUT_IO_NUM 13
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO13
//blue
#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_2_OUT_IO_NUM 14
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO14

#define PWM_3_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_3_OUT_IO_NUM 15
#define PWM_3_OUT_IO_FUNC  FUNC_GPIO15

#define PWM_4_OUT_IO_MUX PERIPHS_IO_MUX_GPIO5_U
#define PWM_4_OUT_IO_NUM 5
#define PWM_4_OUT_IO_FUNC  FUNC_GPIO5


/* define the max\min period for PWM, unit is us*/
#define PWM_PERIOD_MAX	10000
#define PWM_PERIOD_MIN	1000

/* define the max\min duty for PWM */
#define PWM_DUTY_MAX(period)	(period * 1000 / 45)
#define PWM_DUTY_MIN			0

/*Define the channel number of PWM*/
/*In this demo, we can set 3 for 3 PWM channels: RED, GREEN, BLUE*/
/*Or , we can choose 5 channels : RED,GREEN,BLUE,COLD-WHITE,WARM-WHITE*/
#define PWM_CHANNEL 3  /*5:5channel ; 3:3channel*/

struct light_saved_param {
    et_uint32  pwm_period;
    et_uint32  pwm_duty[PWM_CHANNEL];
};

#define LIGHT_RED				0
#define LIGHT_GREEN				1
#define LIGHT_BLUE				2
#define LIGHT_COLD_WHITE		3
#define LIGHT_WARM_WHITE		4

#define RGB_VAL_MIN		0
#define RGB_VAL_MAX		255

// 共阳极  /共阴极二极管
//#define RGB_COMMON_VCC
#define RGB_COMMON_GND

// RGB value turn into duty
#define RGB_TO_DUTY(color, max_duty)	(color * max_duty / RGB_VAL_MAX)

void RGB_light_init(void);
et_uint32 RGB_light_get_duty(et_uint8 channel);
void RGB_light_set_duty(et_uint32 duty, et_uint8 channel);
et_uint32 RGB_light_get_period(void);
void RGB_light_set_period(et_uint32 period);
void RGB_light_set_color(et_uint8 r, et_uint8 g, et_uint8 b);

void RGB_light_start(void);

#endif

