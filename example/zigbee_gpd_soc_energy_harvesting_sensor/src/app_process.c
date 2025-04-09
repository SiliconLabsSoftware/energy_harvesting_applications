/***************************************************************************//**
 * @file app_process.c
 * @brief Implements application specific functions.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "app_init.h"
#include "gpd-components-common.h"
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif
#include "sl_sleeptimer.h"
#include "nvm3.h"
#ifdef SL_CATALOG_ZIGBEE_DEBUG_PRINT_PRESENT
#include "sl_zigbee_debug_print.h"
#endif // SL_CATALOG_ZIGBEE_DEBUG_PRINT_PRESENT

#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
#include "sl_simple_button_instances.h"
#ifdef SL_CATALOG_SIMPLE_BUTTON_BTN0_PRESENT
#include "sl_simple_button_btn0_config.h"
#define BUTTON_INSTANCE_0 sl_button_btn0
#endif //SL_CATALOG_SIMPLE_BUTTON_BTN0_PRESENT
#endif //SL_CATALOG_SIMPLE_BUTTON_PRESENT

#ifdef SL_CATALOG_SIMPLE_LED_PRESENT
#include "sl_simple_led_instances.h"
// LED Indication
#ifdef SL_CATALOG_SIMPLE_LED_LED0_PRESENT
#define ACTIVITY_LED            sl_led_led0 // BOARDLED0
#endif //SL_CATALOG_SIMPLE_LED_LED0_PRESENT
#ifdef SL_CATALOG_SIMPLE_LED_LED1_PRESENT
#define COMMISSIONING_STATE_LED sl_led_led1 // BOARDLED1
#else
#define COMMISSIONING_STATE_LED sl_led_led0 // BOARDLED0
#endif

#define BOARD_LED_ON(led)  sl_led_turn_on(&led)
#define BOARD_LED_OFF(led) sl_led_turn_off(&led)

#else
#define BOARD_LED_ON(led)
#define BOARD_LED_OFF(led)

#endif //SL_CATALOG_SIMPLE_LED_PRESENT

#include "em_cmu.h" // clock management unit
#include "em_burtc.h" // This examples uses BURTC to wake up the device from EM4
#include "sl_rail_util_init.h" // for rail initialization
#include "sl_i2cspm_instances.h" // for i2c instance
#include "sl_i2cspm_mikroe_config.h" // for i2c instance
// PMIC header files
#include "AEM13920_Common.h"
#include "AEM_I2C.h"
#include "HAL_AEM13920.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

#define SL_ZIGBEE_GPD_NV_DATA_TAG       0xA9A1

#define GPD_BUTTON_COUNT                SL_SIMPLE_BUTTON_COUNT

// App button press event types
enum {
  APP_EVENT_ACTION_IDLE = 0,
  APP_EVENT_ACTION_SEND_COMMISSION = 0x01,
  APP_EVENT_ACTION_SEND_DECOMMISSION = 0x02,
  APP_EVENT_ACTION_SEND_TOGGLE = 0x03,
  APP_EVENT_ACTION_SEND_REPORT = 0x04,
  APP_EVENT_ACTION_SLEEPY = 0x05,
};

typedef uint8_t GpdAppEventActionType;

#ifndef APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS
#define APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS 1000
#endif

// If debug print is enabled
#ifdef SL_CATALOG_ZIGBEE_DEBUG_PRINT_PRESENT
#define gpdDebugPrintf                              sl_zigbee_core_debug_print
#else
#define gpdDebugPrintf(...)
#endif

// Duration the device enters EM4 shut off mode
#define EM4_SLEEPY_DURATION_SECOND                  90
// Number of 1 KHz ULFRCO clocks between BURTC interrupts per second
#define BURTC_IRQ_1_SECOND                          1000

// Whether to send storage voltage report. When it's not 1, the device reports fake occupancy sensing data instead.
#define REPORT_STORAGE_VOLTAGE                      1

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

static void processAppEvent(sl_zigbee_gpd_t_t *gpd,
                            GpdAppEventActionType *appAction);
static void leTimeCallback(sl_sleeptimer_timer_handle_t *handle,
                           void *contextData);
static void appSingleEventCommissionTimer(sl_sleeptimer_timer_handle_t *handle,
                                          void *contextData);
static void sendReport(sl_zigbee_gpd_t_t *gpd);

// true to enter EM4; false for EM2
static void gpdEnterLowPowerMode(bool forceEm4);

// Initialize and start BURT
static void startBURTC(uint8_t sec);

// Button hold event
static void buttonReleaseTimeout(sl_sleeptimer_timer_handle_t *handle,
                                 void *contextData);

// Initialize AEM13920
static int32_t initAEM13920(AEM13920_Handler_t *aem_handler,
                            AEM_i2c_cfg *commInfo);

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

static GpdAppEventActionType appAction = APP_EVENT_ACTION_IDLE;
static bool sleepy = false;
static sl_sleeptimer_timer_handle_t le_timer_handle;
static sl_sleeptimer_timer_handle_t app_single_event_commission;

static AEM13920_Handler_t aem13920_handler;
static AEM_i2c_cfg aem13920_i2c_cfg = {
  .usr_callback = NULL,
  .interface = (uint32_t) SL_I2CSPM_MIKROE_PERIPHERAL,
  .slaveAddress = AEM13920_I2CSLAVE_ADDRESS
};
static bool isBtn0Pressed = false, isDischargingMode = false;
static sl_sleeptimer_timer_handle_t button_release_timer_handle;

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * CLI command to send a report
 *****************************************************************************/
