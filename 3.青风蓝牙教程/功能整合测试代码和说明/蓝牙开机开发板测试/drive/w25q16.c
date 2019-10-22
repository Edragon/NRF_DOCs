#include <string.h>
#include "nrf_drv_common.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "w25q16.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
#define SPI_SS_PIN   29
#define SPI_MISO_PIN  28
#define SPI_MOSI_PIN  2
#define SPI_SCK_PIN  3
static volatile bool spi_xfer_done;  //SPI数据传输完成标志
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */

static uint8_t    spi_tx_buf[256];   /**< TX buffer. */
static uint8_t    spi_rx_buf[256];   /**< RX buffer. */


/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
  spi_xfer_done = true;
}


void hal_spi_init(void)
{
//	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG(SPI_INSTANCE);
//  spi_config.ss_pin = SPI_CS_PIN;
//  APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler));	
	
	
	  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
}

/*****************************************************************************
** 描  述：读出一个字节
** 入  参：无
** 返回值：读出的数据
******************************************************************************/
uint8_t SpiFlash_ReadOneByte(void) 
{
    uint8_t len = 1;
	
    spi_tx_buf[0] = 0xFF;
	  spi_xfer_done = false;
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
	  while(!spi_xfer_done)
			;
    return (spi_rx_buf[0]);
}
/*****************************************************************************
** 描  述：写入一个字节
** 入  参：Dat：待写入的数据
** 返回值：无
******************************************************************************/
void SpiFlash_WriteOneByte(uint8_t Dat)
{
    uint8_t len = 1;
	
    spi_tx_buf[0] = Dat;
	  spi_xfer_done = false;
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
    while(!spi_xfer_done)
			;
}

/*****************************************************************************
** 描  述：写入命令
** 入  参：*CMD：指向待写入的命令
** 返回值：RET_SUCCESS
******************************************************************************/
uint8_t SpiFlash_Write_CMD(uint8_t *CMD)
{
	  uint8_t len = 3;
	
	  spi_tx_buf[0] = *CMD;
	  spi_tx_buf[1] = *(CMD+1);
	  spi_tx_buf[2] = *(CMD+2);
	  spi_xfer_done = false;
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
    while(!spi_xfer_done)
			;
    return RET_SUCCESS;
}
/*****************************************************************************
** 描  述：写使能
** 入  参：无
** 返回值：无
******************************************************************************/
void SpiFlash_Write_Enable(void)
{
    spi_xfer_done = false;
	  SpiFlash_WriteOneByte(SPIFlash_WriteEnable_CMD);
	  while(!spi_xfer_done)
			;
}
/*****************************************************************************
** 描  述：擦除扇区，W25Q128FVSIG最小的擦除单位是扇区
** 入  参：Block_Num：块号
**         Sector_Number：扇区号
** 返回值：
******************************************************************************/
void SPIFlash_Erase_Sector(uint8_t Block_Num,uint8_t Sector_Number)
{
    SpiFlash_Write_Enable();
		
	  spi_tx_buf[0] = SPIFlash_SecErase_CMD;
	  spi_tx_buf[1] = Block_Num;
	  spi_tx_buf[2] = Sector_Number<<4;
	  spi_tx_buf[3] = 0x00;
	  spi_xfer_done = false;
		APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 4, spi_rx_buf, 4));
		while(!spi_xfer_done)
			;	
    nrf_delay_ms(10);    //每次擦除数据都要延时等待写入结束

    return ;
}
/*****************************************************************************
** 描  述：向指定的地址写入数据
**         *pBuffer:指向待写入的数据
**         WriteAddr:写入的起始地址
**         WriteBytesNum:读出的字节数
** 返回值：RET_SUCCESS
******************************************************************************/
uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
{
    uint8_t len;
	
	  SpiFlash_Write_Enable();
		
    spi_tx_buf[0] = SPIFlash_PageProgram_CMD;
	  spi_tx_buf[1] = (uint8_t)((WriteAddr&0x00ff0000)>>16);
	  spi_tx_buf[2] = (uint8_t)((WriteAddr&0x0000ff00)>>8);
	  spi_tx_buf[3] = (uint8_t)WriteAddr;
	
	  memcpy(&spi_tx_buf[4],pBuffer,WriteBytesNum);
	
	  len = WriteBytesNum + 4;
	  spi_xfer_done = false;
		APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, 0));
    while(!spi_xfer_done)
			;	

    return RET_SUCCESS;
}
/*****************************************************************************
** 描  述：从指定的地址读出指定长度的数据
** 入  参：pBuffer：指向存放读出数据的首地址       
**         ReadAddr：待读出数据的起始地址
**         ReadBytesNum：读出的字节数
** 返回值：
******************************************************************************/
uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
    uint8_t len;
		
		spi_tx_buf[0] = SPIFlash_ReadData_CMD;
	  spi_tx_buf[1] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
	  spi_tx_buf[2] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
	  spi_tx_buf[3] = (uint8_t)ReadAddr;
		
		len = ReadBytesNum + 4;
	  spi_xfer_done = false;
		APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
	  while(!spi_xfer_done)
			;	
    memcpy(pBuffer,&spi_rx_buf[4],ReadBytesNum);
    
    return RET_SUCCESS;
}
/********************************************END FILE*******************************************/



                                                                                                
