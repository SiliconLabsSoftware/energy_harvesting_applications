/**
  ******************************************************************************
  * \file	HAL_AEM13920.h
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM13920_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \defgroup	HAL_AEM13920_API		HAL Driver API
  * @{
  * 
  * \brief	AEM13920's HAL Driver's API
  * 
  * @details 	
  * This document describes the AEM13920 HAL Driver's API. It provides 
  * information about all the functions and data structures that can be used
  * to interface the AEM.
  * 
  * Please refer to the \ref index page for further guidance on how to use this 
  * driver 
  * 
  * \version	1.0.0
  * 	- Minor bug fixes
  * \version	1.0.1
  * 	- Minor bug fixes
  * \version	1.1.0
  * 	API Improvement
  * 	- Precise in and out parameters in the API documentation
  * 	Deprecation
  * 	- Deprecate GetAPMSource and GetAPMBuck, replaced by GetAPMDataSRC and GetAPMDataBuck
  * \version	1.1.1
  * 	Bug Fix
  * 	- Remove useless condition
  * 
  * @}
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
#ifndef HAL_AEM13920_H_
#define HAL_AEM13920_H_

#include <stdint.h>
#include <stdbool.h>
#include "AEM13920_Common.h"
#include "AEM_I2C.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * \addtogroup	HAL_AEM13920_API
  * @{
  */

/**
  * @brief 	AEM13920's configuration settings
  */
typedef struct _AEM13920_Config {
	/**
	  * @brief 	SRC2 Regulation Voltage (mV)
	  * @pre	src1ReguMode set to AEM13920_SRCREGU_CONST
	  * @note	If the specified voltage is under 0.12V, the AEM will go 
	  * 		to sleep mode.
	  * @note	If it is above 1.47V, it will be forced to 1.47V
	  */
	uint32_t src1ReguV;
	/**
	  * @brief 	SRC2 Regulation Voltage (mV)
	  * @pre	src2ReguMode set to AEM13920_SRCREGU_CONST
	  * @note	If the specified voltage is under 0.12V, the AEM will go 
	  * 		to sleep mode.
	  * @note	If it is above 1.47V, it will be forced to 1.47V
	  */
	uint32_t src2ReguV;
	/**
	  * @brief	Discharge (Vovdis) threshold
	  * @details	Minimum voltage accepted on the storage element
	  *		before stopping to supply LOAD (mV)
	  * \note	If the specified threshold is under 2.4V, it will be
	  * 		forced to 2.4V
	  * \note	If the specified threshold is above 3.581V, it will be 
	  * 		forced to 3.581V
	  */
	uint32_t dischargeThresh;
	/**
	  * @brief	Charge ready (Vchrdy) threshold
	  * @details	Minimum voltage accepted on the storage element
	  * 		before starting to supply LOAD in START STATE (mV)
	  * \note	It the specified threshold is under 2.456V, it will be
	  * 		forced to 2.456V
	  * \note	If the specified threshold is above 3.637V, it will be 
	  * 		forced to 3.637V
	  */
	uint32_t chargeReadyThresh;
	/**
	  * @brief 	Overcharge (Vovch) threshold
	  * @details	Maximum voltage accepted on the storage element
	  *		before disabling its charging (mV)
	  * \note	If the specified threshold is under 2.7V, it will be 
	  * 		forced to 2.7V
	  * \note	If the specified threshold is above 4.594V, it will be 
	  * 		forced to 4.594V
	  */
	uint32_t overchargeThresh;
	/**
	  * @brief 	Impedance of the resistor Rdiv, that creates a resistive 
	  * 		voltage divider together with the thermistor Rth (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t Rdiv;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'cold' threshold 
	  * 		for storage element charging (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t coldChThreshZ;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'hot' threshold 
	  * 		for storage element charging (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t hotChThreshZ;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'cold' threshold 
	  * 		for storage element discharging (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t coldDisThreshZ;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'hot' threshold 
	  * 		for storage element discharging (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t hotDisThreshZ;
	/**
	  * @brief 	SRC1 Regulation mode
	  */
	AEM13920_SRCREGU_MODE src1ReguMode;
	/**
	  * @brief 	SRC1 MPP tracking ratio (Vmpp/Voc)
	  * @pre	src1ReguMode set to AEM13920_SRCREGU_MPPT
	  */	
	AEM13920_MPPT_RATIO src1MpptRatio;
	/**
	  * @brief 	SRC1 MPP tracking duraction
	  * @pre	src1ReguMode set to AEM13920_SRCREGU_MPPT
	  */
	AEM13920_MPPT_DURATION src1MpptDuration;
	/**
	  * @brief 	SRC1 MPP tracking period
	  * @pre	src1ReguMode set to AEM13920_SRCREGU_MPPT
	  */
	AEM13920_MPPT_PERIOD src1MpptPeriod;
	/**
	  * @brief 	SRC2 Regulation mode
	  */
	AEM13920_SRCREGU_MODE src2ReguMode;
	/**
	  * @brief 	SRC2 MPP tracking ratio (Vmpp/Voc)
	  * @pre	src2ReguMode set to AEM13920_SRCREGU_MPPT
	  */	
	AEM13920_MPPT_RATIO src2MpptRatio;
	/**
	  * @brief 	SRC2 MPP tracking duraction
	  * @pre	src2ReguMode set to AEM13920_SRCREGU_MPPT
	  */
	AEM13920_MPPT_DURATION src2MpptDuration;
	/**
	  * @brief 	SRC2 MPP tracking period
	  * @pre	src2ReguMode set to AEM13920_SRCREGU_MPPT
	  */
	AEM13920_MPPT_PERIOD src2MpptPeriod;
	/**
	  * @brief 	Timing multiplier for source 1's boost
	  * @note	This setting has no effect if source 1's boost is disabled
	  */
	AEM13920_TMULT boost1TimingMult;
	/**
	  * @brief 	Timing multiplier for source 2's boost
	  * @note	This setting has no effect if source 2's boost is disabled
	  */
	AEM13920_TMULT boost2TimingMult;
	/**
	  * @brief 	Buck converter output regulation voltage
	  */
	AEM13920_VLOAD buckVLoad;
	/**
	  * @brief 	Timing multiplier for the buck converter
	  */
	AEM13920_TMULT buckTimingMult;
	/**
	  * @brief 	SRC1 sleep threshold
	  */
	AEM13920_SLEEP_THRESHOLD src1SleepThresh;
	/**
	  * @brief 	SRC1 sleep threshold
	  */
	AEM13920_SLEEP_THRESHOLD src2SleepThresh;
	/**
	  * @brief 	Average Power Monitoring operation mode
	  * @note	This setting is only required when using the Average
	  * 		Power Monitoring feature.
	  */
	AEM13920_APM_MODE apmMode;
	/**
	  * @brief 	Computation window for the Average Power Monitoring
	  * @note	This setting is only required when using the Average
	  * 		Power Monitoring feature.
	  */
	AEM13920_APM_WINDOW apmWindow;
	/**
	  * @brief 	IRQs configuration
	  */
	AEM13920_IRQEN irq;
	/**
	  * @brief 	Enable/Disable the boost converter 1
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableBoost1;
	/**
	  * @brief 	Enable/Disable the automatic high power mode for the 
	  * 		boost converter 1
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableHPBoost1;
	/**
	  * @brief 	Enable/Disable the boost converter 2
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableBoost2;
	/**
	  * @brief 	Enable/Disable the automatic high power mode for the
	  * 		boost converter 2
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableHPBoost2;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for SRC1
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableAPMSRC1;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for SRC2
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableAPMSRC2;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for the Buck
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableAPMBuck;
	/**
	  * @brief 	Enable/Disable the ambient Temperature Monitoring
	  * @details	Set to \p true to enable, to \p false to disable
	  */
	bool enableTempMonitoring;
	/**
	  * @brief 	reserved
	  */
	uint16_t _reserved;
} AEM13920_Config;

