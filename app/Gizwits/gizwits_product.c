/**
************************************************************
* @file         gizwits_product.c
* @brief        Control protocol processing, and platform-related hardware initialization
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "driver/hal_key.h"

#include "driver/hal_temp_hum.h"
#include "driver/RGB_light.h"

/** User area The current device state structure */
dataPoint_t currentDataPoint;

bool STA[2] = {0};  // 用于存储继电器和大功率LED状态, 初始化为关闭状态

uint8_t time_updata = 0;

uint32_t mode_Cloud_data[4] = {0};  // 云端数据缓存  0模式 1R 2G 3B

/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t ICACHE_FLASH_ATTR gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t * dataPointPtr = (dataPoint_t *)data;
    moduleStatusInfo_t * wifiData = (moduleStatusInfo_t *)data;

    if((NULL == info) || (NULL == data))
    {
        GIZWITS_LOG("!!! gizwitsEventProcess Error \n");
        return -1;
    }

    for(i = 0; i < info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_switch_led :
            currentDataPoint.valueswitch_led = dataPointPtr->valueswitch_led;
            GIZWITS_LOG("Evt: EVENT_switch_led %d \n", currentDataPoint.valueswitch_led);
            if(0x01 == currentDataPoint.valueswitch_led)
            {
                //user handle
            	STA[0] = 1;  // 打开大功率LED
            }
            else
            {
                //user handle
            	STA[0] = 0;  // 关闭大功率LED
            }
            break;
        case EVENT_switch_relay :
            currentDataPoint.valueswitch_relay = dataPointPtr->valueswitch_relay;
            GIZWITS_LOG("Evt: EVENT_switch_relay %d \n", currentDataPoint.valueswitch_relay);
            if(0x01 == currentDataPoint.valueswitch_relay)
            {
                //user handle
            	STA[1] = 1;  // 打开继电器
            }
            else
            {
                //user handle
            	STA[1] = 0;  // 关闭继电器
            }
            break;

        case EVENT_LED_Color:
            currentDataPoint.valueLED_Color = dataPointPtr->valueLED_Color;
            GIZWITS_LOG("Evt: EVENT_LED_Color %d\n", currentDataPoint.valueLED_Color);
            switch(currentDataPoint.valueLED_Color)
            {
            case LED_Color_VALUE0:
                //user handle
            	mode_Cloud_data[0] = 0;  // 关闭
                break;
            case LED_Color_VALUE1:
                //user handle
            	mode_Cloud_data[0] = 1;  // 自定义
                break;
            case LED_Color_VALUE2:
                //user handle
            	mode_Cloud_data[0] = 2;  // 设置红色
                break;
            case LED_Color_VALUE3:
                //user handle
            	mode_Cloud_data[0] = 3;  // 设置绿色
                break;
            case LED_Color_VALUE4:
                //user handle
            	mode_Cloud_data[0] = 4;  // 设置蓝色
                break;
            case LED_Color_VALUE5:
                //user handle
            	mode_Cloud_data[0] = 5;  // 设置黄色
                break;
            case LED_Color_VALUE6:
                //user handle
            	mode_Cloud_data[0] = 6;  // 设置紫色
                break;
            case LED_Color_VALUE7:
                //user handle
            	mode_Cloud_data[0] = 7;  // 设置粉色
                break;
            case LED_Color_VALUE8:
                //user handle
            	mode_Cloud_data[0] = 8;  // 设置白色
                break;
            default:
                break;
            }
            break;

        case EVENT_LED_R:
            currentDataPoint.valueLED_R= dataPointPtr->valueLED_R;
            GIZWITS_LOG("Evt:EVENT_LED_R %d\n",currentDataPoint.valueLED_R);
            //user handle
            mode_Cloud_data[1] = currentDataPoint.valueLED_R;  // 红色
            if (mode_Cloud_data[1] != 1)
            {
            	mode_Cloud_data[0] = 1;  // 自定义
            }
            break;
        case EVENT_LED_G:
            currentDataPoint.valueLED_G= dataPointPtr->valueLED_G;
            GIZWITS_LOG("Evt:EVENT_LED_G %d\n",currentDataPoint.valueLED_G);
            //user handle
            mode_Cloud_data[2] = currentDataPoint.valueLED_G;  // 绿色
            if (mode_Cloud_data[2] != 1)
            {
            	mode_Cloud_data[0] = 1;  // 自定义
            }
            break;
        case EVENT_LED_B:
            currentDataPoint.valueLED_B= dataPointPtr->valueLED_B;
            GIZWITS_LOG("Evt:EVENT_LED_B %d\n",currentDataPoint.valueLED_B);
            //user handle
            mode_Cloud_data[3] = currentDataPoint.valueLED_B;  // 蓝色
			if (mode_Cloud_data[3] != 1)
			{
				mode_Cloud_data[0] = 1;  // 自定义
			}
            break;

        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
            GIZWITS_LOG("@@@@ connected router\n");
 
            break;
        case WIFI_DISCON_ROUTER:
            GIZWITS_LOG("@@@@ disconnected router\n");
 
            break;
        case WIFI_CON_M2M:
            GIZWITS_LOG("@@@@ connected m2m\n");
			setConnectM2MStatus(0x01);
 
            break;
        case WIFI_DISCON_M2M:
            GIZWITS_LOG("@@@@ disconnected m2m\n");
			setConnectM2MStatus(0x00);
 
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("@@@@ RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
            break;
            
        default:
            break;
        }
    }
    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
    
    return 0; 
}


