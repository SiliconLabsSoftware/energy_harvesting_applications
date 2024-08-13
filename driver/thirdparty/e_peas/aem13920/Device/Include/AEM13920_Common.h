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
  * 	- Minor bug fixes
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


typedef enum _AEM13920_Source {
	AEM13920_SRC1			= 0,	/**< Source 1 */
	AEM13920_SRC2			= 1,	/**< Source 2 */
} AEM13920_Source;

/**
  * @brief 	Source Regulation mode
  */
typedef enum _AEM13920_SRCREGU_MODE {
	AEM13920_SRCREGU_CONST		= 0,	/**< Mode: constant voltage */
	AEM13920_SRCREGU_MPPT		= 1,	/**< Mode: MPP tracking */
} AEM13920_SRCREGU_MODE;

/**
  * @brief 	Maximum Power Point Tracking ratio
  */
typedef enum _AEM13920_MPPT_RATIO {
	AEM13920_MPPT_RATIO_ZMPP 	= 7,	/**< Ratio: none. Use ZMPP pin */
	AEM13920_MPPT_RATIO_85 		= 6,	/**< Ratio: 85pc */
	AEM13920_MPPT_RATIO_80 		= 5,	/**< Ratio: 80pc */
	AEM13920_MPPT_RATIO_75 		= 4,	/**< Ratio: 75pc */
	AEM13920_MPPT_RATIO_70 		= 3,	/**< Ratio: 70pc */
	AEM13920_MPPT_RATIO_65 		= 2,	/**< Ratio: 65pc */
	AEM13920_MPPT_RATIO_50 		= 1,	/**< Ratio: 50pc */
	AEM13920_MPPT_RATIO_35 		= 0	/**< Ratio: 35pc */
} AEM13920_MPPT_RATIO;

/**
  * @brief 	Maximum Power Point Tracking duration
  */
typedef enum _AEM13920_MPPT_DURATION {
	AEM13920_MPPT_DUR2 		= 0,	/**< Duration: 2ms */
	AEM13920_MPPT_DUR4 		= 1,	/**< Duration: 4ms */
	AEM13920_MPPT_DUR8 		= 2,	/**< Duration: 8ms */
	AEM13920_MPPT_DUR16 		= 3,	/**< Duration: 16ms */
	AEM13920_MPPT_DUR32 		= 4,	/**< Duration: 32ms */
	AEM13920_MPPT_DUR128 		= 5,	/**< Duration: 128ms */
	AEM13920_MPPT_DUR256 		= 6,	/**< Duration: 256ms */
	AEM13920_MPPT_DUR512 		= 7	/**< Duration: 512ms */
} AEM13920_MPPT_DURATION;


/**
  * @brief 	Maximum Power Point Tracking period
  */
typedef enum _AEM13920_MPPT_PERIOD {
	AEM13920_MPPT_PER128 		= 0,	/**< Period: 128ms */
	AEM13920_MPPT_PER256 		= 1,	/**< Period: 256ms */
	AEM13920_MPPT_PER512 		= 2,	/**< Period: 512ms */
	AEM13920_MPPT_PER1024 		= 3,	/**< Period: 1024ms */
	AEM13920_MPPT_PER2048 		= 4,	/**< Period: 2048ms */
	AEM13920_MPPT_PER4096 		= 5,	/**< Period: 4096ms */
	AEM13920_MPPT_PER8192 		= 6,	/**< Period: 8192ms */
	AEM13920_MPPT_PER16384 		= 7,	/**< Period: 16384ms */
} AEM13920_MPPT_PERIOD;

/**
  * @brief 	Boost/Buck converters timing multiplier
  */
typedef enum _AEM13920_TMULT {
	AEM13920_TMULT1			= 0,	/**< Multiplier: 1 */
	AEM13920_TMULT2			= 1,	/**< Multiplier: 2 */
	AEM13920_TMULT3			= 2,	/**< Multiplier: 3 */
	AEM13920_TMULT4			= 3,	/**< Multiplier: 4 */
	AEM13920_TMULT6			= 4,	/**< Multiplier: 6 */
	AEM13920_TMULT8			= 5,	/**< Multiplier: 8 */
	AEM13920_TMULT12		= 6,	/**< Multiplier: 12 */
	AEM13920_TMULT16		= 7,	/**< Multiplier: 16 */
} AEM13920_TMULT;

/**
  * @brief 	Buck converter output regulation voltage
  */
typedef enum _AEM13920_VLOAD {
	AEM13920_VLOAD_OFF		= 0,	/**< Off */
	AEM13920_VLOAD_600		= 1,	/**< VLOAD: 600mV */
	AEM13920_VLOAD_900		= 2,	/**< VLOAD: 900mV */
	AEM13920_VLOAD_1200		= 3,	/**< VLOAD: 1200mV */
	AEM13920_VLOAD_1500		= 4,	/**< VLOAD: 1500mV */
	AEM13920_VLOAD_1800		= 5,	/**< VLOAD: 1800mV */
	AEM13920_VLOAD_2200		= 6,	/**< VLOAD: 2200mV */
	AEM13920_VLOAD_2500		= 7,	/**< VLOAD: 2500mV */
} AEM13920_VLOAD;

