/**
  ******************************************************************************
  * \file	AEM13920_Common.h
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM13920_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \defgroup	AEM13920_Common		Common Definitions
  * @{
  * 
  * \brief	Common Definitions
  * 
  * \version	1.0.0
  * 	- First public release
  * \version	2.0.0
  * 	Breaking Change
  * 	- API revision
  * \version	2.0.1
  * 	Bug Fix
  * 	- Fix wrong masks
  * \version	2.0.2
  * 	Bug Fix
  * 	- Remove useless include
  * 
  ***************************************************************************//*
  *                    ___         _ __   ___  __ _ ___ 
  *                   / _ \  ___  | '_ \ / _ \/ _` / __|
  *                  |  __/ |___| | |_) |  __/ (_| \__ \
  *                   \___|       | .__/ \___|\__,_|___/
  *                               |_|                   
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  *   - Redistributions of source code must retain the above copyright
  *     notice, this list of conditions and the following disclaimer.
  *   - Redistributions in binary form must reproduce the above copyright
  *     notice, this list of conditions and the following disclaimer in
  *     the documentation and/or other materials provided with the
  *     distribution.
  *   - Neither the name of E-PEAS S.A. nor the names of its subsidiaries,
  *     partners, contributors may be used to endorse or promote products
  *     derived from this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  * POSSIBILITY OF SUCH DAMAGE.
  *
  * http://www.e-peas.com
  *****************************************************************************/
