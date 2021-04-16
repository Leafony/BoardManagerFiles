/*
 * Stm32Leaf.h
 */
#ifndef STM32LEAF_H_
#define STM32LEAF_H_

#define LPUART1_TX PA_2
#define LPUART1_RX PA_3

#define USART1_TX PB_6
#define USART1_RX PB_7
#define USART2_TX PD_5
#define USART2_RX PD_6
#define USART3_TX PC_4
#define USART3_RX PC_5
//#define USART3_CTS PA_6

#define I2C2_SCL  PB_10 // main
#define I2C2_SDA  PB_11 // main
#define I2C3_SCL  PA_7  // sub
#define I2C3_SDA  PC_9  // sub

#define SPI1_NSS  PA_4
#define SPI1_SCK  PA_5
#define SPI1_MISO PE_14
#define SPI1_MOSI PE_15

// PWM
#define PWM_SP    PA1  // PWM Speaker

// ADC
#define ADC_BAT   PC1
#define A0_IN   PC2
#define A3_IN   PC3

// GPIO
#define D2_INT_N  PE7
#define D3_INT_N  PE8

//#define USART1_CNT  PB5  // USART1 TX-RX cross connection

#define DIP_SW1  PD11
#define DIP_SW2  PD12
#define DIP_SW3  PD13
#define DIP_SW4  PD14

// onboard sensor
#define SENS_PWR_ON PE4 // onboard sensor enable
#define SENS_PIR_IRQN PE0 // PIR sensor interrupt (active Low)
#define SENS_OPT_IRQN PE1 // Optical sensor interrupt (active Low)
#define SENS_6DOF_IRQ PE2 // 6axis sensor interrupt (active High)

// Leaf
#define PERI_PWR_ON PE5 // Leaf Power enable
#define STM_RESET_N PE10 // Leaf Reset drive
//#define RESET_N PE9 // Leaf Reset monitor(input)

// HL7800 modem
#define LTE_PWR_ON PE3  // HL7800 VBATT enable
#define STM_PWR_ON_N PD1  // HL7800 power-on request
#define STM_WAKE_UP PD2   // HL7800 wakeup device
#define STM_RST_OUT_N PD7 // HL7800 hard reset
#define STM_USART3_RTS PB1
//#define STM_USART3_RI PD3

#endif // STM32LEAF_H_
