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
 * This software is provided 'as-is', without any express or implied
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
 ******************************************************************************/
#include "em_common.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_burtc.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "sl_clock_manager.h"
#include "sl_i2cspm_instances.h"
#include "HAL_AEM13920.h"
#include "sl_i2cspm_mikroe_config.h"

#define DEBUG_ENABLE                                (0)
#define USER_OVERRIDE_AEM13920_CONFIG               (0)

#define ADVERTISING_EVENT                           (1) // number of advertising events to send
#define EM2_SLEEP_TIME_MS                           (10) // 10ms
#define EM2_SLEEP_COUNT                             (2)
#define EM4_SLEEP_TIME_MS                           (25000) // 25000ms

// Advertising flags (common).
#define ADVERTISE_FLAGS_LENGTH                      0x02
#define ADVERTISE_FLAGS_TYPE                        0x01
#define ADVERTISE_FLAGS_DATA                        0x06

// Complete local name.
#define DEVICE_NAME_LENGTH                          9
#define DEVICE_NAME_TYPE                            0x09
#define DEVICE_NAME                                 "EH Sensor"

// Manufacturer ID (0x02FF - Silicon Labs' company ID)
#define MANUF_ID                                    0x02FF
// 1+2+8 bytes for type, company ID and the payload
#define MANUF_LENGTH                                5
#define MANUF_TYPE                                  0xFF

#if (DEBUG_ENABLE == 1)
#include "app_log.h"
#define LOG_PRINT                                   app_log
#else
#define LOG_PRINT(...)
#endif

#if (USER_OVERRIDE_AEM13920_CONFIG == 1)
// AEM13920 configurations, for more information, please refer to AEM13920 datasheet.
#define AEM13920_OVERCHARGE_VOLTAGE_THRESHOLD       3788  // VOVCH(Storage element overcharge threshold, used to protect the super cap from overcharge) 3.788V
#define AEM13920_CHARGE_READY_VOLTAGE_THRESHOLD     2456  // VCHRDY(Storage element minimum voltage before starting to supply, should be slightly higher than VOVDIS) 2.456V
#define AEM13920_DISCHARGE_VOLTAGE_THRESHOLD        2419  // VOVDIS(Storage element overdischarge threshold, used to prevent the super cap from being drained) 2419
#define AEM13920_BUCK_OUT_VOLTAGE                   AEM13920_VLOAD_2200  // BUCKCFG(To set the regulation output voltage as 2.2V)
#endif

SL_PACK_START(1)
typedef struct
{
  uint8_t len_flags;
  uint8_t type_flags;
  uint8_t val_flags;

  uint8_t len_manuf;
  uint8_t type_manuf;
  // First two bytes must contain the manufacturer ID (little-endian order)
  uint8_t company_LO;
  uint8_t company_HI;

  uint16_t storage_voltage;

  // length of the name AD element is variable,
  // adding it last to keep things simple
  uint8_t len_name;
  uint8_t type_name;

  // NAME_MAX_LENGTH must be sized
  // so that total length of data does not exceed 31 bytes
  uint8_t name[DEVICE_NAME_LENGTH];

  // These values are NOT included in the actual advertising payload,
  // just for bookkeeping
  char dummy;        // Space for null terminator
  uint8_t data_size; // Actual length of advertising data
} SL_ATTRIBUTE_PACKED advertising_packet_t;
SL_PACK_END()

/**************************************************************************//**
 * Local variables.
 *****************************************************************************/
// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

