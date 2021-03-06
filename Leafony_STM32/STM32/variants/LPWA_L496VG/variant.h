/*
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Match Table 11. NUCLEO-L496ZG, NUCLEO-L496ZG-P pin assignments
// from UM2179 STM32 Nucleo-144 board
#define PD9  0
#define PD8  1
#define PF15 2
#define PE13 3
#define PF14 4
#define PE11 5
#define PE9  6
#define PF13 7
#define PF12 8
#define PD15 9
#define PD14 10
#define PA7  A11
#define PA6  A12
#define PA5  A13
#define PB9  14
#define PB8  15
#define PC6  16
#define PB15 17
#define PB13 18
#define PB12 19
#define PA4  A14
#define PB4  21
#define PB5  22
#define PB3  23
// 24 is PA4 (20)
// 25 is PB4 (21)
#define PA2  A15
#define PB10 27
#define PE15 28
#define PB0  A16
#define PE12 30
#define PE14 31
#define PA0  A17
// 32 is PB0 (29)
#define PE0  34
#define PB11 35
//else 35 is NC
// 36 is PB10 (27)
// 37 is PE15 (28)
// 38 is PE14 (31)
// 39 is PE12 (30)
#define PE10 40
#define PE7  41
#define PE8  42
#define PC8  43
#define PC9  44
#define PC10 45
#define PC11 46
#define PC12 47
#define PD2  48
#define PF3  A18
#define PF5  A19
#define PD7  51
#define PD6  52
#define PD5  53
#define PD4  54
#define PD3  55
#define PE2  56
#define PE4  57
#define PE5  58
#define PE6  59
#define PE3  60
#define PF8  A20
#define PF7  A21
#define PF9  A22
#define PG1  64
#define PG0  65
#define PD1  66
#define PD0  67
#define PF0  68
#define PF1  69
#define PF2  70
#define PB6  71
#define PB2  72
// ST Morpho
#define PA8  73
#define PA9  74
#define PA10 75
#define PA11 76
#define PA12 77
#define PA15 78
#define PB7  79
#define PB14 80
#define PC7  81
#define PC13 82
#define PC14 83
#define PC15 84
#define PD10 85
#define PD11 86
#define PD12 87
#define PD13 88
#define PE1  89
#define PF10 A23
#define PF11 91
#define PG2  92
#define PG3  93
#define PG4  94
#define PG5  95
#define PG6  96
#define PG7  97
#define PG8  98
#define PG9  99
#define PG10 100
#define PG11 101
#define PG12 102
#define PG13 103
#define PG14 104
#define PH0  105
#define PH1  106
#define PH3  119
// Analog pins
#define PA3  A0
#define PC0  A1
#define PC3  A2
#define PC1  A3
#define PC4  A4
#define PC5  A5
#define PB1  A6
#define PC2  A7
#define PA1  A8
#define PF4  A9
#define PF6  A10
#define PG15 118
// PA13 and PA14 are shared with SWD signals connected to ST-LINK/V2-1.
// If ST-LINK part is not cut, it is not recommended to use them as I/O pins.
//#define PA13 132
//#define PA14 133

// This must be a literal with the same value as PEND
#define NUM_DIGITAL_PINS        120

// This must be a literal with a value less than or equal to to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       24

// On-board LED pin number
#define LED_BUILTIN             PC11 // @@@@@
#define LED_GREEN               LED_BUILTIN
#define LED_BLUE                PC12 // @@@@@
#define LED_RED                 PC10 // @@@@@

// On-board user button
#define USER_BTN                PH3 // @@@@@

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#define TIMER_TONE              TIM6
#define TIMER_SERVO             TIM7

// UART Definitions
#define SERIAL_UART_INSTANCE    0 //Connected to ST-Link - LPUART1

// Serial pin used for console (ex: stlink)
// Rerquired by Firmata
#define PIN_SERIAL_RX           PA3 // @@@@@
#define PIN_SERIAL_TX           PA2 // @@@@@

/* Extra HAL modules */
#define HAL_DAC_MODULE_ENABLED

#ifdef __cplusplus
} // extern "C"
#endif
/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR     Serial
#define SERIAL_PORT_HARDWARE    Serial
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
