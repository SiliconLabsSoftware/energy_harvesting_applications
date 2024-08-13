/**
  ******************************************************************************
  * \file	HAL_AEM13920.c
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM13920_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \brief	HAL AEM13920 Driver implementation
  * 
  * \version	1.1.1
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
#include "AEM_I2C.h"
#include "HAL_AEM13920.h"
#include "AEM13920_RegisterMap.h"

#define __AEM13920_SRCREGUCVRT_0		(112UL)
#define __AEM13920_SRCREGUCVRT_0_STP		(0x06UL)
#define __AEM13920_SRCREGUCVRT_1_STP		(0x12UL)
#define __AEM13920_SRCREGUCVRT_1_A		(9000UL)
#define __AEM13920_SRCREGUCVRT_1_B		(9UL)
#define __AEM13920_SRCREGUCVRT_1_C		(750UL)
#define __AEM13920_SRCREGUCVRT_1_D		(100UL)
#define __AEM13920_SRCREGUCVRT_2_STP		(0x39UL)
#define __AEM13920_SRCREGUCVRT_2_A		(30000UL)
#define __AEM13920_SRCREGUCVRT_2_B		(37UL)
#define __AEM13920_SRCREGUCVRT_2_C		(1500UL)
#define __AEM13920_SRCREGUCVRT_2_D		(100UL)
#define __AEM13920_SRCREGUCVRT_3_STP		(0x79UL)
#define __AEM13920_SRCREGUCVRT_3_A		(30000UL)
#define __AEM13920_SRCREGUCVRT_3_B		(165UL)
#define __AEM13920_SRCREGUCVRT_3_C		(1500UL)
#define __AEM13920_SRCREGUCVRT_3_D		(67UL)
#define __AEM13920_SRCREGUCVRT_4_STP		(0xB9UL)
#define __AEM13920_SRCREGUCVRT_4_A		(30000UL)
#define __AEM13920_SRCREGUCVRT_4_B		(293UL)
#define __AEM13920_SRCREGUCVRT_4_C		(1500UL)
#define __AEM13920_SRCREGUCVRT_4_D		(33UL)
#define __AEM13920_SRCREGUCVRT_5		(4409UL)

#define __AEM13920_SRCREGUCFG_MIN		(0x0000UL)
#define __AEM13920_SRCREGUCFG_0_STRTV		(0x046AUL)
#define __AEM13920_SRCREGUCFG_0_STRTR		(0x000CUL)	
#define __AEM13920_SRCREGUCFG_0_STEP		(0x004BUL)
#define __AEM13920_SRCREGUCFG_1_STRTV		(0x0BB8UL)
#define __AEM13920_SRCREGUCFG_1_STRTR		(0x0025UL)	
#define __AEM13920_SRCREGUCFG_1_STEP		(0x0096UL)
#define __AEM13920_SRCREGUCFG_2_STRTV		(0x396CUL)
#define __AEM13920_SRCREGUCFG_2_STRTR		(0x0073UL)
#define __AEM13920_SRCREGUCFG_2_STEP		(0x0050UL)
#define __AEM13920_SRCREGUCFG_3_STRTV		(0x39BCUL)
#define __AEM13920_SRCREGUCFG_3_STRTR		(0x0074UL)
#define __AEM13920_SRCREGUCFG_3_STEP		(0x00E1UL)
#define __AEM13920_SRCREGUCFG_4_STRTV		(0x56FEUL)
#define __AEM13920_SRCREGUCFG_4_STRTR		(0x0095UL)	
#define __AEM13920_SRCREGUCFG_4_STEP		(0x01C7UL)
#define __AEM13920_SRCREGUCFG_MAXV		(0xAE06UL)
#define __AEM13920_SRCREGUCFG_MAX		(0x00C6UL)
#define __AEM13920_SRCREGUCFG_PREC		(0x000AUL)

#define __AEM13920_APM_SCALE			(0x12CUL)
#define __AEM13920_APM_V_IDX_MUL		(56UL)
#define __AEM13920_APM_L_IDX_MUL		(8UL)

#define	__AEM13920_VSTO_FACT			(0x12C0UL)
#define __AEM13920_VSTO_DIV			(0x0100UL)

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

#define __AEM13920_TEMPXTHRESH_FACT		(0x100UL)
#define __AEM13920_TEMPXTHRESH_PREC		(0x3E8UL)

typedef union _AEM13920_irqFlgs {
	uint8_t raw[2];
	AEM13920_IRQFLG bf;	
} __AEM13920_irqFlgs;

typedef union _AEM13920_irqCfg {
	uint8_t raw[2];
	AEM13920_IRQEN bf;
} __AEM13920_irqCfg;

typedef union _AEM13920_status {
	uint8_t raw[2];
	AEM13920_Status bf;
} __AEM13920_status;

typedef union _AEM13920_apmErr {
	uint8_t raw;
	AEM13920_APMERR bf;
} __AEM13920_apmErr;

static int32_t __AEM13920_ReadRegisters(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer)
{
	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if ((i2cCfg->slaveAddress != AEM13920_I2CSLAVE_ADDRESS)) {
		return AEM_I2C_ERR_SLV_ADDR;
	}

	return AEM_I2C_ReadRegisters(i2cCfg, offset, len, buffer);
}

static int32_t __AEM13920_WriteRegisters(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer)
{
	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if ((i2cCfg->slaveAddress != AEM13920_I2CSLAVE_ADDRESS)) {
		return AEM_I2C_ERR_SLV_ADDR;
	}

	return AEM_I2C_WriteRegisters(i2cCfg, offset, len, buffer);
}

static int32_t __AEM13920_GetAPMPowerMeter(	AEM13920_LDCDC Ldcdc, 		\
						uint32_t *apm, uint32_t data,	\
						uint32_t offset, uint32_t Vsrc,	\
						AEM13920_TMULT timing)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint32_t alpha = 0;
	static const uint16_t alphas[224] = {
		2718, 0, 0, 0, 0, 0, 0, 0, 1477, 5653, 11652, 0, 0, 0, 0, 0, 	\
		836, 3647, 7794, 13110, 0, 0, 0, 0, 722, 3071, 6581, 11185, 	\
		21741, 0, 0, 0, 360, 1820, 4022, 7062, 14661, 23683, 0, 0, 149, \
		1120, 2712, 4872, 10440, 17473, 32959, 0, 28, 720, 1883, 3457, 	\
		7687, 13035, 25443, 37181, 2731, 0, 0, 0, 0, 0, 0, 0, 1511, 	\
		5811, 12668, 0, 0, 0, 0, 0, 754, 3669, 8048, 13871, 0, 0, 0, 0, \
		683, 3123, 6788, 11720, 24417, 0, 0, 0, 477, 1892, 4054, 7121, 	\
		15483, 26371, 0, 0, 267, 1187, 2685, 4812, 10681, 18699, 40094, \
		0, 136, 794, 1874, 3433, 7712, 13725, 29711, 50343, 2973, 0, 0, \
		0, 0, 0, 0, 0, 1609, 6232, 13500, 0, 0, 0, 0, 0, 872, 4000, 	\
		8569, 14697, 0, 0, 0, 0, 816, 3370, 7156, 12396, 25992, 0, 0, 0,\
		546, 1995, 4276, 7542, 16442, 28005, 0, 0, 324, 1265, 2852, 	\
		5121, 11355, 19843, 42361, 0, 188, 863, 2016, 3689, 8252, 14637,\
		31633, 52666, 3140, 0, 0, 0, 0, 0, 0, 0, 1687, 6479, 13469, 0, 	\
		0, 0, 0, 0, 1062, 4066, 8588, 14709, 0, 0, 0, 0, 914, 3392, 	\
		7218, 12517, 25758, 0, 0, 0, 529, 2056, 4438, 7816, 16669, 	\
		28252, 0, 0, 325, 1340, 3018, 5389, 11686, 20234, 41871, 0, 207,\
		942, 2188, 3938, 8650, 15035, 31615, 53490
	};

	if (Ldcdc > AEM13920_LDCDC_68) {
		return AEM13920_DRIVER_ERR_LDCDC;
	}
	alpha = alphas[	(timing + (Ldcdc * __AEM13920_APM_L_IDX_MUL) 		\
			+ (((Vsrc / 1000) - (Vsrc / 4000)) 			\
			* __AEM13920_APM_V_IDX_MUL))];
	*apm = (((((uint64_t) data << offset) * alpha) 				\
		+ (__AEM13920_APM_SCALE /2)) / __AEM13920_APM_SCALE);
	return rc;
}

static void __AEM13920_ComputeSRCREGURegConst(uint32_t Vsrc, uint8_t *regs)
{
	uint8_t srcRegu = 0;
	uint32_t dvd = (Vsrc * __AEM13920_SRCREGUCFG_PREC);
	uint32_t dvr = 1;

	if (dvd < __AEM13920_SRCREGUCFG_0_STRTV) {
		srcRegu = __AEM13920_SRCREGUCFG_MIN;
	} else if (dvd >= __AEM13920_SRCREGUCFG_MAXV) {
		srcRegu = __AEM13920_SRCREGUCFG_MAX;
	} else {
		if (dvd < __AEM13920_SRCREGUCFG_1_STRTV) {
			srcRegu = __AEM13920_SRCREGUCFG_0_STRTR;
			dvr = __AEM13920_SRCREGUCFG_0_STEP;
			dvd -= __AEM13920_SRCREGUCFG_0_STRTV;
		} else if (dvd < __AEM13920_SRCREGUCFG_2_STRTV) {
			srcRegu = __AEM13920_SRCREGUCFG_1_STRTR;
			dvr = __AEM13920_SRCREGUCFG_1_STEP;
			dvd -= __AEM13920_SRCREGUCFG_1_STRTV;
		} else if (dvd < __AEM13920_SRCREGUCFG_3_STRTV) {
			srcRegu = __AEM13920_SRCREGUCFG_2_STRTR;
			dvr = __AEM13920_SRCREGUCFG_2_STEP;
			dvd -= __AEM13920_SRCREGUCFG_2_STRTV;
		} else if (dvd < __AEM13920_SRCREGUCFG_4_STRTV) {
			srcRegu = __AEM13920_SRCREGUCFG_3_STRTR;
			dvr = __AEM13920_SRCREGUCFG_3_STEP;
			dvd -= __AEM13920_SRCREGUCFG_3_STRTV;
		} else {
			srcRegu = __AEM13920_SRCREGUCFG_4_STRTR;
			dvr = __AEM13920_SRCREGUCFG_4_STEP;
			dvd -= __AEM13920_SRCREGUCFG_4_STRTV;
		}
		srcRegu += (uint8_t) ((dvd + (dvr / 2)) / dvr);
	}
	regs[0] = (	(srcRegu << AEM13920_SRC1REGU0_CFG0_Pos) 		\
			& AEM13920_SRC1REGU0_CFG0_Msk);
	regs[1] = (	(srcRegu >> 3UL) & (AEM13920_SRC1REGU1_CFG1_Msk 	\
			| AEM13920_SRC1REGU1_CFG2_Msk));
}

static uint32_t __AEM13920_ConvertSRCReg(uint8_t reg)
{
	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t c = 0;
	uint32_t d = 10;

	if (reg <= __AEM13920_SRCREGUCVRT_0_STP) {
		return __AEM13920_SRCREGUCVRT_0;
	} else if (reg <= __AEM13920_SRCREGUCVRT_1_STP) {
		a = __AEM13920_SRCREGUCVRT_1_A;
		b = __AEM13920_SRCREGUCVRT_1_B;
		c = __AEM13920_SRCREGUCVRT_1_C;
		d = __AEM13920_SRCREGUCVRT_1_D;
	} else if (reg <= __AEM13920_SRCREGUCVRT_2_STP) {
		a = __AEM13920_SRCREGUCVRT_2_A;
		b = __AEM13920_SRCREGUCVRT_2_B;
		c = __AEM13920_SRCREGUCVRT_2_C;
		d = __AEM13920_SRCREGUCVRT_2_D;
	} else if (reg <= __AEM13920_SRCREGUCVRT_3_STP) {
		a = __AEM13920_SRCREGUCVRT_3_A;
		b = __AEM13920_SRCREGUCVRT_3_B;
		c = __AEM13920_SRCREGUCVRT_3_C;
		d = __AEM13920_SRCREGUCVRT_3_D;
	} else if (reg <= __AEM13920_SRCREGUCVRT_4_STP) {
		a = __AEM13920_SRCREGUCVRT_4_A;
		b = __AEM13920_SRCREGUCVRT_4_B;
		c = __AEM13920_SRCREGUCVRT_4_C;
		d = __AEM13920_SRCREGUCVRT_4_D;
	} else {
		return __AEM13920_SRCREGUCVRT_5;
	}
	return (((a + (2 * reg - b) * c) + (d / 2)) / d);
}

static uint8_t __AEM13920_ComputeVOVDISReg(uint32_t v)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (v >= __AEM13920_VOVDIS_MAXV) {
		return __AEM13920_VOVDIS_MAXR;
	}
	if (v <= __AEM13920_VOVDIS_MINV) {
		return 0;
	}
	else {
		dvd = 	(v * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VOVDIS_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = (uint8_t)((dvd + (dvr / 2)) / dvr);
		return ((thresh << AEM13920_VOVDIS_THRESH_Pos) 			\
			& AEM13920_VOVDIS_THRESH_Msk);
	}
}

static uint32_t __AEM13920_ConvertVOVDISReg(uint8_t reg)
{
	uint32_t v = 0;
	uint32_t div = __AEM13920_VTHRESH_FACT;
	if (reg > __AEM13920_VOVDIS_MAXR) {
		return __AEM13920_VOVDIS_MAXV;
	}
	v = ((reg * __AEM13920_VTHRESH_DIV) + (div * __AEM13920_VOVDIS_MINV));
	v = ((v + (div / 2)) / div);
	return v;
}

static uint8_t __AEM13920_ComputeVCHRDYReg(uint32_t v)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (v >= __AEM13920_VCHRDY_MAXV) {
		return __AEM13920_VCHRDY_MAXR;
	}
	if (v <= __AEM13920_VCHRDY_MINV) {
		return 0;
	}
	else {
		dvd = 	(v * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VCHRDY_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = (uint8_t)((dvd + (dvr / 2)) / dvr);
		return ((thresh << AEM13920_VCHRDY_THRESH_Pos) 			\
			& AEM13920_VCHRDY_THRESH_Msk);
	}
}

static uint32_t __AEM13920_ConvertVCHRDYReg(uint8_t reg)
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

static uint8_t __AEM13920_ComputeVOVCHReg(uint32_t v)
{
	uint8_t thresh = 0;
	uint32_t dvd = 0;
	uint32_t dvr = 0;

	if (v >= __AEM13920_VOVCH_MAXV) {
		return __AEM13920_VOVCH_MAXR;
	}
	if (v <= __AEM13920_VOVCH_MINV) {
		return 0;
	}
	else {
		dvd = 	(v * __AEM13920_VTHRESH_FACT) 				\
			- (__AEM13920_VTHRESH_FACT * __AEM13920_VOVCH_MINV);
		dvr = __AEM13920_VTHRESH_DIV;
		thresh = (uint8_t)((dvd + (dvr / 2)) / dvr);
		return ((thresh << AEM13920_VOVCH_THRESH_Pos) 			\
			& AEM13920_VOVCH_THRESH_Msk);
	}
}

static uint32_t __AEM13920_ConvertVOVCHReg(uint8_t reg)
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

static uint8_t __AEM13920_ComputeTEMPXTHRESHReg(uint32_t Rth, uint32_t Rdiv)
{
	uint8_t thresh = 0;
	uint64_t dvd = ((uint64_t) Rth * __AEM13920_TEMPXTHRESH_PREC);
	uint64_t dvr = ((uint64_t) Rth + Rdiv);

	dvd = ((dvd + (dvr / 2)) / dvr);
	dvd *= __AEM13920_TEMPXTHRESH_FACT;
	dvr = __AEM13920_TEMPXTHRESH_PREC;
	dvd = ((dvd + (dvr / 2)) / dvr);
	if (dvd > UINT8_MAX) {
		thresh = UINT8_MAX;
	} else {
		thresh = (uint8_t) dvd;
	}

	return ((thresh << AEM13920_TEMPX_THRESH_Pos) & AEM13920_TEMPX_THRESH_Msk); 
}

static uint32_t __AEM13920_ConvertTEMPReg(uint8_t reg, uint32_t Rdiv)
{
	uint64_t dvd = ((uint64_t) reg * (uint64_t) Rdiv);
	uint32_t dvr = (__AEM13920_TEMPXTHRESH_FACT - reg);
	uint64_t therm = ((dvd + (dvr / 2)) / dvr);
	if (therm > UINT32_MAX) {
		return UINT32_MAX;
	} else {
		return (uint32_t) therm;
	}
}

static int32_t __AEM13920_ComputeAPMReg(const AEM13920_Config *cfg, 		\
					uint8_t *cfgReg)
{
	if (cfg->apmMode > AEM13920_APM_MODE_POWER_METER) {
		return AEM13920_DRIVER_ERR_APM_MODE;
	}
	if (cfg->apmWindow > AEM13920_APM_WINDOW_64) {
		return AEM13920_DRIVER_ERR_APM_WINDOW;
	}
	cfgReg[AEM13920_APM_OFFSET - AEM13920_CFG_OFFSET] = 			\
		((((uint8_t) cfg->enableAPMSRC1 << AEM13920_APM_SRC1EN_Pos) & 	\
		AEM13920_APM_SRC1EN_Msk) 					\
		| (((uint8_t) cfg->enableAPMSRC2 << AEM13920_APM_SRC2EN_Pos) 	\
		& AEM13920_APM_SRC2EN_Msk) 					\
		| (((uint8_t) cfg->enableAPMBuck << AEM13920_APM_BUCKEN_Pos) 	\
		& AEM13920_APM_BUCKEN_Msk) 					\
		| ((cfg->apmMode << AEM13920_APM_MODE_Pos) 			\
		& AEM13920_APM_MODE_Msk)					\
		| ((cfg->apmWindow << AEM13920_APM_WINDOW_Pos) 			\
		& AEM13920_APM_WINDOW_Msk));
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertAPMReg(uint8_t reg, AEM13920_Config *cfg)
{
	cfg->enableAPMSRC1 = (bool)(reg & AEM13920_APM_SRC1EN_Msk);
	cfg->enableAPMSRC2 = (bool)(reg & AEM13920_APM_SRC2EN_Msk);
	cfg->enableAPMBuck = (bool)(reg & AEM13920_APM_BUCKEN_Msk);
	cfg->apmMode = (AEM13920_APM_MODE)(					\
		(reg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	cfg->apmWindow = (AEM13920_APM_WINDOW) (				\
		(reg & AEM13920_APM_WINDOW_Msk) >> AEM13920_APM_WINDOW_Pos);
}

static int32_t __AEM13920_ComputeBSTCFGReg(	const AEM13920_Config *cfg, 	\
						uint8_t *cfgReg)
{
	if (cfg->boost2TimingMult > AEM13920_TMULT16) {
		return AEM13920_DRIVER_ERR_TMULT;
	}
	cfgReg[AEM13920_BST1CFG_OFFSET - AEM13920_CFG_OFFSET] = (		\
		((cfg->enableBoost1 << AEM13920_BST1CFG_EN_Pos) 		\
		& AEM13920_BST1CFG_EN_Msk) 					\
		| ((cfg->enableHPBoost1 << AEM13920_BST1CFG_HPEN_Pos) 		\
		& AEM13920_BST1CFG_HPEN_Msk) 					\
		| ((cfg->boost1TimingMult << AEM13920_BST1CFG_TMULT_Pos) 	\
		& AEM13920_BST1CFG_TMULT_Msk));
	cfgReg[AEM13920_BST2CFG_OFFSET - AEM13920_CFG_OFFSET] = (		\
		((cfg->enableBoost2 << AEM13920_BST2CFG_EN_Pos) 		\
		& AEM13920_BST2CFG_EN_Msk) 					\
		| ((cfg->enableHPBoost2 << AEM13920_BST2CFG_HPEN_Pos) 		\
		& AEM13920_BST2CFG_HPEN_Msk) 					\
		| ((cfg->boost2TimingMult << AEM13920_BST2CFG_TMULT_Pos) 	\
		& AEM13920_BST2CFG_TMULT_Msk));
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertBSTCFGReg(uint8_t bst1cfg, uint8_t bst2cfg, 	\
					AEM13920_Config *cfg)
{
	cfg->boost1TimingMult = (AEM13920_TMULT)				\
				((bst1cfg & AEM13920_BST1CFG_TMULT_Msk) 	\
				>> AEM13920_BST1CFG_TMULT_Pos);
	cfg->enableBoost1 = (	(bst1cfg & AEM13920_BST1CFG_EN_Msk) 		\
				>> AEM13920_BST1CFG_EN_Pos);
	cfg->enableHPBoost1 = (	(bst1cfg & AEM13920_BST1CFG_HPEN_Msk) 		\
				>> AEM13920_BST1CFG_HPEN_Pos);
	cfg->boost2TimingMult = (AEM13920_TMULT)				\
				((bst2cfg & AEM13920_BST2CFG_TMULT_Msk) 	\
				>> AEM13920_BST2CFG_TMULT_Pos);
	cfg->enableBoost2 = (	(bst2cfg & AEM13920_BST2CFG_EN_Msk) 		\
				>> AEM13920_BST2CFG_EN_Pos);
	cfg->enableHPBoost2 = (	(bst2cfg & AEM13920_BST2CFG_HPEN_Msk) 		\
				>> AEM13920_BST2CFG_HPEN_Pos);
}

static int32_t __AEM13920_ComputeBUCKCFGReg(	const AEM13920_Config *cfg, 	\
						uint8_t *cfgReg)
{	
	if (cfg->buckVLoad > AEM13920_VLOAD_2500) {
		return AEM13920_DRIVER_ERR_VLOAD;
	}
	if (cfg->buckTimingMult > AEM13920_TMULT16) {
		return AEM13920_DRIVER_ERR_TMULT;
	}
	cfgReg[AEM13920_BUCKCFG_OFFSET - AEM13920_CFG_OFFSET] = 		\
		(	(cfg->buckVLoad << AEM13920_BUCKCFG_VOUT_Pos) & 	\
			AEM13920_BUCKCFG_VOUT_Msk)				\
		| (	(cfg->buckTimingMult << AEM13920_BUCKCFG_TMULT_Pos) & 	\
			AEM13920_BUCKCFG_TMULT_Msk);
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertBUCKCFGReg(uint8_t reg, AEM13920_Config *cfg)
{
	cfg->buckTimingMult = 	(AEM13920_TMULT)				\
				((reg & AEM13920_BUCKCFG_TMULT_Msk) 		\
				>> AEM13920_BUCKCFG_TMULT_Pos);
	cfg->buckVLoad = 	(AEM13920_VLOAD)				\
				((reg & AEM13920_BUCKCFG_VOUT_Msk) 		\
				>> AEM13920_BUCKCFG_VOUT_Pos);
}

static void __AEM13920_ComputeIRQENReg(const AEM13920_IRQEN *irqs, uint8_t *reg)
{
	__AEM13920_irqCfg _cfg;
	_cfg.bf = *irqs;
	reg[0] = _cfg.raw[0] & AEM13920_IRQEN0_ALL_Msk;
	reg[1] = _cfg.raw[1] & AEM13920_IRQEN1_ALL_Msk;
}

static void __AEM13920_ConvertIRQENReg(	uint8_t irqCfg0, uint8_t irqCfg1, 	\
					AEM13920_IRQEN *irqs)
{
	__AEM13920_irqCfg _cfg;
	_cfg.raw[0] = irqCfg0;
	_cfg.raw[1] = irqCfg1;
	*irqs = _cfg.bf;
}

static int32_t __AEM13920_ComputeSLEEPReg(	const AEM13920_Config *cfg, 	\
						uint8_t *cfgReg)
{
	if (	(cfg->src1SleepThresh > AEM13920_SLEEP_THRESH_600)
		|| (cfg->src2SleepThresh > AEM13920_SLEEP_THRESH_600)) {
		return AEM13920_DRIVER_ERR_SLEEP_THRESH;
	}
	cfgReg[AEM13920_SLEEP_OFFSET - AEM13920_CFG_OFFSET] = 			\
		 ((cfg->src1SleepThresh << AEM13920_SLEEP_SRC1THRESH_Pos) & 	\
		AEM13920_SLEEP_SRC1THRESH_Msk)					\
		| ((cfg->src2SleepThresh << AEM13920_SLEEP_SRC2THRESH_Pos) & 	\
		AEM13920_SLEEP_SRC2THRESH_Msk);
	return AEM13920_DRIVER_OK;
}

static void __AEM13920_ConvertSLEEPReg(uint8_t reg, AEM13920_Config *cfg)
{
	cfg->src1SleepThresh = (AEM13920_SLEEP_THRESHOLD)			\
				((reg & AEM13920_SLEEP_SRC1THRESH_Msk) 		\
				>> AEM13920_SLEEP_SRC1THRESH_Pos);
	cfg->src2SleepThresh = (AEM13920_SLEEP_THRESHOLD)			\
				((reg & AEM13920_SLEEP_SRC2THRESH_Msk) 		\
				>> AEM13920_SLEEP_SRC2THRESH_Pos);
}

static void __AEM13920_ConvertSRCREGUReg(const AEM_i2c_cfg *i2cCfg, 
					uint8_t *regs, AEM13920_Config *cfg)
{
	uint8_t reg1 = regs[AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET];
	uint8_t reg2 = regs[AEM13920_SRC1REGU1_OFFSET - AEM13920_CFG_OFFSET];
	if (((reg1 & AEM13920_SRC1REGU0_MODE_Msk) >> AEM13920_SRC1REGU0_MODE_Pos)\
	 	== AEM13920_SRCREGU_CONST) {
		AEM13920_GetSourceRegulationVoltage(i2cCfg, AEM13920_SRC1, &(cfg->src1ReguV));
	} else {
		cfg->src1MpptRatio = 	(AEM13920_MPPT_RATIO)			\
					((reg1 & AEM13920_SRC1REGU0_CFG0_Msk) 	\
					>> AEM13920_SRC1REGU0_CFG0_Pos);
		cfg->src1MpptDuration = (AEM13920_MPPT_DURATION)		\
					((reg2 & AEM13920_SRC1REGU1_CFG1_Msk) 	\
					>> AEM13920_SRC1REGU1_CFG1_Pos);
		cfg->src1MpptPeriod = 	(AEM13920_MPPT_PERIOD)			\
					((reg2 & AEM13920_SRC1REGU1_CFG2_Msk) 	\
					>> AEM13920_SRC1REGU1_CFG2_Pos);
	}
	reg1 = regs[AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET];
	reg2 = regs[AEM13920_SRC2REGU1_OFFSET - AEM13920_CFG_OFFSET];
	if (((reg1 & AEM13920_SRC2REGU0_MODE_Msk) >> AEM13920_SRC2REGU0_MODE_Pos)\
		== AEM13920_SRCREGU_CONST) {
		AEM13920_GetSourceRegulationVoltage(i2cCfg, AEM13920_SRC2, &(cfg->src1ReguV));
	} else {
		cfg->src2MpptRatio = 	(AEM13920_MPPT_RATIO)			\
					((reg1 & AEM13920_SRC2REGU0_CFG0_Msk) 	\
					>> AEM13920_SRC2REGU0_CFG0_Pos);
		cfg->src2MpptDuration = (AEM13920_MPPT_DURATION)		\
					((reg2 & AEM13920_SRC2REGU1_CFG1_Msk) 	\
					>> AEM13920_SRC2REGU1_CFG1_Pos);
		cfg->src2MpptPeriod = 	(AEM13920_MPPT_PERIOD)			\
					((reg2 & AEM13920_SRC2REGU1_CFG2_Msk) 	\
					>> AEM13920_SRC2REGU1_CFG2_Pos);
	}
}

static int32_t __AEM13920_ComputeSRCREGUReg(	const AEM13920_Config *cfg, 	\
						uint8_t *cfgReg)
{
	uint8_t *reg1 = &(cfgReg[AEM13920_SRC1REGU0_OFFSET - AEM13920_CFG_OFFSET]);
	uint8_t *reg2 = &(cfgReg[AEM13920_SRC1REGU1_OFFSET - AEM13920_CFG_OFFSET]);
	if (cfg->src1ReguMode == AEM13920_SRCREGU_CONST) {
		__AEM13920_ComputeSRCREGURegConst(cfg->src1ReguV, reg1);
	} else if (cfg->src1ReguMode == AEM13920_SRCREGU_MPPT) {
		if (cfg->src1MpptRatio > AEM13920_MPPT_RATIO_ZMPP) {
			return AEM13920_DRIVER_ERR_MPPT_RATIO;
		}
		if (cfg->src1MpptDuration > AEM13920_MPPT_DUR512) {
			return AEM13920_DRIVER_ERR_MPPT_DURATION;
		}
		if (cfg->src1MpptPeriod > AEM13920_MPPT_PER16384) {
			return AEM13920_DRIVER_ERR_MPPT_PERIOD;
		}
		*reg1 = (AEM13920_SRC1REGU0_MODE_Msk 				\
			| ((cfg->src1MpptRatio << AEM13920_SRC1REGU0_CFG0_Pos) 	\
			& AEM13920_SRC1REGU0_CFG0_Msk));
		*reg2 = (((cfg->src1MpptDuration << AEM13920_SRC1REGU1_CFG1_Pos)\
			& AEM13920_SRC1REGU1_CFG1_Msk) 				\
			| ((cfg->src1MpptPeriod << AEM13920_SRC1REGU1_CFG2_Pos) \
			& AEM13920_SRC1REGU1_CFG2_Msk));
	} else {
		return AEM13920_DRIVER_ERR_SRCREGU_MODE;
	}
	reg1 = &(cfgReg[AEM13920_SRC2REGU0_OFFSET - AEM13920_CFG_OFFSET]);
	reg2 = &(cfgReg[AEM13920_SRC2REGU1_OFFSET - AEM13920_CFG_OFFSET]);
	if (cfg->src2ReguMode == AEM13920_SRCREGU_CONST) {
		__AEM13920_ComputeSRCREGURegConst(cfg->src2ReguV, reg1);
	} else if (cfg->src2ReguMode == AEM13920_SRCREGU_MPPT) {
		if (cfg->src2MpptRatio > AEM13920_MPPT_RATIO_ZMPP) {
			return AEM13920_DRIVER_ERR_MPPT_RATIO;
		}
		if (cfg->src2MpptDuration > AEM13920_MPPT_DUR512) {
			return AEM13920_DRIVER_ERR_MPPT_DURATION;
		}
		if (cfg->src2MpptPeriod > AEM13920_MPPT_PER16384) {
			return AEM13920_DRIVER_ERR_MPPT_PERIOD;
		}
		*reg1 = (AEM13920_SRC2REGU0_MODE_Msk 				\
			| ((cfg->src2MpptRatio << AEM13920_SRC2REGU0_CFG0_Pos) 	\
			& AEM13920_SRC2REGU0_CFG0_Msk));
		*reg2 = (((cfg->src2MpptDuration << AEM13920_SRC2REGU1_CFG1_Pos)\
			& AEM13920_SRC2REGU1_CFG1_Msk) 				\
			| ((cfg->src2MpptPeriod << AEM13920_SRC2REGU1_CFG2_Pos) \
			& AEM13920_SRC2REGU1_CFG2_Msk));
	} else {
		return AEM13920_DRIVER_ERR_SRCREGU_MODE;
	}
	return AEM13920_DRIVER_OK;
}

int32_t AEM13920_Initialize(const AEM_i2c_cfg *i2cCfg) {
	int32_t rc = AEM13920_DRIVER_OK;

	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}

	rc = AEM_I2C_Initialize(i2cCfg);
	
	return rc;
}

int32_t AEM13920_Deinitialize(const AEM_i2c_cfg *i2cCfg)
{
	int32_t rc = AEM13920_DRIVER_OK;

	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}

	rc = AEM_I2C_Deinitialize(i2cCfg);
	
	return rc;
}

int32_t AEM13920_GetChipVersion(const AEM_i2c_cfg *i2cCfg, uint8_t *version)
{
	if (!version) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	return __AEM13920_ReadRegisters(i2cCfg, AEM13920_VERSION_OFFSET, 1, version);
}

int32_t AEM13920_GetConfigurationSource(const AEM_i2c_cfg *i2cCfg, uint8_t *source)
{
	uint8_t data = 0;
	int32_t rc = AEM13920_DRIVER_OK;
	
	if (!source) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_CTRL_OFFSET, 1, &data);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*source = (data & AEM13920_CTRL_UPDATE_Msk) >> AEM13920_CTRL_UPDATE_Pos;

	return rc;
}

int32_t AEM13920_GetStatus(const AEM_i2c_cfg *i2cCfg, AEM13920_Status *status)
{
	int32_t rc = AEM13920_DRIVER_OK;
	__AEM13920_status _status;
	
	if (!status) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_STATUS0_OFFSET, 2, _status.raw);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	_status.raw[0] &= AEM13920_STATUS0_ALL_Msk;
	_status.raw[1] &= AEM13920_STATUS1_ALL_Msk;
	*status = _status.bf;

	return rc;
}

int32_t AEM13920_GetSynchronizationStatus(const AEM_i2c_cfg *i2cCfg, bool *sync)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_CTRL_OFFSET, 1, &reg);
	*sync = (bool) (reg & AEM13920_CTRL_SYNCBUSY_Msk);
	
	return rc;	
}

int32_t AEM13920_GetIRQFlags(const AEM_i2c_cfg *i2cCfg, AEM13920_IRQFLG *flags)
{
	int32_t rc = AEM13920_DRIVER_OK;
	__AEM13920_irqFlgs _flags;

	if (!flags) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_IRQFLG0_OFFSET, 2, _flags.raw);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	_flags.raw[0] &= AEM13920_IRQFLG0_ALL_Msk;
	_flags.raw[1] &= AEM13920_IRQFLG1_ALL_Msk;
	*flags = _flags.bf;
	
	return rc;
}

int32_t AEM13920_GetAPMSource(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				uint32_t *apm, AEM13920_LDCDC Ldcdc)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t apmMode = 0;
	uint8_t apmCfg = 0;
	AEM13920_TMULT timing = AEM13920_TMULT1;
	uint32_t Vsrc = 0;
	uint32_t data = 0;
	uint32_t offset = 0;

	if (!apm) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = AEM13920_GetAPMDataSRC(i2cCfg, src, &data, &offset);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &apmCfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (!(apmCfg & (AEM13920_APM_SRC1EN_Msk << src))) {
		return AEM13920_DRIVER_ERR_APM_DISABLED;
	}
	apmMode = ((apmCfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	if (apmMode == AEM13920_APM_MODE_PULSE_COUNTER) {
		*apm = data;
		return rc;
	}
	rc = AEM13920_GetSourceRegulationVoltage(i2cCfg, src, &Vsrc);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = AEM13920_GetBoostTiming(i2cCfg, src, &timing);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	return __AEM13920_GetAPMPowerMeter(Ldcdc, apm, data, offset, Vsrc, timing);
}

int32_t AEM13920_GetAPMDataSRC(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				uint32_t *data, uint32_t *offset)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint32_t idx = AEM13920_APM0SRC1_OFFSET + (3 * src);
	uint8_t apmData[3] = {0};
	uint8_t apmMode = AEM13920_APM_MODE_PULSE_COUNTER;
	uint8_t apmCfg = 0;

	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if (!data) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (!offset) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, idx, 3, apmData);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &apmCfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (!(apmCfg & (AEM13920_APM_SRC1EN_Msk << src))) {
		return AEM13920_DRIVER_ERR_APM_DISABLED;
	}
	apmMode = ((apmCfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	if (apmMode == AEM13920_APM_MODE_PULSE_COUNTER) {
		*data = (uint32_t)((apmData[2] << 16) + (apmData[1] << 8) + apmData[0]);
		return rc;
	} else {
		*data = (uint32_t)(						\
			((apmData[2] & AEM13920_APM2SRC2_PM_DATA_Msk) << 16)	\
			+ (apmData[1] << 8) + apmData[0]);
		*offset = (uint32_t)(						\
			(apmData[2] & AEM13920_APM2SRC2_PM_SHIFT_Msk) 		\
			>> AEM13920_APM2SRC2_PM_SHIFT_Pos);
		return rc;
	}
}

int32_t AEM13920_GetAPMBuck(const AEM_i2c_cfg *i2cCfg, uint32_t *apm, AEM13920_LDCDC Ldcdc)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t apmMode = 0;
	uint8_t apmCfg = 0;
	AEM13920_TMULT timing = AEM13920_TMULT1;
	AEM13920_VLOAD Vload = AEM13920_VLOAD_OFF;
	uint32_t Vsrc = 0;
	uint32_t data = 0;
	uint32_t offset = 0;

	if (!apm) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = AEM13920_GetAPMDataBuck(i2cCfg, &data, &offset);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &apmCfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	apmMode = ((apmCfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	if (apmMode == AEM13920_APM_MODE_PULSE_COUNTER) {
		*apm = data;
		return rc;
	}
	rc = AEM13920_GetBuckVLoad(i2cCfg, &Vload);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	Vsrc = (300 + (Vload * 300));
	rc = AEM13920_GetBuckTiming(i2cCfg, &timing);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	return __AEM13920_GetAPMPowerMeter(Ldcdc, apm, data, offset, Vsrc, timing);
}

int32_t AEM13920_GetAPMDataBuck(const AEM_i2c_cfg *i2cCfg, uint32_t *data,	\
				uint32_t *offset)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t apmData[3] = {0};
	uint8_t apmMode = AEM13920_APM_MODE_PULSE_COUNTER;
	uint8_t apmCfg = 0;
	AEM13920_VLOAD Vload = AEM13920_VLOAD_OFF;

	if (!i2cCfg) {
		return AEM_I2C_ERR_MISSING_CFG;
	}
	if (!data) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (!offset) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM0BUCK_OFFSET, 3, apmData);
	if (rc != AEM13920_DRIVER_OK) { 
		return rc;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &apmCfg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (!(apmCfg & AEM13920_APM_BUCKEN_Msk)) {
		return AEM13920_DRIVER_ERR_APM_DISABLED;
	}
	apmMode = ((apmCfg & AEM13920_APM_MODE_Msk) >> AEM13920_APM_MODE_Pos);
	if (apmMode == AEM13920_APM_MODE_PULSE_COUNTER) {
		*data = (uint32_t)((apmData[2] << 16) + (apmData[1] << 8) + apmData[0]);
		return rc;
	}
	rc = AEM13920_GetBuckVLoad(i2cCfg, &Vload);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (Vload == AEM13920_VLOAD_OFF) {
		return AEM13920_DRIVER_ERR_BUCK_DISABLED;
	}
	*data = (uint32_t)(							\
		((apmData[2] & AEM13920_APM2BUCK_PM_DATA_Msk) << 16)		\
		+ (apmData[1] << 8) + apmData[0]);
	*offset = (uint32_t)(							\
		(apmData[2] & AEM13920_APM2BUCK_PM_SHIFT_Msk) 			\
		>> AEM13920_APM2BUCK_PM_SHIFT_Pos);
	return rc;
}

int32_t AEM13920_GetAPMError(const AEM_i2c_cfg *i2cCfg, AEM13920_APMERR *apmErr)
{
	int32_t rc = AEM13920_DRIVER_OK;
	__AEM13920_apmErr _apmErr;
	
	if (!apmErr) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	
	rc = __AEM13920_ReadRegisters(	i2cCfg, AEM13920_APMERR_OFFSET, 2, 	\
					&(_apmErr.raw));
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	_apmErr.raw &= AEM13920_APMERR_ALL_Msk;
	*apmErr = _apmErr.bf;

	return rc;
}

int32_t AEM13920_GetThermistorZ(const AEM_i2c_cfg *i2cCfg, uint32_t *Rth, 	\
				uint32_t Rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t tempData = 0;

	if (!Rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TEMP_OFFSET, 1, &tempData);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Rth = __AEM13920_ConvertTEMPReg(tempData, Rdiv);

	return rc;
}

int32_t AEM13920_GetStorageVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vsto)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t stoData = 0;

	if (!Vsto) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_STO_OFFSET, 1, &stoData);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	*Vsto = (((__AEM13920_VSTO_FACT * stoData) + (__AEM13920_VSTO_DIV / 2)) \
		/ __AEM13920_VSTO_DIV);

	return rc;
}

int32_t AEM13920_GetSourceRegulationVoltage(	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						uint32_t *Vsrc)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t VsrcReg;

	if (!Vsrc) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_SRC1_OFFSET + src, 1, &VsrcReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Vsrc = __AEM13920_ConvertSRCReg(VsrcReg);

	return rc;	
}

int32_t AEM13920_SetSourceRegulationMode( 	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						AEM13920_SRCREGU_MODE mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src * 2);

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (mode > AEM13920_SRCREGU_MPPT) {
		return AEM13920_DRIVER_ERR_SRCREGU;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_SRC1REGU0_MODE_Msk;
	reg |= ((mode << AEM13920_SRC1REGU0_MODE_Pos) & AEM13920_SRC1REGU0_MODE_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_SetMPPTRatio(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_RATIO ratio)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src * 2);

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (ratio > AEM13920_MPPT_RATIO_ZMPP) {
		return AEM13920_DRIVER_ERR_MPPT_RATIO;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_SRC1REGU0_CFG0_Msk;
	reg |= ((ratio << AEM13920_SRC1REGU0_CFG0_Pos) & AEM13920_SRC1REGU0_CFG0_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_SetMPPTTimings(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_DURATION duration,		\
				AEM13920_MPPT_PERIOD period)
{
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (src * 2);

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (duration > AEM13920_MPPT_DUR512) {
		return AEM13920_DRIVER_ERR_MPPT_DURATION;
	}
	if (period > AEM13920_MPPT_PER16384) {
		return AEM13920_DRIVER_ERR_MPPT_PERIOD;
	}

	reg = (	(duration << AEM13920_SRC1REGU1_CFG1_Pos) 			\
		& AEM13920_SRC1REGU1_CFG1_Msk) 					\
		| ((period << AEM13920_SRC1REGU1_CFG2_Pos) 			\
		& AEM13920_SRC1REGU1_CFG2_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);	
}

int32_t AEM13920_SetSourceRegulationVoltage(	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						uint32_t Vsrc)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t mode = 0;
	uint8_t regulV[2] = {0};
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (src * 2);

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &mode);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (((mode & AEM13920_SRC1REGU0_MODE_Msk) >> AEM13920_SRC2REGU0_MODE_Pos) \
		== AEM13920_SRCREGU_MPPT) {
		return AEM13920_DRIVER_ERR_SRCREGU;
	}

	__AEM13920_ComputeSRCREGURegConst(Vsrc, regulV);

	return __AEM13920_WriteRegisters(i2cCfg, offset, 2, regulV);
}

int32_t AEM13920_SetDischargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vovdis)
{
	uint8_t reg = __AEM13920_ComputeVOVDISReg(Vovdis);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_VOVDIS_OFFSET, 1, &reg);
}

int32_t AEM13920_SetChargeReadyVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vchrdy)
{
	uint8_t reg = __AEM13920_ComputeVCHRDYReg(Vchrdy);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_VCHRDY_OFFSET, 1, &reg);
}

int32_t AEM13920_SetOverchargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t Vovch)
{
	uint8_t reg = __AEM13920_ComputeVOVCHReg(Vovch);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_VOVCH_OFFSET, 1, &reg);
}

int32_t AEM13920_EnableBoost(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_BST1CFG_EN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_DisableBoost(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BST1CFG_EN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_SetBoostTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_TMULT tMult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (tMult > AEM13920_TMULT16) {
		return AEM13920_DRIVER_ERR_TMULT;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BST1CFG_TMULT_Msk;
	reg |= ((tMult << AEM13920_BST1CFG_TMULT_Pos) & AEM13920_BST1CFG_TMULT_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_EnableHighPower(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}	
	reg |= AEM13920_BST1CFG_HPEN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_DisableHighPower(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BST1CFG_HPEN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, offset, 1, &reg);
}

int32_t AEM13920_SetBuckVLoad(const AEM_i2c_cfg *i2cCfg, AEM13920_VLOAD vLoad)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (vLoad > AEM13920_VLOAD_2500) {
		return AEM13920_DRIVER_ERR_VLOAD;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BUCKCFG_VOUT_Msk;
	reg |= ((vLoad << AEM13920_BUCKCFG_VOUT_Pos) & AEM13920_BUCKCFG_VOUT_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
}

int32_t AEM13920_SetBuckTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_TMULT tMult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (tMult > AEM13920_TMULT16) {
		return AEM13920_DRIVER_ERR_TMULT;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_BUCKCFG_TMULT_Msk;
	reg |= ((tMult << AEM13920_BUCKCFG_TMULT_Pos) & AEM13920_BUCKCFG_TMULT_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
}

int32_t AEM13920_SetChargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv)
{
	uint8_t reg = __AEM13920_ComputeTEMPXTHRESHReg(Rth, Rdiv);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TEMPCOLDCH_OFFSET,	\
					 1, &reg);
}

int32_t AEM13920_SetDischargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv)
{
	uint8_t reg = __AEM13920_ComputeTEMPXTHRESHReg(Rth, Rdiv);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TEMPCOLDDIS_OFFSET, 	\
					 1, &reg);
}

int32_t AEM13920_SetChargeHotThresholdZ(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t Rth, uint32_t Rdiv)
{
	uint8_t reg = __AEM13920_ComputeTEMPXTHRESHReg(Rth, Rdiv);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TEMPHOTCH_OFFSET, 	\
					 1, &reg);
}

int32_t AEM13920_SetDischargeHotThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t Rth, uint32_t Rdiv)
{
	uint8_t reg = __AEM13920_ComputeTEMPXTHRESHReg(Rth, Rdiv);
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TEMPHOTDIS_OFFSET, 	\
					 1, &reg);
}

int32_t AEM13920_EnableTemperatureMonitoring(const AEM_i2c_cfg *i2cCfg)
{
	uint8_t reg = AEM13920_TMON_EN_Msk;
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TMON_OFFSET, 1, &reg);
}

int32_t AEM13920_DisableTemperatureMonitoring(const AEM_i2c_cfg *i2cCfg)
{
	uint8_t reg = 0;
	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_TMON_OFFSET, 1, &reg);
}

int32_t AEM13920_SetSleepThreshold(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, 			\
					AEM13920_SLEEP_THRESHOLD thresh)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (thresh > AEM13920_SLEEP_THRESH_600) {
		return AEM13920_DRIVER_ERR_SLEEP_THRESH;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_SLEEP_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	if (src == AEM13920_SRC1) {
		reg &= ~AEM13920_SLEEP_SRC1THRESH_Msk;
		reg |= ((thresh << AEM13920_SLEEP_SRC1THRESH_Pos) 		\
			& AEM13920_SLEEP_SRC1THRESH_Msk);
	} else {
		reg &= ~AEM13920_SLEEP_SRC2THRESH_Msk;
		reg |= ((thresh << AEM13920_SLEEP_SRC2THRESH_Pos) 		\
			& AEM13920_SLEEP_SRC2THRESH_Msk);
	}

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_SLEEP_OFFSET, 1, &reg);
}

int32_t AEM13920_SetAPMMode(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_MODE mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (mode > AEM13920_APM_MODE_POWER_METER) {
		return AEM13920_DRIVER_ERR_APM_MODE;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_MODE_Msk;
	reg |= ((mode << AEM13920_APM_MODE_Pos) & AEM13920_APM_MODE_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_SetAPMWindow(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_WINDOW window)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (window > AEM13920_APM_WINDOW_64) {
		return AEM13920_DRIVER_ERR_APM_WINDOW;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_WINDOW_Msk;
	reg |= ((window << AEM13920_APM_WINDOW_Pos) & AEM13920_APM_WINDOW_Msk);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_EnableAPMSource(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= (AEM13920_APM_SRC1EN_Msk << src);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_DisableAPMSource(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~(AEM13920_APM_SRC1EN_Msk << src);

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_EnableAPMBuck(const AEM_i2c_cfg *i2cCfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg |= AEM13920_APM_BUCKEN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_DisableAPMBuck(const AEM_i2c_cfg *i2cCfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	reg &= ~AEM13920_APM_BUCKEN_Msk;

	return __AEM13920_WriteRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
}

int32_t AEM13920_SetIRQConfiguration(	const AEM_i2c_cfg *i2cCfg, 		\
					const AEM13920_IRQEN *irqs)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg[2] = {0};

	if (!irqs) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	__AEM13920_ComputeIRQENReg(irqs, reg);
	rc = __AEM13920_WriteRegisters(i2cCfg, AEM13920_IRQEN0_OFFSET, 2, reg);

	return rc;
}

int32_t AEM13920_LoadConfiguration(const AEM_i2c_cfg *i2cCfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = AEM13920_CTRL_UPDATE_Msk;
	bool syncing = true;

	rc = __AEM13920_WriteRegisters(i2cCfg, AEM13920_CTRL_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	while (syncing) {
		rc = AEM13920_GetSynchronizationStatus(i2cCfg, &syncing);
		if (rc != AEM13920_DRIVER_OK) {
			return rc;
		}
	}

	return rc;
}

int32_t AEM13920_GetSourceRegulationMode( 	const AEM_i2c_cfg *i2cCfg, 	\
						AEM13920_Source src,		\
						AEM13920_SRCREGU_MODE *mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (2 * src);

	if (!mode) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*mode = (AEM13920_SRCREGU_MODE) ((reg & AEM13920_SRC1REGU0_MODE_Msk) 	\
					>> AEM13920_SRC1REGU0_MODE_Pos);

	return rc;
}

int32_t AEM13920_GetMPPTRatio(	const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_MPPT_RATIO *ratio)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU0_OFFSET + (2 * src);

	if (!ratio) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*ratio = (AEM13920_MPPT_RATIO) ((reg & AEM13920_SRC1REGU0_CFG0_Msk) 	\
					>> AEM13920_SRC1REGU0_CFG0_Pos);

	return rc;
}

int32_t AEM13920_GetMPPTTimings(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				AEM13920_MPPT_DURATION *duration,		\
				AEM13920_MPPT_PERIOD *period)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_SRC1REGU1_OFFSET + (2 * src);

	if (!duration || !period) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*duration = (AEM13920_MPPT_DURATION) ((reg & AEM13920_SRC1REGU1_CFG1_Msk)\
					      >> AEM13920_SRC1REGU1_CFG1_Pos);
	*period = (AEM13920_MPPT_PERIOD) ((reg & AEM13920_SRC1REGU1_CFG2_Msk)	\
					  >> AEM13920_SRC1REGU1_CFG2_Pos);
	return rc;
}

int32_t AEM13920_GetDischargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vovdis)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Vovdis) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_VOVDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Vovdis = __AEM13920_ConvertVOVDISReg(reg);

	return rc;
}

int32_t AEM13920_GetChargeReadyVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vchrdy)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Vchrdy) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_VCHRDY_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Vchrdy = __AEM13920_ConvertVCHRDYReg(reg);

	return rc;
}

int32_t AEM13920_GetOverchargeVoltage(const AEM_i2c_cfg *i2cCfg, uint32_t *Vovch)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Vovch) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_VOVCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Vovch = __AEM13920_ConvertVOVCHReg(reg);

	return rc;
}

int32_t AEM13920_GetBoostEnable(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src,	\
				bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (reg & AEM13920_BST1CFG_EN_Msk);

	return rc;
}

int32_t AEM13920_GetBoostTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_Source src, \
				AEM13920_TMULT *tMult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (!tMult) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*tMult = (AEM13920_TMULT) (	(reg & AEM13920_BST1CFG_TMULT_Msk) 	\
					>> AEM13920_BST1CFG_TMULT_Pos);

	return rc;
}

int32_t AEM13920_GetHighPowerEnable(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;
	uint32_t offset = AEM13920_BST1CFG_OFFSET + src;

	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}
	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, offset, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_BST1CFG_HPEN_Msk);

	return rc;
}

int32_t AEM13920_GetBuckVLoad(const AEM_i2c_cfg *i2cCfg, AEM13920_VLOAD *vLoad)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!vLoad) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*vLoad = (AEM13920_VLOAD) (	(reg & AEM13920_BUCKCFG_VOUT_Msk) 	\
					>> AEM13920_BUCKCFG_VOUT_Pos);

	return rc;
}

int32_t AEM13920_GetBuckTiming(const AEM_i2c_cfg *i2cCfg, AEM13920_TMULT *tMult)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!tMult) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_BUCKCFG_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*tMult = (AEM13920_TMULT) (	(reg & AEM13920_BUCKCFG_TMULT_Msk) 	\
					>> AEM13920_BUCKCFG_TMULT_Pos);

	return rc;
}

int32_t AEM13920_GetChargeColdThresholdZ(const AEM_i2c_cfg *i2cCfg, 		\
					 uint32_t *Rth, uint32_t Rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TEMPCOLDCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Rth = __AEM13920_ConvertTEMPReg(reg, Rdiv);

	return rc;
}

int32_t AEM13920_GetChargeHotThresholdZ(const AEM_i2c_cfg *i2cCfg, 		\
					uint32_t *Rth, uint32_t Rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TEMPHOTCH_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Rth = __AEM13920_ConvertTEMPReg(reg, Rdiv);

	return rc;
}

int32_t AEM13920_GetDischargeColdThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t *Rth, uint32_t Rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TEMPCOLDDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Rth = __AEM13920_ConvertTEMPReg(reg, Rdiv);

	return rc;
}

int32_t AEM13920_GetDischargeHotThresholdZ(	const AEM_i2c_cfg *i2cCfg, 	\
						uint32_t *Rth, uint32_t Rdiv)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!Rth) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TEMPHOTDIS_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*Rth = __AEM13920_ConvertTEMPReg(reg, Rdiv);

	return rc;
}

int32_t AEM13920_GetTempMonEnable(const AEM_i2c_cfg *i2cCfg, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_TMON_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_TMON_EN_Msk);

	return rc;
}

int32_t AEM13920_GetAPMMode(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_MODE *mode)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!mode) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*mode = (AEM13920_APM_MODE) (	(reg & AEM13920_APM_MODE_Msk) 		\
					>> AEM13920_APM_MODE_Pos);

	return rc;
}

int32_t AEM13920_GetAPMWindow(const AEM_i2c_cfg *i2cCfg, AEM13920_APM_WINDOW *window)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!window) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*window = (AEM13920_APM_WINDOW) ((reg & AEM13920_APM_WINDOW_Msk) 	\
					>> AEM13920_APM_WINDOW_Pos);

	return rc;
}

int32_t AEM13920_GetAPMSourceEnable(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_Source src, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}
	if (src > AEM13920_SRC2) {
		return AEM13920_DRIVER_ERR_SRC;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & (AEM13920_APM_SRC1EN_Msk << src));

	return rc;
}

int32_t AEM13920_GetAPMBuckEnable(const AEM_i2c_cfg *i2cCfg, bool *en)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg = 0;

	if (!en) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_APM_OFFSET, 1, &reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	*en = (bool) (reg & AEM13920_APM_BUCKEN_Msk);

	return rc;
}

int32_t AEM13920_GetIRQConfiguration(	const AEM_i2c_cfg *i2cCfg, 		\
					AEM13920_IRQEN *irqs)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t reg[2] = {0};

	if (!irqs) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(i2cCfg, AEM13920_IRQEN0_OFFSET, 2, reg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	__AEM13920_ConvertIRQENReg(reg[0], reg[1], irqs);

	return rc;
}

int32_t AEM13920_Configure(const AEM_i2c_cfg *i2cCfg, const AEM13920_Config *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t cfgReg[AEM13920_CFG_LENGTH] = {0};
	bool syncing = true;

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ComputeSRCREGUReg(cfg, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeBSTCFGReg(cfg, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeBUCKCFGReg(cfg, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeSLEEPReg(cfg, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	rc = __AEM13920_ComputeAPMReg(cfg, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}
	__AEM13920_ComputeIRQENReg(	
		&(cfg->irq), &(cfgReg[AEM13920_IRQEN0_OFFSET - AEM13920_CFG_OFFSET]));
	cfgReg[AEM13920_VOVDIS_OFFSET - AEM13920_CFG_OFFSET] = 			\
		__AEM13920_ComputeVOVDISReg(cfg->dischargeThresh);
	cfgReg[AEM13920_VCHRDY_OFFSET - AEM13920_CFG_OFFSET] = 			\
		__AEM13920_ComputeVCHRDYReg(cfg->chargeReadyThresh);
	cfgReg[AEM13920_VOVCH_OFFSET - AEM13920_CFG_OFFSET] =			\
		__AEM13920_ComputeVOVCHReg(cfg->overchargeThresh);
	cfgReg[AEM13920_TEMPCOLDCH_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_ComputeTEMPXTHRESHReg(cfg->coldChThreshZ, cfg->Rdiv);
	cfgReg[AEM13920_TEMPCOLDDIS_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_ComputeTEMPXTHRESHReg(cfg->coldDisThreshZ, cfg->Rdiv);
	cfgReg[AEM13920_TEMPHOTCH_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_ComputeTEMPXTHRESHReg(cfg->hotChThreshZ, cfg->Rdiv);
	cfgReg[AEM13920_TEMPHOTDIS_OFFSET - AEM13920_CFG_OFFSET] = 		\
		__AEM13920_ComputeTEMPXTHRESHReg(cfg->hotDisThreshZ, cfg->Rdiv);
	cfgReg[AEM13920_TMON_OFFSET - AEM13920_CFG_OFFSET] = (uint8_t)		\
		(cfg->enableTempMonitoring << AEM13920_TMON_EN_Pos);
	cfgReg[AEM13920_CTRL_OFFSET - AEM13920_CFG_OFFSET] = 			\
		AEM13920_CTRL_UPDATE_Msk;

	rc = __AEM13920_WriteRegisters(	i2cCfg, AEM13920_CFG_OFFSET, 		\
					AEM13920_CFG_LENGTH, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	while (syncing) {
		rc = AEM13920_GetSynchronizationStatus(i2cCfg, &syncing);
		if (rc != AEM13920_DRIVER_OK) {
			return rc;
		}
	}

	return rc;
}

int32_t AEM13920_GetConfiguration(const AEM_i2c_cfg *i2cCfg, AEM13920_Config *cfg)
{
	int32_t rc = AEM13920_DRIVER_OK;
	uint8_t cfgReg[AEM13920_CFG_LENGTH] = {0};

	if (!cfg) {
		return AEM13920_DRIVER_ERR_PARAMETER;
	}

	rc = __AEM13920_ReadRegisters(	i2cCfg, AEM13920_CFG_OFFSET, 		\
					AEM13920_CFG_LENGTH, cfgReg);
	if (rc != AEM13920_DRIVER_OK) {
		return rc;
	}

	__AEM13920_ConvertSRCREGUReg(i2cCfg, cfgReg, cfg);
	cfg->dischargeThresh =	__AEM13920_ConvertVOVDISReg(			\
		cfgReg[AEM13920_VOVDIS_OFFSET - AEM13920_CFG_OFFSET]);
	cfg->chargeReadyThresh = __AEM13920_ConvertVCHRDYReg(			\
		cfgReg[AEM13920_VCHRDY_OFFSET - AEM13920_CFG_OFFSET]);
	cfg->overchargeThresh =	__AEM13920_ConvertVOVCHReg(			\
		cfgReg[AEM13920_VOVCH_OFFSET - AEM13920_CFG_OFFSET]);
	__AEM13920_ConvertBSTCFGReg(						\
		cfgReg[AEM13920_BST1CFG_OFFSET - AEM13920_CFG_OFFSET], 		\
		cfgReg[AEM13920_BST2CFG_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertBUCKCFGReg(						\
		cfgReg[AEM13920_BUCKCFG_OFFSET - AEM13920_CFG_OFFSET], cfg);
	cfg->coldChThreshZ = __AEM13920_ConvertTEMPReg(				\
		cfgReg[AEM13920_TEMPCOLDCH_OFFSET - AEM13920_CFG_OFFSET], cfg->Rdiv);
	cfg->hotChThreshZ = __AEM13920_ConvertTEMPReg(				\
		cfgReg[AEM13920_TEMPHOTCH_OFFSET - AEM13920_CFG_OFFSET], cfg->Rdiv);
	cfg->coldDisThreshZ = __AEM13920_ConvertTEMPReg(			\
		cfgReg[AEM13920_TEMPCOLDDIS_OFFSET - AEM13920_CFG_OFFSET], cfg->Rdiv);
	cfg->hotDisThreshZ = __AEM13920_ConvertTEMPReg(				\
		cfgReg[AEM13920_TEMPHOTDIS_OFFSET - AEM13920_CFG_OFFSET],cfg->Rdiv);
	cfg->enableTempMonitoring = 						\
		cfgReg[AEM13920_TMON_OFFSET - AEM13920_CFG_OFFSET];
	__AEM13920_ConvertSLEEPReg(						\
		cfgReg[AEM13920_SLEEP_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertAPMReg(						\
		cfgReg[AEM13920_APM_OFFSET - AEM13920_CFG_OFFSET], cfg);
	__AEM13920_ConvertIRQENReg(						\
		cfgReg[AEM13920_IRQEN0_OFFSET - AEM13920_CFG_OFFSET], 		\
		cfgReg[AEM13920_IRQEN1_OFFSET - AEM13920_CFG_OFFSET], &(cfg->irq));

	return rc;
}
