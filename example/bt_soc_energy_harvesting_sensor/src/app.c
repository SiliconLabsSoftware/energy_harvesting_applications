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

static AEM13920_Handler_t aem13920_handler;
static AEM_i2c_cfg aem13920_i2c_cfg = {
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

static int32_t initAEM13920(AEM13920_Handler_t *aem_handler,
                            AEM_i2c_cfg *commInfo);

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

  initAEM13920(&aem13920_handler, &aem13920_i2c_cfg);
}

static int32_t initAEM13920(AEM13920_Handler_t *aem_handler,
                            AEM_i2c_cfg *commInfo)
{
  int32_t ret = AEM13920_DRIVER_OK;
  AEM13920_CONFIG_t aem_cfg;
  aem_handler->i2c_cfg = commInfo;

  ret = AEM13920_Initialize(aem_handler);

  if (ret == AEM13920_DRIVER_OK) {
    ret = AEM13920_GetConfiguration(aem_handler, &aem_cfg);
  }

  if (ret == AEM13920_DRIVER_OK) {
    aem_cfg.src1_regu_mode = AEM13920_SRCREGU_CONST;
    aem_cfg.src1_const_voltage = 600;
    aem_cfg.src1_boost_tmult = AEM13920_TMULT3;
    aem_cfg.src1_boost_enable = true;
    aem_cfg.src1_boost_high_power_enable = true;
    aem_cfg.src1_low_thresh = AEM13920_SRCLOW_THRESH_112;

    aem_cfg.src2_regu_mode = AEM13920_SRCREGU_MPPT;
    aem_cfg.src2_mppt_ratio = AEM13920_MPPT_RATIO_75;
    aem_cfg.src2_mppt_duration = AEM13920_MPPT_DUR8;
    aem_cfg.src2_mppt_period = AEM13920_MPPT_PER512;
    aem_cfg.src2_boost_tmult = AEM13920_TMULT3;
    aem_cfg.src2_boost_enable = true;
    aem_cfg.src2_boost_high_power_enable = true;
    aem_cfg.src2_low_thresh = AEM13920_SRCLOW_THRESH_112;

    aem_cfg.vovdis = 2500;
    aem_cfg.vchrdy = 2550;
    aem_cfg.vovch = 3800;

    aem_cfg.buck_vout = AEM13920_VOUT_2200;
    aem_cfg.buck_tmult = AEM13920_TMULT4;

    aem_cfg.temp_mon_enable = true;
    aem_cfg.temp_rdiv = 22000000;
    aem_cfg.temp_cold_ch_rth = 98180087;
    aem_cfg.temp_hot_ch_rth = 2261276;
    aem_cfg.temp_cold_dis_rth = 98180087;
    aem_cfg.temp_hot_dis_rth = 2261276;

    aem_cfg.apm_src1_enable = false;
    aem_cfg.apm_src2_enable = false;
    aem_cfg.apm_buck_enable = false;
    aem_cfg.apm_mode = AEM13920_APM_MODE_POWER_METER;  // Ignored as APM is disabled
    aem_cfg.apm_window = AEM13920_APM_WINDOW_128;     // Ignored as APM is disabled

    aem_cfg.i2c_rdy_irq_enable = true;
    aem_cfg.apm_done_irq_enable = false;
    aem_cfg.apm_err_irq_enable = false;
    aem_cfg.src_low_irq_enable = false;
    aem_cfg.src1_mppt_start_irq_enable = false;
    aem_cfg.src1_mppt_done_irq_enable = false;
    aem_cfg.src2_mppt_start_irq_enable = false;
    aem_cfg.src2_mppt_done_irq_enable = false;
    aem_cfg.vovdis_irq_enable = false;
    aem_cfg.vchrdy_irq_enable = false;
    aem_cfg.vovch_irq_enable = false;
    aem_cfg.sto_done_irq_enable = false;
    aem_cfg.temp_ch_irq_enable = false;
    aem_cfg.temp_dis_irq_enable = false;
    aem_cfg.temp_done_irq_enable = false;

    // - Write the updated configuration to the I2C registers,
    // - Start the synchronization of the registers,
    // - Wait for it to complete
    ret = AEM13920_SetConfiguration(aem_handler, &aem_cfg, true);
  }

  return ret;
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

      // Reset and stop BURTC counter until advertising timeout.
      BURTC_CounterReset();
      BURTC_Stop();
      BURTC_SyncWait(); // Wait for the stop to synchronize

      if (AEM13920_DRIVER_OK
          == AEM13920_GetStorageVoltage(&aem13920_handler, &storage_voltage)) {
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