void sl_zigbee_gpd_sensor_report_event(void)
{
  appAction = APP_EVENT_ACTION_SEND_REPORT;
}

/******************************************************************************
 * Sets the sleepy flag that will allow the module sleep untill a button
 * is pressed.
 *****************************************************************************/
void sl_zigbee_gpd_sensor_set_sleep(void)
{
  // enable sleep
  sleepy = true;
}

/******************************************************************************
 * Application state machine, called infinitely
 *****************************************************************************/
void app_process_action(void)
{
  sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();
  if (isDischargingMode) {
    BOARD_LED_ON(COMMISSIONING_STATE_LED);
    return;
  } else {
    BOARD_LED_OFF(COMMISSIONING_STATE_LED);
  }

  // Before commissioning completes, enter EM2 while idle
  if ((gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL)
      && (appAction == APP_EVENT_ACTION_IDLE)) {
    gpdEnterLowPowerMode(false);
  } else if (gpd->gpdState == SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
    // Send report while idle, appAction will be changed to SLEEPY afterwards
    if (appAction == APP_EVENT_ACTION_IDLE) {
      appAction = APP_EVENT_ACTION_SEND_REPORT;
    } else if ((appAction == APP_EVENT_ACTION_SLEEPY) && !isBtn0Pressed) { // Do not sleep if button 0 is hold
      sl_zigbee_gpd_sensor_set_sleep();
      gpdEnterLowPowerMode(true);
    }
  }
  // Process application actions
  if (appAction != APP_EVENT_ACTION_IDLE) {
    processAppEvent(gpd, &appAction);
  }
}

void sl_zigbee_gpd_load_le_timer(uint32_t timeInUs)
{
  // in Hz => Time period in micro sec T = 1000000/f
  uint32_t f = sl_sleeptimer_get_timer_frequency();
  // ticks needed = (timeout needed in mico sec)/(T in micro sec) =  ((timeout needed in micro sec) * f)/1000000
  uint32_t tick = (timeInUs * f) / 1000000;
  sl_sleeptimer_restart_timer(&le_timer_handle,
                              tick,
                              leTimeCallback,
                              NULL,
                              0,
                              0);
}

bool sl_zigbee_gpd_le_timer_running(void)
{
  bool running = false;
  sl_sleeptimer_is_timer_running(&le_timer_handle, &running);
  return running;
}

// Application Commissioning that completes all the statest of the commissioning
void sl_zigbee_gpd_app_single_event_commission(void)
{
  sl_sleeptimer_restart_periodic_timer_ms(&app_single_event_commission,
                                          APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS,
                                          appSingleEventCommissionTimer,
                                          NULL,
                                          0,
                                          0);
}