/**
  * @brief 	Initialize the communication interface
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	The AEM is out of its reset sequence (see the datasheet, 8.1.2)
  * @post	The communication interface is initialized, and the driver's
  * 		functions can be used
  */
int32_t AEM13920_Initialize(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Deinitialize the driver
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	Any resources allocated to the communication interface are
  * 		released. Any call to the driver's functions will fail until
  * 		the AEM13920_Initialize() function is called
  */
int32_t AEM13920_Deinitialize(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Get the chip version
  * 
  * @details	This function reads the VERSION register of the AEM, and sets
  * 		\p version accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out] version 	Chip version
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p version is not NULL
  * @post	\p version is set with the chip version
  */
int32_t AEM13920_GetChipVersion(const AEM_i2c_cfg *i2cCfg, uint8_t *version);

/**
  * @brief 	Get the active configuration source
  * 
  * @details	This function reads the UPDATE field of the CTRL register of 
  * 		the AEM, and sets \p source accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out] source 		Source of configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p source is not NULL
  * @post	\p source is set with the reference of the current configuration
  * 		source. Takes its value from \ref AEM13920_CFGSRC
  */
int32_t AEM13920_GetConfigurationSource(const AEM_i2c_cfg *i2cCfg, uint8_t *source);

/**
  * @brief 	Get the AEM's status
  * 
  * @details	This function reads the STATUS register of the AEM, and sets
  * 		\p status accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out]	status 		AEM's status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post	\p status is set with the AEM's status
  */
int32_t AEM13920_GetStatus(const AEM_i2c_cfg *i2cCfg, AEM13920_Status *status);

/**
  * @brief 	Get the synchronization busy flag
  * 
  * @details	This function reads the SYNCBUSY field of the CTRL register of 
  * 		the AEM, and sets \p sync accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out] sync 		Synchronization flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p sync is not NULL
  * @post	\p sync is set to \c true if the synchronization of the I2C 
  * 		registers and the system registers is ongoing, to \c false 
  * 		otherwise
  * @note	Use this function after calls to the AEM13920_LoadConfiguration()
  * 		or AEM13920_Configure() functions to ensure the synchronization 
  * 		is complete before any attempt to read the configuration
  */
int32_t AEM13920_GetSynchronizationStatus(const AEM_i2c_cfg *i2cCfg, bool *sync);

/**
  * @brief 	Get the AEM's interrupt flags
  * 
  * @details	This function reads the IRQFLG register of the AEM, and sets
  * 		\p flags accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out]	flags 		Interrupt flags
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p flags is not NULL
  * @post	\p flags is set with the IRQ flags of the AEM
  */
int32_t AEM13920_GetIRQFlags(const AEM_i2c_cfg *i2cCfg, AEM13920_IRQFLG *flags);

/**
  * @brief 	Get the APM data for the selected source
  * @deprecated Use AEM13920_GetAPMDataSRC() instead
  * 
  * @details	This function reads the APM0SRCx, APM1SRCx and APM2SRCx registers, 
  * 		and sets \p apm accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in]	src		Selected source
  * @param[out]	apm 		APM data
  * @param[in] 	Ldcdc 		Value of the DCDC converter inductor	
  * 				(ignored in Pulse Counter Mode)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	In Power Meter mode, \p Ldcdc must be set with the value of the
  * 		DCDC converter inductor (see \ref AEM13920_LDCDC)
  * @pre	\p src is a valid source
  * @pre	\p apm is not NULL
  * @post	In Power Meter mode, \p apm is set with the energy value Eapm, 
  * 		in nano-Joules
  * @post	In Pulse Counter mode, \p apm is set with the number of pulses
  */
int32_t AEM13920_GetAPMSource(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				uint32_t *apm, AEM13920_LDCDC Ldcdc);

/**
  * @brief 	Get the APM data for the selected source
  * 
  * @details	This function reads the APM0SRCx, APM1SRCx and APM2SRCx registers
  * 		and sets \p data and \p offset accordingly
  *
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out] data 		APM data
  * @param[out]	offset		Offset
  * @return 	int32_t 	\ref AEM00900_STATUS_ERR_CODE
  *
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p data is not NULL
  * @pre	\p offset is not NULL
  * @post	In Pulse Counter mode, \p data is set with the number of pulses,
  *		and \p offset is not used (set to 0).
  * @post	In Power Metor mode, \p data is set with the data, and \p offset 
  *		is set with the offset to be applied to it. To complete the 
  *		conversion into nano-Joules, refer to the formula given into the 
  *		product's datasheet.
  */
int32_t AEM13920_GetAPMDataSRC(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				uint32_t *data, uint32_t *offset);

/**
  * @brief 	Get the APM data for the buck converter
  * @deprecated Use AEM13920_GetAPMDataBuck() instead
  * 
  * @details	This function reads the APM0BUCK, APM1BUCK and APM2BUCK registers, 
  * 		and sets \p apm accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out]	apm 		APM data
  * @param[in] 	Ldcdc 		Value of the DCDC converter inductor	
  * 				(ignored in Pulse Counter Mode)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	In Power Meter mode, \p Ldcdc must be set with the value of the
  * 		DCDC converter inductor (see \ref AEM13920_LDCDC)
  * @pre	\p apm is not NULL
  * @post	In Power Meter mode, \p apm is set with the energy value Eapm, 
  * 		in nano-Joules
  * @post	In Pulse Counter mode, \p apm is set with the number of pulses
  */
int32_t AEM13920_GetAPMBuck(const AEM_i2c_cfg *i2cCfg, uint32_t *apm, AEM13920_LDCDC Ldcdc);

/**
  * @brief 	Get the APM data for the selected source
  * 
  * @details	This function reads the APM0BUCK, APM1BUCK and APM2BUCK registers
  * 		and sets \p data and \p offset accordingly
  *
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out] data 		APM data
  * @param[out]	offset		Offset
  * @return 	int32_t 	\ref AEM00900_STATUS_ERR_CODE
  *
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p data is not NULL
  * @pre	\p offset is not NULL
  * @post	In Pulse Counter mode, \p data is set with the number of pulses,
  *		and \p offset is not used (set to 0).
  * @post	In Power Metor mode, \p data is set with the data, and \p offset 
  *		is set with the offset to be applied to it. To complete the 
  *		conversion into nano-Joules, refer to the formula given into the 
  *		product's datasheet.
  */
int32_t AEM13920_GetAPMDataBuck(const AEM_i2c_cfg *i2cCfg, uint32_t *data, 	\
				uint32_t *offset);

/**
  * @brief 	Get the APM error flags
  *
  * @details	This function reads the APMERR register of the AEM, and sets 
  * 		\p apmErr accordingly.
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out] apmErr 		APM errors
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p apmErr is not NULL
  * @post	\p apmErr is set with the APM errors flags of the AEM
  */
int32_t AEM13920_GetAPMError(const AEM_i2c_cfg *i2cCfg, AEM13920_APMERR *apmErr);

/**
  * @brief 	Get the thermistor's impedance
  * 
  * @details	This function reads the TEMP register of the AEM, and sets 
  * 		\p Rth accordingly.
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	Rdiv 		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @param[out]	Rth 		Impedance of the thermistor used for the thermal 
  * 				monitoring feature (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Rdiv holds the impedance of the resistor Rdiv (mOhms)
  * @pre	\p Rth is not NULL
  * @post	\p Rth is set with the thermistor's impedance (mOhms)
  * 
  * @note	\p Rth can be converted to Celsius degrees either by refering to 
  * 		the correspondance table that should be supplied by the 
  * 		thermistor's manufacturer, or by implementing the Steinhart-Hart 
  * 		equation.
  * @note	The user willing to work with the Steinhart-Hart equation 
  * 		could proceed as follows. This is an implementation of the 
  * 		simplified Steinhart–Hart equation, aka 'beta parameter 
  * 		equation', which returns the temperature in Celsius degrees
  * 		from \p Rth the thermistor's impedance, \c T0 the thermistor's 
  * 		nominal temperature in Celsius degrees (usually 25°C), \p R0 the 
  * 		thermistor's nominal impedance and \p beta the thermistor's beta 
  * 		constant (kelvins):
  * @note @code
  * #include <stdint.h>
  * #include <math.h>
  * 
  * #define __KELVIN_CONV	(273.15f)
  * 
  * int32_t ConvertThermistorZ(uint32_t Rth, uint32_t T0, uint32_t R0, uint32_t beta)
  * {	
  * 	float T0k = T0 + __KELVIN_CONV; 	// Convert T0 to Kelvins
  * 	float dividend = ((float) beta * T0k);
  * 	float divisor = ((float) beta + (T0k * logf((float) Rth / R0)));
  * 	float temp = dividend / divisor;
  * 
  * 	return roundf(temp - __KELVIN_CONV); 	// Convert to Celsius and round
  * }
  * @endcode
  */
int32_t AEM13920_GetThermistorZ(const AEM_i2c_cfg *i2cCfg, 			\
				uint32_t *Rth, uint32_t Rdiv);

/**
  * @brief 	Get the storage voltage
  * 
  * @details 	This function reads the STO register of the AEM, and sets 
  * 		\p Vsto accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out]	Vsto		Storage voltage (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Vsto is not NULL
  * @post	\p Vsto is set with the storage voltage (mV)
  */
int32_t AEM13920_GetStorageVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vsto);

