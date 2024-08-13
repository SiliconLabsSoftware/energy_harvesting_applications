/***************************************************************************//**
 * @file
 * @brief Core application logic.
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
#include "sl_bluetooth.h"
#include "sl_simple_led_instances.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "app_log.h"

// NVM keys are in range from 0x4000 to 0x5FFF
#define KEY_NUM_STORED_SWITCH  0x4065

/*
 * Timer to validate state transitions in milliseconds
 * A SW implemented debounce mechanism to prevent premature transmissions
 * from the kinetic switch from changing the state of the light bulb.
 * Only frames received at an interval > 1000ms will lead to state-changes
 */
#define SW_TIMEOUT             1000

static sl_sleeptimer_timer_handle_t sw_timer;
static void sw_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data);

static bool sw_valid_action = true;
static uint8_t sw_state;

/**************************************************************************//**
 * static variable declaration.
 *****************************************************************************/
// target device's name
static uint8_t sw_name[] = "EH Switch";
static uint8_t se_name[] = "EH Sensor";

/**************************************************************************//**
 * static function declaration.
 *****************************************************************************/
static uint8_t find_name_in_advertisement(uint8_t *data, uint8_t len);
static void scanner_legacy_advertisement_report_handle(sl_bt_msg_t *evt);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
  app_log(
    "\r\n<<<< Bluetooth - SoC Energy Harvesting Application Observer >>>>\r\n");
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Get the status of switch
      sl_bt_nvm_load(KEY_NUM_STORED_SWITCH,
                     sizeof(sw_state),
                     NULL,
                     &sw_state);

      if (sw_state) {
        sl_led_turn_on(&sl_led_led0);
      } else {
        sl_led_turn_off(&sl_led_led0);
      }

      sc = sl_bt_scanner_set_parameters(sl_bt_scanner_scan_mode_passive,
                                        4, // interval = 2.5 ms
                                        4); // window = 2.5 ms
      app_assert_status(sc);

      // start scanning devices
      sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                               sl_bt_scanner_discover_observation);
      app_assert_status(sc);
      app_log("\r\nStart scanning\r\n\r\n");
      break;

    case sl_bt_evt_scanner_legacy_advertisement_report_id:
      scanner_legacy_advertisement_report_handle(evt);
      break;
    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      break;

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

static void scanner_legacy_advertisement_report_handle(sl_bt_msg_t *evt)
{
  sl_bt_evt_scanner_legacy_advertisement_report_t *resp =
    &evt->data.evt_scanner_legacy_advertisement_report;
  uint8_t *adv_data =
    evt->data.evt_scanner_legacy_advertisement_report.data.data;
  uint8_t adv_len =
    evt->data.evt_scanner_legacy_advertisement_report.data.len;
  uint8_t *device_name = NULL;
  uint16_t se_data = 0;

  // An EH Switch is found
  if (find_name_in_advertisement(adv_data, adv_len) == 1) {
    bool timer_is_running;
    sl_status_t sc;
    if (sw_valid_action == true) {
      sw_valid_action = false;

      device_name = sw_name;
      if (sw_state) {
        sw_state = 0;
        sl_led_turn_off(&sl_led_led0);
      } else {
        sw_state = 1;
        sl_led_turn_on(&sl_led_led0);
      }

      // Store new state of the switch to NVM
      sc = sl_bt_nvm_save(KEY_NUM_STORED_SWITCH,
                          sizeof(sw_state),
                          (const uint8_t *)&sw_state);
      app_assert_status(sc);
    }

    sl_sleeptimer_is_timer_running(&sw_timer,
                                   &timer_is_running);
    if (timer_is_running == false) {
      // Start a timer to validate actions
      sl_sleeptimer_start_timer_ms(&sw_timer,
                                   SW_TIMEOUT,
                                   sw_timer_cb,
                                   NULL,
                                   0,
                                   0);
    }
  }
  // An EH Sensor is found
  else if (find_name_in_advertisement(adv_data, adv_len) == 2) {
    device_name = se_name;
    se_data = ((uint16_t)adv_data[7] << 8) + adv_data[8];
  }

  if (device_name) {
    app_log("\r\n>>> [%ld] Found an %s device: %02X:%02X:%02X:%02X:%02X:%02X,"
            " channel: %d, rssi: %d",
            sl_sleeptimer_get_tick_count(),
            device_name,
            resp->address.addr[5],
            resp->address.addr[4],
            resp->address.addr[3],
            resp->address.addr[2],
            resp->address.addr[1],
            resp->address.addr[0],
            resp->channel,
            resp->rssi);
    if (device_name == sw_name) {
      app_log_append(", toggle command received, LED: %s\r\n",
                     (sw_state) ? "ON" : "OFF");
    } else {
      app_log_append(", read storage voltage: %d mV\r\n", se_data);
    }
    app_log("---------------------------------\r\n");
  }
}

/***************************************************************************//**
 * Parse advertisements looking for the name of the peripheral device
 * @param[in] data: Advertisement packet
 * @param[in] len:  Length of the advertisement packet
 * @return
 *   - \ref 0 - if no device is found
 *   - \ref 1 - if a Switch device is found
 *   - \ref 2 - if a Sensor device is found
 ******************************************************************************/
static uint8_t find_name_in_advertisement(uint8_t *data, uint8_t len)
{
  uint8_t ad_field_length;
  uint8_t ad_field_type;
  uint8_t i = 0;

  // Parse advertisement packet
  while (i < len) {
    ad_field_length = data[i];
    ad_field_type = data[i + 1];
    // Shortened Local Name ($08) or Complete Local Name($09)
    if ((ad_field_type == 0x08) || (ad_field_type == 0x09)) {
      // compare name
      if (memcmp(&data[i + 2], sw_name, (ad_field_length - 1)) == 0) {
        return 1;
      } else if (memcmp(&data[i + 2], se_name, (ad_field_length - 1)) == 0) {
        return 2;
      }
    }
    // advance to the next AD struct
    i = i + ad_field_length + 1;
  }
  return 0;
}

static void sw_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data)
{
  (void)timer;
  (void)data;

  sw_valid_action = true;
}
