/**
  ******************************************************************************
  * \file	HAL_AEM13920.c
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM13920_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \brief	AEM13920's HAL Driver's Implementation
  * 
  * \version	2.0.2
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
#include <stdint.h>
#include <stdbool.h>
#include "HAL_AEM13920.h"
#include "AEM13920_Common.h"
#include "AEM13920_RegisterMap.h"

/**
  * @brief	Read from the I2C registers
  *
  * @details	Read the specified number of data from the I2C registers,
  *		starting from the specified offset
  *
  * @param[in]	i2c_cfg		Pointer to the I2C configuration structure
  * @param[in]	offset		Start offset in the I2C register map
  * @param[in]	len		Number of bytes to read
  * @param[in]	buffer		Pointer to the buffer to store the read data
  * @return	int32_t		\ref AEM13920_STATUS_ERR_CODE
  *
  * @post	The specified number of data has been read from the I2C 
  *		registers and stored in the provided buffer
  */
static int32_t __AEM13920_ReadRegisters(const AEM_i2c_cfg *i2c_cfg, 		\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer)
{
	if (!i2c_cfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if ((i2c_cfg->slaveAddress != AEM13920_I2CSLAVE_ADDRESS)) {
		return AEM_I2C_ERR_SLV_ADDR;
	}

	return AEM_I2C_ReadRegisters(i2c_cfg, offset, len, buffer);
}

/**
  * @brief 	Write to the I2C registers
  *
  * @details	Write the specified data to the I2C registers starting from the 
  *		specified offset.
  * 
  * @param[in]	i2c_cfg		Pointer to the I2C configuration structure
  * @param[in]	offset		Start offset in the I2C register map
  * @param[in]	len		Length of the data to write
  * @param[in]	buffer		Pointer to the data to write to the registers
  * @return	int32_t		\ref AEM13920_STATUS_ERR_CODE
  *
  * @post	The data has been written to the I2C registers
  */
static int32_t __AEM13920_WriteRegisters(const AEM_i2c_cfg *i2c_cfg, 		\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer)
{
	if (!i2c_cfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if ((i2c_cfg->slaveAddress != AEM13920_I2CSLAVE_ADDRESS)) {
		return AEM_I2C_ERR_SLV_ADDR;
	}

	return AEM_I2C_WriteRegisters(i2c_cfg, offset, len, buffer);
}

/**
  * @brief 	Write to an I2C register
  *
  * @details	Write the specified data to the I2C register at the specified 
  *		offset.
  * 
  * @param[in]	i2c_cfg		Pointer to the I2C configuration structure
  * @param[in]	offset		Offset in the I2C register map
  * @param[in]	data		Data to write to the register
  * @return	int32_t		\ref AEM13920_STATUS_ERR_CODE
  *
  * @post	The data has been written to the I2C register
  */
static int32_t __AEM13920_WriteRegister(					\
	const AEM_i2c_cfg *i2c_cfg, 						\
	uint32_t offset, 							\
	uint8_t data)
{
	if (!i2c_cfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if ((i2c_cfg->slaveAddress != AEM13920_I2CSLAVE_ADDRESS)) {
		return AEM_I2C_ERR_SLV_ADDR;
	}

	return AEM_I2C_WriteRegisters(i2c_cfg, offset, 1, &data);
}

/**
  * @brief 	Performs rounding for integer division
  */
#define ROUND_NEAREST_DIV(a, b) (((a) + ((b) >> 1)) / (b))

/* ~~~~~~~~ SRCREGU ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define __AEM13920_SRCREGU_SCALE_FACT		(10UL)

#define __AEM13920_SRCREGU_SLEEP		(0x00UL)
#define __AEM13920_SRCREGU_SLEEPV		(113UL)

#define __AEM13920_SRCREGU_RGE0_STARTV		(120UL)
#define __AEM13920_SRCREGU_RGE0_OFFSET		(0x0DU)
#define __AEM13920_SRCREGU_RGE0_STEP		(75UL)
#define __AEM13920_SRCREGU_RGE0_STOPV		(293UL)
#define __AEM13920_SRCREGU_RGE0_MIN		__AEM13920_SRCREGU_RGE0_STARTV

#define __AEM13920_SRCREGU_RGE1_STARTV		(300UL)
#define __AEM13920_SRCREGU_RGE1_OFFSET		(0x25U)
#define __AEM13920_SRCREGU_RGE1_STEP		(150UL)
#define __AEM13920_SRCREGU_RGE1_STOPV		(1470UL)
#define __AEM13920_SRCREGU_RGE1_MIN						\
	(__AEM13920_SRCREGU_RGE1_STARTV 					\
	- ((__AEM13920_SRCREGU_RGE1_STARTV - __AEM13920_SRCREGU_RGE0_STOPV) >> 1))

#define __AEM13920_SRCREGU_RGE2_STARTV		(1478UL)
#define __AEM13920_SRCREGU_RGE2_OFFSET		(0x74U)
#define __AEM13920_SRCREGU_RGE2_STEP		(224UL)
#define __AEM13920_SRCREGU_RGE2_STOPV		(2194UL)
#define __AEM13920_SRCREGU_RGE2_MIN						\
	(__AEM13920_SRCREGU_RGE2_STARTV 					\
	- ((__AEM13920_SRCREGU_RGE2_STARTV - __AEM13920_SRCREGU_RGE1_STOPV) >> 1))

#define __AEM13920_SRCREGU_RGE3_STARTV		(2227UL)
#define __AEM13920_SRCREGU_RGE3_OFFSET		(0x95U)
#define __AEM13920_SRCREGU_RGE3_STEP		(455UL)
#define __AEM13920_SRCREGU_RGE3_STOPV		(4455UL)
#define __AEM13920_SRCREGU_RGE3_MIN 						\
	(__AEM13920_SRCREGU_RGE3_STARTV 					\
	- ((__AEM13920_SRCREGU_RGE3_STARTV - __AEM13920_SRCREGU_RGE2_STOPV) >> 1))

#define __AEM13920_SRCREGU_MAX_OFFSET		(0xC6UL)
#define __AEM13920_SRCREGU_MAX_V		(__AEM13920_SRCREGU_RGE3_STOPV)

#define __AEM13920_SRCREGU_CFG1_SHIFT		(3UL)

/**
  * @brief 	Converts millivolts (mv) to SRCREGU register value
  */
#define __AEM13920_SRCREGU_VOLT_TO_REG(MV, STRTV, STEP, OFFSET) 		\
	ROUND_NEAREST_DIV(((MV) - (STRTV)) * __AEM13920_SRCREGU_SCALE_FACT, STEP)\
	+ (OFFSET)

/**
  * @brief 	Converts SRCREGU register value (reg) to millivolts
  */
#define __AEM13920_SRCREGU_REG_TO_VOLT(REG, OFFSET, STEP, STRTV) 		\
	ROUND_NEAREST_DIV((((REG) - (OFFSET)) * (STEP)), __AEM13920_SRCREGU_SCALE_FACT) + (STRTV)

/**
  * @brief 	Computes the register value from the given voltage (mv)
  * 
  * @param[in] 	mv 		The voltage (mv)
  * @param[out] regs		The registers values that better matches the 
  *				given voltage
  */
static void __AEM13920_SRCREGUVoltToReg(uint32_t mv, uint8_t *regs)
{
	uint8_t reg_val = __AEM13920_SRCREGU_SLEEP;
	if (mv >= __AEM13920_SRCREGU_MAX_V) {
		reg_val = __AEM13920_SRCREGU_MAX_OFFSET;
	} else if (mv >= __AEM13920_SRCREGU_RGE3_MIN) {
		reg_val= __AEM13920_SRCREGU_VOLT_TO_REG(
				mv, __AEM13920_SRCREGU_RGE3_STARTV, 		\
				__AEM13920_SRCREGU_RGE3_STEP, 			\
				__AEM13920_SRCREGU_RGE3_OFFSET);
	} else if (mv >= __AEM13920_SRCREGU_RGE2_MIN) {
		reg_val = __AEM13920_SRCREGU_VOLT_TO_REG(
				mv, __AEM13920_SRCREGU_RGE2_STARTV, 		\
				__AEM13920_SRCREGU_RGE2_STEP, 			\
				__AEM13920_SRCREGU_RGE2_OFFSET);
	} else if (mv >= __AEM13920_SRCREGU_RGE1_MIN) {
		reg_val = __AEM13920_SRCREGU_VOLT_TO_REG(
				mv, __AEM13920_SRCREGU_RGE1_STARTV, 		\
				__AEM13920_SRCREGU_RGE1_STEP, 			\
				__AEM13920_SRCREGU_RGE1_OFFSET);
	} else if (mv >= __AEM13920_SRCREGU_RGE0_MIN) {
		reg_val = __AEM13920_SRCREGU_VOLT_TO_REG(
				mv, __AEM13920_SRCREGU_RGE0_STARTV, 		\
				__AEM13920_SRCREGU_RGE0_STEP, 			\
				__AEM13920_SRCREGU_RGE0_OFFSET);
	}
	
	regs[0] = AEM13920_SRCREGU_CONST;
	regs[0] |= (	(reg_val << AEM13920_SRCxREGU0_CFG0_Pos) 
			& AEM13920_SRCxREGU0_CFG0_Msk);
	regs[1] = (	(reg_val >> __AEM13920_SRCREGU_CFG1_SHIFT) 
			& (AEM13920_SRCxREGU1_CFG1_Msk 
			| AEM13920_SRCxREGU1_CFG2_Msk));
}

/**
  * @brief 	Converts the register value to voltage (mv)
  * 
  * @param[in] 	regs 		The registers
  * @return 	uint32_t	The voltage (mv) corresponding to the configured
  *				register
  */
static uint32_t __AEM13920_SRCREGURegToVolt(uint8_t *regs)
{
	
	uint8_t raw_data = 0;
	raw_data = (	(regs[0] & AEM13920_SRCxREGU0_CFG0_Msk) 		\
			>> AEM13920_SRCxREGU0_CFG0_Pos);
	raw_data |= (	(regs[1] 
			& (AEM13920_SRCxREGU1_CFG1_Msk 
			| AEM13920_SRCxREGU1_CFG2_Msk))
			<< __AEM13920_SRCREGU_CFG1_SHIFT);
	if (raw_data < __AEM13920_SRCREGU_RGE0_OFFSET) {
		return __AEM13920_SRCREGU_SLEEPV;
	}
	if (raw_data < __AEM13920_SRCREGU_RGE1_OFFSET) {
		return __AEM13920_SRCREGU_REG_TO_VOLT(				\
			raw_data, __AEM13920_SRCREGU_RGE0_OFFSET, 		\
			__AEM13920_SRCREGU_RGE0_STEP, 				\
			__AEM13920_SRCREGU_RGE0_STARTV);
	} 
	if (raw_data < __AEM13920_SRCREGU_RGE2_OFFSET) {
		return __AEM13920_SRCREGU_REG_TO_VOLT(				\
			raw_data, __AEM13920_SRCREGU_RGE1_OFFSET, 		\
			__AEM13920_SRCREGU_RGE1_STEP, 				\
			__AEM13920_SRCREGU_RGE1_STARTV);
	} 
	if (raw_data < __AEM13920_SRCREGU_RGE3_OFFSET) {
		return __AEM13920_SRCREGU_REG_TO_VOLT(				\
			raw_data, __AEM13920_SRCREGU_RGE2_OFFSET, 		\
			__AEM13920_SRCREGU_RGE2_STEP, 				\
			__AEM13920_SRCREGU_RGE2_STARTV);
	} 
	if (raw_data < __AEM13920_SRCREGU_MAX_OFFSET) {
		return __AEM13920_SRCREGU_REG_TO_VOLT(				\
			raw_data, __AEM13920_SRCREGU_RGE3_OFFSET, 		\
			__AEM13920_SRCREGU_RGE3_STEP, 				\
			__AEM13920_SRCREGU_RGE3_STARTV);
	}
	return __AEM13920_SRCREGU_MAX_V;
}

static int32_t __AEM13920_ComputeSRCREGUReg(					\
	const AEM13920_CONFIG_t *cfg, 						\
	uint8_t *regs)
{
	if (cfg->src1_regu_mode == AEM13920_SRCREGU_CONST) {
		__AEM13920_SRCREGUVoltToReg(					\
			cfg->src1_const_voltage, 				\
			regs + (AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET));
	} else if (cfg->src1_regu_mode == AEM13920_SRCREGU_MPPT) {
		regs[AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET] = 	\
			(AEM13920_SRCxREGU0_MODE_Msk 				\
			| ((cfg->src1_mppt_ratio << AEM13920_SRCxREGU0_CFG0_Pos)\
			& AEM13920_SRCxREGU0_CFG0_Msk));
		regs[AEM13920_SRC1REGU1_OFFSET - AEM13920_CFG_OFFSET] = 	\
			(((cfg->src1_mppt_duration << AEM13920_SRCxREGU1_CFG1_Pos)\
			& AEM13920_SRCxREGU1_CFG1_Msk) 				\
			| ((cfg->src1_mppt_period << AEM13920_SRCxREGU1_CFG2_Pos)\
			& AEM13920_SRCxREGU1_CFG2_Msk));
	} else {
		return AEM13920_DRIVER_ERR_SRCREGU_MODE;
	}
	if (cfg->src2_regu_mode == AEM13920_SRCREGU_CONST) {
		__AEM13920_SRCREGUVoltToReg(					\
			cfg->src2_const_voltage, 				\
			regs + (AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET));
	} else if (cfg->src2_regu_mode == AEM13920_SRCREGU_MPPT) {
		regs[AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET] = 	\
			(AEM13920_SRCxREGU0_MODE_Msk 				\
			| ((cfg->src2_mppt_ratio << AEM13920_SRCxREGU0_CFG0_Pos)\
			& AEM13920_SRCxREGU0_CFG0_Msk));
		regs[AEM13920_SRC2REGU1_OFFSET - AEM13920_CFG_OFFSET] = 	\
			(((cfg->src2_mppt_duration << AEM13920_SRCxREGU1_CFG1_Pos)\
			& AEM13920_SRCxREGU1_CFG1_Msk) 				\
			| ((cfg->src2_mppt_period << AEM13920_SRCxREGU1_CFG2_Pos)\
			& AEM13920_SRCxREGU1_CFG2_Msk));
	} else {
		return AEM13920_DRIVER_ERR_SRCREGU_MODE;
	}
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertSRCREGUReg(					\
	uint8_t *regs,								\
	AEM13920_CONFIG_t *cfg)
{
	if (	((regs[AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET] 	\
		& AEM13920_SRCxREGU0_MODE_Msk) >> AEM13920_SRCxREGU0_MODE_Pos) 	\
		== AEM13920_SRCREGU_CONST) {
		cfg->src1_regu_mode = AEM13920_SRCREGU_CONST;
		cfg->src1_const_voltage = __AEM13920_SRCREGURegToVolt(		\
			regs + (AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET));
	} else {
		cfg->src1_regu_mode = AEM13920_SRCREGU_MPPT;
		cfg->src1_mppt_ratio = (AEM13920_MPPT_RATIO)			\
			((regs[AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU0_CFG0_Msk) 				\
			>> AEM13920_SRCxREGU0_CFG0_Pos);
		cfg->src1_mppt_duration = (AEM13920_MPPT_DURATION)		\
			((regs[AEM13920_SRC1REGU1_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU1_CFG1_Msk) 				\
			>> AEM13920_SRCxREGU1_CFG1_Pos);
		cfg->src1_mppt_period = (AEM13920_MPPT_PERIOD)			\
			((regs[AEM13920_SRC1REGU1_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU1_CFG2_Msk) 				\
			>> AEM13920_SRCxREGU1_CFG2_Pos);
	}
	if (	((regs[AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET] 	\
		& AEM13920_SRCxREGU0_MODE_Msk) >> AEM13920_SRCxREGU0_MODE_Pos) 	\
		== AEM13920_SRCREGU_CONST) {
		cfg->src2_regu_mode = AEM13920_SRCREGU_CONST;
		cfg->src2_const_voltage = __AEM13920_SRCREGURegToVolt(		\
			regs + (AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET));
	} else {
		cfg->src2_regu_mode = AEM13920_SRCREGU_MPPT;
		cfg->src2_mppt_ratio = (AEM13920_MPPT_RATIO)			\
			((regs[AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU0_CFG0_Msk) 				\
			>> AEM13920_SRCxREGU0_CFG0_Pos);
		cfg->src2_mppt_duration = (AEM13920_MPPT_DURATION)		\
			((regs[AEM13920_SRC2REGU1_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU1_CFG1_Msk) 				\
			>> AEM13920_SRCxREGU1_CFG1_Pos);
		cfg->src2_mppt_period = (AEM13920_MPPT_PERIOD)			\
			((regs[AEM13920_SRC2REGU1_OFFSET - AEM13920_CFG_OFFSET] \
			& AEM13920_SRCxREGU1_CFG2_Msk) 				\
			>> AEM13920_SRCxREGU1_CFG2_Pos);
	}
}

/* ~~~~~~~~ SRC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define __AEM13920_SRC_MIN_VOLT		(113UL)

#define __AEM13920_SRC_RGE1_START	(0x07UL)
#define __AEM13920_SRC_RGE1_END		(0x12UL)
#define __AEM13920_SRC_RGE1_OFFSET	(90UL)
#define __AEM13920_SRC_RGE1_DIFF_OFFSET	(9UL)
#define __AEM13920_SRC_RGE1_FACT	(75UL)

#define __AEM13920_SRC_RGE2_START	(0x13UL)
#define __AEM13920_SRC_RGE2_END		(0x39UL)
#define __AEM13920_SRC_RGE2_OFFSET	(300UL)
#define __AEM13920_SRC_RGE2_DIFF_OFFSET	(37UL)
#define __AEM13920_SRC_RGE2_FACT	(150UL)

#define __AEM13920_SRC_RGE3_START	(0x68UL)
#define __AEM13920_SRC_RGE3_END		(0x79UL)
#define __AEM13920_SRC_RGE3_OFFSET	(300UL)
#define __AEM13920_SRC_RGE3_DIFF_OFFSET	(165UL)
#define __AEM13920_SRC_RGE3_FACT	(150UL)
#define __AEM13920_SRC_RGE3_DIV		(67UL)

#define __AEM13920_SRC_RGE4_START	(0x9FUL)
#define __AEM13920_SRC_RGE4_END		(0xB9UL)
#define __AEM13920_SRC_RGE4_OFFSET	(300UL)
#define __AEM13920_SRC_RGE4_DIFF_OFFSET	(293UL)
#define __AEM13920_SRC_RGE4_FACT	(150UL)
#define __AEM13920_SRC_RGE4_DIV		(33UL)

#define __AEM13920_SRC_MAX_V		(4409UL)

#define __AEM13920_SRC_SCALE_DIV	(10UL)
#define __AEM13920_SRC_SCALE_FACT	(100UL)

/**
  * @brief 	Converts SRC register value to voltage (mV)
  * 
  * @param[in]	reg		Register value
  * @return 	uint32_t	The voltage (mV)
  */
static uint32_t __AEM13920_SRCRegToVolt(uint8_t reg)
{
	uint32_t vsrcregu = 0;
	uint32_t offset = 0;
	uint32_t scale_fact = 0;
	uint32_t div = 0;
	if (reg < __AEM13920_SRC_RGE1_START) {
		return __AEM13920_SRC_MIN_VOLT;
	} else if (reg <= __AEM13920_SRC_RGE1_END) {
		vsrcregu = 	__AEM13920_SRC_RGE1_OFFSET;
		offset = 	__AEM13920_SRC_RGE1_DIFF_OFFSET;
		scale_fact = 	__AEM13920_SRC_RGE1_FACT;
	} else if (reg <= __AEM13920_SRC_RGE2_END) {
		vsrcregu = 	__AEM13920_SRC_RGE2_OFFSET;
		offset = 	__AEM13920_SRC_RGE2_DIFF_OFFSET;
		scale_fact = 	__AEM13920_SRC_RGE2_FACT;
	} else if (	(__AEM13920_SRC_RGE3_START <= reg) 			\
			&& (reg <= __AEM13920_SRC_RGE3_END)) {
		vsrcregu = 	__AEM13920_SRC_RGE3_OFFSET;
		offset = 	__AEM13920_SRC_RGE3_DIFF_OFFSET;
		scale_fact = 	__AEM13920_SRC_RGE3_FACT;
		div = 		__AEM13920_SRC_RGE3_DIV;
	} else if(	(__AEM13920_SRC_RGE4_START <= reg) 			\
			&& (reg <= __AEM13920_SRC_RGE4_END)) {
		vsrcregu = 	__AEM13920_SRC_RGE4_OFFSET;
		offset = 	__AEM13920_SRC_RGE4_DIFF_OFFSET;
		scale_fact = 	__AEM13920_SRC_RGE4_FACT;
		div = 		__AEM13920_SRC_RGE4_DIV;
	} else if (reg > __AEM13920_SRC_RGE4_END) {
		return __AEM13920_SRC_MAX_V;
	} else {
		return 0;
	}
	vsrcregu += ROUND_NEAREST_DIV(	(((reg << 1) - offset) * scale_fact), 	\
					__AEM13920_SRC_SCALE_DIV);
	if (div > 0) {
		return ROUND_NEAREST_DIV(vsrcregu * __AEM13920_SRC_SCALE_FACT, div);
	} else {
		return vsrcregu;
	}
}

/* ~~~~~~~~ STO PROTECT THRESH ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define __AEM13920_VTHRESH_DIV			(0x753UL)
#define __AEM13920_VTHRESH_FACT			(0x64UL)
#define __AEM13920_VOVDIS_MAXV			(0x0DFDUL)
#define __AEM13920_VOVDIS_MAXR			(AEM13920_VOVDIS_THRESH_Msk)
#define __AEM13920_VOVDIS_MINV			(0x0960UL)
#define __AEM13920_VCHRDY_MAXV			(0xE35UL)
#define __AEM13920_VCHRDY_MAXR			(AEM13920_VCHRDY_THRESH_Msk)
#define __AEM13920_VCHRDY_MINV			(0x998UL)
#define __AEM13920_VOVCH_MAXV			(0x11F2UL)
#define __AEM13920_VOVCH_MAXR			(0x65UL)
#define __AEM13920_VOVCH_MINV			(0x0A8CUL)

/**
  * @brief 	Converts voltage to VOVDIS register value
  * 
  * @param[in] 	mv 		The voltage (mv)
  * @return 	uint32_t	The register value that better matches the given
  *				voltage
  */
static uint8_t __AEM13920_VOVDISVoltToReg(uint32_t mv)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (mv >= __AEM13920_VOVDIS_MAXV) {
		return __AEM13920_VOVDIS_MAXR;
	}
	if (mv <= __AEM13920_VOVDIS_MINV) {
		return 0;
	}
	else {
		dvd = 	(mv * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VOVDIS_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = ROUND_NEAREST_DIV(dvd, dvr);
		return ((thresh << AEM13920_VOVDIS_THRESH_Pos) 			\
			& AEM13920_VOVDIS_THRESH_Msk);
	}
}

/**
  * @brief 	Converts VOVDIS register value to millivolts
  * 
  * @param[in] 	reg 		The register value
  * @return 	uint32_t	The voltage (mv) corresponding to the configured
  *				register
  */
static uint32_t __AEM13920_VOVDISRegToVolt(uint8_t reg)
{
	uint32_t v = 0;
	uint32_t div = __AEM13920_VTHRESH_FACT;
	if (reg > __AEM13920_VOVDIS_MAXR) {
		return __AEM13920_VOVDIS_MAXV;
	}
	v = ((reg * __AEM13920_VTHRESH_DIV) + (div * __AEM13920_VOVDIS_MINV));
	v = ROUND_NEAREST_DIV(v, div);
	return v;
}

/**
  * @brief 	Converts voltage to VCHRDY register value
  * 
  * @param[in] 	mv		The voltage (mV)
  * @return 	uint8_t		The register value that better matches the given
  *				voltage
  */
static uint8_t __AEM13920_VCHRDYVoltToReg(uint32_t mv)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (mv >= __AEM13920_VCHRDY_MAXV) {
		return __AEM13920_VCHRDY_MAXR;
	}
	if (mv <= __AEM13920_VCHRDY_MINV) {
		return 0;
	}
	else {
		dvd = 	(mv * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VCHRDY_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = (uint8_t)((dvd + (dvr / 2)) / dvr);
		return ((thresh << AEM13920_VCHRDY_THRESH_Pos) 			\
			& AEM13920_VCHRDY_THRESH_Msk);
	}
}

/**
  * @brief 	Converts CHRDY register value to millivolts
  * 
  * @param[in] 	reg 		The register value
  * @return 	uint32_t	The voltage (mv) corresponding to the configured
  *				register
  */
static uint32_t __AEM13920_VCHRDYRegToVolt(uint8_t reg)
{
	uint32_t v = 0;
	uint32_t div = __AEM13920_VTHRESH_FACT;
	if (reg > __AEM13920_VCHRDY_MAXR) {
		return __AEM13920_VCHRDY_MAXV;
	}
	v = ((reg * __AEM13920_VTHRESH_DIV) + (div * __AEM13920_VCHRDY_MINV));
	v = ((v + (div / 2)) / div);
	return v;
}

/**
  * @brief 	Converts voltage to VOVCH register value
  * 
  * @param[in] 	mv		The voltage (mV)
  * @return 	uint8_t		The register value that better matches the given 
  *				voltage
  */
static uint8_t __AEM13920_VOVCHVoltToReg(uint32_t mv)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (mv >= __AEM13920_VOVCH_MAXV) {
		return __AEM13920_VOVCH_MAXR;
	}
	if (mv <= __AEM13920_VOVCH_MINV) {
		return 0;
	}
	else {
		dvd = 	(mv * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VOVCH_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = (uint8_t)((dvd + (dvr / 2)) / dvr);
		return ((thresh << AEM13920_VOVCH_THRESH_Pos) 			\
			& AEM13920_VOVCH_THRESH_Msk);
	}
}

/**
  * @brief 	Converts VOVCH register value to millivolts
  * 
  * @param[in] 	reg 		The register value
  * @return 	uint32_t	The voltage (mv) corresponding to the configured 
  *				register
  */
static uint32_t __AEM13920_VOVCHRegToVolt(uint8_t reg)
{
	uint32_t v = 0;
	uint32_t div = __AEM13920_VTHRESH_FACT;

	if (reg >= __AEM13920_VOVCH_MAXR) {
		return __AEM13920_VOVCH_MAXV;
	}

	v = ((reg * __AEM13920_VTHRESH_DIV) + (div * __AEM13920_VOVCH_MINV));
	v = ((v + (div / 2)) / div);
	return v;
}

/* ~~~~~~~~ BSTCFG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
  * @brief	Boost Config cast structure
  */
typedef union __AEM13920_BSTCFG_UNION {
	uint8_t raw;
	AEM13920_BSTCFG_t bf;
} __AEM13920_BSTCFG_UNION_t;

/**
  * @brief 	Converts the boost configuration structure to BSTCFG register value
  * 
  * @param[in] 	cfg 		The configuration structure	
  * @return 	uint8_t		The register value
  */
static uint8_t __AEM13920_BSTCFGHRToReg(const AEM13920_BSTCFG_t *cfg)
{
	__AEM13920_BSTCFG_UNION_t _cfg;
	_cfg.bf = *cfg;
	return _cfg.raw;
}

/**
  * @brief 	Converts a BOOSTxCFG register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg		Register value 
  * @param[out] cfg		The corresponding configuration
  */
static void __AEM13920_BSTCFGRegToHR(uint8_t reg, AEM13920_BSTCFG_t *cfg)
{
	__AEM13920_BSTCFG_UNION_t _cfg;
	_cfg.raw = reg;
	*cfg = _cfg.bf;
}

static int32_t __AEM13920_ComputeBSTCFGReg(					\
	const AEM13920_CONFIG_t *cfg, 						\
	uint8_t *regs)
{
	regs[AEM13920_BST1CFG_OFFSET - AEM13920_CFG_OFFSET] = (			\
		((cfg->src1_boost_enable << AEM13920_BSTxCFG_EN_Pos) 		\
		& AEM13920_BSTxCFG_EN_Msk) 					\
		| ((cfg->src1_boost_high_power_enable << AEM13920_BSTxCFG_HPEN_Pos)\
		& AEM13920_BSTxCFG_HPEN_Msk) 					\
		| ((cfg->src1_boost_tmult << AEM13920_BSTxCFG_TMULT_Pos) 	\
		& AEM13920_BSTxCFG_TMULT_Msk));
	regs[AEM13920_BST2CFG_OFFSET - AEM13920_CFG_OFFSET] = (			\
		((cfg->src2_boost_enable << AEM13920_BSTxCFG_EN_Pos) 		\
		& AEM13920_BSTxCFG_EN_Msk) 					\
		| ((cfg->src2_boost_high_power_enable << AEM13920_BSTxCFG_HPEN_Pos)\
		& AEM13920_BSTxCFG_HPEN_Msk) 					\
		| ((cfg->src2_boost_tmult << AEM13920_BSTxCFG_TMULT_Pos) 	\
		& AEM13920_BSTxCFG_TMULT_Msk));
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertBSTCFGReg(					\
	uint8_t bst1, 								\
	uint8_t bst2, 								\
	AEM13920_CONFIG_t *cfg)
{
	cfg->src1_boost_tmult = 						\
		(AEM13920_TMULT) ((bst1 & AEM13920_BSTxCFG_TMULT_Msk) 		\
		>> AEM13920_BSTxCFG_TMULT_Pos);
	cfg->src1_boost_enable = 						\
		(bool) (bst1 & AEM13920_BSTxCFG_EN_Msk);
	cfg->src1_boost_high_power_enable = 					\
		(bool) (bst1 & AEM13920_BSTxCFG_HPEN_Msk);
	cfg->src2_boost_tmult = 						\
		(AEM13920_TMULT) ((bst2 & AEM13920_BSTxCFG_TMULT_Msk) 		\
		>> AEM13920_BSTxCFG_TMULT_Pos);
	cfg->src2_boost_enable = 						\
		(bool) (bst2 & AEM13920_BSTxCFG_EN_Msk);
	cfg->src2_boost_high_power_enable = 					\
		(bool) (bst2 & AEM13920_BSTxCFG_HPEN_Msk);
}

/* ~~~~~~~~ BUCKCFG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	Buck Config cast structure
  */
typedef union __AEM13920_BUCKCFG_UNION {
	uint8_t raw;
	AEM13920_BUCKCFG_t bf;
} __AEM13920_BUCKCFG_UNION_t;

/**
  * @brief 	Converts a buck configuration structure to the corresponding 
  *		BUCKCFG register value
  * 
  * @param[in] 	cfg 		The configuration structure
  * @return 	uint8_t		The register value
  */
static uint8_t __AEM13920_BUCKCFGHRToReg(const AEM13920_BUCKCFG_t *cfg)
{
	__AEM13920_BUCKCFG_UNION_t _cfg;
	_cfg.bf = *cfg;
	return _cfg.raw;
}

/**
  * @brief 	Converts the BUCKCFG register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_BUCKCFGRegToHR(uint8_t reg, AEM13920_BUCKCFG_t *cfg)
{
	__AEM13920_BUCKCFG_UNION_t _cfg;
	_cfg.raw = reg;
	*cfg = _cfg.bf;
}

static int32_t __AEM13920_ComputeBUCKCFGReg(					\
	const AEM13920_CONFIG_t *cfg, 						\
	uint8_t *regs)
{
	regs[AEM13920_BUCKCFG_OFFSET - AEM13920_CFG_OFFSET] = 			\
		(	(cfg->buck_vout << AEM13920_BUCKCFG_VOUT_Pos) & 	\
			AEM13920_BUCKCFG_VOUT_Msk)				\
		| (	(cfg->buck_tmult << AEM13920_BUCKCFG_TMULT_Pos) & 	\
			AEM13920_BUCKCFG_TMULT_Msk);
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertBUCKCFGReg(uint8_t reg, AEM13920_CONFIG_t *cfg)
{
	cfg->buck_tmult = 							\
		(AEM13920_TMULT) ((reg & AEM13920_BUCKCFG_TMULT_Msk) 		\
		>> AEM13920_BUCKCFG_TMULT_Pos);
	cfg->buck_vout = 							\
		(AEM13920_VOUT)	((reg & AEM13920_BUCKCFG_VOUT_Msk) 		\
		>> AEM13920_BUCKCFG_VOUT_Pos);
}

/* ~~~~~~~~ TEMPCOLD/HOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define AEM13920_TEMPXTHRESH_CONV_FACT	(0x100UL)
#define AEM13920_TEMPXTHRESH_SCALE_FACT	(0x3E8UL)

/**
  * @brief 	Converts rth to the register value
  * 
  * @param[in] 	rth		Thermistor's impedance (Rth) at a specific 
  *				threshold (mOhms)
  * @param[in] 	rdiv 		Impedance of the resistor that creates a 
  *				resistive voltage divider with Rth (mOhms) 
  * @return 	uint8_t	The register value
  */
static uint8_t __AEM13920_TEMPOhmToReg(uint32_t rth, uint32_t rdiv)
{
	uint64_t thresh = AEM13920_TEMPXTHRESH_CONV_FACT * ROUND_NEAREST_DIV(	\
		(uint64_t) rth * AEM13920_TEMPXTHRESH_SCALE_FACT, 		\
		(uint64_t) rth + rdiv);
	thresh = ROUND_NEAREST_DIV(thresh, AEM13920_TEMPXTHRESH_SCALE_FACT);
	if (thresh > UINT8_MAX) {
		thresh = UINT8_MAX;
	}

	return ((thresh << AEM13920_TEMPx_THRESH_Pos) & AEM13920_TEMPx_THRESH_Msk);
}

/**
  * @brief 	Converts the register value to the corresponding RTH value (mOhms)
  * 
  * @param[in] 	reg		The register value
  * @param[in]	rdiv 		Impedance of the resistor that creates a resistive 
  *				voltage divider with Rth (mOhms)
  * @return 	uint32_t 	RTH (mOhms)
  */
static uint32_t __AEM13920_TEMPRegToOhm(uint8_t reg, uint32_t rdiv)
{
	uint64_t therm = ROUND_NEAREST_DIV(					\
		(uint64_t) reg * rdiv, (AEM13920_TEMPXTHRESH_CONV_FACT - reg));
	if (therm > UINT32_MAX) {
		return UINT32_MAX;
	} else {
		return (uint32_t) therm;
	}
}

/* ~~~~~~~~ SRCLOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	SRCLOW config cast structure
  */
typedef union __AEM13920_SRCLOW_UNION {
	uint8_t raw;
	AEM13920_SRCLOWCFG_t bf;
} __AEM13920_SRCLOW_UNION_t;

/**
  * @brief 	Converts a SRCLOWCFG configuration structure to the corresponding 
  *		SRCLOW register value
  * 
  * @param[in] 	cfg 		The configuration structure
  * @return 	uint8_t		The register value
  */
static uint8_t __AEM13920_SRCLOWHRToReg(const AEM13920_SRCLOWCFG_t *cfg)
{
	__AEM13920_SRCLOW_UNION_t _cfg;
	_cfg.bf = *cfg;
	return _cfg.raw;
}

/**
  * @brief 	Converts the SRCLOW register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_SRCLOWRegToHR(uint8_t reg, AEM13920_SRCLOWCFG_t *cfg)
{
	__AEM13920_SRCLOW_UNION_t _cfg;
	_cfg.raw = reg;
	*cfg = _cfg.bf;
}

static int32_t __AEM13920_ComputeSRCLOWReg(					\
	const AEM13920_CONFIG_t *cfg, 						\
	uint8_t *regs)
{
	regs[AEM13920_SRCLOW_OFFSET - AEM13920_CFG_OFFSET] = 			\
		 ((cfg->src1_low_thresh << AEM13920_SRCLOW_SRC1THRESH_Pos) & 	\
		AEM13920_SRCLOW_SRC1THRESH_Msk)					\
		| ((cfg->src2_low_thresh << AEM13920_SRCLOW_SRC2THRESH_Pos) & 	\
		AEM13920_SRCLOW_SRC2THRESH_Msk);
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertSRCLOWReg(uint8_t reg, AEM13920_CONFIG_t *cfg)
{
	cfg->src1_low_thresh = (AEM13920_SRCLOW_THRESH)				\
				((reg & AEM13920_SRCLOW_SRC1THRESH_Msk) 	\
				>> AEM13920_SRCLOW_SRC1THRESH_Pos);
	cfg->src2_low_thresh = (AEM13920_SRCLOW_THRESH)				\
				((reg & AEM13920_SRCLOW_SRC2THRESH_Msk) 	\
				>> AEM13920_SRCLOW_SRC2THRESH_Pos);
}

/* ~~~~~~~~ APMCFG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	APM Config cast structure
  */
typedef union __AEM13920_APMCFG_UNION {
	uint8_t raw;
	AEM13920_APMCFG_t bf;
} __AEM13920_APMCFG_UNION_t;

/**
  * @brief 	Converts a APMCFG configuration structure to the corresponding 
  *		APM register value
  * 
  * @param[in] 	cfg 		The configuration structure
  * @return 	uint8_t		The register value
  */
static uint8_t __AEM13920_APMHRToReg(const AEM13920_APMCFG_t *cfg)
{
	__AEM13920_APMCFG_UNION_t _cfg;
	_cfg.bf = *cfg;
	return _cfg.raw;
}

/**
  * @brief 	Converts the APM register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_APMRegToHR(uint8_t reg, AEM13920_APMCFG_t *cfg)
{
	__AEM13920_APMCFG_UNION_t _cfg;
	_cfg.raw = reg;
	*cfg = _cfg.bf;
}

static int32_t __AEM13920_ComputeAPMReg(					\
	const AEM13920_CONFIG_t *cfg, 						\
	uint8_t *regs)
{
	regs[AEM13920_APM_OFFSET - AEM13920_CFG_OFFSET] = 			\
		((((uint8_t) cfg->apm_src1_enable << AEM13920_APM_SRC1EN_Pos) & \
		AEM13920_APM_SRC1EN_Msk) 					\
		| (((uint8_t) cfg->apm_src2_enable << AEM13920_APM_SRC2EN_Pos) 	\
		& AEM13920_APM_SRC2EN_Msk) 					\
		| (((uint8_t) cfg->apm_buck_enable << AEM13920_APM_BUCKEN_Pos) 	\
		& AEM13920_APM_BUCKEN_Msk) 					\
		| ((cfg->apm_mode << AEM13920_APM_MODE_Pos) 			\
		& AEM13920_APM_MODE_Msk)					\
		| ((cfg->apm_window << AEM13920_APM_WINDOW_Pos) 		\
		& AEM13920_APM_WINDOW_Msk));
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertAPMReg(uint8_t reg, AEM13920_CONFIG_t *cfg)
{
	cfg->apm_src1_enable = (bool)(reg & AEM13920_APM_SRC1EN_Msk);
	cfg->apm_src2_enable = (bool)(reg & AEM13920_APM_SRC2EN_Msk);
	cfg->apm_buck_enable = (bool)(reg & AEM13920_APM_BUCKEN_Msk);
	cfg->apm_mode = (AEM13920_APM_MODE)(					\
		(reg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	cfg->apm_window = (AEM13920_APM_WINDOW) (				\
		(reg & AEM13920_APM_WINDOW_Msk) >> AEM13920_APM_WINDOW_Pos);
}


/* ~~~~~~~~ IRQEN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	IRQEN Config cast structure
  */
typedef union _AEM13920_IRQEN_UNION {
	uint8_t raw[2];
	AEM13920_IRQEN_t bf;
} __AEM13920_IRQEN_UNION_t;

/**
  * @brief 	Converts a IRQEN configuration structure to the corresponding 
  *		IRQEN register value
  * 
  * @param[in] 	cfg 		The configuration structure
  * @return 	uint8_t		The register value
  */
static void __AEM13920_IRQENHRToReg(const AEM13920_IRQEN_t *cfg, uint8_t regs[2])
{
	__AEM13920_IRQEN_UNION_t _cfg;
	_cfg.bf = *cfg;
	regs[0] = _cfg.raw[0];
	regs[1] = _cfg.raw[1];
}

/**
  * @brief 	Converts the IRQEN register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_IRQENRegToHR(uint8_t regs[2], AEM13920_IRQEN_t *cfg)
{
	__AEM13920_IRQEN_UNION_t _cfg;
	_cfg.raw[0] = regs[0];
	_cfg.raw[1] = regs[1];
	*cfg = _cfg.bf;
}

static void __AEM13920_ComputeIRQENReg(const AEM13920_CONFIG_t *cfg, uint8_t *regs)
{
	regs[AEM13920_IRQEN0_OFFSET - AEM13920_CFG_OFFSET] = (			\
		((uint8_t) cfg->i2c_rdy_irq_enable 				\
		<< AEM13920_IRQEN0_I2CRDY_Pos)					\
		| ((uint8_t) cfg->vovdis_irq_enable 				\
		<< AEM13920_IRQEN0_VOVDIS_Pos)					\
		| ((uint8_t) cfg->vchrdy_irq_enable 				\
		<< AEM13920_IRQEN0_VCHRDY_Pos)					\
		| ((uint8_t) cfg->vovch_irq_enable 				\
		<< AEM13920_IRQEN0_VOVCH_Pos)					\
		| ((uint8_t) cfg->src_low_irq_enable 				\
		<< AEM13920_IRQEN0_SRCLOW_Pos)					\
		| ((uint8_t) cfg->temp_ch_irq_enable 				\
		<< AEM13920_IRQEN0_TEMPCH_Pos)					\
		| ((uint8_t) cfg->temp_dis_irq_enable 				\
		<< AEM13920_IRQEN0_TEMPDIS_Pos)					\
	);
	regs[AEM13920_IRQEN1_OFFSET - AEM13920_CFG_OFFSET] = (			\
		((uint8_t) cfg->src1_mppt_start_irq_enable 			\
		<< AEM13920_IRQEN1_SRC1MPPTSTART_Pos)				\
		| ((uint8_t) cfg->src1_mppt_done_irq_enable 			\
		<< AEM13920_IRQEN1_SRC1MPPTDONE_Pos)				\
		| ((uint8_t) cfg->src2_mppt_start_irq_enable 			\
		<< AEM13920_IRQEN1_SRC2MPPTSTART_Pos)				\
		| ((uint8_t) cfg->src2_mppt_done_irq_enable 			\
		<< AEM13920_IRQEN1_SRC2MPPTDONE_Pos)				\
		| ((uint8_t) cfg->sto_done_irq_enable 				\
		<< AEM13920_IRQEN1_STODONE_Pos)					\
		| ((uint8_t) cfg->temp_done_irq_enable 				\
		<< AEM13920_IRQEN1_TEMPDONE_Pos)				\
		| ((uint8_t) cfg->apm_done_irq_enable 				\
		<< AEM13920_IRQEN1_APMDONE_Pos)					\
		| ((uint8_t) cfg->apm_err_irq_enable 				\
		<< AEM13920_IRQEN1_APMERR_Pos)					\
	);
}

static void __AEM13920_ConvertIRQENReg(uint8_t irqen0, uint8_t irqen1, AEM13920_CONFIG_t *cfg)
{
	cfg->i2c_rdy_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_I2CRDY_Msk);
	cfg->vovdis_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_VOVDIS_Msk);
	cfg->vchrdy_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_VCHRDY_Msk);
	cfg->vovch_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_VOVCH_Msk);
	cfg->src_low_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_SRCLOW_Msk);
	cfg->temp_ch_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_TEMPCH_Msk);
	cfg->temp_dis_irq_enable = (bool) (irqen0 & AEM13920_IRQEN0_TEMPDIS_Msk);
	cfg->src1_mppt_start_irq_enable = 					\
		(bool) (irqen1 & AEM13920_IRQEN1_SRC1MPPTSTART_Msk);
	cfg->src1_mppt_done_irq_enable = 					\
		(bool) (irqen1 & AEM13920_IRQEN1_SRC1MPPTDONE_Msk);
	cfg->src2_mppt_start_irq_enable = 					\
		(bool) (irqen1 & AEM13920_IRQEN1_SRC2MPPTSTART_Msk);
	cfg->src2_mppt_done_irq_enable = 					\
		(bool) (irqen1 & AEM13920_IRQEN1_SRC2MPPTDONE_Msk);
	cfg->sto_done_irq_enable = 						\
		(bool) (irqen1 & AEM13920_IRQEN1_STODONE_Msk);
	cfg->temp_done_irq_enable = 						\
		(bool) (irqen1 & AEM13920_IRQEN1_TEMPDONE_Msk);
	cfg->apm_done_irq_enable = 						\
		(bool) (irqen1 & AEM13920_IRQEN1_APMDONE_Msk);
	cfg->apm_err_irq_enable = 						\
		(bool) (irqen1 & AEM13920_IRQEN1_APMERR_Msk);
}

/* ~~~~~~~~ IRQFLG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	IRQFLG Config cast structure
  */
typedef union _AEM13920_IRQFLG_UNION {
	uint8_t raw[2];
	AEM13920_IRQFLG_t bf;	
} __AEM13920_IRQFLG_UNION_t;


/**
  * @brief 	Converts the IRQFLG register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_IRQFLGRegToHR(uint8_t regs[2], AEM13920_IRQFLG_t *cfg)
{
	__AEM13920_IRQFLG_UNION_t _cfg;
	_cfg.raw[0] = regs[0];
	_cfg.raw[1] = regs[1];
	*cfg = _cfg.bf;
}

/* ~~~~~~~~ STATUS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	STATUS Config cast structure
  */
typedef union _AEM13920_STATUS_UNION {
	uint8_t raw[2];
	AEM13920_STATUS_t bf;
} __AEM13920_STATUS_UNION_t;


/**
  * @brief 	Converts the STATUS register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_STATUSRegToHR(uint8_t regs[2], AEM13920_STATUS_t *cfg)
{
	__AEM13920_STATUS_UNION_t _cfg;
	_cfg.raw[0] = regs[0];
	_cfg.raw[1] = regs[1];
	*cfg = _cfg.bf;
}

/* ~~~~~~~~ APM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void __AEM13920_APMRegToVolt(						\
	uint8_t regs[3], 							\
	uint64_t *data, 							\
	AEM13920_APM_MODE mode)
{
	if (mode == AEM13920_APM_MODE_PULSE_COUNTER) {
		*data = (uint32_t)						\
				(((regs[2] & AEM13920_APM2x_PC_Msk) << 16) 	\
				+ (regs[1] << 8) + regs[0]);
	} else {
		*data = ((uint64_t)						\
				(((regs[2] & AEM13920_APM2x_PM_DATA_Msk) << 0x10)\
				+ (regs[1] << 8) + regs[0])			\
			<< (uint8_t)						\
				((regs[2] & AEM13920_APM2x_PM_SHIFT_Msk) 	\
				>> AEM13920_APM2x_PM_SHIFT_Pos));
	}
}

/* ~~~~~~~~ APMERR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/**
  * @brief	APMERR Config cast structure
  */
typedef union _AEM13920_APMERR_UNION {
	uint8_t raw;
	AEM13920_APMERR_t bf;
} __AEM13920_APMERR_UNION_t;

/**
  * @brief 	Converts the APMERR register value and sets the configuration
  *		structure accordingly
  * 
  * @param[in] 	reg	The register value 
  * @param[out] cfg	The corresponding configuration
  */
static void __AEM13920_APMERRRegToHR(uint8_t reg, AEM13920_APMERR_t *cfg)
{
	__AEM13920_APMERR_UNION_t _cfg;
	_cfg.raw = reg;
	*cfg = _cfg.bf;
}

/* ~~~~~~~~ TEMP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define AEM13920_TEMP_CONV_DIV		(0x0100UL)

/* ~~~~~~~~ STO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define	AEM13920_VSTO_CONV_FACT		(0x12C0UL)
#define AEM13920_VSTO_CONV_DIV		(0x0100UL)



/* ~~~~~~~~ API ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int32_t AEM13920_Initialize(AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	
	if (!handler) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	
	rc = AEM_I2C_Initialize(handler->i2c_cfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, 						\
		AEM13920_VERSION_OFFSET, 					\
		1, &(handler->revision)						\
	);
	
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if ((handler->revision == 0) || (handler->revision == 1)) {
		uint8_t tmp = 0;
		rc = __AEM13920_ReadRegisters(handler->i2c_cfg, 0x51, 1, &tmp);
		tmp &= ~0x08;
		rc |= __AEM13920_WriteRegister(handler->i2c_cfg, 0x51, tmp);
	}
	
	return rc;
}

int32_t AEM13920_Deinitialize(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;

	if (!handler) {
		return AEM_I2C_ERR_MISSING_CFG;
	}

	rc = AEM_I2C_Deinitialize(handler->i2c_cfg);
	
	return rc;
}

int32_t AEM13920_GetSourceRegulationMode(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_SRCREGU_MODE *mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);
	
	if (!mode) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*mode = (AEM13920_SRCREGU_MODE) ((reg & AEM13920_SRCxREGU0_MODE_Msk) 	\
					>> AEM13920_SRCxREGU0_MODE_Pos);

	return rc;
}

int32_t AEM13920_SetSourceRegulationMode(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_SRCREGU_MODE mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_SRCxREGU0_MODE_Msk;
	reg |= ((mode << AEM13920_SRCxREGU0_MODE_Pos) & AEM13920_SRCxREGU0_MODE_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetSourceRegulationVoltage(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	uint32_t *srcregu)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[2];
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);
	AEM13920_SRCREGU_MODE mode = AEM13920_SRCREGU_CONST;
	
	if (!srcregu) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 2, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	mode = ((regs[0] & AEM13920_SRCxREGU0_MODE_Msk) 			\
	>> AEM13920_SRCxREGU0_MODE_Pos);
	if (mode != AEM13920_SRCREGU_CONST) {
		offset = AEM13920_SRC1_OFFSET + src;
		rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, regs);
		if (rc != AEM13920_DRIVER_OK) {
			return rc;
		}
		*srcregu = __AEM13920_SRCRegToVolt(regs[0]);
	} else {
		*srcregu = __AEM13920_SRCREGURegToVolt(regs);
	}

	return rc;
}

int32_t AEM13920_SetSourceConstantVoltage(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	uint32_t srcregu)
{
	uint8_t regs[2] = {0};
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);

	__AEM13920_SRCREGUVoltToReg(srcregu, regs);

	return __AEM13920_WriteRegisters(handler->i2c_cfg, offset, 2, regs);
}

int32_t AEM13920_GetMPPTConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPTCFG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[2] = {0};
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 2, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	cfg->ratio = (AEM13920_MPPT_RATIO) (
		(regs[0] & AEM13920_SRCxREGU0_CFG0_Msk) 			\
		>> AEM13920_SRCxREGU0_CFG0_Pos);
	cfg->duration = (AEM13920_MPPT_DURATION) (				\
		(regs[1] & AEM13920_SRCxREGU1_CFG1_Msk)				\
		>> AEM13920_SRCxREGU1_CFG1_Pos);
	cfg->period = (AEM13920_MPPT_PERIOD) (					\
		(regs[1] & AEM13920_SRCxREGU1_CFG2_Msk)				\
		>> AEM13920_SRCxREGU1_CFG2_Pos);
	
	return rc;
}

int32_t AEM13920_SetMPPTConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	const AEM13920_MPPTCFG_t *cfg)
{
	uint8_t regs[2] = {0};
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src << 1);

	regs[0] = AEM13920_SRCREGU_MPPT;
	regs[0] |= (	(cfg->ratio << AEM13920_SRCxREGU0_CFG0_Pos) 		\
			& AEM13920_SRCxREGU0_CFG0_Msk);
	regs[1] = (	(cfg->duration << AEM13920_SRCxREGU1_CFG1_Pos) 		\
			& AEM13920_SRCxREGU1_CFG1_Msk) 				\
			| ((cfg->period << AEM13920_SRCxREGU1_CFG2_Pos) 	\
			& AEM13920_SRCxREGU1_CFG2_Msk);

	return __AEM13920_WriteRegisters(handler->i2c_cfg, offset, 2, regs);
}

int32_t AEM13920_GetMPPTRatio(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_RATIO *ratio)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (2 * src);

	if (!ratio) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*ratio = (AEM13920_MPPT_RATIO) ((reg & AEM13920_SRCxREGU0_CFG0_Msk) 	\
					>> AEM13920_SRCxREGU0_CFG0_Pos);

	return rc;
}

int32_t AEM13920_SetMPPTRatio(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_RATIO ratio)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src * 2);

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_SRCxREGU0_CFG0_Msk;
	reg |= ((ratio << AEM13920_SRCxREGU0_CFG0_Pos) & AEM13920_SRCxREGU0_CFG0_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetMPPTDuration(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_DURATION *duration)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (2 * src);

	if (!duration) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*duration = (AEM13920_MPPT_DURATION) ((reg & AEM13920_SRCxREGU1_CFG1_Msk)\
					      >> AEM13920_SRCxREGU1_CFG1_Pos);
	
	return rc;
}

int32_t AEM13920_SetMPPTDuration(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_DURATION duration)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (src * 2);

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	reg &= ~AEM13920_SRCxREGU1_CFG1_Msk;
	reg |= ((duration << AEM13920_SRCxREGU1_CFG1_Pos) 			\
		& AEM13920_SRCxREGU1_CFG1_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetMPPTPeriod(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_PERIOD *period)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (2 * src);

	if (!period) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*period = (AEM13920_MPPT_PERIOD) ((reg & AEM13920_SRCxREGU1_CFG2_Msk)	\
					  >> AEM13920_SRCxREGU1_CFG2_Pos);
	
	return rc;
}

int32_t AEM13920_SetMPPTPeriod(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_MPPT_PERIOD period)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (src * 2);

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	reg &= ~AEM13920_SRCxREGU1_CFG2_Msk;
	reg |= ((period << AEM13920_SRCxREGU1_CFG2_Pos) 			\
		& AEM13920_SRCxREGU1_CFG2_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetOverdischargeVoltage(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vovdis)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vovdis) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_VOVDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vovdis = __AEM13920_VOVDISRegToVolt(reg & AEM13920_VOVDIS_THRESH_Msk);

	return rc;
}

int32_t AEM13920_SetOverdischargeVoltage(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vovdis)
{
	uint8_t reg = __AEM13920_VOVDISVoltToReg(vovdis);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_VOVDIS_OFFSET, reg);
}

int32_t AEM13920_GetChargeReadyVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vchrdy)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vchrdy) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_VCHRDY_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vchrdy = __AEM13920_VCHRDYRegToVolt(reg);

	return rc;
}

int32_t AEM13920_SetChargeReadyVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vchrdy)
{
	uint8_t reg = __AEM13920_VCHRDYVoltToReg(vchrdy);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_VCHRDY_OFFSET, reg);
}

int32_t AEM13920_GetOverchargeVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vovch)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vovch) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_VOVCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vovch = __AEM13920_VOVCHRegToVolt(reg);

	return rc;
}

int32_t AEM13920_SetOverchargeVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t vovch)
{
	uint8_t reg = __AEM13920_VOVCHVoltToReg(vovch);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_VOVCH_OFFSET, reg);
}

int32_t AEM13920_SetBoostConfig(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	const AEM13920_BSTCFG_t *cfg)
{
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;
	
	reg = __AEM13920_BSTCFGHRToReg(cfg);
	
	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}
	
int32_t AEM13920_GetBoostConfig(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	AEM13920_BSTCFG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	__AEM13920_BSTCFGRegToHR(reg, cfg);

	return rc;
}

int32_t AEM13920_EnableBoost(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_BSTxCFG_EN_Msk;

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_DisableBoost(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BSTxCFG_EN_Msk;

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_IsEnabledBoost(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, 							\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (reg & AEM13920_BSTxCFG_EN_Msk);

	return rc;
}

int32_t AEM13920_EnableHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}	
	reg |= AEM13920_BSTxCFG_HPEN_Msk;

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_DisableHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BSTxCFG_HPEN_Msk;

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_IsEnabledHighPower(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_BSTxCFG_HPEN_Msk);

	return rc;
}

int32_t AEM13920_SetBoostTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_TMULT tmult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BSTxCFG_TMULT_Msk;
	reg |= ((tmult << AEM13920_BSTxCFG_TMULT_Pos) & AEM13920_BSTxCFG_TMULT_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetBoostTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src,							\
	AEM13920_TMULT *tmult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (!tmult) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*tmult = (AEM13920_TMULT) (	(reg & AEM13920_BSTxCFG_TMULT_Msk) 	\
					>> AEM13920_BSTxCFG_TMULT_Pos);

	return rc;
}

int32_t AEM13920_SetBuckConfig(							\
	const AEM13920_Handler_t *handler,					\
	const AEM13920_BUCKCFG_t *cfg)
{
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BUCKCFG_OFFSET;

	reg = __AEM13920_BUCKCFGHRToReg(cfg);

	return __AEM13920_WriteRegister(handler->i2c_cfg, offset, reg);
}

int32_t AEM13920_GetBuckConfig(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_BUCKCFG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BUCKCFG_OFFSET;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	__AEM13920_BUCKCFGRegToHR(reg, cfg);

	return rc;
}

int32_t AEM13920_SetBuckVOUT(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_VOUT vout)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BUCKCFG_VOUT_Msk;
	reg |= ((vout << AEM13920_BUCKCFG_VOUT_Pos) & AEM13920_BUCKCFG_VOUT_Msk);

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, reg);
}

int32_t AEM13920_GetBuckVOUT(							\
	const AEM13920_Handler_t *handler,					\
	AEM13920_VOUT *vout)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vout) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vout = (AEM13920_VOUT) (	(reg & AEM13920_BUCKCFG_VOUT_Msk) 	\
					>> AEM13920_BUCKCFG_VOUT_Pos);

	return rc;
}

int32_t AEM13920_SetBuckTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_TMULT tmult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BUCKCFG_TMULT_Msk;
	reg |= ((tmult << AEM13920_BUCKCFG_TMULT_Pos) & AEM13920_BUCKCFG_TMULT_Msk);

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, reg);
}

int32_t AEM13920_GetBuckTimingMultiplier(					\
	const AEM13920_Handler_t *handler,					\
	AEM13920_TMULT *tmult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!tmult) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*tmult = (AEM13920_TMULT) (	(reg & AEM13920_BUCKCFG_TMULT_Msk) 	\
					>> AEM13920_BUCKCFG_TMULT_Pos);

	return rc;
}

int32_t AEM13920_GetTempColdChargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t *rth, 								\
	uint32_t rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TEMPCOLDCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*rth = __AEM13920_TEMPRegToOhm(reg, rdiv);

	return rc;
}

int32_t AEM13920_SetTempColdChargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t rth, 								\
	uint32_t rdiv)
{
	uint8_t reg = __AEM13920_TEMPOhmToReg(rth, rdiv);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TEMPCOLDCH_OFFSET, reg);
}

int32_t AEM13920_GetTempHotChargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *rth, 								\
	uint32_t rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TEMPHOTCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*rth = __AEM13920_TEMPRegToOhm(reg, rdiv);

	return rc;
}

int32_t AEM13920_SetTempHotChargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rth, 								\
	uint32_t rdiv)
{
	uint8_t reg = __AEM13920_TEMPOhmToReg(rth, rdiv);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TEMPHOTCH_OFFSET, reg);
}

int32_t AEM13920_GetTempColdDischargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t *rth, 								\
	uint32_t rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TEMPCOLDDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*rth = __AEM13920_TEMPRegToOhm(reg, rdiv);

	return rc;
}

int32_t AEM13920_SetTempColdDischargeThresh(					\
	const AEM13920_Handler_t *handler,					\
	uint32_t rth, 								\
	uint32_t rdiv)
{
	uint8_t reg = __AEM13920_TEMPOhmToReg(rth, rdiv);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TEMPCOLDDIS_OFFSET, reg);
}

int32_t AEM13920_GetTempHotDischargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *rth, 								\
	uint32_t rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TEMPHOTDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*rth = __AEM13920_TEMPRegToOhm(reg, rdiv);

	return rc;
}

int32_t AEM13920_SetTempHotDischargeThresh(					\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rth, 								\
	uint32_t rdiv)
{
	uint8_t reg = __AEM13920_TEMPOhmToReg(rth, rdiv);
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TEMPHOTDIS_OFFSET, reg);
}

int32_t AEM13920_EnableTempMon(const AEM13920_Handler_t *handler)
{
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TMON_OFFSET, AEM13920_TMON_EN_Msk);
}

int32_t AEM13920_DisableTempMon(const AEM13920_Handler_t *handler)
{
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_TMON_OFFSET, 0);
}

int32_t AEM13920_IsEnabledTempMon(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TMON_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_TMON_EN_Msk);

	return rc;
}

int32_t AEM13920_SetSRCLOWConfig(						\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_SRCLOWCFG_t *cfg)
{
	uint8_t reg = 0;

	reg = __AEM13920_SRCLOWHRToReg(cfg);

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_SRCLOW_OFFSET, reg);
}

int32_t AEM13920_GetSRCLOWConfig(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SRCLOWCFG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_SRCLOW_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_SRCLOWRegToHR(reg, cfg);
	
	return rc;
}

int32_t AEM13920_SetSRCLowThresh(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_SRCLOW_THRESH thresh)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_SRCLOW_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg &= ~AEM13920_SRCLOW_SRC1THRESH_Msk;
		reg |= ((thresh << AEM13920_SRCLOW_SRC1THRESH_Pos) 		\
			& AEM13920_SRCLOW_SRC1THRESH_Msk);
	} else {
		reg &= ~AEM13920_SRCLOW_SRC2THRESH_Msk;
		reg |= ((thresh << AEM13920_SRCLOW_SRC2THRESH_Pos) 		\
			& AEM13920_SRCLOW_SRC2THRESH_Msk);
	}

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_SRCLOW_OFFSET, reg);
}

int32_t AEM13920_GetSRCLowThresh(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	AEM13920_SRCLOW_THRESH *thresh)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!thresh) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_SRCLOW_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	if (src == AEM13920_SRC1) {
		*thresh = (	(reg & AEM13920_SRCLOW_SRC1THRESH_Msk) 		\
				>> AEM13920_SRCLOW_SRC1THRESH_Pos);
	} else {
		*thresh = (	(reg & AEM13920_SRCLOW_SRC2THRESH_Msk) 		\
				>> AEM13920_SRCLOW_SRC2THRESH_Pos);
	}
	
	return rc;
}

int32_t AEM13920_SetAPMConfig(							\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_APMCFG_t *cfg)
{
	uint8_t reg = 0;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	reg = __AEM13920_APMHRToReg(cfg);

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_GetAPMConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APMCFG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_APMRegToHR(reg, cfg);

	return rc;
}

int32_t AEM13920_EnableAPMSource(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= (AEM13920_APM_SRC1EN_Msk << src);

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_DisableAPMSource(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;


	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~(AEM13920_APM_SRC1EN_Msk << src);

	return __AEM13920_WriteRegister(handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_IsEnabledAPMSource(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src,							\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & (AEM13920_APM_SRC1EN_Msk << src));

	return rc;
}

int32_t AEM13920_EnableAPMBuck(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_APM_BUCKEN_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_DisableAPMBuck(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_BUCKEN_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_IsEnabledAPMBuck(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_APM_BUCKEN_Msk);

	return rc;
}

int32_t AEM13920_SetAPMMode(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_MODE mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_MODE_Msk;
	reg |= ((mode << AEM13920_APM_MODE_Pos) & AEM13920_APM_MODE_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_GetAPMMode(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_MODE *mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!mode) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*mode = (AEM13920_APM_MODE) (	(reg & AEM13920_APM_MODE_Msk) 		\
					>> AEM13920_APM_MODE_Pos);

	return rc;
}

int32_t AEM13920_SetAPMWindow(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_WINDOW window)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_WINDOW_Msk;
	reg |= ((window << AEM13920_APM_WINDOW_Pos) & AEM13920_APM_WINDOW_Msk);

	return __AEM13920_WriteRegister(handler->i2c_cfg, AEM13920_APM_OFFSET, reg);
}

int32_t AEM13920_GetAPMWindow(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APM_WINDOW *window)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!window) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*window = (AEM13920_APM_WINDOW) ((reg & AEM13920_APM_WINDOW_Msk) 	\
					>> AEM13920_APM_WINDOW_Pos);

	return rc;
}

int32_t AEM13920_SetIRQConfig(							\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_IRQEN_t *irqs)
{
	uint8_t regs[2] = {0};

	if (!irqs) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	__AEM13920_IRQENHRToReg(irqs, regs);

	return __AEM13920_WriteRegisters(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 2, regs);
}

int32_t AEM13920_GetIRQConfig(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_IRQEN_t *irqs)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[2] = {0};

	if (!irqs) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 2, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_IRQENRegToHR(regs, irqs);

	return rc;
}

int32_t AEM13920_EnableI2CReadyIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_I2CRDY_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableI2CReadyIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_I2CRDY_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledI2CReadyIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_I2CRDY_Msk) 			\
			>> AEM13920_IRQEN0_I2CRDY_Pos);
	
	return rc;
}

int32_t AEM13920_EnableOverdischargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_VOVDIS_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableOverdischargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_VOVDIS_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledOverdischargeIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_VOVDIS_Msk) 			\
			>> AEM13920_IRQEN0_VOVDIS_Pos);
	
	return rc;
}

int32_t AEM13920_EnableChargeReadyIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_VCHRDY_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableChargeReadyIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_VCHRDY_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledChargeReadyIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_VCHRDY_Msk) 			\
			>> AEM13920_IRQEN0_VCHRDY_Pos);
	
	return rc;
}

int32_t AEM13920_EnableOverchargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_VOVCH_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableOverchargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_VOVCH_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledOverchargeIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_VOVCH_Msk) 			\
			>> AEM13920_IRQEN0_VOVCH_Pos);
	
	return rc;
}

int32_t AEM13920_EnableSRCLowIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_SRCLOW_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableSRCLowIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_SRCLOW_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledSRCLowIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_SRCLOW_Msk) 			\
			>> AEM13920_IRQEN0_SRCLOW_Pos);
	
	return rc;
}

int32_t AEM13920_EnableTempChargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_TEMPCH_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableTempChargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_TEMPCH_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledTempChargeIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_TEMPCH_Msk) 			\
			>> AEM13920_IRQEN0_TEMPCH_Pos);
	
	return rc;
}

int32_t AEM13920_EnableTempDischargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_IRQEN0_TEMPDIS_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_DisableTempDischargeIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_IRQEN0_TEMPDIS_Msk;

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, reg);
}

int32_t AEM13920_IsEnabledTempDischargeIRQ(					\
	const AEM13920_Handler_t *handler, 					\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN0_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN0_TEMPDIS_Msk) 			\
			>> AEM13920_IRQEN0_TEMPDIS_Pos);
	
	return rc;
}

int32_t AEM13920_EnableMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg |= AEM13920_IRQEN1_SRC1MPPTSTART_Msk;
	} else {
		reg |= AEM13920_IRQEN1_SRC2MPPTSTART_Msk;
	}

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler,					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg &= ~AEM13920_IRQEN1_SRC1MPPTSTART_Msk;
	} else {
		reg &= ~AEM13920_IRQEN1_SRC2MPPTSTART_Msk;
	}

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledMPPTStartIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	if (src == AEM13920_SRC1) {
		*en = (bool)(	(reg & AEM13920_IRQEN1_SRC1MPPTSTART_Msk) 	\
				>> AEM13920_IRQEN1_SRC1MPPTSTART_Pos);
	} else {
		*en = (bool)(	(reg & AEM13920_IRQEN1_SRC2MPPTSTART_Msk) 	\
				>> AEM13920_IRQEN1_SRC2MPPTSTART_Pos);
	}
	
	return rc;
}

int32_t AEM13920_EnableMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg |= AEM13920_IRQEN1_SRC1MPPTDONE_Msk;
	} else {
		reg |= AEM13920_IRQEN1_SRC2MPPTDONE_Msk;
	}

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg &= ~AEM13920_IRQEN1_SRC1MPPTDONE_Msk;
	} else {
		reg &= ~AEM13920_IRQEN1_SRC2MPPTDONE_Msk;
	}

	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledMPPTDoneIRQ(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	if (src == AEM13920_SRC1) {
		*en = (bool)(	(reg & AEM13920_IRQEN1_SRC1MPPTDONE_Msk) 	\
				>> AEM13920_IRQEN1_SRC1MPPTDONE_Pos);
	} else {
		*en = (bool)(	(reg & AEM13920_IRQEN1_SRC2MPPTDONE_Msk) 	\
				>> AEM13920_IRQEN1_SRC2MPPTDONE_Pos);
	}
	
	return rc;
}

int32_t AEM13920_EnableSTODoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg |= AEM13920_IRQEN1_STODONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableSTODoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg &= ~AEM13920_IRQEN1_STODONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledSTODoneIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN1_STODONE_Msk) 			\
			>> AEM13920_IRQEN1_STODONE_Pos);
	
	return rc;
}

int32_t AEM13920_EnableTempDoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg |= AEM13920_IRQEN1_TEMPDONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableTempDoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg &= ~AEM13920_IRQEN1_TEMPDONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledTempDoneIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN1_TEMPDONE_Msk) 			\
			>> AEM13920_IRQEN1_TEMPDONE_Pos);
	
	return rc;
}

int32_t AEM13920_EnableAPMDoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg |= AEM13920_IRQEN1_APMDONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableAPMDoneIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg &= ~AEM13920_IRQEN1_APMDONE_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledAPMDoneIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN1_APMDONE_Msk) 			\
			>> AEM13920_IRQEN1_APMDONE_Pos);
	
	return rc;
}

int32_t AEM13920_EnableAPMErrIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg |= AEM13920_IRQEN1_APMERR_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_DisableAPMErrIRQ(const AEM13920_Handler_t *handler)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	reg &= ~AEM13920_IRQEN1_APMERR_Msk;
	
	return __AEM13920_WriteRegister(					\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, reg);
}

int32_t AEM13920_IsEnabledAPMErrIRQ(const AEM13920_Handler_t *handler, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQEN1_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool)(	(reg & AEM13920_IRQEN1_APMERR_Msk) 			\
			>> AEM13920_IRQEN1_APMERR_Pos);
	
	return rc;
}

int32_t AEM13920_SetConfigurationMode(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_MODE mode)
{
	uint8_t reg = (mode & AEM13920_CTRL_UPDATE_Msk);

	return __AEM13920_WriteRegisters(					\
		handler->i2c_cfg, AEM13920_CTRL_OFFSET, 1, &reg);
}

int32_t AEM13920_GetConfigurationMode(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_MODE *mode)
{
	uint8_t data = 0;
	int32_t rc = AEM13920_DRIVER_OK;
	
	if (!mode) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_CTRL_OFFSET, 1, &data);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*mode = ((data & AEM13920_CTRL_UPDATE_Msk) >> AEM13920_CTRL_UPDATE_Pos);
	
	return rc;
}

int32_t AEM13920_SyncI2CConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	bool blocking)
{
	uint8_t reg = AEM13920_CTRL_UPDATE_Msk;
	int32_t rc = AEM13920_DRIVER_OK;
	bool syncing = blocking;

	rc = __AEM13920_WriteRegisters(						\
		handler->i2c_cfg, AEM13920_CTRL_OFFSET, 1, &reg);
	while (syncing) {
		if (rc != AEM13920_DRIVER_OK) {
			return rc;
		}
		rc = AEM13920_IsSyncingI2CConfiguration(handler, &syncing);
	}

	return rc;
}

int32_t AEM13920_IsSyncingI2CConfiguration(					\
	const AEM13920_Handler_t *handler,					\
	bool *syncing)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_CTRL_OFFSET, 1, &reg);

	*syncing = (bool)(reg & AEM13920_CTRL_SYNCBUSY_Msk);
	
	return rc;
}

int32_t AEM13920_GetChipVersion(						\
	const AEM13920_Handler_t *handler, 					\
	uint8_t *version)
{
	if (!version) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	return __AEM13920_ReadRegisters(					\
		handler->i2c_cfg, AEM13920_VERSION_OFFSET, 1, version);
}

int32_t AEM13920_GetIRQFlags(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_IRQFLG_t *flags)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[2] = {0};

	if (!flags) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 2, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_IRQFLGRegToHR(regs, flags);
	
	return rc;
}

int32_t AEM13920_GetI2CReadyFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_I2CRDY_Msk);
	
	return rc;
}

int32_t AEM13920_GetOverdischargeFlag(						\
	const AEM13920_Handler_t *handler, 					\
	bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_VOVDIS_Msk);
	
	return rc;
}

int32_t AEM13920_GetChargeReadyFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_VCHRDY_Msk);
	
	return rc;
}


int32_t AEM13920_GetOverchargeFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_VOVCH_Msk);
	
	return rc;
}

int32_t AEM13920_GetSRCLowFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_SRCLOW_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempChargeFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_TEMPCH_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempDischargeFlag(						\
	const AEM13920_Handler_t *handler, 					\
	bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG0_OFFSET, 1, &reg);
	*flag = (bool)(reg & AEM13920_IRQFLG0_TEMPDIS_Msk);
	
	return rc;
}

int32_t AEM13920_GetMPPTStartFlag(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src , 							\
	bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	if (src == AEM13920_SRC1) {
		*flag = (bool)(reg & AEM13920_IRQFLG1_SRC1MPPTSTART_Msk);
	} else {
		*flag = (bool)(reg & AEM13920_IRQFLG1_SRC2MPPTSTART_Msk);
	}
	
	return rc;
}

int32_t AEM13920_GetMPPTDoneFlag(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src , 							\
	bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	if (src == AEM13920_SRC1) {
		*flag = (bool)(reg & AEM13920_IRQFLG1_SRC1MPPTDONE_Msk);
	} else {
		*flag = (bool)(reg & AEM13920_IRQFLG1_SRC2MPPTDONE_Msk);
	}
	
	return rc;
}

int32_t AEM13920_GetSTODoneFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	*flag = (bool)(reg & AEM13920_IRQFLG1_STODONE_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempDoneFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	*flag = (bool)(reg & AEM13920_IRQFLG1_TEMPDONE_Msk);
	
	return rc;
}

int32_t AEM13920_GetAPMDoneFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	*flag = (bool)(reg & AEM13920_IRQFLG1_APMDONE_Msk);
	
	return rc;
}

int32_t AEM13920_GetAPMErrFlag(const AEM13920_Handler_t *handler, bool *flag)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!flag) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_IRQFLG1_OFFSET, 1, &reg);
	
	*flag = (bool)(reg & AEM13920_IRQFLG1_APMERR_Msk);
	
	return rc;
}

int32_t AEM13920_GetStatus(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_STATUS_t *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[2] = {0};
	
	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS0_OFFSET, 2, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_STATUSRegToHR(regs, status);

	return rc;
}

int32_t AEM13920_GetOverdischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS0_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS0_VOVDIS_Msk);
	
	return rc;
}

int32_t AEM13920_GetChargeReadyStatus(						\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS0_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS0_VCHRDY_Msk);
	
	return rc;
}

int32_t AEM13920_GetOverchargeStatus(						\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS0_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS0_VOVCH_Msk);
	
	return rc;
}

int32_t AEM13920_GetSRCLowStatus(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS0_OFFSET, 1, &reg);
	
	if (src == AEM13920_SRC1) {
		*status = (bool)(reg & AEM13920_STATUS0_SRC1SRCLOW_Msk);
	} else  {
		*status = (bool)(reg & AEM13920_STATUS0_SRC2SRCLOW_Msk);
	}
	
	return rc;
}

int32_t AEM13920_GetTempHotChargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS1_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS1_TEMPHOTCH_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempHotDischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS1_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS1_TEMPHOTDIS_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempColdChargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS1_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS1_TEMPCOLDCH_Msk);
	
	return rc;
}

int32_t AEM13920_GetTempColdDischargeStatus(					\
	const AEM13920_Handler_t *handler, 					\
	bool *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STATUS1_OFFSET, 1, &reg);
	*status = (bool)(reg & AEM13920_STATUS1_TEMPCOLDDIS_Msk);
	
	return rc;
}

int32_t AEM13920_GetAPMSourceData(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	uint64_t *data)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint32_t offset = AEM13920_APM0SRC1_OFFSET + (3 * src);
	uint8_t regs[3] = {0};
	AEM13920_APM_MODE apm_mode = AEM13920_APM_MODE_PULSE_COUNTER;
	uint8_t apm_cfg = 0;

	if (!data) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 3, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &apm_cfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (!(apm_cfg & (AEM13920_APM_SRC1EN_Msk << src))) {
		return AEM13920_DRIVER_ERR_APM_DISABLED;
	}
	apm_mode = ((apm_cfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);

	__AEM13920_APMRegToVolt(regs, data, apm_mode);

	return rc;
}

int32_t AEM13920_GetAPMBuckData(const AEM13920_Handler_t *handler, uint64_t *data)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint32_t offset = AEM13920_APM0BUCK_OFFSET;
	uint8_t regs[3] = {0};
	AEM13920_APM_MODE apm_mode = AEM13920_APM_MODE_PULSE_COUNTER;
	AEM13920_VOUT vout = AEM13920_VOUT_OFF;
	uint8_t apm_cfg = 0;

	if (!data) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(handler->i2c_cfg, offset, 3, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APM_OFFSET, 1, &apm_cfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (!(apm_cfg & (AEM13920_APM_BUCKEN_Msk))) {
		return AEM13920_DRIVER_ERR_APM_DISABLED;
	}
	rc = AEM13920_GetBuckVOUT(handler, &vout);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (vout == AEM13920_VOUT_OFF) {
		return AEM13920_DRIVER_ERR_BUCK_DISABLED;
	}
	apm_mode = ((apm_cfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);

	__AEM13920_APMRegToVolt(regs, data, apm_mode);

	return rc;
}

int32_t AEM13920_GetAPMErrors(							\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_APMERR_t *errors)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	
	if (!errors) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APMERR_OFFSET, 2, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	__AEM13920_APMERRRegToHR(reg, errors);

	return rc;
}

int32_t AEM13920_IsAPMSourceOverflow(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *error)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	
	if (!error) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APMERR_OFFSET, 2, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	if (src == AEM13920_SRC1) {
		*error = (bool) (reg & AEM13920_APMERR_SRC1OV_Msk);
	} else  {
		*error = (bool) (reg & AEM13920_APMERR_SRC2OV_Msk);
	}

	return rc;
}

int32_t AEM13920_IsAPMSourceCorrupted(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_SOURCE src, 							\
	bool *error)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	
	if (!error) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APMERR_OFFSET, 2, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	if (src == AEM13920_SRC1) {
		*error = (bool) (reg & AEM13920_APMERR_SRC1NVLD_Msk);
	} else  {
		*error = (bool) (reg & AEM13920_APMERR_SRC2NVLD_Msk);
	}

	return rc;
}

int32_t AEM13920_IsAPMBuckOverflow(const AEM13920_Handler_t *handler, bool *error)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	
	if (!error) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APMERR_OFFSET, 2, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	*error = (bool) (reg & AEM13920_APMERR_BUCKOV_Msk);

	return rc;
}

int32_t AEM13920_IsAPMBuckCorrupted(const AEM13920_Handler_t *handler, bool *error)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	
	if (!error) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_APMERR_OFFSET, 2, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	
	*error = (bool) (reg & AEM13920_APMERR_BUCKNVLD_Msk);

	return rc;
}

int32_t AEM13920_GetThermistorZ(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t rdiv,								\
	uint64_t *rth)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint64_t dvr = 0;

	if (!handler) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if (!rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_TEMP_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	dvr = (AEM13920_TEMP_CONV_DIV - reg);
	*rth = ROUND_NEAREST_DIV((uint64_t) rdiv * reg, dvr);

	return rc;
}

int32_t AEM13920_GetStorageVoltage(						\
	const AEM13920_Handler_t *handler, 					\
	uint32_t *vsto)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vsto) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, AEM13920_STO_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vsto = ROUND_NEAREST_DIV(						\
		AEM13920_VSTO_CONV_FACT * reg, AEM13920_VSTO_CONV_DIV);

	return rc;
}

int32_t AEM13920_SetConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	const AEM13920_CONFIG_t *cfg,						\
	bool blocking)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[AEM13920_CFG_LENGTH] = {0};
	bool syncing = blocking;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ComputeSRCREGUReg(cfg, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	regs[AEM13920_VOVDIS_OFFSET - AEM13920_CFG_OFFSET] = 			\
		__AEM13920_VOVDISVoltToReg(cfg->vovdis);
	regs[AEM13920_VCHRDY_OFFSET - AEM13920_CFG_OFFSET] = 			\
		__AEM13920_VCHRDYVoltToReg(cfg->vchrdy);
	regs[AEM13920_VOVCH_OFFSET - AEM13920_CFG_OFFSET] =			\
		__AEM13920_VOVCHVoltToReg(cfg->vovch);
	rc = __AEM13920_ComputeBSTCFGReg(cfg, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeBUCKCFGReg(cfg, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	regs[AEM13920_TEMPCOLDCH_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_TEMPOhmToReg(cfg->temp_cold_ch_rth, cfg->temp_rdiv);
	regs[AEM13920_TEMPCOLDDIS_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_TEMPOhmToReg(cfg->temp_cold_dis_rth, cfg->temp_rdiv);
	regs[AEM13920_TEMPHOTCH_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_TEMPOhmToReg(cfg->temp_hot_ch_rth, cfg->temp_rdiv);
	regs[AEM13920_TEMPHOTDIS_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_TEMPOhmToReg(cfg->temp_hot_dis_rth, cfg->temp_rdiv);
	regs[AEM13920_TMON_OFFSET - AEM13920_CFG_OFFSET] = (uint8_t)		\
		(cfg->temp_mon_enable << AEM13920_TMON_EN_Pos);
	rc = __AEM13920_ComputeSRCLOWReg(cfg, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeAPMReg(cfg, regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	__AEM13920_ComputeIRQENReg(cfg, regs);
	
	regs[AEM13920_CTRL_OFFSET - AEM13920_CFG_OFFSET] = 			\
		AEM13920_CTRL_UPDATE_Msk;

	rc = __AEM13920_WriteRegisters(						\
		handler->i2c_cfg, 						\
		AEM13920_CFG_OFFSET, 						\
		AEM13920_CFG_LENGTH, 						\
		regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	while (syncing) {
		rc = AEM13920_IsSyncingI2CConfiguration(handler, &syncing);
		if (rc != AEM13920_DRIVER_OK) {
			return rc;
		}
	}

	return rc;
}

int32_t AEM13920_GetConfiguration(						\
	const AEM13920_Handler_t *handler, 					\
	AEM13920_CONFIG_t *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t regs[AEM13920_CFG_LENGTH] = {0};

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(						\
		handler->i2c_cfg, 						\
		AEM13920_CFG_OFFSET, 						\
		AEM13920_CFG_LENGTH, 						\
		regs);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_ConvertSRCREGUReg(regs, cfg);
	cfg->vovdis = __AEM13920_VOVDISRegToVolt(				\
		regs[AEM13920_VOVDIS_OFFSET - AEM13920_CFG_OFFSET]);
	cfg->vchrdy = __AEM13920_VCHRDYRegToVolt(				\
		regs[AEM13920_VCHRDY_OFFSET - AEM13920_CFG_OFFSET]);
	cfg->vovch = __AEM13920_VOVCHRegToVolt(					\
		regs[AEM13920_VOVCH_OFFSET - AEM13920_CFG_OFFSET]);
	__AEM13920_ConvertBSTCFGReg(						\
		regs[AEM13920_BST1CFG_OFFSET - AEM13920_CFG_OFFSET], 		\
		regs[AEM13920_BST2CFG_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertBUCKCFGReg(						\
		regs[AEM13920_BUCKCFG_OFFSET - AEM13920_CFG_OFFSET], cfg);
	cfg->temp_cold_ch_rth = __AEM13920_TEMPRegToOhm(			\
		regs[AEM13920_TEMPCOLDCH_OFFSET - AEM13920_CFG_OFFSET], 	\
		cfg->temp_rdiv);
	cfg->temp_hot_ch_rth = __AEM13920_TEMPRegToOhm(				\
		regs[AEM13920_TEMPHOTCH_OFFSET - AEM13920_CFG_OFFSET], 		\
		cfg->temp_rdiv);
	cfg->temp_cold_dis_rth = __AEM13920_TEMPRegToOhm(			\
		regs[AEM13920_TEMPCOLDDIS_OFFSET - AEM13920_CFG_OFFSET], 	\
		cfg->temp_rdiv);
	cfg->temp_hot_dis_rth = __AEM13920_TEMPRegToOhm(			\
		regs[AEM13920_TEMPHOTDIS_OFFSET - AEM13920_CFG_OFFSET], 	\
		cfg->temp_rdiv);
	cfg->temp_mon_enable = regs[AEM13920_TMON_OFFSET - AEM13920_CFG_OFFSET];
	__AEM13920_ConvertSRCLOWReg(						\
		regs[AEM13920_SRCLOW_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertAPMReg(						\
		regs[AEM13920_APM_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertIRQENReg(						\
		regs[AEM13920_IRQEN0_OFFSET - AEM13920_CFG_OFFSET], 		\
		regs[AEM13920_IRQEN1_OFFSET - AEM13920_CFG_OFFSET], cfg);

	return rc;
}