/**
  * @brief 	Get the regulation voltage for a specific source
  * 
  * @details	This function reads the SRCx register and sets \p Vsrc 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[out]	Vsrc 		Source Regulation Voltage (mV)
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p Vsrc is not NULL
  * @post	\p Vsrc is set with the regulation voltage of the specified
  * 		\p src
  */
int32_t AEM13920_GetSourceRegulationVoltage(	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						uint32_t *Vsrc);

/**
  * @brief 	Set the whole configuration and update the AEM accordingly
  * 
  * @details	This function writes the whole writable area of the AEM's 
  * 		register map according to the configuration specified in \p cfg,
  * 		then sets the UPDATE field of the CTRL register to ensure the 
  * 		system registers are synchronized, and blocks until the 
  * 		synchronisation is complete
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	cfg		Configuration settings
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL and has been correctly initialized
  * @post	The AEM configuration has been updated according to the settings 
  * 		defined in \p cfg
  */
int32_t AEM13920_Configure(const AEM_i2c_cfg *i2cCfg, const AEM13920_Config *cfg);

/**
  * @brief 	Configure the regulation mode for the selected source
  * 
  * @details	This function writes the SRCxREGU0 register to match the 
  * 		the selected regulation mode
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in]	src		Selected source
  * @param[in]	mode		Selected regulation mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The regulation mode for the \c src has been configured according
  * 		to \p mode
  * @see 	AEM13920_SetSourceRegulationVoltage() in case of constant voltage
  * 		regulation
  * @see 	AEM13920_SetMPPTRatio() and AEM13920_SetMPPTTimings() in case
  * 		of MMP tracking
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetSourceRegulationMode( 	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						AEM13920_SRCREGU_MODE mode);

/**
  * @brief 	Configure the MPPT ratio
  * 
  * @details	This function writes the CFG0 field of the SRCxREGU0 register to 
  * 		match the ratio defined in \p ratio
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[in] 	ratio 		Selected ratio
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE "Status/Error code"
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	The source regulation mode is set to AEM13920_SRCREGU_MPPT
  * @post	The ratio between the maximum power point voltage and the open
  * 		circuit voltage is set according to \p ratio
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetMPPTRatio(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_RATIO ratio);

/**
  * @brief 	Configure the MPPT timings
  * 
  * @details	This function writes the CFG1 and CFG2 fields of the SRCxREGU1 
  * 		register to match the selected \p duration and \p period
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[in] 	duration 	Selected duration
  * @param[in]	period		Selected period
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE "Status/Error code"
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	The source regulation mode is set to AEM13920_SRCREGU_MPPT
  * @post	The sampling period and the duration of the open circuit voltage
  * 		evaluation are set according to \p duration and \p period
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetMPPTTimings(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_DURATION duration,		\
				AEM13920_MPPT_PERIOD period);

/**
  * @brief 	Configure the regulation voltage of the selected source
  * 
  * @details	This function writes the SRCxREGUx registers to match the 
  * 		voltage defined in \p Vsrc
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in] 	Vsrc 		Source Regulation Voltage (mV)
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	The source regulation mode is set to AEM13920_SRCREGU_CONST
  * @post	The source regulation voltage is set according to \p Vsrc
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @note	If \c Vsrc is under 0.113V, the AEM will go to sleep mode
  * @note	If \c Vsrc is above 4.455V, it will be forced to 4.455V
  */