/** @brief Called from the imcomming command handler context for all the incoming
 *         command before the command handler handles it. based on the return code
 *         of this callback the internal command handler skips the processing.
 *
 * @param gpdCommand CommandId.
 * @param length length of the command
 * @param commandPayload The pointer to the commissioning reply payload.
 * Ver.: always
 *
 * @return true if application handled it.
 */
bool sl_zigbee_gpd_af_plugin_incoming_command_cb(uint8_t gpdCommand,
                                                 uint8_t length,
                                                 uint8_t *commandPayload)
{
  gpdDebugPrintf("RX: cmd=%x len=%x payload[", gpdCommand, length);
  if ((commandPayload != NULL)
      && (length > 0)
      && (length < 0xFE)) {
    for (int i = 0; i < length; i++) {
      gpdDebugPrintf("%x", commandPayload[i]);
    }
  }
  gpdDebugPrintf("]\n");
  // Unused variable warning suppression when print is not available.
  (void)gpdCommand;
  (void)length;
  (void)commandPayload;
  return false;
}

// NVM Initialization
void sl_zigbee_gpd_af_plugin_nv_init_cb(void)
{
  // Initialize the NV
  // use default NVM space and handle
  Ecode_t status = nvm3_open(nvm3_defaultHandle, nvm3_defaultInit);
  if (status != ECODE_NVM3_OK) {
    // Handle error
  }

  // Do re-packing if needed
  if (nvm3_repackNeeded(nvm3_defaultHandle)) {
    status = nvm3_repack(nvm3_defaultHandle);
    if (status != ECODE_NVM3_OK) {
      // Handle error
    }
  }
}

/** @brief Called to the application to give a chance to load or store the GPD Context
 *.        in a non volatile context. This can help the application to use any other
 *         non volatile storage.
 *
 * @param nvmData The pointer to the data that needs saving or retrieving to or from
 *                the non-volatile memory.
 * @param sizeOfNvmData The size of the data non-volatile data.
 * @param loadStore indication whether to load or store the context.
 * Ver.: always
 *
 * @return true if application handled it.
 */
bool sl_zigbee_gpd_af_plugin_nv_save_and_load_cb(sl_zigbee_gpd_t_t *gpd,
                                                 uint8_t *nvmData,
                                                 uint8_t sizeOfNvmData,
                                                 EmebrGpdNvLoadStore_t loadStore)
{
  // The application is attempting to read the Green Power Device context from NVM to RAM
  if (loadStore == EMEBER_GPD_AF_CALLBACK_LOAD_GPD_FROM_NVM) {
    uint32_t objectType;
    size_t dataLen;
    // If either the NVM is empty or the object type is not valid,
    // erase the data and write default setting
    if ((0 == nvm3_countObjects(nvm3_defaultHandle))
        || ((nvm3_getObjectInfo(nvm3_defaultHandle,
                                SL_ZIGBEE_GPD_NV_DATA_TAG,
                                &objectType,
                                &dataLen) == ECODE_NVM3_OK)
            && (objectType != NVM3_OBJECTTYPE_DATA))) {
      // Fresh chip , erase, create a storage with default setting.
      // Erase all objects and write initial data to NVM3
      nvm3_eraseAll(nvm3_defaultHandle);
      // First write to the NVM shadow so that it updated with default ones
      sl_zigbee_gpd_copy_to_shadow(gpd);
      // Write the data to NVM
      nvm3_writeData(nvm3_defaultHandle,
                     SL_ZIGBEE_GPD_NV_DATA_TAG,
                     nvmData,
                     sizeOfNvmData);
      // There is valid data existing in NVM, read it to RAM
    } else {
      nvm3_readData(nvm3_defaultHandle,
                    SL_ZIGBEE_GPD_NV_DATA_TAG,
                    nvmData,
                    sizeOfNvmData);
    }
    // The application is attempting to write the Green Power Device context from RAM to NVM
  } else if (loadStore == EMEBER_GPD_AF_CALLBACK_STORE_GPD_TO_NVM) {
    nvm3_writeData(nvm3_defaultHandle,
                   SL_ZIGBEE_GPD_NV_DATA_TAG,
                   nvmData,
                   sizeOfNvmData);
    // This is an exception; there should be only read or write command.
  } else {
    // bad command
  }
  return false;
}