/**
  * @brief 	Sleep threshold
  */
typedef enum _AEM13920_SLEEP_THRESHOLD {
	AEM13920_SLEEP_THRESH_112 	= 0,	/**< Threshold: 112mV */
	AEM13920_SLEEP_THRESH_202 	= 1,	/**< Threshold: 202mV */
	AEM13920_SLEEP_THRESH_255 	= 2,	/**< Threshold: 255mV */
	AEM13920_SLEEP_THRESH_300 	= 3,	/**< Threshold: 300mV */
	AEM13920_SLEEP_THRESH_360 	= 4,	/**< Threshold: 360mV */
	AEM13920_SLEEP_THRESH_405 	= 5,	/**< Threshold: 405mV */
	AEM13920_SLEEP_THRESH_510 	= 6,	/**< Threshold: 510mV */
	AEM13920_SLEEP_THRESH_600 	= 7	/**< Threshold: 600mV */
} AEM13920_SLEEP_THRESHOLD;

/**
  * @brief 	APM mode of operation
  */
typedef enum _AEM13920_APM_MODE {
	AEM13920_APM_MODE_PULSE_COUNTER = 0,	/**< Mode: pulse counter */
	AEM13920_APM_MODE_POWER_METER 	= 1	/**< Mode: power meter */
} AEM13920_APM_MODE;

/**
  * @brief 	APM computation window
  */
typedef enum _AEM13920_APM_WINDOW {
	AEM13920_APM_WINDOW_128 	= 0,	/**< Window: 128ms, Refresh rate: 256ms */
	AEM13920_APM_WINDOW_64 		= 1,	/**< Window: 64ms, Refresh rate: 128ms */
} AEM13920_APM_WINDOW;

/**
  * @brief 	Source of configuration
  */
typedef enum _AEM13920_CFGSRC {
	AEM13920_CFGSRC_I2C		= 1,	/**< Source: I2C's registers */
	AEM13920_CFGSRC_PINS		= 0	/**< Source: configuration pins */
} AEM13920_CFGSRC;

/**
  * @brief 	Ldcdc value
  */
typedef enum _AEM13920_LDCDC {
	AEM13920_LDCDC_4_7		= 0,	/**< Ldcdc: 4.7uH */
	AEM13920_LDCDC_10		= 1,	/**< Ldcdc: 10uH */
	AEM13920_LDCDC_15		= 2,	/**< Ldcdc: 15uH */
	AEM13920_LDCDC_22		= 3,	/**< Ldcdc: 22uH */
	AEM13920_LDCDC_33		= 4,	/**< Ldcdc: 33uH */
	AEM13920_LDCDC_47		= 5,	/**< Ldcdc: 47uH */
	AEM13920_LDCDC_68		= 6,	/**< Ldcdc: 68uH */
} AEM13920_LDCDC;

/**
  * @brief 	IRQ flags
  */
typedef struct _AEM13920_IRQFLG {
	/**
	  * @brief 	Out of reset flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool outOfReset 		:1;
	/**
	  * @brief 	Discharge flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool discharge 			:1;
	/**
	  * @brief 	Charge ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool chargeReady 		:1;
	/**
	  * @brief 	Overcharge flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool overcharge 		:1;
	/**
	  * @brief 	Sleep threshold flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool sleepThresh 		:1;
	/**
	  * @brief 	Temperature threshold for storage element charging flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool tempChThresh 		:1;
	/**
	  * @brief 	Temeperature threshold for storage element discharging flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool tempDisThresh 		:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __reserved		:1;
	/**
	  * @brief 	SRC1 MPPT start flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src1MPPTStart		:1;
	/**
	  * @brief 	SRC1 MPPT done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src1MPPTDone		:1;
	/**
	  * @brief 	SRC2 MPPT start flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src2MPPTStart		:1;
	/**
	  * @brief 	SRC2 MPPT done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool src2MPPTDone		:1;
	/**
	  * @brief	STO ADC done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool storageDone		:1;
	/**
	  * @brief	Temperature ADC done flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool tempDone			:1;
	/**
	  * @brief	APM data ready flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool APMDone 			:1;
	/**
	  * @brief	APM error flag
	  * @details 	Set to \c true if the flag is raised, to \c false otherwise
	  */
	bool APMError 			:1;
} AEM13920_IRQFLG;

/**
  * @brief 	IRQ settings
  */