int32_t AEM13920_SetSourceRegulationVoltage(	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						uint32_t Vsrc);

/**
  * @brief 	Configure the minimum voltage accepted on the storage element
  *		before stopping to supply LOAD (mV)
  * 
  * @details	This function writes the VOVDIS register to match the voltage
  * 		defined in \p Vovdis
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in] 	Vovdis 		Discharge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The discharge threshold is set according to \p Vovdis
  * \note	If \p Vovdis is under 2.4V, the effective threshold will be 
  * 		forced to 2.4V
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetDischargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vovdis);

/**
  * @brief 	Configure the minimum voltage accepted on the storage element
  * 		before starting to supply LOAD in START STATE (mV)
  * 
  * @details	This function writes the VCHRDY register to match the voltage
  * 		defined in \p Vchrdy
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in] 	Vchrdy 		Charge ready voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The 'charge ready' threshold is set according to \p Vchrdy
  * \note	If \p Vchrdy is under 2.456V, the effective threshold will be 
  * 		forced to 2.456V
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetChargeReadyVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vchrdy);

/**
  * @brief 	Configure the maximum voltage accepted on the storage element
  *		before disabling its charging (mV)
  * 
  * @details	This function writes the VOVCH register to match the voltage
  * 		defined in \p Vovch
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in] 	Vovch 		Overcharge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The overcharge threshold is set according to \p Vovch
  * \note	If \p Vovch is under 2.7V, the effective threshold will be 
  * 		forced to 2.7V. If \p Vovch is above 4.594V, it will be forced
  * 		to 4.594V
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetOverchargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vovch);

/**
  * @brief 	Enable the boost converter for the selected source
  * 
  * @details 	This function sets the EN field of the BSTxCFG register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The boost converter is enabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_EnableBoost(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Disable the boost converter for the selected source
  * 
  * @details 	This function resets the EN field of the BSTxCFG register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The boost converter is disabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_DisableBoost(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Configure the timing multiplier for the selected source's boost
  * 
  * @details	This function writes the TMULT field ot the BSTxCFG register
  * 		according to the selected multiplier
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[in]	tMult		Timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @details	The timing multplier for the selected source's boost is set 
  * 		according to \p tMult
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetBoostTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_TMULT tMult);

/**
  * @brief 	Enable the automatic high power mode for the selected source
  * 
  * @details	This function sets the HPEN field of the BSTxCFG register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The automatic high power mode is enabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_EnableHighPower(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Disable the automatic high power mode for the selected source
  * 
  * @details	This function resets the HPEN field of the BSTxCFG register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The automatic high power mode is disabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_DisableHighPower(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Configure the buck converter's output regulation voltage
  * 
  * @details	This function writes the VOUT field of the BUCKCFG register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in] 	vLoad		Buck converter output regulation voltage
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The buck converter output regulation voltage is set according
  * 		to \p vLoad
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetBuckVLoad(const AEM_i2c_cfg *i2cCfg, AEM13920_VLOAD vLoad);

/**
  * @brief 	Configure the timing multiplier for the buck converter
  * 
  * @details	This function writes the TMULT field ot the BUCKCFG register
  * 		according to the selected multiplier
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	tMult		Timing configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The timing multplier for the buck converter is set according to 
  * 		\p tMult
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetBuckTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_TMULT tMult);

/**
  * @brief 	Set the minimum impedance of the thermistor for the storage 
  * 		element charging
  * 
  * @details	This function writes the TEMPCOLDCH register to match the 
  * 		impedance defined in \p Rth
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	Rth		Thermistor's impedance (Rth) at the temperature 
  * 				threshold (mOhms)
  * @param[in] 	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post 	The temperature threshold is set according to \p Rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold will have no effect
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see	AEM13920_EnableTemperatureMonitoring()
  */
