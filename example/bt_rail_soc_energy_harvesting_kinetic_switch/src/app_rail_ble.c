/***************************************************************************//**
 * @file
 * @brief app_rail_ble.c
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
#include "app_assert.h"

#include "rail.h"
#include "rail_features.h"
#include "rail_ble.h"
#include "rail_types.h"
#include "pa_conversions_efr32.h"
#include "sli_rail_util_callbacks.h" // for internal-only callback signatures
#include "app_rail_ble.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// Source of TX Data
#define RAIL_BLE_INIT_DATA_FORMAT_BLE_TX_SOURCE     TX_PACKET_DATA

// Method of Providing TX Data
#define RAIL_BLE_INIT_DATA_FORMAT_BLE_TX_MODE       PACKET_MODE

// Source of RX Data
#define RAIL_BLE_INIT_DATA_FORMAT_BLE_RX_SOURCE     RX_PACKET_DATA

// Method of Retrieving RX Data
#define RAIL_BLE_INIT_DATA_FORMAT_BLE_RX_MODE       PACKET_MODE

// Event mask
#define RAIL_BLE_INIT_EVENT_BLE_MASK                RAIL_EVENT_TX_PACKET_SENT \
  | RAIL_EVENT_TX_ABORTED                                                     \
  | RAIL_EVENT_TX_BLOCKED                                                     \
  | RAIL_EVENT_TX_UNDERFLOW                                                   \
  | RAIL_EVENT_TX_CHANNEL_BUSY                                                \
  | RAIL_EVENT_SCHEDULED_RX_STARTED                                           \
  | RAIL_EVENT_TX_SCHEDULED_TX_MISSED                                         \
  | RAIL_EVENT_CONFIG_UNSCHEDULED                                             \
  | RAIL_EVENT_CONFIG_SCHEDULED                                               \
  | RAIL_EVENT_SCHEDULER_STATUS

// Transition on TX Success
#define RAIL_BLE_INIT_TRANSITION_BLE_TX_SUCCESS     RAIL_RF_STATE_RX
// Transition on TX Error
#define RAIL_BLE_INIT_TRANSITION_BLE_TX_ERROR       RAIL_RF_STATE_RX

// Transition on RX Success
#define RAIL_BLE_INIT_TRANSITION_BLE_RX_SUCCESS     RAIL_RF_STATE_RX
// Transition on RX Error
#define RAIL_BLE_INIT_TRANSITION_BLE_RX_ERROR       RAIL_RF_STATE_RX

// Transition time (microseconds) from idle to RX
#define RAIL_BLE_PROTOCOL_BLE_TIMING_IDLE_TO_RX_US  100

// Transition time (microseconds) from TX to RX
#define RAIL_BLE_PROTOCOL_BLE_TIMING_TX_TO_RX_US    150

// Transition time (microseconds) from idle to TX
#define RAIL_BLE_PROTOCOL_BLE_TIMING_IDLE_TO_TX_US  100

// Transition time (microseconds) from RX to TX
#define RAIL_BLE_PROTOCOL_BLE_TIMING_RX_TO_TX_US    150

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

// RAIL handle and scheduler settings for BLE
static RAIL_Handle_t sl_rail_handle = RAIL_EFR32_HANDLE;
static RAIL_Config_t sl_rail_config = {
  .eventsCallback = &sli_rail_util_on_event,
};

static RAIL_Status_t rail_ble_protocol_config_ble
(
  RAIL_Handle_t handle,
  rail_ble_protocol_type_t protocol
)
{
  RAIL_Status_t status;
  // Override BLE's default timings to get rid of the default rx search timeout
  RAIL_StateTiming_t timings = {
    .idleToRx = RAIL_BLE_PROTOCOL_BLE_TIMING_IDLE_TO_RX_US,
    .txToRx = RAIL_BLE_PROTOCOL_BLE_TIMING_TX_TO_RX_US,
    .idleToTx = RAIL_BLE_PROTOCOL_BLE_TIMING_IDLE_TO_TX_US,
    .rxToTx = RAIL_BLE_PROTOCOL_BLE_TIMING_RX_TO_TX_US,
    .rxSearchTimeout = 0,
    .txToRxSearchTimeout = 0,
  };

  RAIL_BLE_Init(handle);
  switch (protocol) {
    case RAIL_BLE_PROTOCOL_BLE_1MBPS:
      status = RAIL_BLE_ConfigPhy1MbpsViterbi(handle);
      break;
    case RAIL_BLE_PROTOCOL_BLE_2MBPS:
      status = RAIL_BLE_ConfigPhy2MbpsViterbi(handle);
      break;
    case RAIL_BLE_PROTOCOL_BLE_CODED_125KBPS:
      status = RAIL_BLE_ConfigPhyCoded(handle, RAIL_BLE_Coding_125kbps);
      break;
    case RAIL_BLE_PROTOCOL_BLE_CODED_500KBPS:
      status = RAIL_BLE_ConfigPhyCoded(handle, RAIL_BLE_Coding_500kbps);
      break;
    case RAIL_BLE_PROTOCOL_BLE_QUUPPA_1MBPS:
      status = RAIL_BLE_ConfigPhyQuuppa(handle);
      break;
    default:
      status = RAIL_STATUS_INVALID_PARAMETER;
      break;
  }
  if (RAIL_STATUS_NO_ERROR == status) {
    status = RAIL_SetStateTiming(handle, &timings);
  }
  if (RAIL_STATUS_NO_ERROR != status) {
    RAIL_BLE_Deinit(handle);
  }
  return status;
}

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

void rail_ble_init(rail_ble_protocol_type_t protocol_type)
{
  RAIL_Status_t status;
  // RAIL initialization
  sl_rail_handle = RAIL_Init(&sl_rail_config,
                             NULL
                             );
  app_assert((NULL != sl_rail_handle),
             "RAIL_Init failed, return value: NULL");

  // data configuration for BLE
  RAIL_DataConfig_t data_config = {
    .txSource = RAIL_BLE_INIT_DATA_FORMAT_BLE_TX_SOURCE,
    .rxSource = RAIL_BLE_INIT_DATA_FORMAT_BLE_RX_SOURCE,
    .txMethod = RAIL_BLE_INIT_DATA_FORMAT_BLE_TX_MODE,
    .rxMethod = RAIL_BLE_INIT_DATA_FORMAT_BLE_RX_MODE,
  };
  status = RAIL_ConfigData(sl_rail_handle, &data_config);
  app_assert((RAIL_STATUS_NO_ERROR == status),
             "RAIL_ConfigData failed, return value: %d",
             (int)status);

  // channels confguration
  const RAIL_ChannelConfig_t *channel_config = NULL;
  (void) RAIL_ConfigChannels(sl_rail_handle,
                             channel_config,
                             &sli_rail_util_on_channel_config_change);
  // BLE protocol configuration based on the chosen BLE protocol
  status = rail_ble_protocol_config_ble(sl_rail_handle, protocol_type);
  app_assert((RAIL_STATUS_NO_ERROR == status),
             "sl_rail_util_protocol_config failed, return value: %d",
             (int)status);

  // Events configuration
  status = RAIL_ConfigEvents(sl_rail_handle,
                             RAIL_EVENTS_ALL,
                             RAIL_BLE_INIT_EVENT_BLE_MASK);
  app_assert((RAIL_STATUS_NO_ERROR == status),
             "RAIL_ConfigEvents failed, return value: %d",
             (int)status);

  // transitions settings
  RAIL_StateTransitions_t tx_transitions = {
    .success = RAIL_BLE_INIT_TRANSITION_BLE_TX_SUCCESS,
    .error = RAIL_BLE_INIT_TRANSITION_BLE_TX_ERROR
  };
  RAIL_StateTransitions_t rx_transitions = {
    .success = RAIL_BLE_INIT_TRANSITION_BLE_RX_SUCCESS,
    .error = RAIL_BLE_INIT_TRANSITION_BLE_RX_ERROR
  };
  status = RAIL_SetTxTransitions(sl_rail_handle,
                                 &tx_transitions);
  app_assert((RAIL_STATUS_NO_ERROR == status),
             "RAIL_SetTxTransitions failed, return value: %d",
             (int)status);
  status = RAIL_SetRxTransitions(sl_rail_handle,
                                 &rx_transitions);
  app_assert((RAIL_STATUS_NO_ERROR == status),
             "RAIL_SetRxTransitions failed, return value: %d",
             status);
}

/**
 * Get the RAIL handle created during initialization.
 * @param[in] handle The enum name of the desired RAIL handle.
 *
 * @return A valid RAIL handle. If the RAIL handle hasn't been set up, the
 * invalid value of \ref RAIL_EFR32_HANDLE will be returned.
 */
RAIL_Handle_t rail_ble_get_handle(void)
{
  return sl_rail_handle;
}
