/***************************************************************************//**
 * @file app_process.c
 * @brief Implements application specific functions.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
#include "sl_simple_button_instances.h"
#ifdef SL_CATALOG_SIMPLE_BUTTON_BTN0_PRESENT
#include "sl_simple_button_btn0_config.h"
#define BUTTON_INSTANCE_0 sl_button_btn0
#endif // SL_CATALOG_SIMPLE_BUTTON_BTN0_PRESENT
#endif // SL_CATALOG_SIMPLE_BUTTON_PRESENT

#include "sl_rail_util_init.h" // for rail initialization
#include "em_msc.h"
#include "em_cmu.h" // clock management unit

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// App button press event types
typedef enum {
  APP_EVENT_ACTION_IDLE = 0,
  APP_EVENT_ACTION_SEND_COMMISSION = 0x01,
  APP_EVENT_ACTION_SEND_DECOMMISSION = 0x02,
  APP_EVENT_ACTION_COMMISSION_DONE = 0x03,
  APP_EVENT_ACTION_SEND_TOGGLE = 0x04,
  APP_EVENT_ACTION_DISCHARGE = 0x05,
} app_event_action_t;

typedef uint8_t GpdAppEventActionType;

#ifndef APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS
#define APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS 1000
#endif

#define NVM_START_ADDRESS                           0x00020000
#define NVM_END_ADDRESS                             0x00070000
#define NVM_INVALID_ADDR                            0xFFFFFFFF
#define NVM_BLANK_DATA                              { 0xFF, 0xFF, 0xFF, 0xFF }

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

static void processAppEvent(sl_zigbee_gpd_t_t *gpd,
                            GpdAppEventActionType *appAction);
static void leTimeCallback(sl_sleeptimer_timer_handle_t *handle,
                           void *contextData);
static void appSingleEventCommissionTimer(sl_sleeptimer_timer_handle_t *handle,
                                          void *contextData);
static void sendToggle(sl_zigbee_gpd_t_t *gpd);

// Set sleeptimer and them enter EM2
static void gpdEnterEM2(void);

static uint8_t *findLastValidNvmChunk(uint8_t *startAddr, uint8_t *endAddr);

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

static GpdAppEventActionType appAction = APP_EVENT_ACTION_IDLE;
static sl_sleeptimer_timer_handle_t le_timer_handle;
static sl_sleeptimer_timer_handle_t app_single_event_commission;
static bool is_comm_info_stored = false;

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
 * Application state machine, called infinitely
 *****************************************************************************/
