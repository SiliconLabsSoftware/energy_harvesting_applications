/***************************************************************************//**
 * @file
 * @brief app.c
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

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "app_assert.h"
#include "em_emu.h"
#include "efr32mg22_devinfo.h"

#include "rail.h"
#include "rail_features.h"
#include "rail_ble.h"
#include "rail_types.h"

#include "app_rail_ble.h"
#include "app.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

/// Size of the FIFO buffers
#define RAIL_FIFO_SIZE                    256

#define RAIL_NUM_OF_TX                    60
#define TX_INTERVAL_US                    30

#define RAIL_BLE_ADV_CHANNEL              37

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

/******************************************************************************
 * Initialization of RAIL TX FIFO
 *****************************************************************************/
static void app_set_rail_tx_fifo(RAIL_Handle_t rail_handle);

/**************************************************************************//**
 * The function initializes the all states, status for application process.
 *****************************************************************************/
static void app_rail_ble_adv_init(RAIL_Handle_t rail_handle);

/*******************************************************************************
 * The API set the rail back to idle state
 ******************************************************************************/
static void set_radio_to_idle_state(RAIL_Handle_t rail_handle);

/**************************************************************************//**
 * Transmits the data packet.
 *
 * @param[in] rail_handle Handle to the RAIL context
 * @param[in] packet      The packet that is desired to send
 * @param[in] rail_handle The size of the packet
 * @return None
 *****************************************************************************/
static void handle_transmit(RAIL_Handle_t rail_handle);

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

/// Transmit FIFO
static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t tx_fifo[RAIL_FIFO_SIZE];

#define ADV_RAIL_PACKET_LENGTH       26
#define ADV_RAIL_ADVA_OFFSET         2
static __ALIGNED(RAIL_FIFO_ALIGNMENT)
uint8_t adv_rail_packet[ADV_RAIL_PACKET_LENGTH] =
{
  0x02, // packet type: ADV_NONCONN_­IND, non-connectable
  0x18, // length: 24
  0x2f, 0x45, 0x0a, 0x6f, 0x0d, 0x00, // AdvA: advertiser address
  0x02, // adv data length 2
  0x01, // adv data type 1 (flags)
  0x06, // flags: LE General Discoverable Mode, BR/EDR Not Supported
  0x03, // adv data length 5
  0xff, // adv data type: manufacturer specific
  0xff, 0x02, // manufacturer: Silicon Labs
  0x0a, // adv data length 9
  0x09, // adv data type 9 (complete local name)
  'E', 'H', ' ', 'S', 'w', 'i', 't', 'c', 'h'// "EH Switch"
};

static RAIL_ScheduleTxConfig_t shedule_tx_config = {
  .when = TX_INTERVAL_US,
  .mode = RAIL_TIME_DELAY,
  .txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX
};

static volatile bool tx_requested = false;

/// Notify RAIL Tx, Rx calibration error
static bool rail_error = false;

/// Notify end of packet transmission
static bool rail_packet_sent = false;

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * The function is used for some basic initialization related to the app.
 *****************************************************************************/
RAIL_Handle_t app_init(void)
{
  RAIL_Handle_t rail_handle;

  rail_ble_init(RAIL_BLE_PROTOCOL_BLE_1MBPS);

  // Get RAIL handle, used later by the application
  rail_handle = rail_ble_get_handle();

  // sets the RAIL TX FIFO
  app_set_rail_tx_fifo(rail_handle);

  // initializes the ble adv
  app_rail_ble_adv_init(rail_handle);

  return rail_handle;
}

/******************************************************************************
 * Application state machine, called infinitely
 *****************************************************************************/
void app_process_action(RAIL_Handle_t rail_handle)
{
  if (tx_requested) {
    tx_requested = false;

    // sends the adv packet
    handle_transmit(rail_handle);
  }

  // wait until the packet is sent
  if (rail_packet_sent) {
    rail_packet_sent = false;
    tx_requested = true;
  }
}

/******************************************************************************
 * RAIL callback, called if a RAIL event occurs
 *****************************************************************************/
void sl_rail_util_on_event(RAIL_Handle_t rail_handle,
                           RAIL_Events_t events)
{
  (void) rail_handle;

  // Handle Tx events
  if (events & RAIL_EVENTS_TX_COMPLETION) {
    if ((events & RAIL_EVENT_TX_PACKET_SENT)
        || (events & RAIL_EVENT_TX_SCHEDULED_TX_MISSED)) {
      rail_packet_sent = true;
    } else {
      rail_error = true;
    }
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * Initialization of RAIL TX FIFO
 *****************************************************************************/
static void app_set_rail_tx_fifo(RAIL_Handle_t rail_handle)
{
  // RAIL FIFO size allocated by RAIL_SetTxFifo() call
  uint16_t allocated_tx_fifo_size = 0;

  allocated_tx_fifo_size = RAIL_SetTxFifo(rail_handle, tx_fifo,
                                          0U, RAIL_FIFO_SIZE);
  app_assert(allocated_tx_fifo_size == RAIL_FIFO_SIZE,
             "RAIL_SetTxFifo() failed to allocate a large enough fifo"
             " (%d bytes instead of %d bytes)\n",
             allocated_tx_fifo_size, RAIL_FIFO_SIZE);
}

static void app_rail_ble_adv_init(RAIL_Handle_t rail_handle)
{
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET] = DEVINFO->EUI48L & 0xff;
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET + 1] = (DEVINFO->EUI48L >> 8) & 0xff;
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET + 2] = (DEVINFO->EUI48L >> 16) & 0xff;
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET + 3] = (DEVINFO->EUI48L >> 24) & 0xff;
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET + 4] = DEVINFO->EUI48H & 0xff;
  adv_rail_packet[ADV_RAIL_ADVA_OFFSET + 5] = (DEVINFO->EUI48H >> 8) & 0xff;

  // Configures us for the first advertising channel (Physical: 0, Logical: 37).
  // The CRC init value and Access Address come from the BLE specification.
  RAIL_BLE_ConfigChannelRadioParams(rail_handle,
                                    0x555555,
                                    0x8E89BED6,
                                    RAIL_BLE_ADV_CHANNEL,
                                    false);
  tx_requested = true;
}

/*******************************************************************************
 * The API set the rail back to idle state
 ******************************************************************************/
static void set_radio_to_idle_state(RAIL_Handle_t rail_handle)
{
  RAIL_StopTxStream(rail_handle);
  RAIL_Idle(rail_handle, RAIL_IDLE, true);
}

static sl_status_t rail_ble_advertise(RAIL_Handle_t rail_handle,
                                      uint8_t *packet, uint16_t packet_len)
{
  RAIL_Status_t status;

  set_radio_to_idle_state(rail_handle);

  // Add packet to RAIL fifo
  status = RAIL_WriteTxFifo(rail_handle,
                            packet,
                            packet_len,
                            true);

  // Schedule to send packet
  status |= RAIL_StartScheduledTx(rail_handle, 0,
                                  RAIL_TX_OPTIONS_DEFAULT,
                                  &shedule_tx_config, NULL);

  return status == RAIL_STATUS_NO_ERROR ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/*******************************************************************************
 * Use RAIL to transmit a data packet
 ******************************************************************************/
static void handle_transmit(RAIL_Handle_t rail_handle)
{
  rail_ble_advertise(rail_handle,
                     adv_rail_packet, sizeof(adv_rail_packet));
}
