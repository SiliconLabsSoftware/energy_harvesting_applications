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
  * 	- First public release
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
  * \version	2.0.0
  * 	Breaking Change
  * 	- Complete API revision
  * \version	2.0.1
  * 	Bug Fix
  * 	- Small fix in IsAPMSourceOverflow and IsAPMSourceCorrupted functions
  * 	- Small fix in GetAPMXXXData functions
  * 	- Bug fix in GetTempXXXStatus
  * \version	2.0.2
  * 	Bug Fix
  * 	- Make GetConfiguration also return the regulation mode
  * 	- Fix error in SetConfiguration for sources' config
  * 	- Remove useless include
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
  * @brief 	AEM13920 handler
  */
typedef struct _AEM13920_Handler_t {
	/**
	  * @brief 	Communication interface's required information
	  */
	AEM_i2c_cfg *i2c_cfg;
	/**
	  * @brief 	Revision
	  */
	uint8_t revision;
} AEM13920_Handler_t;

/**
  * @brief 	AEM13920's configuration settings
  */
typedef struct _AEM13920_CONFIG {

	/* ~~~ Average Power Monitoring ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	
	/**
	  * @brief 	Average Power Monitoring operation mode
	  * @note	This setting is only required when using the Average
	  * 		Power Monitoring feature.
	  */
	AEM13920_APM_MODE apm_mode;
	/**
	  * @brief 	Computation window for the Average Power Monitoring
	  * @note	This setting is only required when using the Average
	  * 		Power Monitoring feature.
	  */
	AEM13920_APM_WINDOW apm_window;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for SRC1
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_src1_enable;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for SRC2
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_src2_enable;
	/**
	  * @brief	Enable/Disable the Average Power Monitoring for the buck
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_buck_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		new APM data are available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_done_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when an
	  *		APM error occurs
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool apm_err_irq_enable;

	/* ~~~ I2C Ready ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when the 
	  * 		AEM is out of reset and ready to communicate through I2C
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool i2c_rdy_irq_enable;

	/* ~~~ SRC Low Thresholds ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) on any
	  *		transition between a state where at least one source 
	  *		voltage is above its low threshold and a state where 
	  *		both are below their respective thresholds.
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src_low_irq_enable;
	/**
	  * @brief 	SRC1 Low threshold
	  * @details	Set the SRC1 voltage below which the AEM enters SLEEP 
	  *		STATE
	  */
	AEM13920_SRCLOW_THRESH src1_low_thresh;
	/**
	  * @brief 	SRC2 Low threshold
	  * @details	Set the SRC2 voltage below which the AEM enters SLEEP 
	  *		STATE
	  */
	AEM13920_SRCLOW_THRESH src2_low_thresh;

	/* ~~~ SRC1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	SRC1 Regulation mode
	  * @details 	Set the SRC1 regulation mode
	  */
	AEM13920_SRCREGU_MODE src1_regu_mode;
	/**
	  * @brief 	SRC1 Regulation Voltage (mV)
	  * @details 	Set the SRC1 regulation voltage
	  * @warning 	This setting is only used when the SRC1's regulation 
	  *		mode is set to constant voltage
	  */
	uint32_t src1_const_voltage;
	/**
	  * @brief 	MPPT ratio
	  * @details 	Set the SRC1 Maximum Power Point tracking ratio
	  * @warning 	This setting is only used when the SRC1's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_RATIO src1_mppt_ratio;
	/**
	  * @brief 	MPPT duration
	  * @details 	Set the SRC1 Maximum Power Point Tracking duration
	  * @warning 	This setting is only used when the SRC1's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_DURATION src1_mppt_duration;
	/**
	  * @brief 	MPPT period
	  * @details 	Set the SRC1 Maximum Power Point Tracking period
	  * @warning 	This setting is only used when the SRC1's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_PERIOD src1_mppt_period;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT starts for SRC1
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_mppt_start_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT is done for SRC1
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_mppt_done_irq_enable;
	/**
	  * @brief 	Enable/Disable the SRC1 boost converter
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_boost_enable;
	/**
	  * @brief 	Enable/Disable the High Power feature for the SRC1 boost 
	  *		converter
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src1_boost_high_power_enable;
	/**
	  * @brief 	SRC1 boost converter's timing multiplier
	  * @details	Set the timing multiplier for the SRC1 boost converter.
	  *		The higher the timing multiplier, the higher the average
	  *		source current pulled from the SRC1 to charge the storage
	  *		element
	  */
	AEM13920_TMULT src1_boost_tmult;

	/* ~~~ SRC2 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	SRC2 Regulation mode
	  * @details 	Set the SRC2 regulation mode
	  */
	AEM13920_SRCREGU_MODE src2_regu_mode;
	/**
	  * @brief 	SRC2 Regulation Voltage (mV)
	  * @details 	Set the SRC2 regulation voltage
	  * @warning 	This setting is only used when the SRC2's regulation 
	  *		mode is set to constant voltage
	  */
	uint32_t src2_const_voltage;
	/**
	  * @brief 	MPPT ratio
	  * @details 	Set the SRC2 Maximum Power Point tracking ratio
	  * @warning 	This setting is only used when the SRC2's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_RATIO src2_mppt_ratio;
	/**
	  * @brief 	MPPT duration
	  * @details 	Set the SRC2 Maximum Power Point Tracking duration
	  * @warning 	This setting is only used when the SRC2's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_DURATION src2_mppt_duration;
	/**
	  * @brief 	MPPT period
	  * @details 	Set the SRC2 Maximum Power Point Tracking period
	  * @warning 	This setting is only used when the SRC2's regulation 
	  *		mode is set to MPPT
	  */
	AEM13920_MPPT_PERIOD src2_mppt_period;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT starts for SRC2
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_mppt_start_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the MPPT is done for SRC2
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_mppt_done_irq_enable;
	/**
	  * @brief 	Enable/Disable the SRC2 boost converter
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_boost_enable;
	/**
	  * @brief 	Enable/Disable the High Power feature for the SRC2 boost
	  *		converter
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool src2_boost_high_power_enable;
	/**
	  * @brief 	SRC2 boost converter's timing multiplier
	  * @details	Set the timing multiplier for the SRC2 boost converter.
	  *		The higher the timing multiplier, the higher the average
	  *		source current pulled from the SRC2 to charge the storage
	  *		element
	  */
	AEM13920_TMULT src2_boost_tmult;

	/* ~~~ Buck converter configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	Buck converter output regulation voltage (mV)
	  * @details 	Set the buck converter output regulation voltage
	  */
	AEM13920_VOUT buck_vout;
	/**
	  * @brief 	Buck converter's timing multiplier
	  * @details 	Set the timing multiplier for the buck converter
	  */
	AEM13920_TMULT buck_tmult;

	/* ~~~ Storage Element Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief	Overdischarge (VOVDIS) voltage threshold
	  * @details	Minimum voltage accepted on the storage element
	  *		before stopping to supply the LOAD (mV)
	  */
	uint32_t vovdis;
	/**
	  * @brief 	Charge Ready (VCHRDY) voltage threshold (mV)
	  * @details	Minimum voltage accepted on the storage element before 
	  *		starting to supply LOAD (mV)
	  */
	uint32_t vchrdy;
	/**
	  * @brief 	Overcharge (VOVCH) threshold
	  * @details	Maximum voltage accepted on the storage element
	  *		before disabling is charging (mV)
	  */
	uint32_t vovch;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the overdischarge threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vovdis_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the charge ready threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vchrdy_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the storage level crosses the overcharge threshold
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool vovch_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		a new data from the STO ADC is available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool sto_done_irq_enable;

	/* ~~~ Temperature Monitoring ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	/**
	  * @brief 	Enable/Disable the ambient Temperature Monitoring
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_mon_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the ambient temperature is out of the defined range for
	  *		storage element charging
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_ch_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		the ambient temperature is out of the defined range for
	  *		storage element discharging
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_dis_irq_enable;
	/**
	  * @brief 	Enable/Disable the IRQ pin to be asserted (HIGH) when 
	  *		a new data from the temperature ADC is available
	  * @details	Set to \c true to enable, to \c false to disable
	  */
	bool temp_done_irq_enable;
	/**
	  * @brief 	Impedance of the resistor Rdiv, that creates a resistive 
	  * 		voltage divider together with the thermistor Rth (mOhms)
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t temp_rdiv;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'cold' threshold 
	  * 		(mOhms) for storage element charging
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t temp_cold_ch_rth;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'hot' threshold 
	  * 		(mOhms) for storage element charging
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t temp_hot_ch_rth;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'cold' threshold 
	  * 		(mOhms) for storage element discharging
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t temp_cold_dis_rth;
	/**
	  * @brief 	Impedance of the thermistor (Rth) at the 'hot' threshold 
	  * 		(mOhms) for storage element discharging
	  * @note	This setting is only required when using the Temperature
	  * 		Monitoring feature.
	  */
	uint32_t temp_hot_dis_rth;

} AEM13920_CONFIG_t;