int32_t AEM13920_SetChargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv);

/**
  * @brief 	Set the minimum impedance of the thermistor for the storage 
  * 		element discharging
  * 
  * @details	This function writes the TEMPCOLDDIS register to match the 
  * 		impedance defined in \p Rth
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	Rth		Thermistor's impedance (Rth) at the temperature 
  * 				threshold (mOhms)
  * @param[in] 	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post 	The temperature threshold is set according to \p Rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold will have no effect
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see	AEM13920_EnableTemperatureMonitoring()
  */
int32_t AEM13920_SetDischargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv);

/**
  * @brief 	Set the maximum impedance of the thermistor for storage element
  * 		charging
  * 
  * @details	This function writes the TEMPHOTCH register to match the 
  * 		impedance defined in \p Rth
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	Rth		Thermistor's impedance (Rth) at the temperature 
  * 				threshold (mOhms)
  * @param[in] 	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post 	The temperature threshold is set according to \p Rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold will have no effect
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see	AEM13920_EnableTemperatureMonitoring()
  */
int32_t AEM13920_SetChargeHotThresholdZ(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t Rth, uint32_t Rdiv);

/**
  * @brief 	Set the maximum impedance of the thermistor for storage element
  * 		discharging
  * 
  * @details	This function writes the TEMPHOTDIS register to match the 
  * 		impedance defined in \p Rth
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	Rth		Thermistor's impedance (Rth) at the temperature 
  * 				threshold (mOhms)
  * @param[in] 	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post 	The temperature threshold is set according to \p Rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold will have no effect
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see	AEM13920_EnableTemperatureMonitoring()
  */
