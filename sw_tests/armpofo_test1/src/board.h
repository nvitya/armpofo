/* -----------------------------------------------------------------------------
 * This file is a part of the NVCM Tests project: https://github.com/nvitya/nvcmtests
 * Copyright (c) 2018 Viktor Nagy, nvitya
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 * --------------------------------------------------------------------------- */
/*
 *  file:     board.h (blinkled)
 *  brief:    Required board definition by the NVCM core (platform.h)
 *  version:  1.00
 *  date:     2018-02-10
 *  authors:  nvitya
*/

#ifndef BOARD_H_
#define BOARD_H_

// Special, non-builtin boards

#if defined(BOARD_NONE_MK20DN128)

  #define BOARD_NAME "Breakout board for K20 (48 pin)"
  #define MCU_MK20DN
  #define MCU_INPUT_FREQ   12000000

#elif defined(BOARD_NONE_MKV30F)

  #define BOARD_NAME "Breakout board for MKV30F (32 pin)"
  #define MCU_MKV30F
  #define MCU_CLOCK_SPEED   96000000
  #define MCU_INTRC_SPEED   48000000

#elif defined(BOARD_NONE_F070F6)

  #define BOARD_NAME "Breakout board for STM32F070F6"
  #define MCU_STM32F070F6
  #define MCU_INPUT_FREQ   12000000

#else

  #include "boards_builtin.h"

#endif


#endif /* BOARD_H_ */
