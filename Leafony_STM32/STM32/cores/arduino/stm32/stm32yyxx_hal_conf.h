#ifndef __STM32YYxx_HAL_CONF_H
#define __STM32YYxx_HAL_CONF_H

/*
 * Mandatory HAL modules
 */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED /* Required by other modules */
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED

/*
 * Optional HAL modules, can be enabled/disabled using
 * variant.h, build_opt.h or hal_conf_extra.h
 */
/*
 * Defined by default
 */
#if !defined(HAL_ADC_MODULE_DISABLED)
  #define HAL_ADC_MODULE_ENABLED
#else
  #undef HAL_ADC_MODULE_ENABLED
#endif

#if !defined(HAL_I2C_MODULE_DISABLED)
  #define HAL_I2C_MODULE_ENABLED
#else
  #undef HAL_I2C_MODULE_ENABLED
#endif

#if !defined(HAL_RTC_MODULE_DISABLED)
  #define HAL_RTC_MODULE_ENABLED
#else
  #undef HAL_RTC_MODULE_ENABLED
#endif

#if !defined(HAL_SPI_MODULE_DISABLED)
  #define HAL_SPI_MODULE_ENABLED
#else
  #undef HAL_SPI_MODULE_ENABLED
#endif

#if !defined(HAL_TIM_MODULE_DISABLED)
  #define HAL_TIM_MODULE_ENABLED
#else
  #undef HAL_TIM_MODULE_ENABLED
#endif

/*
 * Not defined by default
 */
#if !defined(HAL_DAC_MODULE_DISABLED)
  /*#define HAL_DAC_MODULE_ENABLED*/
#else
  #undef HAL_DAC_MODULE_ENABLED
#endif

/* Note: interrupt API does not used HAL EXTI module */
/* anyway API is cleaned with HAL_EXTI_MODULE_DISABLED */
#if !defined(HAL_EXTI_MODULE_DISABLED)
  /*#define HAL_EXTI_MODULE_ENABLED*/
#else
  #undef HAL_EXTI_MODULE_ENABLED
#endif

#if !defined(HAL_ETH_MODULE_DISABLED)
  /*#define HAL_ETH_MODULE_ENABLED*/
#else
  #undef HAL_ETH_MODULE_ENABLED
#endif

#if !defined(HAL_SD_MODULE_DISABLED)
  /*#define HAL_SD_MODULE_ENABLED*/
#else
  #undef HAL_SD_MODULE_ENABLED
#endif

#if !defined(HAL_QSPI_MODULE_DISABLED)
  /*#define HAL_QSPI_MODULE_ENABLED*/
#else
  #undef HAL_QSPI_MODULE_ENABLED
#endif

/*
 * Disabled HAL modules, handled thanks Arduino menu
 */
/*#define HAL_UART_MODULE_ENABLED*/
/*#define HAL_PCD_MODULE_ENABLED*/
/*#define HAL_IPCC_MODULE_ENABLED*/

/*
 * Unused HAL modules
 */
#if 0
  HAL_CAN_LEGACY_MODULE_ENABLED
  HAL_CAN_LEGACY_MODULE_ENABLED
  HAL_CEC_MODULE_ENABLED
  HAL_COMP_MODULE_ENABLED
  HAL_CORDIC_MODULE_ENABLED
  HAL_CRC_MODULE_ENABLED
  HAL_CRYP_MODULE_ENABLED
  HAL_DCMI_MODULE_ENABLED
  HAL_DFSDM_MODULE_ENABLED
  HAL_DMA2D_MODULE_ENABLED
  HAL_DSI_MODULE_ENABLED
  HAL_EXTI_MODULE_ENABLED // interrupt API does not use the module
  HAL_FDCAN_MODULE_ENABLED
  HAL_FIREWALL_MODULE_ENABLED
  HAL_FMAC_MODULE_ENABLED
  HAL_FMPI2C_MODULE_ENABLED
  HAL_GFXMMU_MODULE_ENABLED
  HAL_HASH_MODULE_ENABLED
  HAL_HCD_MODULE_ENABLED
  HAL_HRTIM_MODULE_ENABLED
  HAL_HSEM_MODULE_ENABLED
  HAL_I2S_MODULE_ENABLED
  HAL_IPCC_MODULE_ENABLED
  HAL_IRDA_MODULE_ENABLED
  HAL_IWDG_MODULE_ENABLED // IWD built-in library uses LL
  HAL_JPEG_MODULE_ENABLED
  HAL_LCD_MODULE_ENABLED
  HAL_LPTIM_MODULE_ENABLED
  HAL_LTDC_MODULE_ENABLED
  HAL_MDIOS_MODULE_ENABLED
  HAL_MDMA_MODULE_ENABLED
  HAL_MMC_MODULE_ENABLED
  HAL_NAND_MODULE_ENABLED
  HAL_NOR_MODULE_ENABLED
  HAL_OPAMP_MODULE_ENABLED
  HAL_OSPI_MODULE_ENABLED
  HAL_PCCARD_MODULE_ENABLED
  HAL_PKA_MODULE_ENABLED
  HAL_RAMECC_MODULE_ENABLED
  HAL_RNG_MODULE_ENABLED
  HAL_SAI_MODULE_ENABLED
  HAL_SDADC_MODULE_ENABLED
  HAL_SDRAM_MODULE_ENABLED
  HAL_SMARTCARD_MODULE_ENABLED
  HAL_SMBUS_MODULE_ENABLED
  HAL_SPDIFRX_MODULE_ENABLED
  HAL_SRAM_MODULE_ENABLED
  HAL_SWPMI_MODULE_ENABLED
  HAL_TSC_MODULE_ENABLED
  HAL_USART_MODULE_ENABLED
  HAL_WWDG_MODULE_ENABLED
#endif

#endif /* __STM32YYxx_HAL_CONF_H */