int32_t AEM13920_SetDischargeHotThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv);

/**
  * @brief 	Enable the monitoring of the ambient temperature
  * 
  * @details	This function sets the EN field of the TMON register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The monitoring of the ambient temperature is enabled
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see	AEM13920_SetTemperatureThresholds()
  */
int32_t AEM13920_EnableTemperatureMonitoring(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Disable the monitoring of the ambient temperature
  * 
  * @details	This function resets the EN field of the TMON register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The monitoring of the ambient temperature is disabled
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_DisableTemperatureMonitoring(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Set the sleep threshold for the selected source
  * 
  * @details	This function sets the SRCxTHRESH field of the SLEEP register to
  * 		match the threshold defined in \p thresh
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in]	src		Selected source
  * @param[in] 	threshold 	Sleep voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The sleep voltage threshold for the \p src is set to 
  * 		\p thresh
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetSleepThreshold(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, 			\
					AEM13920_SLEEP_THRESHOLD thresh);

/**
  * @brief 	Select the APM functioning mode
  * 
  * @details	This function writes the MODE field of the APM register
  * 		according to \p mode
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	mode 		APM mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p mode is a valid \ref AEM13920_APM_MODE
  * @post	The APM functioning mode is set according to \p mode
  * @note	This configuration will have no effect if the Average Power
  * 		Monitoring is not enabled
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * @see 	AEM13920_EnableAPM()
  */
int32_t AEM13920_SetAPMMode(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_MODE mode);

/**
  * @brief 	Set the APM computation window
  * 
  * @details	This function writes the WINDOW field of the APM register
  * 		according to \p window
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	window 		Computation window
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p window is a valid \ref AEM13920_APM_WINDOW
  * @post	The APM computation window is set according to \p window
  * @note	This configuration will have no effect if the Average Power
  * 		Monitoring is not enabled
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * 
  * @see 	AEM13920_EnableAPMSource()
  * @see	AEM13920_EnableAPMBuck()
  */
int32_t AEM13920_SetAPMWindow(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_WINDOW window);

/**
  * @brief 	Enable the Average Power Monitoring for the selected source
  * 
  * @details	This function sets the SRCxEN field of the APM register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The Average Power Monitoring is enabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * 
  * @see	AEM13920_SetAPMMode()
  * @see	AEM13920_SetAPMWindow()
  */
int32_t AEM13920_EnableAPMSource(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Disable the Average Power Monitoring for the selected source
  * 
  * @details	This function resets the SRCxEN field of the APM register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @post	The Average Power Monitoring is disabled for the selected source
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_DisableAPMSource(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src);

/**
  * @brief 	Enable the Average Power Monitoring for the buck converter
  * 
  * @details	This function sets the BUCKEN field of the APM register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is enabled for the buck converter
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  * 
  * @see	AEM13920_SetAPMMode()
  * @see	AEM13920_SetAPMWindow()
  */
int32_t AEM13920_EnableAPMBuck(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Disable the Average Power Monitoring for the buck converter
  * 
  * @details	This function resets the BUCKEN field of the APM register
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is disabled for the buck converter
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_DisableAPMBuck(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Configure the AEM's IRQs
  * 
  * @details	This function writes the IRQEN0 and IRQEN1 registers according 
  * 		to \p irqs
  * 
  * @param[in] 	i2cCfg		Communication interface's required information
  * @param[in] 	irqs 		IRQ's configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The AEM's IRQ are configured according to \p irqs
  * @warning	The AEM configuration will not be updated until the 
  * 		AEM13920_LoadConfiguration() function has been called
  */
int32_t AEM13920_SetIRQConfiguration(	const AEM_i2c_cfg *i2cCfg, 		\
					const AEM13920_IRQEN *irqs);

/**
  * @brief	Update the AEM's behavior according to the configuration set 
  * 		into the I2C registers
  * 
  * @details	This function sets the UPDATE field of the CTRL register and
  * 		blocks until the synchronisation is done
  *
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @post	The AEM configuration has been updated
  */
int32_t AEM13920_LoadConfiguration(const AEM_i2c_cfg *i2cCfg);

/**
  * @brief 	Read the whole AEM configuration and set \p cfg accordingly
  * 
  * @details	This function reads the whole configurable area of the register
  * 		map and sets \p cfg accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out] cfg		Configuration settings
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @pre	\p cfg->Rdiv is correctly set (so that the temperature thresholds
  * 		can be computed)
  * @post	\p cfg is set with information corresponding to the AEM's
  * 		configuration
  */
int32_t AEM13920_GetConfiguration(const AEM_i2c_cfg *i2cCfg, AEM13920_Config *cfg);

/**
  * @brief 	Get the configured regulation mode for the selected source
  * 
  * @details	This function reads the SRCxREGU0 register and sets \p mode 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	mode		Configured regulation mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @pre	\p src is a valid source
  * @post	\p mode is set with the configured regulation mode for the \c src
  */
int32_t AEM13920_GetSourceRegulationMode( 	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						AEM13920_SRCREGU_MODE *mode);


/**
  * @brief 	Get the configured MPPT ratio for the selected source
  * 
  * @details	This function reads the CFG0 field of the SRCxREGU0 register and 
  * 		sets \p ratio accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	ratio 		Configured MPPT ratio
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE "Status/Error code"
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p ratio is not NULL
  * @pre	The source regulation mode is set to AEM13920_SRCREGU_MPPT
  * @post	\p ratio is set with the configured ratio between the maximum 
  * 		power point voltage and the open circuit voltage
  */
int32_t AEM13920_GetMPPTRatio(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_MPPT_RATIO *ratio);

/**
  * @brief 	Get the configured MPPT timings
  * 
  * @details	This function reads the CFG1 and CFG2 fields of the SRCxREGU1 
  * 		register and sets \p timings accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	duration 	Configured duration
  * @param[out]	period		Configured period
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE "Status/Error code"
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p duration is not NULL
  * @pre	\p period is not NULL
  * @pre	The source regulation mode is set to AEM13920_SRCREGU_MPPT
  * @post	\p duration and \p period are set with the configured sampling 
  * 		duration and period of the open circuit voltage evaluation
  */
int32_t AEM13920_GetMPPTTimings(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_DURATION *duration,		\
				AEM13920_MPPT_PERIOD *period);

/**
  * @brief 	Get the configured minimum voltage accepted on the storage element
  *		before stopping to supply LOAD (mV)
  * 
  * @details	This function reads the VOVDIS register and sets \p Vovdis 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Vovdis 		Configured 'discharge' voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Vovdis is not NULL
  * @post	\p Vovdis is set according to the configured 'discharge' voltage 
  * 		threshold
  */
int32_t AEM13920_GetDischargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vovdis);

/**
  * @brief 	Get the configured minimum voltage accepted on the storage element
  * 		before starting to supply LOAD in START STATE (mV)
  * 
  * @details	This function reads the VCHRDY register and sets \p Vchrdy 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Vchrdy 		Configured 'charge ready' voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Vchrdy is not NULL
  * @post	\p Vchrdy is set according to the configured 'charge ready' 
  * 		voltage threshold
  */
int32_t AEM13920_GetChargeReadyVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vchrdy);

/**
  * @brief 	Get the configured maximum voltage accepted on the storage element
  *		before disabling its charging (mV)
  * 
  * @details	This function reads the VOVCH register and sets \p Vovch 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Vovch 		Configured 'overcharge' voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Vovch is not NULL
  * @post	\p Vovch is set according to the configured 'overcharge' voltage 
  * 		threshold
  */
int32_t AEM13920_GetOverchargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vovch);

/**
  * @brief 	Get the enabling status of the selected source's boost
  * 
  * @details	This function reads the EN field of the BSTxCFG register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	en 		Enabling status of the selected source's boost
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the selected source's boost is enabled,
  * 		to \c false otherwise
  */
int32_t AEM13920_GetBoostEnable(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				bool *en);

/**
  * @brief 	Get the configured timing multiplier of the selected source's boost
  * 
  * @details	This function reads the TMULT field of the BSTxCFG register and 
  * 		sets \p tMult accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	tMult 		Timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p tMult is not NULL
  * @post	\p tMult is set with the configured timing multiplier for the 
  * 		selected source's boost
  */
int32_t AEM13920_GetBoostTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_TMULT *tMult);