void app_process_action(void)
{
  sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();

  // Has the GPD completed commissioning process?
  if ((gpd->gpdState == SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL)
      && (appAction == APP_EVENT_ACTION_IDLE)) {
    if (is_comm_info_stored == false) {
      // Commissioning is just done, save GPD state in NVM
      appAction = APP_EVENT_ACTION_COMMISSION_DONE;
    } else {
      // Send toggling command, appAction will be changed to APP_EVENT_ACTION_DISCHARGE afterwards
      appAction = APP_EVENT_ACTION_SEND_TOGGLE;
    }
  } else if (gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
    // Commissioning not done yet, enter EM2 and proceeds the net action after APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS
    if (appAction == APP_EVENT_ACTION_IDLE) {
      gpdEnterEM2();
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
  if ((commandPayload != NULL)
      && (length > 0)
      && (length < 0xFE)) {
    // Access the incoming command here
  }

  // Unused variable warning suppression
  (void)gpdCommand;
  (void)length;
  (void)commandPayload;
  return false;
}

// NVM Initialization
void sl_zigbee_gpd_af_plugin_nv_init_cb(void)
{
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
  uint8_t *validNvmDataAddr =
    findLastValidNvmChunk((uint8_t *)NVM_START_ADDRESS,
                          (uint8_t *)NVM_END_ADDRESS);
  (void)gpd;
  // The application is attempting to read the Green Power Device context from NVM to RAM
  if (loadStore == EMEBER_GPD_AF_CALLBACK_LOAD_GPD_FROM_NVM) {
    // If either the NVM is empty or the object type is not valid,
    // erase the data and write default setting
    if (validNvmDataAddr == (uint8_t *)NVM_INVALID_ADDR) {
      sl_zigbee_gpd_copy_to_shadow(gpd);
      MSC_Init();
      MSC_ErasePage((uint32_t *)NVM_START_ADDRESS);
      MSC_WriteWord((uint32_t *)NVM_START_ADDRESS,
                    (void *)nvmData,
                    sizeOfNvmData);
      MSC_Deinit();
      // Read data from NVM to RAM if there is existing data
    } else {
      memcpy(nvmData, validNvmDataAddr, sizeOfNvmData);
    }
    // The application is attempting to write the Green Power Device context from RAM to NVM
  } else if (loadStore == EMEBER_GPD_AF_CALLBACK_STORE_GPD_TO_NVM) {
    uint8_t *nextAddr = (validNvmDataAddr + SL_ZIGBEE_GPD_NV_DATA_SIZE);
    MSC_Init();
    // TODO: NVM is full
    // The power is not sufficient to perform flash erasing
    if (nextAddr >= (uint8_t *)NVM_END_ADDRESS) {
      // MSC_ErasePage((uint32_t *)NVM_START_ADDRESS);
      // nextAddr = NVM_START_ADDRESS;
    }
    MSC_WriteWord((uint32_t *)nextAddr, (void *)nvmData, sizeOfNvmData);
    MSC_Deinit();
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

  // Provide the rail handle from the configured RAIL instance
  sl_zigbee_gpd_rail_provide_rail_handle(sl_rail_util_get_handle(
                                           SL_RAIL_UTIL_HANDLE_GPDRAIL));

  // Initialize Radio
  sl_zigbee_gpd_radio_init();

  // Initialize the GPD
  sl_zigbee_gpd_init();

  // If button 0 is pressed on booting, perform de-commissioning.
  if (sl_button_get_state(&BUTTON_INSTANCE_0) == SL_SIMPLE_BUTTON_PRESSED) {
    appAction = APP_EVENT_ACTION_SEND_DECOMMISSION;
  } else {
    sl_zigbee_gpd_t_t *gpd = sl_zigbee_gpd_get_gpd();
    // In this example, GPD state is not stored in NVM until commissioning is done
    // The flag is_comm_info_stored is to track whether the commissioning information has been stored or not.
    is_comm_info_stored =
      (gpd->gpdState == SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) ? true : false;
    // The GPD has not completed commissioning process?
    if (gpd->gpdState < SL_ZIGBEE_GPD_APP_STATE_OPERATIONAL) {
      RAIL_SetTxPowerDbm(sl_rail_util_get_handle(SL_RAIL_UTIL_HANDLE_GPDRAIL),
                         -60);                                                       // Lower TX power(-6dBm) because the power is more restricted in commissioning
      appAction = APP_EVENT_ACTION_SEND_COMMISSION; // Perform commissioning action
    }
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
  // If appAction is marked as APP_EVENT_ACTION_DISCHARGE, the device has completed all its tasks.
  // Do nothing and the power will be drained.
  if (*appAction == APP_EVENT_ACTION_DISCHARGE) {
    return;
  }

  // Check what action the device is going to do based on the action type
  if (*appAction != APP_EVENT_ACTION_IDLE) {
    switch (*appAction)
    {
      // Send decommissioning GPDF to paired device and save GPD context.
      case APP_EVENT_ACTION_SEND_DECOMMISSION:
        sl_zigbee_gpd_af_plugin_de_commission(gpd);
        sl_zigbee_gpd_store_sec_data_to_nv(gpd);
        break;
      // Perform commissioning process based on current GPD state
      // We don't write NVM during commissioning to ensure the minimal up time.
      case APP_EVENT_ACTION_SEND_COMMISSION:
        sl_zigbee_gpd_af_plugin_commission(gpd);
        break;
      case APP_EVENT_ACTION_COMMISSION_DONE:
        // Save the GPD state once commissioning is done
        sl_zigbee_gpd_store_sec_data_to_nv(gpd);
        is_comm_info_stored = true;
        break;
      // Send toggle to the paired device and save GPD context
      case APP_EVENT_ACTION_SEND_TOGGLE:
        sendToggle(gpd);
        sl_zigbee_gpd_store_sec_data_to_nv(gpd);
        // No need to save the power after toggling
        *appAction = APP_EVENT_ACTION_DISCHARGE;
        return;
      default:
        break;
    }
    *appAction = APP_EVENT_ACTION_IDLE;
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
  }
  sl_sleeptimer_stop_timer(handle);
}

static void sendToggle(sl_zigbee_gpd_t_t *gpd)
{
  uint8_t command[] = { GP_CMD_TOGGLE }; // GPD Toggle Command
  gpd->rxAfterTx = false;
  sl_zigbee_af_gpdf_send(SL_ZIGBEE_GPD_NWK_FC_FRAME_TYPE_DATA,
                         gpd,
                         command,
                         sizeof(command),
                         SL_ZIGBEE_AF_PLUGIN_APPS_CMD_RESEND_NUMBER);
}

// Set sleeptimer and then enter EM2
static void gpdEnterEM2(void)
{
  sl_sleeptimer_start_timer_ms(&app_single_event_commission,
                               APP_SINGLE_EVENT_COMMISSIONING_LOOP_TIME_MS,
                               appSingleEventCommissionTimer,
                               NULL,
                               0,
                               0);
  EMU_EnterEM2(true);
}

/******************************************************************************
 * Find the latest chunk between startAddr and endAddr
 *****************************************************************************/
static uint8_t *findLastValidNvmChunk(uint8_t *startAddr, uint8_t *endAddr)
{
  uint8_t *chkAddr = startAddr;
  uint8_t *ret = (uint8_t *)NVM_INVALID_ADDR;
  uint8_t myMask[] = NVM_BLANK_DATA;
  while (chkAddr <= endAddr) {
    if (memcmp(chkAddr, myMask, sizeof(myMask)) == 0) {
      // The first chunk is blank, it means no NVM data being written at all
      // Return NVM_INVALID_ADDR to load default configurations from RAM
      if (chkAddr == startAddr) {
        ret = (uint8_t *)NVM_INVALID_ADDR;
        break;
        // If the chunk is blank, return the address of previous chunk which contains valid value
      } else {
        ret = (chkAddr - SL_ZIGBEE_GPD_NV_DATA_SIZE);
        break;
      }
    }
    chkAddr += SL_ZIGBEE_GPD_NV_DATA_SIZE;
  }
  return ret;
}
