/***************************************************************************//**
 * @file
 * @brief app.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "em_system.h"
#include "sl_component_catalog.h"
#include "app_rail_ble.h"
#include "app_assert.h"
#include "app.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

/// Size of the FIFO buffers
#define RAIL_FIFO_SIZE                              256
#define RAIL_NUM_OF_TX                              60
#define TX_INTERVAL_US                              30
#define RAIL_BLE_ADV_CHANNEL                        37
#define RAIL_CRC_INIT                               0x555555
#define RAIL_ACCESS_ADDR                            0x8E89BED6
#define RAIL_ADV_PACKET_NUMBER                      10
#define ADV_RAIL_ADVA_OFFSET                        2

#define ADV_RAIL_VOLTAGE_OFFSET                     15
#define EM2_SLEEP_PERIOD_MS                         (1000)
#define EM4_SLEEP_PERIOD_MS                         (25000)

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

/*******************************************************************************
 * Used to force device go into EM2.
 ******************************************************************************/
static void enter_em2(uint32_t period_ms);

/*******************************************************************************
 * Used to force device go into EM4.
 ******************************************************************************/
static void enter_em4(uint32_t period_ms);

/*******************************************************************************
 * Used to configure AEM19320
 ******************************************************************************/
static int32_t initAEM13920(AEM13920_Handler_t *aem_handler,
                            AEM_i2c_cfg *commInfo);

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

/// / CLI transmit request has occurred)
volatile bool tx_requested = false;
static uint8_t adv_count = 0;
static uint32_t storage_voltage;
static uint16_t u16_storage_voltage;

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

/// Contains the status of RAIL Calibration
static volatile RAIL_Status_t calibration_status = 0;

/// Transmit FIFO
static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t tx_fifo[RAIL_FIFO_SIZE];

static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t adv_rail_packet[] =
{ 0x02,     // packet type: AUX_ADV_IND, non-connectable
  0x1A,     // length: 29
  0x2f, 0x45, 0x0a, 0x6f, 0x0d, 0x00,     // AdvA: advertiser address
  0x02,     // adv data length 2
  0x01,     // adv data type 1 (flags)
  0x06,     // flags: LE General Discoverable Mode, BR/EDR Not Supported
  0x05,     // adv data length 4
  0xff,     // adv data type: manufacturer specific
  0xff, 0x02,  // manufacturer: Silicon Labs
  0x00, 0x00, // storage voltage mv
  0x0A,     // adv data length 10
  0x09,     // adv data type 9 (complete local name)
  'E', 'H', ' ', 'S', 'e', 'n', 's', 'o', 'r' };

static RAIL_ScheduleTxConfig_t shedule_tx_config = {
  .when = TX_INTERVAL_US,
  .mode = RAIL_TIME_DELAY,
  .txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX
};

/// Notify RAIL Tx, Rx calibration error
static bool rail_error = false;

/// Notify end of packet transmission
static bool rail_packet_sent = false;

static AEM13920_Handler_t aem13920_handler;
static AEM_i2c_cfg aem13920_i2c_cfg = {
  .usr_callback = NULL,
  .interface = (uint32_t) SL_I2CSPM_MIKROE_PERIPHERAL,
  .slaveAddress = AEM13920_I2CSLAVE_ADDRESS
};

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * The function is used for some basic initialization related to the app.
 *****************************************************************************/
RAIL_Handle_t app_init(void)
{
  RAIL_Handle_t rail_handle;
  BURTC_Init_TypeDef init_burtc = BURTC_INIT_DEFAULT;
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

  // Init EM4
  EMU_EM4Init(&init_em4);

  initAEM13920(&aem13920_handler, &aem13920_i2c_cfg);

  rail_ble_init(RAIL_BLE_PROTOCOL_BLE_1MBPS);

  // Get RAIL handle, used later by the application
  rail_handle = rail_ble_get_handle();

  // sets the RAIL TX FIFO
  app_set_rail_tx_fifo(rail_handle);

  // initializes the ble adv
  app_rail_ble_adv_init(rail_handle);

  return rail_handle;
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

/******************************************************************************
 * Initialization of RAIL BLE ADV
 *****************************************************************************/
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
                                    RAIL_CRC_INIT,
                                    RAIL_ACCESS_ADDR,
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

/******************************************************************************
 * RAIL BLE start advertising.
 *****************************************************************************/
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
  adv_rail_packet[ADV_RAIL_VOLTAGE_OFFSET] = u16_storage_voltage >> 8;
  adv_rail_packet[ADV_RAIL_VOLTAGE_OFFSET + 1] = u16_storage_voltage & 0xFF;

  rail_ble_advertise(rail_handle,
                     adv_rail_packet, sizeof(adv_rail_packet));
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

/*******************************************************************************
 * Used to force device go into EM4.
 ******************************************************************************/
static void enter_em4(uint32_t period_ms)
{
  init_wakeup_timer(period_ms);
  EMU_EnterEM4();
}

/*******************************************************************************
 * Used to force device go into EM2.
 ******************************************************************************/
static void enter_em2(uint32_t period_ms)
{
  init_wakeup_timer(period_ms);
  EMU_EnterEM2(true);
}

/*******************************************************************************
 * Used to configure AEM19320
 ******************************************************************************/
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

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * RAIL callback, called if a RAIL event occurs
 *****************************************************************************/
void sl_rail_util_on_event(RAIL_Handle_t rail_handle, RAIL_Events_t events)
{
  (void) rail_handle;
  (void) events;

  ///////////////////////////////////////////////////////////////////////////
  // Put your RAIL event handling here!                                    //
  // This is called from ISR context.                                      //
  // Do not call blocking functions from here!                             //
  ///////////////////////////////////////////////////////////////////////////

  // Handle Tx events
  if (events & RAIL_EVENTS_TX_COMPLETION) {
    if ((events & RAIL_EVENT_TX_PACKET_SENT)
        || (events & RAIL_EVENT_TX_SCHEDULED_TX_MISSED)) {
      rail_packet_sent = true;
    } else {
      rail_error = true;
    }
  }

  // Perform all calibrations when needed
  if (events & RAIL_EVENT_CAL_NEEDED) {
    calibration_status =
      RAIL_Calibrate(rail_handle, NULL, RAIL_CAL_ALL_PENDING);
    if (calibration_status != RAIL_STATUS_NO_ERROR) {
      rail_error = true;
    }
  }

#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif
}

/******************************************************************************
 * Application state machine, called infinitely
 *****************************************************************************/
void app_process_action(RAIL_Handle_t rail_handle)
{
#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif

  if (adv_count == 0) {
    (void)AEM13920_GetStorageVoltage(&aem13920_handler, &storage_voltage);
    u16_storage_voltage = (uint16_t)storage_voltage;
  }

  if (tx_requested) {
    tx_requested = false;

    // sends the packet for IEEE 802.15.4 and BLE standards
    handle_transmit(rail_handle);
  }

  if (rail_packet_sent) {
    rail_packet_sent = false;
    tx_requested = true;

    if ((adv_count == RAIL_ADV_PACKET_NUMBER)
        || (adv_count == RAIL_ADV_PACKET_NUMBER * 2)) {
      set_radio_to_idle_state(rail_handle);
      enter_em2(EM2_SLEEP_PERIOD_MS);
    } else if (adv_count == RAIL_ADV_PACKET_NUMBER * 3) {
      adv_count = 0;
      enter_em4(EM4_SLEEP_PERIOD_MS);
    }
    adv_count++;
  }
}

void BURTC_IRQHandler(void)
{
  BURTC_IntClear(BURTC_IF_COMP);
}