/**
  * @brief 	Initialize the driver
  *
  * @details 	Initialize the communication interface and retrieve the IC version
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	The AEM is out of its reset sequence (see the datasheet, 
  *		"Configuration by I2C")
  * @post	The communication interface is initialized, and the driver's
  * 		functions can be used
  * @post 	\p handler->version is set with the version of the chip
  */
int32_t AEM13920_Initialize(AEM13920_Handler_t *handler);

/**
  * @brief 	Deinitialize the driver
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	Any resources allocated to the communication interface are
  * 		released. Any call to the driver's functions will fail until
  * 		the AEM13920_Initialize() function is called
  */
int32_t AEM13920_Deinitialize(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the SRCx regulation mode
  * @details 	This function reads the SRCxREGU0 register and sets \p mode
  *		accordingly, where \c x is the selected source
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	mode		Source regulation mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @post	\p mode is set with the source regulation mode
  */
int32_t AEM13920_GetSourceRegulationMode(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_SRCREGU_MODE *mode						\
);

/**
  * @brief 	Set SRCx' regulation mode
  * @details 	This function writes the SRCxREGU0 register to match the mode
  *		defined in \p mode, where \c x is the selected source
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	mode		Source regulation mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The source regulation mode is set according to \p mode
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called. 
  */
int32_t AEM13920_SetSourceRegulationMode(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_SRCREGU_MODE mode						\
);

/**
  * @brief 	Get the voltage at which the SRCx is regulated
  * 
  * @details	If SRCx' regulation mode is set to constant voltage, this 
  *		function reads the SRCxREGU0 and SRCxREGU1 registers, where 
  *		\c x is the selected source, and sets \p srcregu accordingly.
  *		Otherwise, it reads the SRCx register and sets \p srcregu 
  *		accordingly.
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	srcregu 	Regulation Voltage (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p srcregu is not NULL
  * @post	If SRCx' regulation mode is set to constant voltage, \p srcregu 
  *		is set with the configured source regulation voltage. 
  *		Otherwise, it is set with the voltage at which the source is 
  *		actually regulated via the MPPT.
  */
int32_t AEM13920_GetSourceRegulationVoltage(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	uint32_t *srcregu							\
);

/**
  * @brief 	Set SRCx's regulation to a constant voltage
  * 
  * @details	This function writes the SRCxREGU0 and SRCxREGU1 registers, where 
  *		\c x is the selected source.
  *		It sets the source regulation mode to constant voltage and 
  *		configures the regulation voltage according to \p srcregu. The 
  *		source regulation voltage is set to the closest authorized value.
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	srcregu 	Regulation Voltage (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The source regulation voltage is set according to \p srcregu
  * @post 	The source regulation mode is set to constant voltage
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @warning 	Calling this function sets the source regulation mode to
  *		constant voltage, no matter what the previous mode was.
  */
int32_t AEM13920_SetSourceConstantVoltage(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	uint32_t srcregu							\
);

/**
  * @brief 	Get the MPPT configuration for SRCx (ratio, period, and duration)
  *
  * @details 	This function reads the SRCxREGU0 and SRCxREGU1 registers and 
  *		sets \p cfg accordingly, where \c x is the selected source
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in] 	src		Source
  * @param[out]	cfg		MPPT configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @post	If the SRCx regulation mode is set to MPPT, \p cfg is set with 
  *		the MPPT configuration of SRCx. Otherwise, \p cfg remains
  *		unchanged.
  * @warning 	\p cfg is only valid when the source regulation mode is set to
  *		MPPT.
  */
int32_t AEM13920_GetMPPTConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPTCFG_t *cfg							\
);

/**
  * @brief 	Set the MPPT configuration for SRCx (ratio, period, and duration)
  *
  * @details	This function writes the SRCxREGU0 and SRCxREGU1 registers, where 
  *		\c x is the selected source,
  *		It sets the source regulation mode to MPPT and configures the
  *		MPPT according to \p cfg.
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in] 	src 		Source to configure
  * @param[in] 	cfg 		MPPT configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is correctly initialized
  * @post	The MPPT configuration is set according to \p cfg
  * @post	The source regulation mode is set to MPPT
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @warning 	Calling this function sets the source regulation mode to MPPT, 
  *		no matter what the previous mode was.
  */
int32_t AEM13920_SetMPPTConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	const AEM13920_MPPTCFG_t *cfg						\
);

/**
  * @brief 	Get the configured MPPT ratio for SRCx
  *
  * @details 	This function reads the CFG0 field of the SRCxREGU0 register and 
  *		sets \p ratio accordingly, where \c x is the selected source.
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	ratio		MPPT ratio
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p ratio is not NULL
  * @post	If the SRCx regulation mode is set to MPPT, \p ratio is set with 
  *		the configured MPPT ratio. Otherwise, \p ratio remains unchanged.
  */
int32_t AEM13920_GetMPPTRatio(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_RATIO *ratio						\
);

/**
  * @brief 	Set the MPPT ratio for SRCx
  *
  * @details 	This function writes CFG0 field of the SRCxREGU0 register to 
  *		match the ratio set in \p ratio, where \c x is the selected 
  *		source.
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	ratio		MPPT ratio
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The MPPT ratio is set according to \p ratio
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @warning   	This function should only be used if the source regulation 
  *		mode has been previously set to MPPT via a call to the 
  *		AEM13920_SetSourceRegulationMode() function. Otherwise, it would 
  *		overwrite the constant voltage configuration, leading to 
  *		unpredictable behavior.
  */
int32_t AEM13920_SetMPPTRatio(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_RATIO ratio						\
);

/**
  * @brief 	Get the configured MPPT duration for SRCx
  *
  * @details 	This function reads the CFG1 field of the SRCxREGU0 register and
  *		sets \p duration accordingly, where \c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	duration	MPPT duration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p duration is not NULL
  * @post	If the SRCx regulation mode is set to MPPT, \p duration is set 
  *		with the configured MPPT duration. Otherwise, \p ratio remains 
  *		unchanged.
  */
int32_t AEM13920_GetMPPTDuration(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_DURATION *duration					\
);

/**
  * @brief 	Set the MPPT duration for SRCx
  *
  * @details 	This function writes the CFG1 field of the SRCxREGU1 register to
  *		match the duration set in \p duration, where \c x is the 
  *		selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	duration	MPPT duration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The MPPT duration is set according to \p duration
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @warning   	This function should only be used if the source regulation 
  *		mode has been previously set to MPPT via a call to the 
  *		AEM13920_SetSourceRegulationMode() function. Otherwise, it would 
  *		overwrite the constant voltage configuration, leading to 
  *		unpredictable behavior.
  */
int32_t AEM13920_SetMPPTDuration(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_DURATION duration						\
);

/**
  * @brief 	Get the configured MPPT period for SRCx
  *
  * @details 	This function reads the CFG2 field of the SRCxREGU1 register and
  *		sets \p period accordingly, where \c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	period		MPPT period
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p period is not NULL
  * @post	If the SRCx regulation mode is set to MPPT, \p period is set with 
  *		the configured MPPT period. Otherwise, \p period remains unchanged.
  * 
  */
