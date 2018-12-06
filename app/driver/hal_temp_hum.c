/*********************************************************
*
* @file      hal_temp_hum.c
*********************************************************/
#include "driver/hal_temp_hum.h"
#include "osapi.h"
#include "eagle_soc.h"
th_typedef_t temphum_typedef;
static void ICACHE_FLASH_ATTR tempHumDelay(unsigned int us)
{
    /* 定义延迟函数 */
    os_delay_us(us);
}

//复位 DHT11
static void ICACHE_FLASH_ATTR dht11Rst(void)
{
    DHT11_IO_OUT;                                               //SET OUTPUT
    DHT11_OUT_LOW;                                              //GPIOA.0=0
    tempHumDelay(18*1000);                                    //Pull down Least 18ms
    DHT11_OUT_HIGH;                                             //GPIOA.0=1
}

static uint8_t ICACHE_FLASH_ATTR dht11Check(void)
{
    uint8_t retry=0;
	
    DHT11_IO_IN;                                                //SET INPUT
    while (DHT11_IN&&retry<100)                                 //DHT11 Pull down 40~80us
    {
        retry++;
        tempHumDelay(1);
    }

    if(retry>=100)
        return 1;
    else
        retry=0;

    while (!DHT11_IN&&retry<100)                                //DHT11 Pull up 40~80us
    {
        retry++;
        tempHumDelay(1);
    }

    if(retry>=100)
        return 1;                                               //chack error

    return 0;
}

static uint8_t ICACHE_FLASH_ATTR dht11ReadBit(void)
{
    uint8_t retry=0;
	
    while(DHT11_IN&&retry<100)                                  //wait become Low level
    {
        retry++;
        tempHumDelay(1);
    }

    retry=0;
    while(!DHT11_IN&&retry<100)                                 //wait become High level
    {
        retry++;
        tempHumDelay(1);
    }

    tempHumDelay(40);                                         //wait 40us

    if(DHT11_IN)
        return 1;
    else
        return 0;
}

static uint8_t ICACHE_FLASH_ATTR hdt11ReadByte(void)
{
    uint8_t i;
    uint8_t dat=0;
	
    for (i=0; i<8; i++)
    {
        dat<<=1;
        dat |= dht11ReadBit(); 
    }

    return dat;
}

static uint8_t ICACHE_FLASH_ATTR dht11ReadData(u8 * temperature, u8 * humidity)
{
	uint8_t i;
    uint8_t buf[5];
	
    dht11Rst(); 
    if(0 == dht11Check()) 
    {
        for(i=0; i<5; i++)
        {
            buf[i] = hdt11ReadByte(); 
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *humidity=buf[0];
            *temperature=buf[2];
        }
    }
    else
	{
    	return 1;
	}

    return 0;
}

uint8_t ICACHE_FLASH_ATTR dh11Read(uint8_t * temperature, uint8_t * humidity)
{
	uint8_t ret = 0; 
	uint8_t cur_i = 0;
    uint8_t curTem = 0;
	uint8_t curHum = 0;
    uint16_t temMeans = 0;
	uint16_t hum_means = 0;

    ret = dht11ReadData(&curTem, &curHum);

    if(0 == ret) 
    {
        //循环采集10次数据
        if(MEAN_NUM > temphum_typedef.th_num)
        {
            temphum_typedef.th_bufs[temphum_typedef.th_num][0] = curTem;
            temphum_typedef.th_bufs[temphum_typedef.th_num][1] = curHum;

            temphum_typedef.th_num++;
        }
        else
        {
            temphum_typedef.th_num = 0;

            temphum_typedef.th_bufs[temphum_typedef.th_num][0] = curTem;
            temphum_typedef.th_bufs[temphum_typedef.th_num][1] = curHum;

            temphum_typedef.th_num++;
        }
    }
    else
    {
        return 1; 
    }
    
    if(MEAN_NUM <= temphum_typedef.th_num) 
    {
        temphum_typedef.th_amount = MEAN_NUM;
    }

    if(0 == temphum_typedef.th_amount) 
    {
        //在十之前计算平均值
        for(cur_i = 0; cur_i < temphum_typedef.th_num; cur_i++)
        {
            temMeans += temphum_typedef.th_bufs[cur_i][0];
            hum_means += temphum_typedef.th_bufs[cur_i][1];
        }

        temMeans = temMeans / temphum_typedef.th_num;
        hum_means = hum_means / temphum_typedef.th_num; 
        
        *temperature = temMeans;
        *humidity = hum_means;
    }
    else if(MEAN_NUM == temphum_typedef.th_amount) 
    {
        //平均计算十次
        for(cur_i = 0; cur_i < temphum_typedef.th_amount; cur_i++) 
        {
            temMeans += temphum_typedef.th_bufs[cur_i][0];
            hum_means += temphum_typedef.th_bufs[cur_i][1];
        }

        temMeans = temMeans / temphum_typedef.th_amount; 
        hum_means = hum_means / temphum_typedef.th_amount; 
        
        *temperature = (uint8_t)temMeans; 
        *humidity = (uint8_t)hum_means; 
    }

    return 0;
}

uint8_t ICACHE_FLASH_ATTR dh11Init(void)
{
    /* 移植驱动程序代码 */
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    dht11Rst(); 
    
    os_memset((uint8_t *)&temphum_typedef, 0, sizeof(th_typedef_t)); 
    
    GIZWITS_LOG("dh11Init \r\n");
    
    return dht11Check(); 
}

void ICACHE_FLASH_ATTR dh11SensorTest(void)
{
    uint8_t curTem = 0;
	uint8_t curHum = 0;
    /* 温度测试 */
    dht11ReadData(&curTem, &curHum); 
    currentDataPoint.valueTemperature = curTem;//温度
    currentDataPoint.valueHumidity = curHum;//湿度
    GIZWITS_LOG("Temperature : %d , Humidity : %d", curTem, curHum);
}
