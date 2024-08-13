/***************************************************************************//**
 * @file
 * @brief app_rail_ble.h
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#ifndef RAIL_BLE_INIT_H_
#define RAIL_BLE_INIT_H_

#include "rail_types.h"

typedef enum sl_flex_util_ble_protocol_type{
  RAIL_BLE_PROTOCOL_BLE_1MBPS,
  RAIL_BLE_PROTOCOL_BLE_2MBPS,
  RAIL_BLE_PROTOCOL_BLE_CODED_125KBPS,
  RAIL_BLE_PROTOCOL_BLE_CODED_500KBPS,
  RAIL_BLE_PROTOCOL_BLE_QUUPPA_1MBPS,
} rail_ble_protocol_type_t;

/**
 * Initialize the RAIL for BLE protocol.
 *
 * @note: This function should be called during application initialization.
 */
void rail_ble_init(rail_ble_protocol_type_t protocol_type);

/**
 * Get the RAIL handle created during initialization.
 * @param[in] handle The enum name of the desired RAIL handle.
 *
 * @return A valid RAIL handle. If the RAIL handle hasn't been set up, the
 * invalid value of \ref RAIL_EFR32_HANDLE will be returned.
 */
RAIL_Handle_t rail_ble_get_handle(void);

#endif /* RAIL_BLE_H_ */