/**
  * @brief 	Get the enabling status of the Automatic High Power feature for
  * 		the selected source
  * 
  * @details	This function reads the HPEN field of the BSTxCFG register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	en 		Enabling status of the Automatic High Power feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the Automatic High Power feature 
  * 		is enabled for the selected source, to \c false otherwise
  */
int32_t AEM13920_GetHighPowerEnable(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, bool *en);

/**
  * @brief 	Get the configured buck converter's output regulation voltage
  * 
  * @details	This function reads the VOUT field of the BUCKCFG register and 
  * 		sets vLoad accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	vLoad		Buck converter output regulation voltage
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p vLoad is not NULL
  * @post	\p vLoad is set with the buck converter output regulation voltage
  */
int32_t AEM13920_GetBuckVLoad(const AEM_i2c_cfg *i2cCfg, AEM13920_VLOAD *vLoad);

/**
  * @brief 	Get the configured timing multiplier for the buck converter
  * 
  * @details	This function reads the TMULT field ot the BUCKCFG register
  * 		and sets tMult accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	tMult		Timing configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p tMult is not NULL
  * @post	\p tMult is set with the configured timing multplier for the 
  * 		buck converter
  */
int32_t AEM13920_GetBuckTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_TMULT *tMult);

/**
  * @brief 	Get the configured minimum impedance of the thermistor for the 
  * 		storage element charging
  * 
  * @details	This function reads the TEMPCOLDCH register and sets \p Rth 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Rth		Configured thermistor's impedance (Rth) at the 
  * 				temperature threshold (mOhms)
  * @param[in] 	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Rth is not NULL
  * @post 	\p Rth is set according to the configured thermistor's impedance 
  * 		(Rth) at the temperature threshold (mOhms)
  */