typedef struct _AEM13920_IRQ_cfg {
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		AEM is out of reset and ready to communicate
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool enableOutOfReset 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		storage level is under the discharge threshold defined 
	  * 		in \ref AEM13920_Config.dischargeThresh
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableDischarge 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		storage level is above the charge ready threshold defined 
	  * 		in \ref AEM13920_Config.chargeReadyThresh
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableChargeReady 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		storage level is above the overcharge threshold defined 
	  * 		in \ref AEM13920_Config.overchargeThresh
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableOvercharge 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		source voltage drops below the defined threshold
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableSleepThresh 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		ambient temperature is out of the defined range for 
	  * 		storage element charging
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableTempChThresh 		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts when the 
	  * 		ambient temperature is out of the defined range for 
	  * 		storage element discharging
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableTempDisThresh 		:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __reserved			:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts on MPPT start
	  * 		for SRC1
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableSrc1MPPTStart		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts on MPPT done
	  * 		for SRC1
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableSrc1MPPTDone		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts on MPPT start
	  * 		for SRC2
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableSrc2MPPTStart		:1;
	/**
	  * @brief 	Enable/Disable the generation of interrupts on MPPT done
	  * 		for SRC2
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableSrc2MPPTDone		:1;
	/**
	  * @brief	Enable/Disable the generation of interrupts on STO 
	  * 		ADC done
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableStorageDone		:1;
	/**
	  * @brief	Enable/Disable the generation of interrupts on temperature
	  * 		ADC done
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableTempDone		:1;
	/**
	  * @brief	Enable/Disable the generation of interrupts when the 
	  * 		APM data is ready
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableAPMDone 		:1;
	/**
	  * @brief	Enable/Disable the generation of interrupts on APM errors
	  * @details	Set to \p 1 to enable, to \c false to disable
	  */
	bool enableAPMError		:1;
} AEM13920_IRQEN;

/**
  * @brief 	Status
  */
typedef struct _AEM13920_Status {
	/**
	  * @brief 	Discharge status
	  * @details 	Set to \c true when the storage level is under the 
	  * 		discharge threshold defined in 
	  * 		\ref AEM13920_Config.dischargeThresh, to \c false 
	  * 		otherwise
	  */
	bool discharge 		:1;
	/**
	  * @brief 	Charge ready status
	  * @details 	Set to \c true  when the storage level is above the 
	  * 		charge ready threshold defined in
	  * 		\ref AEM13920_Config.chargeReadyThresh, to \c false 
	  * 		otherwise
	  */
	bool chargeReady 	:1;
	/**
	  * @brief 	Overcharge status
	  * @details 	Set to \c true when the storage level is above the 
	  * 		overcharge threshold defined in 
	  * 		\ref AEM13920_Config.overchargeThresh, to \c false 
	  * 		otherwise
	  */
	bool overcharge		:1;
	/**
	  * @brief 	Sleep threshold on SRC1 status
	  * @details 	Set to \c true when the source voltage is under the 
	  * 		defined sleep threshold, to \c false otherwise
	  */
	bool src1Sleep 		:1;
	/**
	  * @brief 	Sleep threshold on SRC2 status
	  * @details 	Set to \c true when the source voltage is under the 
	  * 		defined sleep threshold, to \c false otherwise
	  */
	bool src2Sleep 		:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __reserved0	:3;
	/**
	  * @brief 	Cold temperature threshold for storage element charging
	  * 		status
	  * @details 	Set to \c true when the temperature is out of the 
	  * 		defined range, to \c false otherwise
	  */
	bool tempColdCharge	:1;
	/**
	  * @brief 	Hot temperature threshold for storage element charging
	  * 		status
	  * @details 	Set to \c true when the temperature is out of the 
	  * 		defined range, to \c false otherwise
	  */
	bool tempHotCharge	:1;
	/**
	  * @brief 	Cold Temperature threshold for storage element discharging
	  * 		status
	  * @details 	Set to \c true when the temperature is out of the 
	  * 		defined range, to \c false otherwise
	  */
	bool tempColdDischarge	:1;
	/**
	  * @brief 	Hot temperature threshold for storage element discharging
	  * 		status
	  * @details 	Set to \c true when the temperature is out of the 
	  * 		defined range, to \c false otherwise
	  */
	bool tempHotDischarge	:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __reserved1	:4;
} AEM13920_Status;

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
	bool src1CountOverflow	:1;
	/**
	  * @brief 	SRC1 counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool src1CountCorrupted	:1;
	/**
	  * @brief 	SRC2 counter overflow
	  * @details	Set to \c true when a counter overflow has occured, to 
	  * 		\c false otherwise
	  */
	bool src2CountOverflow	:1;
	/**
	  * @brief 	SRC2 counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool src2CountCorrupted	:1;
	/**
	  * @brief 	BUCK counter overflow
	  * @details	Set to \c true when a counter overflow has occured, to 
	  * 		\c false otherwise
	  */
	bool buckCountOverflow	:1;
	/**
	  * @brief 	Buck counter corrupted
	  * @details	Set to \c true when the counter is corrupted, to \c false
	  * 		otherwise
	  */
	bool buckCountCorrupted	:1;
	/**
	  * @brief 	reserved
	  */
	uint8_t __reserved	:2;
} AEM13920_APMERR;




#ifdef __cplusplus
}
#endif

/** @} *//* (end defgroup AEM13920_COMMON) */
#endif /* AEM13920_COMMON_H_ */