int32_t AEM13920_GetMPPTPeriod(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_PERIOD *period						\
);

/**
  * @brief 	Set the MPPT period for SRCx
  *
  * @details 	This function writes the CFG2 field of the SRCxREGU1 register to
  *		match the period set in \p period, where \c x is the selected 
  *		source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	period		MPPT period
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The MPPT period is set according to \p period
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @warning   	This function should only be used if the source regulation 
  *		mode has been previously set to MPPT via a call to the 
  *		AEM13920_SetSourceRegulationMode() function. Otherwise, it would
  *		overwrite the constant voltage configuration, leading to 
  *		unpredictable behavior.
  */
int32_t AEM13920_SetMPPTPeriod(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_PERIOD period						\
);

/**
  * @brief 	Get the configured VOVDIS threshold (mV)
  * 
  * @details	This function reads the VOVDIS register and sets \p vovdis 
  * 		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	vovdis 		Configured overdischarge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p vovdis is not NULL
  * @post	\p vovdis is set according to the configured 'overdischarge' 
  * 		voltage threshold
  */
int32_t AEM13920_GetOverdischargeVoltage(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vovdis							\
);

/**
  * @brief 	Set the VOVDIS threshold (mV)
  * 
  * @details	This function writes the VOVDIS register to match the voltage
  * 		defined in \p vovdis
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in] 	vovdis 		Overdischarge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The overdischarge threshold is set according to \p vovdis
  * @note	If \p vovdis is under 2.4V, the effective threshold is set to 
  *		2.4V. If \p vovdis is above 3.581V, the effective threshold is 
  *		set to 3.581V.
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetOverdischargeVoltage(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vovdis								\
);

/**
  * @brief 	Get the configured VCHRDY threshold (mV)
  *
  * @details	This function reads the VCHRDY register and sets \p vchrdy
  * 		accordingly
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	vchrdy 		Configured charge ready voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p vchrdy is not NULL
  * @post	\p vchrdy is set according to the configured 'charge ready'
  * 		voltage threshold
  */
int32_t AEM13920_GetChargeReadyVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vchrdy							\
);

/**
  * @brief 	Set the VCHRDY threshold (mV)
  *
  * @details	This function writes the VCHRDY register to match the voltage
  * 		defined in \p vchrdy
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in] 	vchrdy 		Charge ready voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The charge ready threshold is set according to \p vchrdy
  * @note	If \p vchrdy is under 2.456V, the effective threshold is set to 
  *		2.456V. If \p vchrdy is above 3.637V, the effective threshold is 
  *		set to 3.637V.
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetChargeReadyVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vchrdy								\
);

/**
  * @brief 	Get the configured VOVCH threshold (mV)
  * 
  * @details	This function reads the VOVCH register and sets \p vovch 
  * 		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	vovch 		Configured overcharge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p vovch is not NULL
  * @post	\p vovch is set according to the configured 'overcharge' 
  * 		voltage threshold
  */
int32_t AEM13920_GetOverchargeVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vovch								\
);

/**
  * @brief 	Set the VOVCH threshold (mV)
  * 
  * @details	This function writes the VOVCH register to match the voltage
  * 		defined in \p vovch
  * 
  * @param[in] 	i2c_cfg		Communication interface's required information
  * @param[in]	vovch 		Overcharge voltage threshold (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The overcharge threshold is set according to \p vovch
  * @note	If \p vovch is under 2.7V, the effective threshold is set to 
  *		2.7V. If \p vovch is above 4.594V, the effective threshold is 
  *		set to 4.594V.
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetOverchargeVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vovch								\
);

/**
  * @brief 	Get the boost configuration for SRCx
  *
  * @details 	This function reads the BSTxCFG and sets \p cfg accordingly,
  *		where \c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	cfg		Boost configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @post	\p cfg is set with the boost configuration
  */
int32_t AEM13920_GetBoostConfig(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_BSTCFG_t *cfg							\
);

/**
  * @brief 	Set the boost configuration for SRCx
  * 
  * @details 	This function writes the BSTxCFG register to match the
  *		configuration defined in \p cfg, where \c x is the selected 
  *		source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	cfg		Boost configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p cfg is correctly initialized
  * @post	The boost configuration is set according to \p cfg
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetBoostConfig(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	const AEM13920_BSTCFG_t *cfg						\
);

/**
  * @brief 	Enable the boost converter for SRCx
  * 
  * @details 	This function sets the EN field of the BSTxCFG register, where 
  *		\c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	SRCx' boost converter is enabled
  * @warning 	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableBoost(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Disable the boost converter for SRCx
  * 
  * @details 	This function clears the EN field of the BSTxCFG register, where 
  *		\c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	SRCx' boost converter is disabled
  * @warning 	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableBoost(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Get the enabling status of the boost converter for SRCx
  * 
  * @details 	This function reads the EN field of the BSTxCFG register, where 
  *		\c x is the selected source, and sets \p en accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	en		Boost converter's enabling status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set with the enabling status of SRCx' boost converter
  */
int32_t AEM13920_IsEnabledBoost(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	bool *en								\
);

/**
  * @brief 	Enable the high power mode for SRCx
  * 
  * @details 	This function sets the HPEN field of the BSTxCFG register, where 
  *		\c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	SRCx' high power mode is enabled
  * @warning 	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Diable the high power mode for SRCx
  * 
  * @details 	This function clears the HPEN field of the BSTxCFG register, where 
  *		\c x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	SRCx' high power mode is disabled
  * @warning 	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Get the enabling status of the high power mode for SRCx
  * 
  * @details 	This function reads the HPEN field of the BSTxCFG register, where 
  *		\c x is the selected source, and sets \p en accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	en		High power mode's enabling status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set with the enabling status of SRCx' high power mode
  */
int32_t AEM13920_IsEnabledHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, bool *en						\
);

/**
  * @brief 	Set the boost converter's timing multiplier for SRCx
  * 
  * @details 	This function writes the TMULT field of the BSTxCFG register, 
  *		where \c x is the selected source, to match the value defined in
  *		\p tmult.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source to configure
  * @param[in]	tmult		Timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The timing multiplier is set according to \p tmult
  * @warning	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetBoostTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_TMULT tmult							\
);

/**
  * @brief 	Get the configured boost converter's timing multiplier for SRCx
  * 
  * @details 	This function reads the TMULT field of the BSTxCFG register, 
  *		where \c x is the selected source, and sets \p tmult accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Source
  * @param[out]	tmult		Boost converter's timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p tmult is not NULL
  * @post	\p tmult is set with the configured timing multiplier
  */
int32_t AEM13920_GetBoostTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_TMULT *tmult							\
);

/**
  * @brief 	Set the buck converter's configuration
  * 
  * @details 	This function writes the BUCKCFG register to match the
  *		configuration defined in \p cfg.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	cfg		Buck configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p cfg is correctly initialized
  * @post	The buck configuration is set according to \p cfg
  * @warning	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetBuckConfig(							\
	const AEM13920_Handler_t *handler,					\
	const AEM13920_BUCKCFG_t *cfg						\
);

/**
  * @brief 	Get the buck converter's configuration
  * 
  * @details 	This function reads the BUCKCFG register and sets \p cfg
  *		accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	cfg		Buck configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @post	\p cfg is set with the buck configuration
  */
int32_t AEM13920_GetBuckConfig(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_BUCKCFG_t *cfg							\
);

/**
  * @brief 	Set the buck converter's output regulation voltage
  * 
  * @details 	This function writes the VOUT field of the BUCKCFG register to
  *		match the voltage defined in \p vout.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	vout		Buck output voltage
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The buck output voltage is set according to \p vout
  * @warning	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetBuckVOUT(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_VOUT vout							\
);

/**
  * @brief 	Get the buck converter's output regulation voltage
  * 
  * @details 	This function reads the VOUT field of the BUCKCFG register and
  *		sets \p vout accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	vout		Buck output voltage (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p vout is not NULL
  * @post	\p vout is set with the buck output voltage
  */