/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void ICACHE_FLASH_ATTR userHandle(void)
{
    /*
    currentDataPoint.valueTemperature = ;//Add Sensor Data Collection
    currentDataPoint.valueHumidity = ;//Add Sensor Data Collection

    */
	GPIO_OUTPUT_SET(GPIO_ID_PIN(4), !STA[0]);  // 大功率LED开关, 低电平有效, 取反操作
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), !STA[1]);  // 继电器开关, 低电平有效, 取反操作

	if (20 == time_updata)  // 定时50ms*20, 修改定时器后不改变上报频率
	{
		time_updata = 0;

		currentDataPoint.valueswitch_led = STA[0];  // 大功率LED开关状态上报
		currentDataPoint.valueswitch_relay = STA[1];  // 继电器开关状态上报

		dh11SensorTest();  // 获取温度并上报
	}
	else
	{
		time_updata++;
	}

	switch(mode_Cloud_data[0])
	{
		case 0 :
			RGB_light_set_color(0,0,0);//关闭灯
			currentDataPoint.valueLED_Color = 0;
			currentDataPoint.valueLED_R = 0;
			currentDataPoint.valueLED_G = 0;
			currentDataPoint.valueLED_B = 0;
			break;

		case 1 :
			RGB_light_set_color(mode_Cloud_data[1],mode_Cloud_data[2],mode_Cloud_data[3]);//自定义
			currentDataPoint.valueLED_Color = 1;
			currentDataPoint.valueLED_R = mode_Cloud_data[1];
			currentDataPoint.valueLED_G = mode_Cloud_data[2];
			currentDataPoint.valueLED_B = mode_Cloud_data[3];
			break;

		case 2 :
			RGB_light_set_color(255,0,0);//红色
			currentDataPoint.valueLED_Color = 2;
			currentDataPoint.valueLED_R = 255;
			currentDataPoint.valueLED_G = 0;
			currentDataPoint.valueLED_B = 0;
			break;

		case 3 :
			RGB_light_set_color(0,255,0);//绿色
			currentDataPoint.valueLED_Color = 3;
			currentDataPoint.valueLED_R = 0;
			currentDataPoint.valueLED_G = 255;
			currentDataPoint.valueLED_B = 0;
			break;

		case 4 :
			RGB_light_set_color(0,0,255);//蓝色
			currentDataPoint.valueLED_Color = 4;
			currentDataPoint.valueLED_R = 0;
			currentDataPoint.valueLED_G = 0;
			currentDataPoint.valueLED_B = 255;
			break;

		case 5 :
			RGB_light_set_color(255,255,0);//黄色
			currentDataPoint.valueLED_Color = 5;
			currentDataPoint.valueLED_R = 255;
			currentDataPoint.valueLED_G = 255;
			currentDataPoint.valueLED_B = 0;
			break;

		case 6 :
			RGB_light_set_color(255,0,255);//紫色
			currentDataPoint.valueLED_Color = 6;
			currentDataPoint.valueLED_R = 255;
			currentDataPoint.valueLED_G = 0;
			currentDataPoint.valueLED_B = 255;
			break;

		case 7 :
			RGB_light_set_color(255,52,179);//粉色（估计不是粉色）
			currentDataPoint.valueLED_Color = 7;
			currentDataPoint.valueLED_R = 255;
			currentDataPoint.valueLED_G = 52;
			currentDataPoint.valueLED_B = 179;
			break;

		case 8 :
			RGB_light_set_color(255,255,255);//白色
			currentDataPoint.valueLED_Color = 8;
			currentDataPoint.valueLED_R = 255;
			currentDataPoint.valueLED_G = 255;
			currentDataPoint.valueLED_B = 255;
			break;

		default:
			break;
	}

    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
}


/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void ICACHE_FLASH_ATTR userInit(void)
{
    gizMemset((uint8_t *)&currentDataPoint, 0, sizeof(dataPoint_t));

 	/** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
   		currentDataPoint.valueswitch_led = ;
   		currentDataPoint.valueswitch_relay = ;
   		currentDataPoint.valueLED_Color = ;
   		currentDataPoint.valueLED_R = ;
   		currentDataPoint.valueLED_G = ;
   		currentDataPoint.valueLED_B = ;
   		currentDataPoint.valueTemperature = ;
   		currentDataPoint.valueHumidity = ;
    */
	currentDataPoint.valueswitch_led = 0;
	currentDataPoint.valueswitch_relay = 0;
	currentDataPoint.valueLED_Color = 0;
	currentDataPoint.valueLED_R = 0;
	currentDataPoint.valueLED_G = 0;
	currentDataPoint.valueLED_B = 0;
	currentDataPoint.valueTemperature = 0;
	currentDataPoint.valueHumidity = 0;

}