/******************************************************************************
 * Application initialization
 *****************************************************************************/
void app_init(void)
{
  // Initialize NV
  sl_zigbee_gpd_nv_init();

  // Initialize Radio
  sl_zigbee_gpd_radio_init();

  // Provide the rail handle from the configured RAIL instance
  sl_zigbee_gpd_rail_provide_rail_handle(sl_rail_util_get_handle(
                                           SL_RAIL_UTIL_HANDLE_GPDRAIL));

  // Initialize the GPD
  (void)sl_zigbee_gpd_init();

  // Check button 0 state on booting
  if (sl_button_get_state(&BUTTON_INSTANCE_0) == SL_SIMPLE_BUTTON_PRESSED) {
    sl_sleeptimer_restart_timer_ms(&button_release_timer_handle,
                                   5000,
                                   buttonReleaseTimeout,
                                   NULL,
                                   0,
                                   0);
    sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();
    // Enter discharging mode if the device is not in operational mode and button 0 is pressed on booting
    if (gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
      gpdDebugPrintf("Entering discharging mode, press RST button to exit\n");
      isDischargingMode = true;
    }
    isBtn0Pressed = true;
  }

  // initialize AEM13920
  if (initAEM13920(&aem13920_handler,
                   &aem13920_i2c_cfg) != AEM13920_DRIVER_OK) {
    isDischargingMode = true; // Enter discharging mode if encountering any failure
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * Application event processing
 *****************************************************************************/
static void processAppEvent(sl_zigbee_gpd_t_t *gpd,
                            GpdAppEventActionType *appAction)
{
  // If appAction is marked as APP_EVENT_ACTION_SLEEPY, the device is about to sleepy.
  // Do nothing and wait for entering EM4 in app_process_action().
  if (*appAction == APP_EVENT_ACTION_SLEEPY) {
    return;
  }

  // Check what action the device is going to do based on the action type
  if (*appAction != APP_EVENT_ACTION_IDLE) {
    BOARD_LED_ON(ACTIVITY_LED);
    // Send decommissioning GPDF to paired device and save GPD context.
    if (*appAction == APP_EVENT_ACTION_SEND_DECOMMISSION) {
      sl_zigbee_gpd_af_plugin_de_commission(gpd);
      sl_zigbee_gpd_store_sec_data_to_nv(gpd);
      gpdDebugPrintf("Decomm Cmd : ");
      // Perform commissioning process based on current GPD state
    } else if (*appAction == APP_EVENT_ACTION_SEND_COMMISSION) {
      sl_zigbee_gpd_af_plugin_commission(gpd);
      // Save current GPD state if required
      if (gpd->gpdState > SL_ZIGBEE_GPD_APP_STATE_NOT_COMMISSIONED) {
        sl_zigbee_gpd_store_sec_data_to_nv(gpd);
      }
      gpdDebugPrintf("Comm. Cmd : ");
      // Send report to the paired device and save GPD context
    } else if (*appAction == APP_EVENT_ACTION_SEND_REPORT) {
      sendReport(gpd);
      sl_zigbee_gpd_store_sec_data_to_nv(gpd);
      gpdDebugPrintf("Reporting : ");
      *appAction = APP_EVENT_ACTION_SLEEPY;
      return;
    }
    gpdDebugPrintf("Comm. State :%d\n", gpd->gpdState);
    *appAction = APP_EVENT_ACTION_IDLE;
    BOARD_LED_OFF(ACTIVITY_LED);
  }
}

static void leTimeCallback(sl_sleeptimer_timer_handle_t *handle,
                           void *contextData)
{
  (void)contextData;
  sl_sleeptimer_stop_timer(handle);
}

static void appSingleEventCommissionTimer(sl_sleeptimer_timer_handle_t *handle,
                                          void *contextData)
{
  (void)contextData;
  sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();
  if (gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
    appAction = APP_EVENT_ACTION_SEND_COMMISSION;
  } else {
    sl_sleeptimer_stop_timer(handle);
  }
}

static void sendReport(sl_zigbee_gpd_t_t *gpd)
{
  uint32_t volt = 0;

  (void)AEM13920_GetStorageVoltage(&aem13920_handler, &volt);
#if REPORT_STORAGE_VOLTAGE
  uint8_t command[] = {
    GP_CMD_ATTRIBUTE_REPORTING,
    0x01, 0x00, // Cluster Id, power configuration cluster
    0x20, 0x00, // Attribute Id, Battery Voltage
    0x20,       // Attribute Type, uint8
    (uint8_t)(volt / 100) // The unit is 100mV
  };
#else
  // If not reporting storage voltage, generate fake occupancy sensing report
  uint8_t command[] = {
    GP_CMD_ATTRIBUTE_REPORTING,
    0x06, 0x04, // Cluster Id, occupancy sensing cluster
    0x00, 0x00, // Attribute Id, occupancy
    0x18,       // Attribute Type, map8
    0x01        // Attribute Value, occupied
  };
#endif
  gpd->rxAfterTx = false; // Turn off GPD RX after TX feature
  sl_zigbee_af_gpdf_send(SL_ZIGBEE_GPD_NWK_FC_FRAME_TYPE_DATA,
                         gpd,
                         command,
                         sizeof(command),
                         SL_ZIGBEE_AF_PLUGIN_APPS_CMD_RESEND_NUMBER);
}

// Low Power Mode with option to force EM4 or EM2 mode.
static void gpdEnterLowPowerMode(bool forceEm4)
{
  if (forceEm4) {
    EMU_EM4Init_TypeDef em4Init = EMU_EM4INIT_DEFAULT;

    em4Init.pinRetentionMode = emuPinRetentionEm4Exit;
    em4Init.em4State = emuEM4Shutoff;
    EMU_EM4Init(&em4Init);

    startBURTC(EM4_SLEEPY_DURATION_SECOND);
    gpdDebugPrintf("EM4\n");
    EMU_EnterEM4();
  } else {
    gpdDebugPrintf("EM2\n");
    EMU_EnterEM2(true);
  }
}

// Initialize and start a BURTC timer
static void startBURTC(uint8_t sec)
{
  CMU_ClockSelectSet(cmuClock_EM4GRPACLK, cmuSelect_ULFRCO);
  CMU_ClockEnable(cmuClock_BURTC, true);
  CMU_ClockEnable(cmuClock_BURAM, true);

  BURTC_Init_TypeDef burtcInit = BURTC_INIT_DEFAULT;
  burtcInit.start = false;
  burtcInit.compare0Top = true; // reset counter when counter reaches compare value
  burtcInit.em4comp = true;     // BURTC compare interrupt wakes from EM4 (causes reset)
  BURTC_Init(&burtcInit);

  BURTC_CounterReset();
  BURTC_CompareSet(0, sec * BURTC_IRQ_1_SECOND);

  BURTC_IntEnable(BURTC_IEN_COMP);    // compare match
  NVIC_EnableIRQ(BURTC_IRQn);
  BURTC_Enable(true);
}

void BURTC_IRQHandler(void)
{
  BURTC_IntClear(BURTC_IF_COMP); // compare match
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

// Button Events
static void buttonReleaseTimeout(sl_sleeptimer_timer_handle_t *handle,
                                 void *contextData)
{
  (void)contextData;
  sl_sleeptimer_stop_timer(handle);
  appAction = APP_EVENT_ACTION_SEND_DECOMMISSION;
}

void sl_button_on_change(const sl_button_t *handle)
{
  sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();
  if ((sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)
      && (&BUTTON_INSTANCE_0 == handle)) {
    if (gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
      appAction = APP_EVENT_ACTION_SEND_COMMISSION;
    } else {
      appAction = APP_EVENT_ACTION_SEND_REPORT;
    }
    sl_sleeptimer_restart_timer_ms(&button_release_timer_handle,
                                   5000,
                                   buttonReleaseTimeout,
                                   NULL,
                                   0,
                                   0);
    isBtn0Pressed = true;
  } else {
    sl_sleeptimer_stop_timer(&button_release_timer_handle);
    isBtn0Pressed = false;
  }
}
