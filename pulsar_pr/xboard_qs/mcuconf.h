/* Copyright 2020 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include_next <mcuconf.h>

#undef STM32_GPT_USE_TIM3
#define STM32_GPT_USE_TIM3                  TRUE

/* enable SPI2, used for WS2812underglow(RGB BackLight)/WS2812 RGB-Matrix SPI driver, PIN B15 */
#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2 TRUE

#undef STM32_HSE_ENABLED 
#define STM32_HSE_ENABLED                   TRUE
#undef STM32_PLLSRC 
#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#undef STM32_PREDIV_VALUE  
#define STM32_PREDIV_VALUE                  1
#undef STM32_PLLMUL_VALUE 
#define STM32_PLLMUL_VALUE                  4
#undef STM32_USBSW  
#define STM32_USBSW                         STM32_USBSW_PCLK
#undef STM32_ST_USE_TIMER   
#define STM32_ST_USE_TIMER                  2