int32_t AEM13920_GetChargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t *Rth, uint32_t Rdiv);

/**
  * @brief 	Get the configured minimum impedance of the thermistor for the 
  * 		storage element discharging
  * 
  * @details	This function reads the TEMPCOLDDIS register and sets \p Rth 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Rth		Configured thermistor's impedance (Rth) at the 
  * 				temperature threshold (mOhms)
  * @param[in]	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Rth is not NULL
  * @post 	\p Rth is set according to the configured thermistor's impedance 
  * 		(Rth) at the temperature threshold (mOhms)
  */
int32_t AEM13920_GetDischargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t *Rth, uint32_t Rdiv);

/**
  * @brief 	Get the configured maximum impedance of the thermistor for 
  * 		storage element charging
  * 
  * @details	This function reads the TEMPHOTCH register and sets \p Rth 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	Rth		Configured thermistor's impedance (Rth) at the 
  * 				temperature threshold (mOhms)
  * @param[in]	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Rth is not NULL
  * @post 	\p Rth is set according to the configured thermistor's impedance 
  * 		(Rth) at the temperature threshold (mOhms)
  */
int32_t AEM13920_GetChargeHotThresholdZ(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t *Rth, uint32_t Rdiv);

/**
  * @brief 	Get the configured maximum impedance of the thermistor for 
  * 		storage element discharging
  * 
  * @details	This function reads the TEMPHOTDIS register and sets \p Rth 
  * 		accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out] Rth		Configured thermistor's impedance (Rth) at the 
  * 				temperature threshold (mOhms)
  * @param[in]	Rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p Rth is not NULL
  * @post 	\p Rth is set according to the configured thermistor's impedance 
  * 		(Rth) at the temperature threshold (mOhms)
  */
int32_t AEM13920_GetDischargeHotThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t *Rth, uint32_t Rdiv);

/**
  * @brief 	Get the enabling status of the Temperature Monitoring feature
  * 
  * @details	This function reads the EN field of the TMON register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	en 		Enabling status of the Temperature Monitoring feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the Temperature Monitoring feature 
  * 		is enabled, to \c false otherwise
  */
int32_t AEM13920_GetTempMonEnable(const AEM_i2c_cfg *i2cCfg, bool *en);

/**
  * @brief 	Get the configured APM functioning mode
  * 
  * @details	This function reads the MODE field of the APM register and 
  * 		sets \p mode accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	mode 		Configured APM mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @post	\p mode is set with the configured APM mode
  */
int32_t AEM13920_GetAPMMode(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_MODE *mode);

/**
  * @brief 	Get the configured APM computation window
  * 
  * @details	This function reads the WINDOW field of the APM register and 
  * 		sets \p window accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	window 		Configured APM computation window
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p window is not NULL
  * @post	\p window is set with the configured APM computation window
  */
int32_t AEM13920_GetAPMWindow(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_WINDOW *window);

/**
  * @brief 	Get the enabling status of the Average Power Monitoring for the
  * 		selected source
  * 
  * @details	This function reads the SRCxEN field of the APM register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	en 		Enabling status of the APM feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p src is a valid source
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the APM feature is enabled for the 
  * 		selected source, to \c false otherwise
  */
int32_t AEM13920_GetAPMSourceEnable(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, bool *en);

/**
  * @brief 	Get the enabling status of the Average Power Monitoring for the
  * 		buck converter
  * 
  * @details	This function reads the BUCKEN field of the APM register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	en 		Enabling status of the APM feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the APM feature is enabled for the 
  * 		buck converter, to \c false otherwise
  */
int32_t AEM13920_GetAPMBuckEnable(const AEM_i2c_cfg *i2cCfg, bool *en);

/**
  * @brief 	Get the configuration of the AEM's IRQ
  * 
  * @details	This function reads the IRQEN0 and IRQEN1 registers and sets 
  * 		\p irqs accordingly
  * 
  * @param[in] 	i2cCfg		Communication interface's required information 
  * @param[out]	irqs 		Configuration of the AEM's IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p i2cCfg is not NULL
  * @pre	\p i2cCfg has been correctly initialized, as described in the 
  * 		\ref AEM_Driver_Comm API
  * @pre	\p irqs is not NULL
  * @post	\p irqs is set with the configuration of the IRQs
  */
int32_t AEM13920_GetIRQConfiguration(const AEM_i2c_cfg *i2cCfg, AEM13920_IRQEN *irqs);

#ifdef __cplusplus
}
#endif

/** @} *//* (end \defgroup AEM13920_Driver_API) */
#endif /* HAL_AEM13920_H_ */