int32_t AEM13920_GetBuckVOUT(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_VOUT *vout							\
);

/**
  * @brief 	Set the buck converter's timing multiplier
  * 
  * @details 	This function writes the TMULT field of the BUCKCFG register to
  *		match the value defined in \p tmult.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	tmult		Timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post	The timing multiplier is set according to \p tmult
  * @warning	The AEM configuration is not updated until the
  *		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetBuckTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_TMULT tmult							\
);

/**
  * @brief 	Get the buck converter's configured timing multiplier
  * 
  * @details 	This function reads the TMULT field of the BUCKCFG register and
  *		sets \p tmult accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	tmult		Buck converter's timing multiplier
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p tmult is not NULL
  * @post	\p tmult is set with the buck converter's timing multiplier
  */
int32_t AEM13920_GetBuckTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_TMULT *tmult							\
);

/**
  * @brief 	Get the configured thermistor impedance (RTH) at the minimum 
  *		accepted temperature for charging the storage element
  * 
  * @details	This function reads the TEMPCOLDCH register and sets \p rth 
  * 		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	rth		Configured thermistor's impedance (Rth) at the 
  * 				'cold' temperature threshold for charging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p rth is not NULL
  * @post 	\p rth is set according to the configured 'cold' temperature 
  *		threshold for charging
  */
int32_t AEM13920_GetTempColdChargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t *rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Set the thermistor impedance (RTH) at the minimum accepted
  *		temperature for charging the storage element
  * 
  * @details	This function writes the TEMPCOLDCH register to match the 
  *		impedance defined in \p rth
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in] 	rth		Thermistor's impedance (Rth) at the 'cold' 
  * 				temperature threshold for charging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The 'cold' temperature threshold for charging is set according 
  *		to \p rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold has no effect
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_EnableTempMon()
  */
int32_t AEM13920_SetTempColdChargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Get the configured thermistor impedance at the maximum
  *		temperature for storage element charging
  * 
  * @details	This function reads the TEMPHOTCH register and sets \p rth 
  * 		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	rth		Configured thermistor's impedance (Rth) at the 
  * 				'hot' temperature threshold for charging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p rth is not NULL
  * @post 	\p rth is set according to the configured 'hot' temperature 
  * 		threshold for charging
  */
int32_t AEM13920_GetTempHotChargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Set the thermistor impedance at the maximum temperature for 
  *		storage element charging
  * 
  * @details	This function writes the TEMPHOTCH register to match the 
  *		impedance defined in \p rth
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	rth		Thermistor's impedance (Rth) at the 'hot' 
  * 				temperature threshold for charging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The 'hot' temperature threshold for charging is set according 
  *		to \p rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold has no effect
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_EnableTempMon()
  */
int32_t AEM13920_SetTempHotChargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Get the configured thermistor impedance at the minimum
  *		temperature for storage element discharging
  *
  * @details	This function reads the TEMPCOLDDIS register and sets \p rth
  * 		accordingly
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	rth		Configured thermistor's impedance (Rth) at the
  * 				'cold' temperature threshold for discharging 
  *				(mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p rth is not NULL
  * @post 	\p rth is set according to the configured 'cold' temperature
  *		threshold for discharging
  */
int32_t AEM13920_GetTempColdDischargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t *rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Configure the thermistor impedance at the minimum temperature for
  *		storage element discharging
  *
  * @details	This function writes the TEMPCOLDDIS register to match the
  *		impedance defined in \p rth
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in] 	rth		Thermistor's impedance (Rth) at the 'cold'
  * 				temperature threshold for discharging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post 	The 'cold' temperature threshold for discharging is set according
  *		to \p rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold has no effect
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_EnableTempMon()
  */
int32_t AEM13920_SetTempColdDischargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Get the configured thermistor impedance at the maximum
  *		temperature for storage element discharging
  *
  * @details	This function reads the TEMPHOTDIS register and sets \p rth
  * 		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	rth		Configured thermistor's impedance (Rth) at the
  * 				'hot' temperature threshold for discharging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p rth is not NULL
  * @post 	\p rth is set according to the configured 'hot' temperature
  *		threshold for discharging
  */
int32_t AEM13920_GetTempHotDischargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Set the thermistor impedance at the maximum temperature for
  *		storage element discharging
  *
  * @details	This function writes the TEMPHOTDIS register to match the impedance
  * 		defined in \p rth
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	rth		Thermistor's impedance (Rth) at the 'hot'
  * 				temperature threshold for discharging (mOhms)
  * @param[in]	rdiv		Impedance of the resistor that creates a
  * 				resistive voltage divider with Rth (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post 	The 'hot' temperature threshold for discharging is set according
  *		to \p rth
  * @note	If the temperature monitoring feature is not enabled, the
  * 		configured threshold has no effect
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_EnableTempMon()
  */
int32_t AEM13920_SetTempHotDischargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rth, 								\
	uint32_t rdiv								\
);

/**
  * @brief 	Enable the monitoring of the ambient temperature
  * 
  * @details	This function sets the EN field of the TMON register
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The monitoring of the ambient temperature is enabled
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableTempMon(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the monitoring of the ambient temperature
  * 
  * @details	This function clears the EN field of the TMON register
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The monitoring of the ambient temperature is disabled
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableTempMon(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the Temperature Monitoring
  * 
  * @details	This function reads the EN field of the TMON register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		Enabling status of the Temperature Monitoring
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the Temperature Monitoring is 
  *		enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledTempMon(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Configure both of the SRCLOW thresholds (SRC1 and SRC2)
  *
  * @details	This function writes the SRCLOW register to match the 
  *		configuration set in \p cfg
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in] 	cfg 		SRCLOW thresholds configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is correctly initialized
  * @post	The SRCLOW thresholds are set according to \p cfg
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetSRCLOWConfig(						\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_SRCLOWCFG_t *cfg						\
);

/**
  * @brief 	Get both of the SRCLOW thresholds (SRC1 and SRC2)
  *
  * @details 	This function reads the SRCLOW register and sets \p cfg 
  *		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	cfg		SRCLOW thresholds configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @post	\p cfg is set with the configured SRCLOW thresholds
  */
int32_t AEM13920_GetSRCLOWConfig(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SRCLOWCFG_t *cfg						\
);

/**
  * @brief 	Set SRCx' low threshold
  * 
  * @details	This function writes the SRCxTHRESH field of the SRCLOW register 
  *		to match the threshold defined in \p thresh, where \c x is the 
  *		selected source.
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src		Source
  * @param[in]	thresh		SRCx low voltage threshold
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The SRCx low voltage threshold is set according to \p thresh
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_EnableSleep()
  */
int32_t AEM13920_SetSRCLowThresh(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_SRCLOW_THRESH thresh						\
);

/**
  * @brief 	Get the configured low threshold for SRCx
  * 
  * @details	This function reads the SRCxTHRESH field of the SRCLOW register 
  *		and sets \p thresh accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src		Source
  * @param[out]	thresh		Configured SRCx low voltage threshold
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p thresh is not NULL
  * @post 	\p thresh is set according to the configured threshold
  */
int32_t AEM13920_GetSRCLowThresh(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_SRCLOW_THRESH *thresh						\
);

/**
  * @brief 	Set the APM configuration
  *
  * @details	This function writes the APM register according to \p cfg
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	cfg 		APM configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre 	\p cfg is correctly initialized
  * @post 	The APM configuration is set according to \p cfg
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetAPMConfig(							\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_APMCFG_t *cfg						\
);

/**
  * @brief 	Get the APM configuration
  *
  * @details	This function reads the APM register and sets \p cfg accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	cfg 		APM configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is not NULL
  * @post 	\p cfg is set according to the APM configuration
  */
int32_t AEM13920_GetAPMConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APMCFG_t *cfg							\
);