#ifndef AEM13920_COMMON_H_
#define AEM13920_COMMON_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @defgroup	AEM13920_STATUS_ERR_CODE	Status/Error code
  * @{
  */
/**
  * @brief 	Operation succeeded
  * 
  * @details 	The function execution completed without error
  */
#define AEM13920_DRIVER_OK 			(0)
/**
  * @brief 	Unspecified error
  * 
  * @details 	An unspecified error occurred during the function 
  * 		execution
  */
#define AEM13920_DRIVER_ERR			(-1)
/**
  * @brief 	Parameter error
  * 
  * @details 	A paramater error occurred during the function execution
  */
#define AEM13920_DRIVER_ERR_PARAMETER		(-2)
/**
  * @brief 	Unsupported feature error
  * 
  * @details 	A error occurred during the function execution because it 
  * 		required an unsupported feature
  */
#define AEM13920_DRIVER_ERR_UNSUPPORTED		(-3)
/**
  * @brief 	Source regulation configuration error
  * 
  * @details 	A error occurred during the function execution because of a
  * 		mismatch in the source regulation configuration. e.g.
  * 		trying to set a constant source regulation voltage, while the 
  * 		source regulation mode is set to MPPT (or inversely)
  */
#define AEM13920_DRIVER_ERR_SRCREGU		(-4)
/**
  * @brief 	Source regulation configuration error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected source is not valid (e.g. selecting source 3 on an AEM
  * 		only providing for dual source)
  */
#define AEM13920_DRIVER_ERR_SRC			(-5)
/**
  * @brief 	APM mode error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected APM mode is not valid
  */
#define AEM13920_DRIVER_ERR_APM_MODE		(-6)
/**
  * @brief 	APM window error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected APM window is not valid
  */
#define AEM13920_DRIVER_ERR_APM_WINDOW		(-7)
/**
  * @brief 	Sleep threshold error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected sleep threshold is not valid
  */
#define AEM13920_DRIVER_ERR_SLEEP_THRESH	(-8)
/**
  * @brief 	Timing multiplier error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected timing multiplier is not valid
  */
#define AEM13920_DRIVER_ERR_TMULT		(-9)
/**
  * @brief 	VLOAD error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected VLOAD is not valid
  */
#define AEM13920_DRIVER_ERR_VLOAD		(-10)
/**
  * @brief 	MPPT ratio error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected MPPT ratio is not valid
  */
#define AEM13920_DRIVER_ERR_MPPT_RATIO		(-11)
/**
  * @brief 	MPPT duration error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected MPPT duration is not valid
  */
#define AEM13920_DRIVER_ERR_MPPT_DURATION	(-12)
/**
  * @brief 	MPPT period error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected MPPT period is not valid
  */
#define AEM13920_DRIVER_ERR_MPPT_PERIOD		(-13)
/**
  * @brief 	Source regulation mode error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected source regulation mode is not valid
  */
#define AEM13920_DRIVER_ERR_SRCREGU_MODE	(-14)
/**
  * @brief 	APM disabled error
  * 
  * @details 	A error occurred during the function execution because the
  * 		APM is disabled for the selected source
  */
#define AEM13920_DRIVER_ERR_APM_DISABLED	(-15)
/**
  * @brief 	Buck disabled error
  * 
  * @details 	A error occurred during the function execution because the
  * 		buck converter is disabled
  */
#define AEM13920_DRIVER_ERR_BUCK_DISABLED	(-16)
/**
  * @brief 	Wrong DCDC inductance error
  * 
  * @details 	A error occurred during the function execution because the
  * 		selected DCDC inductance is not valid
  */
#define AEM13920_DRIVER_ERR_LDCDC		(-17)
/** 
  * @copydoc AEM_I2C_ERR_CODE
  * @} *//* endgroup AEM13920_STATUS_ERR_CODE */

/**
  * @anchor 	AEM13920_I2C_SLV_ADDR
  * @name	AEM13920 I2C Slave addresses
  * @{
  */
#define AEM13920_I2CSLAVE_ADDRESS	(0x41UL)/**< I2C slave address: 0x41 */
/** @} *//* end name AEM13920_I2C_SLV_ADDR */


typedef enum _AEM13920_SOURCE {
	AEM13920_SRC1			= 0x00,	/**< Source 1 */
	AEM13920_SRC2			= 0x01,	/**< Source 2 */
} AEM13920_SOURCE;

/**
  * @brief 	Source Regulation mode
  */
typedef enum _AEM13920_SRCREGU_MODE {
	AEM13920_SRCREGU_CONST		= 0x00,	/**< Mode: constant voltage */
	AEM13920_SRCREGU_MPPT		= 0x01,	/**< Mode: MPP tracking */
} AEM13920_SRCREGU_MODE;

/**
  * @brief 	Maximum Power Point Tracking ratio
  */
typedef enum _AEM13920_MPPT_RATIO {
	AEM13920_MPPT_RATIO_ZMPP 	= 0x07,	/**< MPPT ratio: none. Use ZMPP pin */
	AEM13920_MPPT_RATIO_85 		= 0x06,	/**< MPPT ratio: 85% */
	AEM13920_MPPT_RATIO_80 		= 0x05,	/**< MPPT ratio: 80% */
	AEM13920_MPPT_RATIO_75 		= 0x04,	/**< MPPT ratio: 75% */
	AEM13920_MPPT_RATIO_70 		= 0x03,	/**< MPPT ratio: 70% */
	AEM13920_MPPT_RATIO_65 		= 0x02,	/**< MPPT ratio: 65% */
	AEM13920_MPPT_RATIO_50 		= 0x01,	/**< MPPT ratio: 50% */
	AEM13920_MPPT_RATIO_35 		= 0x00	/**< MPPT ratio: 35% */
} AEM13920_MPPT_RATIO;

/**
  * @brief 	Maximum Power Point Tracking duration
  */
typedef enum _AEM13920_MPPT_DURATION {
	AEM13920_MPPT_DUR2 		= 0x00,	/**< MPPT duration: 2ms */
	AEM13920_MPPT_DUR4 		= 0x01,	/**< MPPT duration: 4ms */
	AEM13920_MPPT_DUR8 		= 0x02,	/**< MPPT duration: 8ms */
	AEM13920_MPPT_DUR16 		= 0x03,	/**< MPPT duration: 16ms */
	AEM13920_MPPT_DUR32 		= 0x04,	/**< MPPT duration: 32ms */
	AEM13920_MPPT_DUR128 		= 0x05,	/**< MPPT duration: 128ms */
	AEM13920_MPPT_DUR256 		= 0x06,	/**< MPPT duration: 256ms */
	AEM13920_MPPT_DUR512 		= 0x07	/**< MPPT duration: 512ms */
} AEM13920_MPPT_DURATION;


/**
  * @brief 	Maximum Power Point Tracking period
  */
typedef enum _AEM13920_MPPT_PERIOD {
	AEM13920_MPPT_PER128 		= 0x00,	/**< MPPT period: 128ms */
	AEM13920_MPPT_PER256 		= 0x01,	/**< MPPT period: 256ms */
	AEM13920_MPPT_PER512 		= 0x02,	/**< MPPT period: 512ms */
	AEM13920_MPPT_PER1024 		= 0x03,	/**< MPPT period: 1024ms */
	AEM13920_MPPT_PER2048 		= 0x04,	/**< MPPT period: 2048ms */
	AEM13920_MPPT_PER4096 		= 0x05,	/**< MPPT period: 4096ms */
	AEM13920_MPPT_PER8192 		= 0x06,	/**< MPPT period: 8192ms */
	AEM13920_MPPT_PER16384 		= 0x07,	/**< MPPT period: 16384ms */
} AEM13920_MPPT_PERIOD;

/**
  * @brief 	Boost/Buck converters timing multiplier
  */
typedef enum _AEM13920_TMULT {
	AEM13920_TMULT1			= 0x00,	/**< Timing multiplier: 1 */
	AEM13920_TMULT2			= 0x01,	/**< Timing multiplier: 2 */
	AEM13920_TMULT3			= 0x02,	/**< Timing multiplier: 3 */
	AEM13920_TMULT4			= 0x03,	/**< Timing multiplier: 4 */
	AEM13920_TMULT6			= 0x04,	/**< Timing multiplier: 6 */
	AEM13920_TMULT8			= 0x05,	/**< Timing multiplier: 8 */
	AEM13920_TMULT12		= 0x06,	/**< Timing multiplier: 12 */
	AEM13920_TMULT16		= 0x07,	/**< Timing multiplier: 16 */
} AEM13920_TMULT;

/**
  * @brief 	Buck converter output regulation voltage
  */
typedef enum _AEM13920_VOUT {
	AEM13920_VOUT_OFF		= 0x00,	/**< Off */
	AEM13920_VOUT_600		= 0x01,	/**< VOUT: 600mV */
	AEM13920_VOUT_900		= 0x02,	/**< VOUT: 900mV */
	AEM13920_VOUT_1200		= 0x03,	/**< VOUT: 1200mV */
	AEM13920_VOUT_1500		= 0x04,	/**< VOUT: 1500mV */
	AEM13920_VOUT_1800		= 0x05,	/**< VOUT: 1800mV */
	AEM13920_VOUT_2200		= 0x06,	/**< VOUT: 2200mV */
	AEM13920_VOUT_2500		= 0x07,	/**< VOUT: 2500mV */
} AEM13920_VOUT;

/**
  * @brief 	SRC low threshold
  */
typedef enum _AEM13920_SRCLOW_THRESH {
	AEM13920_SRCLOW_THRESH_112 	= 0x00,	/**< SRC low threshold: 112mV */
	AEM13920_SRCLOW_THRESH_202 	= 0x01,	/**< SRC low threshold: 202mV */
	AEM13920_SRCLOW_THRESH_255 	= 0x02,	/**< SRC low threshold: 255mV */
	AEM13920_SRCLOW_THRESH_300 	= 0x03,	/**< SRC low threshold: 300mV */
	AEM13920_SRCLOW_THRESH_360 	= 0x04,	/**< SRC low threshold: 360mV */
	AEM13920_SRCLOW_THRESH_405 	= 0x05,	/**< SRC low threshold: 405mV */
	AEM13920_SRCLOW_THRESH_510 	= 0x06,	/**< SRC low threshold: 510mV */
	AEM13920_SRCLOW_THRESH_600 	= 0x07	/**< SRC low threshold: 600mV */
} AEM13920_SRCLOW_THRESH;

/**
  * @brief 	APM mode of operation
  */
typedef enum _AEM13920_APM_MODE {
	AEM13920_APM_MODE_PULSE_COUNTER = 0x00,	/**< APM Mode: pulse counter */
	AEM13920_APM_MODE_POWER_METER 	= 0x01	/**< APM Mode: power meter */
} AEM13920_APM_MODE;

/**
  * @brief 	APM computation window
  */
typedef enum _AEM13920_APM_WINDOW {
	AEM13920_APM_WINDOW_128 	= 0x00,	/**< APM window: 128ms, Refresh rate: 256ms */
	AEM13920_APM_WINDOW_64 		= 0x01,	/**< APM window: 64ms, Refresh rate: 128ms */
} AEM13920_APM_WINDOW;

/**
  * @brief 	Configuration Mode
  */
typedef enum _AEM13920_CONFIG_MODE {
	AEM13920_CONFIG_MODE_GPIO	= 0x00,	/**< Configuration from GPIO */
	AEM13920_CONFIG_MODE_I2C	= 0x01	/**< Configuration from I2C */
} AEM13920_CONFIG_MODE;

typedef struct _AEM13920_MPPTCFG {
	/**
	  * @brief 	Reserved
	  */
	uint8_t rsvd0			:1;
	/**
	  * @brief 	MPPT ratio
	  * @details 	Set the MPPT ratio
	  */
	AEM13920_MPPT_RATIO ratio	:3;
	/**
	  * @brief 	Reserved
	  */
	uint8_t rsvd1			:4;
	/**
	  * @brief 	MPPT duration
	  * @details 	Set the MPPT sampling duration
	  */
	AEM13920_MPPT_DURATION duration	:3;
	/**
	  * @brief 	MPPT period
	  * @details 	Set the MPPT period
	  */
	AEM13920_MPPT_PERIOD period	:3;
	/**
	  * @brief 	Reserved
	  */
	uint8_t rsvd2			:2;
} AEM13920_MPPTCFG_t;

/**
  * @brief 	Bost converter configuration
  */
typedef struct _AEM13920_BSTCFG {
	/**
	  * @brief 	Enable/Disable the boost converter
	  * @details 	Set to \c true to enable, to \c false to disable
	  */
	bool enable			:1;
	/**
	  * @brief 	Enable/Disable the High Power feature for the boost 
	  *		converter
	  * @details 	Set to \c true to enable, to \c false to disable
	  */
	bool high_power_enable		:1;
	/**
	  * @brief 	Boost converter's timing multiplier
	  * @details 	Set the timing multiplier for the boost converter.
	  *		The higher the timing multiplier, the higher the average
	  *		current pulled from the source to charge the storage 
	  *		element
	  */
	AEM13920_TMULT tmult		:3;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd			:3;
} AEM13920_BSTCFG_t;

/**
  * @brief 	Buck converter configuration
  */
typedef struct _AEM13920_BUCKCFG {
	/**
	  * @brief 	Buck converter output regulation voltage
	  * @details 	Set the buck converter output regulation voltage
	  */
	AEM13920_VOUT vout		:3;
	/**
	  * @brief 	Timing multiplier
	  * @details 	Set the timing multiplier for the buck converter
	  */
	AEM13920_TMULT tmult		:3;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd			:2;
} AEM13920_BUCKCFG_t;

/**
  * @brief 	SRC Low thresholds configuration
  */
typedef struct _AEM13920_SRCLOWCFG {
	/**
	  * @brief 	SRC1 threshold
	  * @details 	Set the SRC1 regulation voltage below which the AEM 
	  *		enters SLEEP STATE
	  */
	AEM13920_SRCLOW_THRESH src1_thresh	:3;
	/**
	  * @brief 	SRC2 threshold
	  * @details 	Set the SRC2 regulation voltage below which the AEM 
	  *		enters SLEEP STATE
	  */
	AEM13920_SRCLOW_THRESH src2_thresh	:3;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd				:2;
} AEM13920_SRCLOWCFG_t;

/**
  * @brief 	APM configuration
  */
typedef struct _AEM13920_APMCFG {
	/**
	  * @brief 	Enable/Disable APM for SRC1
	  * @details 	Set to \c true to enable, to \c false to disable
	  */
	bool src1_enable		:1;
	/**
	  * @brief 	Enable/Disable APM for SRC2
	  * @details 	Set to \c true to enable, to \c false to disable
	  */
	bool src2_enable		:1;
	/**
	  * @brief 	Enable/Disable APM for the buck converter
	  * @details 	Set to \c true to enable, to \c false to disable
	  */
	bool buck_enable		:1;
	/**
	  * @brief 	Average Power Monitoring operation mode
	  * @details 	Set the APM mode of operation
	  */
	AEM13920_APM_MODE mode		:1;
	/**
	  * @brief 	Computation window for the Average Power Monitoring
	  * @details 	Set the APM computation window
	  */
	AEM13920_APM_WINDOW window	:1;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd			:3;
} AEM13920_APMCFG_t;

/**
  * @brief 	IRQ flags
  */
typedef struct _AEM13920_IRQFLG {
	/** 
	  * @brief 	I2C ready flag 
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool i2c_rdy 		:1;
	/** 
	  * @brief 	Overdischarge flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool vovdis		:1;
	/**
	  * @brief 	Charge ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool vchrdy	:1;
	/** 
	  * @brief 	Overcharge flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool vovch		:1;
	/** 
	  * @brief 	SRC Low Threshold flag 
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src_low		:1;
	/**
	  * @brief 	Charge temperature threshold flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool temp_ch 		:1;
	/**
	  * @brief 	Discharge temperature threshold flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool temp_dis 		:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __rsvd		:1;
	/**
	  * @brief 	SRC1 MPPT start flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src1_mppt_start	:1;
	/**
	  * @brief 	SRC1 MPPT done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src1_mppt_done	:1;
	/**
	  * @brief 	SRC2 MPPT start flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src2_mppt_start	:1;
	/**
	  * @brief 	SRC2 MPPT done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src2_mppt_done	:1;
	/**
	  * @brief	STO data ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool sto_done		:1;
	/**
	  * @brief	Temperature data ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool temp_done		:1;
	/**
	  * @brief	APM data ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool apm_done 		:1;
	/**
	  * @brief	APM error flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool apm_err		:1;
} AEM13920_IRQFLG_t;

/**
  * @brief 	IRQ settings
  */
typedef struct _AEM13920_IRQEN {
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		AEM is out of reset and ready to communicate
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool i2c_rdy 		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the overdischarge threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vovdis 		:1;
	/**
	  * @brief 	Enable/Disablethe IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the charge ready threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vchrdy 		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the overcharge threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vovch 		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) on any
	  *		transition between a state where at least one source 
	  *		voltage is above its low threshold and a state where 
	  *		both are below their respective thresholds.
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src_low 		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the ambient temperature is out of the defined range for
	  *		storage element charging
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_ch 		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the ambient temperature is out of the defined range for
	  *		storage element discharging
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_dis 		:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __rsvd		:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT starts for SRC1
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_mppt_start	:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT is done for SRC1
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_mppt_done	:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT starts for SRC2
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_mppt_start	:1;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT is done for SRC2
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_mppt_done	:1;
	/**
	  * @brief	Enable/Disable the IRQ pin to be asserted (HIGH) wwhen 
	  *		a new data from the STO ADC is available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool sto_done		:1;
	/**
	  * @brief	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		a new data from the temperature ADC is available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_done		:1;
	/**
	  * @brief	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		new APM data are available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_done 		:1;
	/**
	  * @brief	Enable/Disable the IRQ pin to be asserted (HIGH) when
	  *		an APM error occurs
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_err		:1;
} AEM13920_IRQEN_t;

/**
  * @brief 	Status
  */
typedef struct _AEM13920_STATUS {
	/**
	  * @brief 	Overdischarge status
	  * @details 	Set to \c true when the storage level is under the 
	  * 		overdischarge threshold, to \c false otherwise
	  */
	bool vovdis 		:1;
	/**
	  * @brief 	Charge ready status
	  * @details 	Set to \c true  when the storage level is above the 
	  * 		charge ready threshold, to \c false otherwise
	  */
	bool vchrdy 	:1;
	/**
	  * @brief 	Overcharge status
	  * @details 	Set to \c true when the storage level is above the 
	  * 		overcharge threshold, to \c false otherwise
	  */
	bool vovch		:1;
	/**
	  * @brief 	SRC1LOW threshold status
	  * @details 	Set to \c true when the SRC1's voltage is under the 
	  * 		defined threshold, to \c false otherwise
	  */
	bool src1_low 		:1;
	/**
	  * @brief 	SRC2LOW threshold status
	  * @details 	Set to \c true when the SRC2 voltage is under the 
	  * 		defined threshold, to \c false otherwise
	  */
	bool src2_low 		:1;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd0		:3;
	/**
	  * @brief 	Cold temperature threshold for storage element charging
	  * 		status
	  * @details 	Set to \c true when the temperature is below the cold
	  *		threshold, to \c false otherwise
	  */
	bool temp_cold_ch	:1;
	/**
	  * @brief 	Hot temperature threshold for storage element charging
	  * 		status
	  * @details 	Set to \c true when the temperature is above the hot
	  *		threshold, to \c false otherwise
	  */
	bool temp_hot_ch	:1;
	/**
	  * @brief 	Cold Temperature threshold for storage element discharging
	  * 		status
	  * @details 	Set to \c true when the temperature is below the cold
	  *		threshold, to \c false otherwise
	  */
	bool temp_cold_dis	:1;
	/**
	  * @brief 	Hot temperature threshold for storage element discharging
	  * 		status
	  * @details 	Set to \c true when the temperature is above the hot 
	  *		threshold, to \c false otherwise
	  */
	bool temp_hot_dis	:1;
	/**
	  * @brief 	Reserved
	  */
	uint8_t __rsvd1		:4;
} AEM13920_STATUS_t;

/**
  * @brief 	APM Error
  * 
  */
typedef struct _AEM13920_APMERR {
	/**
	  * @brief 	SRC1 counter overflow
	  * @details	Set to \c true when a counter overflow has occured, to 
	  * 		\c false otherwise
	  */
	bool src1_ov	:1;
	/**
	  * @brief 	SRC1 counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool src1_nvld	:1;
	/**
	  * @brief 	SRC2 counter overflow
	  * @details	Set to \c true when a counter overflow has occured, to 
	  * 		\c false otherwise
	  */
	bool src2_ov	:1;
	/**
	  * @brief 	SRC2 counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool src2_nvld	:1;
	/**
	  * @brief 	BUCK counter overflow
	  * @details	Set to \c true when a counter overflow has occured, to 
	  * 		\c false otherwise
	  */
	bool buck_ov	:1;
	/**
	  * @brief 	Buck counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool buck_nvld	:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __rsvd	:2;
} AEM13920_APMERR_t;

#ifdef __cplusplus
}
#endif

/** @} *//* (end defgroup AEM13920_COMMON) */
#endif /* AEM13920_COMMON_H_ */