// Advertising data.
static advertising_packet_t advertising_data = {
  .len_flags = ADVERTISE_FLAGS_LENGTH,
  .type_flags = ADVERTISE_FLAGS_TYPE,
  .val_flags = ADVERTISE_FLAGS_DATA,

  .len_manuf = MANUF_LENGTH,
  .type_manuf = MANUF_TYPE,
  .company_LO = MANUF_ID & 0xFF,
  .company_HI = (MANUF_ID >> 8) & 0xFF,

  // length of name element is the name string length + 1 for the AD type
  .len_name = DEVICE_NAME_LENGTH + 1,
  .type_name = DEVICE_NAME_TYPE,

  // Initialize for custom data
  .storage_voltage = 0,

  .name = DEVICE_NAME,
  // Calculate total length of advertising data
  .data_size = 3 + (1 + MANUF_LENGTH) + (1 + DEVICE_NAME_LENGTH + 1),
};

const AEM_i2c_cfg aem13920 = {
  .usr_callback = NULL,
  .interface = (uint32_t) SL_I2CSPM_MIKROE_PERIPHERAL,
  .slaveAddress = AEM13920_I2CSLAVE_ADDRESS
};

/**************************************************************************//**
 * Local function prototypes.
 *****************************************************************************/
// Advertiser time out event handler.
static void bt_advertiser_timeout_event_handle(void);
static void init_wakeup_timer(uint32_t period_ms);
static void enter_em2(uint32_t period_ms);
static void enter_em4(uint32_t period_ms);

#if (USER_OVERRIDE_AEM13920_CONFIG == 1)
static int32_t initAEM13920(const AEM_i2c_cfg *commInfo);

#endif

static uint32_t storage_voltage;

/***************************************************************************//**
 * @brief
 *    Application initialization for sensor device.
 *
 ******************************************************************************/

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
  LOG_PRINT("BLE - Optimized energy consuming sensor\r\n");

  BURTC_Init_TypeDef init_burtc = BURTC_INIT_DEFAULT;
  EMU_EM23Init_TypeDef init_em23 = EMU_EM23INIT_DEFAULT;
  EMU_EM4Init_TypeDef init_em4 = EMU_EM4INIT_DEFAULT;

  // Select reference clock/oscillator for the desired clock branch (BURTC Clk).
  // Reference selected for clocking: ULFRCO
  CMU_ClockSelectSet(cmuClock_BURTC, cmuSelect_ULFRCO);

  // Enable BURTC Clk
  CMU_ClockEnable(cmuClock_BURTC, true);

  // Enable BURTC
  BURTC_Enable(true);

  init_burtc.compare0Top = true;
  init_burtc.em4comp = true;
  init_burtc.em4overflow = true;

  // Enable Interrupt from BURTC
  NVIC_EnableIRQ(BURTC_IRQn);
  // Enable compare interrupt flag
  BURTC_IntEnable(BURTC_IF_COMP);

  // Init BURTC
  BURTC_Init(&init_burtc);

  // Init EM2
  EMU_EM23Init(&init_em23);

  // Init EM4
  EMU_EM4Init(&init_em4);

#if (USER_OVERRIDE_AEM13920_CONFIG == 1)
  initAEM13920(&aem13920);
#endif
}

#if (USER_OVERRIDE_AEM13920_CONFIG == 1)
static int32_t initAEM13920(const AEM_i2c_cfg *commInfo)
{
  int32_t ret = AEM13920_DRIVER_OK;

  AEM_I2C_Initialize(commInfo);

  // default 0x3A(3.788V), this examples uses the same config.
  ret = AEM13920_SetOverchargeVoltage(commInfo,
                                      AEM13920_OVERCHARGE_VOLTAGE_THRESHOLD);

  // Only set next configuration while previous one succeeds
  // default 0x05(2.55V), this example uses 0(2.456V)
  if (ret == AEM13920_DRIVER_OK) {
    ret = AEM13920_SetChargeReadyVoltage(commInfo,
                                         AEM13920_CHARGE_READY_VOLTAGE_THRESHOLD);
  }

  // Only set next configuration while previous one succeeds
  // default 0x06(2.513V), this example uses 0x01(2.419V)
  if (ret == AEM13920_DRIVER_OK) {
    ret = AEM13920_SetDischargeVoltage(commInfo,
                                       AEM13920_DISCHARGE_VOLTAGE_THRESHOLD);
  }

  // Only set next configuration while previous one succeeds
  // default 0x00(Buck Converter off), this example uses 0x06(2.2V)
  if (ret == AEM13920_DRIVER_OK) {
    ret = AEM13920_SetBuckVLoad(commInfo,
                                AEM13920_BUCK_OUT_VOLTAGE);
  }

  // Load the configurations into PMIC
  if (ret == AEM13920_DRIVER_OK) {
    ret = AEM13920_LoadConfiguration(commInfo);
  }

  return ret;
}