/**
  * @brief 	Enable the Average Power Monitoring for the selected source
  * 
  * @details	This function sets the SRCxEN field of the APM register, where 
  *		\c x is the selected source
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is enabled for the selected source
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_SetAPMMode()
  * @see	AEM13920_SetAPMWindow()
  */
int32_t AEM13920_EnableAPMSource(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Disable the Average Power Monitoring for the selected source
  * 
  * @details	This function clears the SRCxEN field of the APM register, where 
  *		\c x is the selected source
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is disabled for the selected source
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableAPMSource(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Get the enabling status of the Average Power Monitoring for the
  * 		selected source
  * 
  * @details	This function reads the SRCxEN field of the APM register, where 
  *		\c x is the selected source, and sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src		Selected source
  * @param[out]	en 		Enabling status of the APM feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the APM feature is enabled for the 
  * 		selected source, to \c false otherwise
  */
int32_t AEM13920_IsEnabledAPMSource(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	bool *en								\
);

/**
  * @brief 	Enable the Average Power Monitoring for the buck converter
  * 
  * @details	This function sets the BUCKEN field of the APM register
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is enabled for the buck converter
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see	AEM13920_SetAPMMode()
  * @see	AEM13920_SetAPMWindow()
  */
int32_t AEM13920_EnableAPMBuck(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the Average Power Monitoring for the buck converter
  * 
  * @details	This function clears the BUCKEN field of the APM register
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	The Average Power Monitoring is disabled for the buck converter
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableAPMBuck(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the Average Power Monitoring for the
  * 		buck converter
  * 
  * @details	This function reads the BUCKEN field of the APM register and 
  * 		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		Enabling status of the APM feature
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post	\p en is set to \c true if the APM feature is enabled for the 
  * 		buck converter, to \c false otherwise
  */
int32_t AEM13920_IsEnabledAPMBuck(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Configure the APM functioning mode
  * 
  * @details	This function writes the MODE field of the APM register
  * 		according to \p mode
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	mode 		APM mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p mode is a valid \ref AEM13920_APM_MODE
  * @post	The APM functioning mode is set according to \p mode
  * @note	This configuration has no effect if the Average Power Monitoring
  *		is not enabled
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see 	AEM13920_EnableAPMSource() and AEM13920_EnableAPMBuck()
  */
int32_t AEM13920_SetAPMMode(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_MODE mode							\
);

/**
  * @brief 	Get the configured APM mode
  * 
  * @details	This function reads the MODE field of the APM register and 
  * 		sets \p mode accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	mode 		Configured APM mode
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @post	\p mode is set with the configured APM mode
  */
int32_t AEM13920_GetAPMMode(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_MODE *mode							\
);

/**
  * @brief 	Configure the APM computation window
  * 
  * @details	This function writes the WINDOW field of the APM register
  * 		according to \p window
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	window 		Computation window
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p window is a valid \ref AEM13920_APM_WINDOW
  * @post	The APM computation window is set according to \p window
  * @note	This configuration has no effect if the Average Power Monitoring
  *		is not enabled
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  * @see 	AEM13920_EnableAPMSource() and AEM13920_EnableAPMBuck()
  */
int32_t AEM13920_SetAPMWindow(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_WINDOW window						\
);

/**
  * @brief 	Get the configured APM computation window
  * 
  * @details	This function reads the WINDOW field of the APM register and 
  * 		sets \p window accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	window 		Configured APM computation window
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p window is not NULL
  * @post	\p window is set with the configured APM computation window
  */
int32_t AEM13920_GetAPMWindow(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_WINDOW *window						\
);

/**
  * @brief 	Configure the AEM's IRQs
  * 
  * @details	This function writes the IRQEN0 and IRQEN1 registers according 
  *		to \p irqs
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	irqs 		IRQ's configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p irqs is correctly initialized
  * @post	The AEM's IRQ are configured according to \p irqs
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_SetIRQConfig(							\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_IRQEN_t *irqs						\
);

/**
  * @brief 	Get the configuration of the IRQs
  * 
  * @details	This function reads the IRQEN0 and IRQEN1 registers and sets 
  *		\p irqs accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	irqs 		Configuration of the IRQs
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p irqs is not NULL
  * @post	\p irqs is set with the configuration of the IRQs
  */
int32_t AEM13920_GetIRQConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_IRQEN_t *irqs							\
);

/**
  * @brief 	Enable the I2CRDY IRQ
  *
  * @details 	This function sets the I2CRDY field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the AEM is out of reset and 
  *		ready to communicate through I2C
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableI2CReadyIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the I2CRDY IRQ
  * 
  * @details 	This function clears the I2CRDY field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the AEM is out of reset and 
  *		ready to communicate through I2C
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableI2CReadyIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the I2CRDY IRQ
  *
  * @details 	This functions reads the I2CRDY field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the I2C ready IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on I2C 
  *		ready condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledI2CReadyIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable the VOVDIS IRQ
  *
  * @details 	This function sets the VOVDIS field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the storage level crosses 
  *		the overdischarge threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableOverdischargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the VOVDIS IRQ
  * 
  * @details 	This function clears the VOVDIS field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the storage level crosses 
  *		the overdischarge threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableOverdischargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the VOVDIS IRQ
  *
  * @details 	This functions reads the VOVDIS field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the VOVDIS IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on VOVDIS 
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledOverdischargeIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en								\
);

/**
  * @brief 	Enable the VCHRDY IRQ
  * 
  * @details 	This function sets the VCHRDY field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the storage level crosses
  *		the charge ready threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableChargeReadyIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the VCHRDY IRQ
  * 
  * @details 	This function clears the VCHRDY field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the storage level crosses
  *		the charge ready threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableChargeReadyIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the VCHRDY IRQ
  *
  * @details 	This functions reads the VCHRDY field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	en 		The enabling status of the Charge Ready IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on VCHRDY
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledChargeReadyIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en								\
);

/**
  * @brief 	Enable VOVCH IRQ
  *
  * @details 	This function sets the VOVCH field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the storage level crosses 
  *		the overcharge threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableOverchargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the VOVCH IRQ
  * 
  * @details 	This function clears the VOVCH field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the storage level crosses 
  *		the overcharge threshold
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableOverchargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the VOVCH IRQ
  *
  * @details 	This functions reads the VOVCH field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the VOVCH IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on VOVCH 
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledOverchargeIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable SRCLOW IRQ
  *
  * @details 	This function sets the SRCLOW field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) on any transition between a state
  *		where at least one source voltage is above its low threshold and
  *		a state where both are below their respective low thresholds.
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableSRCLowIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the SRCLOW IRQ
  * 
  * @details 	This function clears the SRCLOW field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted on SRCLOW condition.
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableSRCLowIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the SRCLOW IRQ
  *
  * @details 	This functions reads the SRCLOW field of the IRQEN0 register 
  *		and sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the SRCLOW IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on 
  *		SRCLOW condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledSRCLowIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable TEMPCH IRQ
  *
  * @details 	This function sets the TEMPCH field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the ambient temperature is 
  *		out of the defined range for charging
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableTempChargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the TEMPCH IRQ
  * 
  * @details 	This function clears the TEMPCH field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the ambient temperature is 
  *		out of the defined range for charging
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableTempChargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the TEMPCH IRQ
  *
  * @details 	This functions reads the TEMPCH field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the TEMP IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on TEMPCH 
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledTempChargeIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable TEMPDIS IRQ
  *
  * @details 	This function sets the TEMPDIS field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the ambient temperature is 
  *		out of the defined range for discharging
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableTempDischargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the TEMPDIS IRQ
  * 
  * @details 	This function clears the TEMPDIS field of the IRQEN0 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the ambient temperature is 
  *		out of the defined range for discharging
  * @warning	The AEM configuration is not updated until the 
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableTempDischargeIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the TEMPDIS IRQ
  *
  * @details 	This functions reads the TEMPDIS field of the IRQEN0 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the TEMPDIS IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on 
  *		TEMPDIS condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledTempDischargeIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en								\
);

/**
  * @brief 	Enable the MPPTSTART IRQ for the selected source
  *
  * @details 	This function sets the SRCxMPPTSTART field of the IRQEN1 
  *		register, where x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @post 	The IRQ pin is asserted (HIGH) when the MPPT starts for the
  *		selected source
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Disable the MPPTSTART IRQ for the selected source
  *
  * @details 	This function clears the SRCxMPPTSTART field of the IRQEN1
  *		register, where x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @post 	The IRQ pin is not asserted when the MPPT starts for the
  *		selected source
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called. 
  */
int32_t AEM13920_DisableMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Get the enabling status of the MPPTSTART IRQ for the selected 
  *		source
  *
  * @details 	This functions reads the SRCxMPPTSTART field of the IRQEN1
  *		register, where x is the selected source, and sets \p en 
  *		accordingly.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @param[out]	en 		The enabling status of the MPPT Start IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on
  *		SRCxMPPTSTART condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *en								\
);

/**
  * @brief 	Enable the MPPTDONE IRQ for the selected source
  *
  * @details 	This function sets the SRCxMPPTDONE field of the IRQEN1
  *		register, where x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @post 	The IRQ pin is asserted (HIGH) when the MPPT is done for the
  *		selected source
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src
);

/**
  * @brief 	Disable the MPPTDONE IRQ for the selected source
  *
  * @details 	This function clears the SRCxMPPTDONE field of the IRQEN1
  *		register, where x is the selected source.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @post 	The IRQ pin is not asserted when the MPPT is done for the
  *		selected source
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src							\
);

/**
  * @brief 	Get the enabling status of the MPPTDONE IRQ for the selected 
  *		source
  *
  * @details 	This functions reads the SRCxMPPTDONE field of the IRQEN1
  *		register, where x is the selected source, and sets \p en 
  *		accordingly
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @param[out]	en 		The enabling status of the MPPT Done IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on
  *		SRCxMPPTDONE condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *en								\
);

/**
  * @brief 	Enable the STODONE IRQ
  *
  * @details 	This function sets the STODONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the STO measurement is done
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableSTODoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the STODONE IRQ
  * 
  * @details 	This function clears the STODONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the STO measurement is done
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableSTODoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the STODONE IRQ
  *
  * @details 	This functions reads the STODONE field of the IRQEN1 register 
  *		and sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the STO Done IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on 
  *		STODONE condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledSTODoneIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable the TEMPDONE IRQ
  *
  * @details 	This function sets the TEMPDONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when the temperature measurement 
  *		is done
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableTempDoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the TEMPDONE IRQ
  * 
  * @details 	This function clears the TEMPDONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when the temperature measurement is 
  *		done
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableTempDoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the TEMPDONE IRQ
  *
  * @details 	This functions reads the TEMPDONE field of the IRQEN1 register 
  *		and sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the TEMP Done IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on 
  *		TEMPDONE condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledTempDoneIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable APMDONE IRQ
  *
  * @details 	This function sets the APMDONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when new APM data is available
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableAPMDoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the APMDONE IRQ
  * 
  * @details 	This function clears the APMDONE field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when new APM data is available
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableAPMDoneIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the APMDONE IRQ
  *
  * @details 	This functions reads the APMDONE field of the IRQEN1 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the APMDONE IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on APMDONE 
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledAPMDoneIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Enable the APMERR IRQ
  *
  * @details 	This function sets the APMERR field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is asserted (HIGH) when an APM error occurs
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_EnableAPMErrIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Disable the APMERR IRQ
  * 
  * @details 	This function clears the APMERR field of the IRQEN1 register
  *
  * @param[in] 	handler		Communication interface's required information 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post 	The IRQ pin is not asserted when an APM error occurs
  * @warning	The AEM configuration is not updated until the
  * 		AEM13920_SyncI2CConfiguration() function is called.
  */
int32_t AEM13920_DisableAPMErrIRQ(const AEM13920_Handler_t *handler);

/**
  * @brief 	Get the enabling status of the APMERR IRQ
  *
  * @details 	This functions reads the APMERR field of the IRQEN1 register and
  *		sets \p en accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	en 		The enabling status of the APMERR IRQ
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p en is not NULL
  * @post 	\p en is set to \c true if the assertion of the IRQ pin on APMERR 
  *		condition is enabled, to \c false otherwise
  */
int32_t AEM13920_IsEnabledAPMErrIRQ(const AEM13920_Handler_t *handler, bool *en);

/**
  * @brief 	Set the configuration mode (I2C or GPIO)
  * 
  * @details	This function writes the UPDATE field of the CTRL register
  *		according to \p mode
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	mode 		Mode of configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @post	The configuration mode is set according to \p mode
  * @note 	When selecting the I2C mode, this function has the side-effect
  *		of starting a synchronization of the I2C registers, making it
  *		equivalent to a call to the AEM13920_SyncI2CConfiguration() 
  *		function
  */
int32_t AEM13920_SetConfigurationMode(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_MODE mode						\
);

/**
  * @brief 	Get the active configuration mode (I2C or GPIO)
  * 
  * @details	This function reads the UPDATE field of the CTRL register, and 
  *		sets \p mode accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	mode 		Mode of configuration
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p mode is not NULL
  * @post	\p mode is set according to the AEM configuration mode
  */
int32_t AEM13920_GetConfigurationMode(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_MODE *mode						\
);

/**
  * @brief	Load the I2C configuration
  * 
  * @details	This function sets the UPDATE field of the CTRL register. If \p 
  *		blocking is set to \c true, the function blocks until the 
  *		synchronisation is complete.
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	blocking	Whether the call shall be blocking 
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @post	A synchronization of the I2C registers has been requested
  * @post 	If \p blocking is set to \c true, the function blocks until
  *		the synchronization is complete
  * @note 	If \p blocking is not set to true, the function does not block,
  *		and the synchronization status can be monitored using the 
  *		AEM13920_IsSyncingI2CConfiguration() function
  */
int32_t AEM13920_SyncI2CConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	bool blocking								\
);

/**
  * @brief 	Get the I2C synchronization busy flag
  * 
  * @details	This function reads the SYNCBUSY field of the CTRL register, and 
  *		sets \p syncing accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	syncing 	Synchronization flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p syncing is not NULL
  * @post	\p syncing is set to \c true if the synchronization of the I2C 
  * 		registers and the system registers is ongoing, to \c false 
  * 		otherwise
  * @note	Use this function after calls to the AEM13920_SyncI2CConfiguration()
  * 		or AEM13920_Configure() functions to ensure the synchronization 
  * 		is complete before any attempt to read the configuration
  */
int32_t AEM13920_IsSyncingI2CConfiguration(					\
	const AEM13920_Handler_t *handler,					\
	bool *syncing								\
);

/**
  * @brief 	Get the chip version
  * 
  * @details	This function reads the VERSION register, and sets \p version 
  *		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	version 	Chip version
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p version is not NULL
  * @post	\p version is set with the chip version
  */
int32_t AEM13920_GetChipVersion(						\
	const AEM13920_Handler_t *handler, 					\
	uint8_t *version							\
);

/**
  * @brief 	Get the AEM's interrupt flags
  * 
  * @details	This function reads the IRQFLG0 and IRQFLG1 registers, and sets 
  *		\p flags accordingly.
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flags 		Interrupt flags
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flags is not NULL
  * @post	\p flags is set with the IRQ flags
  */
int32_t AEM13920_GetIRQFlags(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_IRQFLG_t *flags						\
);

/**
  * @brief 	Get the I2C ready IRQ flag
  *
  * @details 	This functions reads the I2CRDY field of the IRQFLG0 register and
  *		sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		I2CRDY IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the I2C ready IRQ flag is raised, 
  *		to \c false otherwise
  */
int32_t AEM13920_GetI2CReadyFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the overdischarge IRQ flag
  *
  * @details 	This functions reads the VOVDIS field of the IRQFLG0 register and
  *		sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		VOVDIS IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the overdischarge IRQ flag is 
  *		raised, to \c false otherwise
  */
int32_t AEM13920_GetOverdischargeFlag(						\
	const AEM13920_Handler_t *handler, 					\
	bool *flag								\
);

/**
  * @brief 	Get the charge ready IRQ flag
  *
  * @details 	This functions reads the VCHRDY field of the IRQFLG0 register and
  *		sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[out]	flag 		VCHRDY IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the charge ready IRQ flag is raised,
  *		to \c false otherwise
  */
int32_t AEM13920_GetChargeReadyFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the overcharge IRQ flag
  *
  * @details 	This functions reads the VOVCH field of the IRQFLG0 register and
  *		sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		VOVCH IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the overcharge IRQ flag is raised, 
  *		to \c false otherwise
  */
int32_t AEM13920_GetOverchargeFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the SRCLOW IRQ flag
  *
  * @details 	This functions reads the SRCLOW field of the IRQFLG0 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		SRCLOW IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the SRCLOW IRQ flag is raised, to 
  *		\c false otherwise
  */
int32_t AEM13920_GetSRCLowFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the temperature (charge) IRQ flag
  *
  * @details 	This functions reads the TEMPCH field of the IRQFLG0 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		TEMPCH IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the temperature (charge) IRQ flag 
  *		is raised, to \c false otherwise
  */
int32_t AEM13920_GetTempChargeFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the temperature (discharge) IRQ flag
  *
  * @details 	This functions reads the TEMPDIS field of the IRQFLG0 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		TEMPDIS IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the temperature (discharge) IRQ flag
  *		is raised, to \c false otherwise
  */
int32_t AEM13920_GetTempDischargeFlag(						\
	const AEM13920_Handler_t *handler, 					\
	bool *flag								\
);

/**
  * @brief 	Get the MPPT Start IRQ flag for the selected source
  *
  * @details 	This functions reads the SRCxMPPTSTART field of the IRQFLG1 
  *		register, where x is the selected source, and sets \p flag 
  *		accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src 		Selected source
  * @param[out]	flag 		SRCxMPPTSTART IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the MPPT start IRQ flag is raised 
  *		for the selected source, to \c false otherwise
  */
int32_t AEM13920_GetMPPTStartFlag(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src , 							\
	bool *flag								\
);

/**
  * @brief 	Get the MPPT Done IRQ flag for the selected source
  *
  * @details 	This functions reads the SRCxMPPTDONE field of the IRQFLG1 
  *		register, where x is the selected source, and sets \p flag 
  *		accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src 		Selected source
  * @param[out]	flag 		SRCxMPPTDONE IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the MPPT done IRQ flag is raised 
  *		for the selected source, to \c false otherwise
  */
int32_t AEM13920_GetMPPTDoneFlag(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src , 							\
	bool *flag								\
);

/**
  * @brief 	Get the STO Done IRQ flag
  *
  * @details 	This functions reads the STODONE field of the IRQFLG1 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		STODONE IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the STO done IRQ flag is raised, 
  *		to \c false otherwise
  */
int32_t AEM13920_GetSTODoneFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the TEMP Done IRQ flag
  *
  * @details 	This functions reads the TEMPDONE field of the IRQFLG1 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		TEMPDONE IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the temperature done IRQ flag is 
  *		raised, to \c false otherwise
  */
int32_t AEM13920_GetTempDoneFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the APM done IRQ flag
  *
  * @details 	This functions reads the APMDONE field of the IRQFLG1 register 
  *		and sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		APMDONE IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the APM done IRQ flag is raised, 
  *		to \c false otherwise
  */
int32_t AEM13920_GetAPMDoneFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the APM error IRQ flag
  *
  * @details 	This functions reads the APMERR field of the IRQFLG1 register and
  *		sets \p flag accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	flag 		APMERR IRQ flag
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p flag is not NULL
  * @post 	\p flag is set to \c true if the APM error IRQ flag is raised, 
  *		to \c false otherwise
  */
int32_t AEM13920_GetAPMErrFlag(const AEM13920_Handler_t *handler, bool *flag);

/**
  * @brief 	Get the AEM's status
  * 
  * @details	This function reads the STATUS0 and STATUS1 registers, and sets
  * 		\p status accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		AEM's status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post	\p status is set with the AEM's status
  */
int32_t AEM13920_GetStatus(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_STATUS_t *status						\
);

/**
  * @brief 	Get the overdischarge status
  *
  * @details 	This functions reads the VOVDIS field of the STATUS0 register and
  *		sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		VOVDIS status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the storage element's voltage is  
  *		under the overdischarge threshold, to \c false otherwise
  */
int32_t AEM13920_GetOverdischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the charge ready status
  *
  * @details 	This functions reads the VCHRDY field of the STATUS0 register and
  *		sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		VCHRDY status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the storage element's voltage is  
  *		above the charge ready threshold, to \c false otherwise
  */
int32_t AEM13920_GetChargeReadyStatus(						\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the overcharge status
  *
  * @details 	This functions reads the VOVCH field of the STATUS0 register and
  *		sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		VOVCH status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the storage element's voltage is  
  *		above the overcharge threshold, to \c false otherwise
  */
int32_t AEM13920_GetOverchargeStatus(						\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the SRC Threshold status for the selected source
  *
  * @details 	This functions reads the SRCxSRCLOW field of the STATUS0 register, 
  *		where x is the selected source, and sets \p status accordingly.
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src 		Selected source
  * @param[out]	status 		SRC threshold status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the source voltage is under the 
  *		defined threshold, to \c false otherwise
  */
int32_t AEM13920_GetSRCLowStatus(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *status								\
);

/**
  * @brief 	Get the status of the hot temperature threshold (charge)
  *
  * @details 	This functions reads the TEMPHOTCH field of the STATUS1 register
  *		and sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		TEMPHOTCH status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the temperature is above the 
  *		defined charge hot threshold, to \c false otherwise
  */
int32_t AEM13920_GetTempHotChargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the status of the hot temperature threshold (discharge)
  *
  * @details 	This functions reads the TEMPHOTDIS field of the STATUS1 register
  *		and sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		TEMPHOTDIS status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the temperature is above the 
  *		defined discharge hot threshold, to \c false otherwise
  */
int32_t AEM13920_GetTempHotDischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the status of the cold temperature threshold (charge)
  *
  * @details 	This functions reads the TEMPCOLDCH field of the STATUS1 register
  *		and sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		TEMPCOLDCH status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the temperature is below the 
  *		defined charge cold threshold, to \c false otherwise
  */
int32_t AEM13920_GetTempColdChargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the status of the cold temperature threshold (discharge)
  *
  * @details 	This functions reads the TEMPCOLDDIS field of the STATUS1 register
  *		and sets \p status accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	status 		TEMPCOLDDIS status
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p status is not NULL
  * @post 	\p status is set to \c true if the temperature is below the 
  *		defined discharge cold threshold, to \c false otherwise
  */
int32_t AEM13920_GetTempColdDischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status								\
);

/**
  * @brief 	Get the APM data for the specified source
  *
  * @details 	This functions reads the APM0SRCx, APM1SRCx and APM2SRCx 
  *		registers, where x is the selected source, and sets \p data 
  *		accordingly
  *
  * @param[in] 	handler		Communication interface's required information
  * @param[in]	src 		Selected source
  * @param[out] data 		APM data
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p data is not NULL
  * @post	In Pulse Counter mode, \p data is set with the number of pulses.
  * @post	In Power Meter mode, \p data is set with the left-shifted data 
  *		from the register (DATA << OFFSET). To complete the conversion 
  *		to nano-Joules, refer to the formula given into the product's 
  *		datasheet.
  */
int32_t AEM13920_GetAPMSourceData(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	uint64_t *data								\
);

/**
  * @brief 	Get the APM data
  * 
  * @details	This function reads the APM0BUCK, APM1BUCK and APM2BUCK 
  *		registers, and sets \p data accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out] data 		APM data
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p data is not NULL
  * @post	In Pulse Counter mode, \p data is set with the number of pulses.
  * @post	In Power Meter mode, \p data is set with the left-shifted data 
  *		from the register (DATA << OFFSET). To complete the conversion 
  *		to nano-Joules, refer to the formula given into the product's 
  *		datasheet.
  */
int32_t AEM13920_GetAPMBuckData(const AEM13920_Handler_t *handler, uint64_t *data);

/**
  * @brief 	Get the APM errors
  *
  * @details 	This functions reads the APMERR register and sets \p errors 
  *		accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out] errors 		APM errors
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p errors is not NULL
  * @post	\p errors is set with the APM errors
  */
int32_t AEM13920_GetAPMErrors(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APMERR_t *errors						\
);

/**
  * @brief 	Return whether an APM counter overflow occured for the specified
  *		source
  *
  * @details 	This function reads the SRCxOV field of the APMERR register, 
  *		where x is the selected source, and sets \p error accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	src 		Selected source
  * @param[out] error 		APM error
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p src is a valid \ref AEM13920_SOURCE
  * @pre	\p error is not NULL
  * @post	\p error is set to true if an overflow occured for the specified
  *		source, to false otherwise
  */
int32_t AEM13920_IsAPMSourceOverflow(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *error								\
);

/**
  * @brief 	Return whether the APM data for the selected source is corrupted
  *
  * @details 	This function reads the SRCxNVLD field of the APMERR register 
  *		and sets \p error accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out] error 		APM error
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p error is not NULL
  * @post	\p error is set to true if the APM data for the selected source
  *		is corrupted, to \c false otherwise
  */
int32_t AEM13920_IsAPMSourceCorrupted(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *error								\
);

/**
  * @brief 	Return whether an APM counter overflow occured for the buck 
  *		converter
  *
  * @details 	This function reads the BUCKOV field of the APMERR register 
  *		and sets \p error accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out] error 		APM error
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p error is not NULL
  * @post	\p error is set to true if an overflow occured, to \c false 
  *		otherwise
  */
int32_t AEM13920_IsAPMBuckOverflow(const AEM13920_Handler_t *handler, bool *error);

/**
  * @brief 	Return whether the APM data for the buck converter is corrupted
  *
  * @details 	This function reads the BUCKNVLD field of the APMERR register 
  *		and sets \p error accordingly
  *
  * @param[in] 	handler		Communication interface's required information 
  * @param[out] error 		APM error
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  *
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p error is not NULL
  * @post	\p error is set to true if the APM data for the buck converter
  *		is corrupted, to \c false otherwise
  */
int32_t AEM13920_IsAPMBuckCorrupted(const AEM13920_Handler_t *handler, bool *error);

/**
  * @brief 	Get the thermistor's impedance
  * 
  * @details	This function reads the TEMP register, and sets 
  * 		\p rth accordingly.
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	rdiv 		Impedance of the resistor that creates a 
  * 				resistive voltage divider with Rth (mOhms)
  * @param[out]	rth 		Impedance of the thermistor used for the thermal 
  * 				monitoring feature (mOhms)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p rdiv holds the impedance of the resistor Rdiv (mOhms)
  * @pre	\p rth is not NULL
  * @post	\p rth is set with the thermistor's impedance (mOhms)
  * 
  * @note	\p rth can be converted to Celsius degrees either by refering to 
  * 		the correspondance table that should be supplied by the 
  * 		thermistor's manufacturer, or by implementing the Steinhart-Hart 
  * 		equation.
  * @note	The user willing to work with the Steinhart-Hart equation 
  * 		could proceed as follows. This is an implementation of the 
  * 		simplified Steinhart–Hart equation, aka 'beta parameter 
  * 		equation', which returns the temperature in Celsius degrees
  * 		from \p rth the thermistor's impedance, \p t0 the thermistor's 
  * 		nominal temperature in Celsius degrees (usually 25°C), \p r0 the 
  * 		thermistor's nominal impedance and \p beta the thermistor's beta 
  * 		constant (kelvins):
  * @note @code
  * #include <stdint.h>
  * #include <math.h>
  * 
  * int32_t ConvertThermZToTemp(uint32_t rth, float t0, uint32_t r0, uint32_t beta)
  * {	
  * 	const float kelvin_conversion = 273.15f;// Kelvin offset
  *	float t0k = t0 + kelvin_conversion;	// Convert t0 to Kelvins
  * 	float numerator = ((float) beta * t0k);
  * 	float denominator = ((float) beta + (t0k * logf((float) rth / (float)r0)));
  * 	float temp = (numerator / denominator);
  * 	return roundf(temp - kelvin_conversion);// Convert to Celsius and round
  * }

  * uint32_t ConvertTempToThermZ(float temp, float t0, uint32_t r0, uint32_t beta) {
  *     const float kelvin_conversion = 273.15f;   // Kelvin offset
  *     float tk = temp + kelvin_conversion;       // Convert temp to Kelvin
  *     float t0k = t0 + kelvin_conversion;        // Convert t0 to Kelvin
  *     float exp_term = beta * ((1.0f / tk) - (1.0f / t0k)); // β * (1/T - 1/T0)
  *     float resistance = r0 * expf(exp_term);    // R = R0 * e^(exp_term)
  *     return (uint32_t)roundf(resistance);       // Return resistance as uint32_t
  * }
  *
  * @endcode
  */
int32_t AEM13920_GetThermistorZ(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rdiv,								\
	uint64_t *rth								\
);

/**
  * @brief 	Get the storage voltage
  * 
  * @details 	This function reads the STO register, and sets \p vsto 
  *		accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	vsto		Storage voltage (mV)
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p vsto is not NULL
  * @post	\p vsto is set with the storage voltage (mV)
  */
int32_t AEM13920_GetStorageVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vsto								\
);

/**
  * @brief 	Set the whole configuration and update the AEM accordingly
  * 
  * @details	This function writes the whole writable area of the register 
  *		map according to the configuration specified in \p cfg, and sets
  *		the UPDATE field of the CTRL register to ensure the system 
  *		registers are synchronized. If \p blocking is set to \c true, it 
  *		then blocks until the synchronisation is complete
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[in]	cfg		Configuration settings
  * @param[in]	blocking	Whether the call shall be blocking
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg is correctly initialized
  * @pre 	cfg->temp_rdiv must be correctly set (so that the temperature
  *		thresholds can be computed).
  * @post	The AEM configuration is updated according to the settings 
  * 		defined in \p cfg
  * @post 	If \p blocking is set to \c true, the function blocks until
  *		the synchronization is complete
  * @note 	If \p blocking is not set to true, the function does not block,
  *		and the synchronization status can be monitored using the 
  *		AEM13920_IsSyncingI2CConfiguration() function
  * @warning 	The \p cfg parameter must be correctly set, as the function
  *		does not check the validity of the configuration. The user must
  *		ensure that the configuration is valid. An optimal way to do so
  *		is to use the AEM13920_GetConfiguration() function to get the
  *		current configuration, modify it, and then set it back using
  *		this function.
  */
int32_t AEM13920_SetConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_CONFIG_t *cfg,						\
	bool blocking								\
);

/**
  * @brief 	Read the whole AEM configuration
  * 
  * @details	This function reads the whole configurable area of the register
  * 		map and sets \p cfg accordingly
  * 
  * @param[in] 	handler		Communication interface's required information 
  * @param[out]	cfg		Configuration settings
  * @return 	int32_t 	\ref AEM13920_STATUS_ERR_CODE
  * 
  * @pre	\p handler is not NULL
  * @pre	\p handler->i2c_cfg is correctly initialized, as described 
  *		in the \ref AEM_Driver_Comm API
  * @pre	\p cfg->Rdiv is correctly set (so that the temperature thresholds
  * 		can be computed)
  * @pre	\p cfg is not NULL
  * @post	\p cfg is set with information corresponding to the AEM's
  * 		configuration
  */
int32_t AEM13920_GetConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_t *cfg							\
);

#ifdef __cplusplus
}
#endif

/** @} *//* (end \defgroup AEM13920_Driver_API) */
#endif /* HAL_AEM13920_H_ */