#endif

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

      // Reset and stop BURTC counter until advertising timeout.
      BURTC_CounterReset();
      BURTC_Stop();
      BURTC_SyncWait(); // Wait for the stop to synchronize

      if (AEM13920_DRIVER_OK
          == AEM13920_GetStorageVoltage(&aem13920, &storage_voltage)) {
        uint16_t u16_storage_voltage = (uint16_t)storage_voltage;
        LOG_PRINT("Storage voltage = %d(mV)\r\n", u16_storage_voltage);

        // Convert to big-endian
        advertising_data.storage_voltage =
          (u16_storage_voltage << 8) | (u16_storage_voltage >> 8);
      }

      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        32,
        32,
        0,
        ADVERTISING_EVENT);
      app_assert_status(sc);

      // Set custom advertising payload
      sc = sl_bt_legacy_advertiser_set_data(
        advertising_set_handle,
        sl_bt_advertiser_advertising_data_packet,
        advertising_data.data_size,
        (uint8_t *)&advertising_data);
      app_assert_status(sc);

      sc = sl_bt_advertiser_set_tx_power(advertising_set_handle, 0, NULL);
      app_assert_status(sc);

      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(
        advertising_set_handle,
        sl_bt_advertiser_scannable_non_connectable);
      app_assert_status(sc);
      LOG_PRINT("Start advertising ...\r\n");

      break;

    case sl_bt_evt_advertiser_timeout_id:
      bt_advertiser_timeout_event_handle();
      break;

    default:
      break;
  }
}

/**************************************************************************//**
 * Handler function for advertiser time out event.
 *****************************************************************************/
static void bt_advertiser_timeout_event_handle(void)
{
  sl_status_t sc;
  static uint8_t wakeup_number = 0;

  if (wakeup_number < EM2_SLEEP_COUNT) {
    LOG_PRINT("EMU_EnterEM 2\r\n");
    enter_em2(EM2_SLEEP_TIME_MS);

    sc = sl_bt_legacy_advertiser_start(
      advertising_set_handle,
      sl_bt_advertiser_scannable_non_connectable);
    app_assert_status(sc);
    LOG_PRINT("Start advertising ...\r\n");
  }
  if (wakeup_number == EM2_SLEEP_COUNT) {
    LOG_PRINT("EMU_EnterEM 4\r\n");
    enter_em4(EM4_SLEEP_TIME_MS);
  }
  wakeup_number++;
}

static void init_wakeup_timer(uint32_t period_ms)
{
  uint32_t freq;
  uint32_t timer_value;

  sl_clock_manager_get_oscillator_frequency(SL_OSCILLATOR_ULFRCO, &freq);
  timer_value = period_ms * freq / 1000;
  BURTC_CounterReset();
  BURTC_CompareSet(0, timer_value - 1);
  BURTC_Start();
  BURTC_SyncWait();
}

static void enter_em2(uint32_t period_ms)
{
  init_wakeup_timer(period_ms);
  EMU_EnterEM2(true);
}

static void enter_em4(uint32_t period_ms)
{
  init_wakeup_timer(period_ms);
  EMU_EnterEM4();
}

/**************************************************************************//**
 * BURTC IRQ Handle function.
 *****************************************************************************/
void BURTC_IRQHandler(void)
{
  // Clear interrupt source
  BURTC_IntClear(BURTC_IEN_COMP);
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
}
