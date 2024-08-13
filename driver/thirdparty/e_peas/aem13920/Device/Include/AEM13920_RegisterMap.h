/**
  ******************************************************************************
  * \file	AEM13920_RegisterMap.h
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM13920_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \brief	Register Map's Documentation
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
#ifndef AEM13920_REGISTERMAP_H_
#define AEM13920_REGISTERMAP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define AEM13920_VERSION_OFFSET			(0x00)	/**< VERSION register offset */
#define AEM13920_VERSION_MINOR_Msk		(0x0F)	/**< MINOR field mask */
#define AEM13920_VERSION_MINOR_Pos		(0x00)	/**< MINOR field offset */
#define AEM13920_VERSION_MAJOR_Msk		(0xF0)	/**< MAJOR field mask */
#define AEM13920_VERSION_MAJOR_Pos		(0x04)	/**< MAJOR field offset */

#define AEM13920_SRC1REGU0_OFFSET		(0x01)	/**< SRC1REGU0 register offset */
#define AEM13920_SRC1REGU0_MODE_Msk		(0x01)	/**< MODE field mask */
#define AEM13920_SRC1REGU0_MODE_Pos		(0x00)	/**< MODE field offset */
#define AEM13920_SRC1REGU0_CFG0_Msk		(0x0E)	/**< CFG0 field mask */
#define AEM13920_SRC1REGU0_CFG0_Pos		(0x01)	/**< CFG0 field offset */

#define AEM13920_SRC1REGU1_OFFSET		(0x02)	/**< SRC1REGU1 register offset */
#define AEM13920_SRC1REGU1_CFG1_Msk		(0x07)	/**< CFG1 field mask */
#define AEM13920_SRC1REGU1_CFG1_Pos		(0x00)	/**< CFG1 field offset */
#define AEM13920_SRC1REGU1_CFG2_Msk		(0x38)	/**< CFG1 field mask */
#define AEM13920_SRC1REGU1_CFG2_Pos		(0x03)	/**< CFG1 field offset */

#define AEM13920_SRC2REGU0_OFFSET		(0x03)	/**< SRC2REGU0 register offset */
#define AEM13920_SRC2REGU0_MODE_Msk		(0x01)	/**< MODE field mask */
#define AEM13920_SRC2REGU0_MODE_Pos		(0x00)	/**< MODE field offset */
#define AEM13920_SRC2REGU0_CFG0_Msk		(0x0E)	/**< CFG0 field mask */
#define AEM13920_SRC2REGU0_CFG0_Pos		(0x01)	/**< CFG0 field offset */

#define AEM13920_SRC2REGU1_OFFSET		(0x04)	/**< SRC2REGU1 register offset */
#define AEM13920_SRC2REGU1_CFG1_Msk		(0x07)	/**< CFG1 field mask */
#define AEM13920_SRC2REGU1_CFG1_Pos		(0x00)	/**< CFG1 field offset */
#define AEM13920_SRC2REGU1_CFG2_Msk		(0x38)	/**< CFG1 field mask */
#define AEM13920_SRC2REGU1_CFG2_Pos		(0x03)	/**< CFG1 field offset */

#define AEM13920_VOVDIS_OFFSET			(0x05)	/**< VOVDIS register offset */
#define AEM13920_VOVDIS_THRESH_Msk		(0x3F)	/**< THRESH field mask */
#define AEM13920_VOVDIS_THRESH_Pos		(0x00)	/**< THRESH field offset */

#define AEM13920_VCHRDY_OFFSET			(0x06)	/**< VCHRDY register offset */
#define AEM13920_VCHRDY_THRESH_Msk		(0x3F)	/**< THRESH field mask */
#define AEM13920_VCHRDY_THRESH_Pos		(0x00)	/**< THRESH field offset */

#define AEM13920_VOVCH_OFFSET			(0x07)	/**< VOVCH register offset */
#define AEM13920_VOVCH_THRESH_Msk		(0x7F)	/**< THRESH field mask */
#define AEM13920_VOVCH_THRESH_Pos		(0x00)	/**< THRESH field offset */

#define AEM13920_BST1CFG_OFFSET			(0x08)	/**< BST1CFG register offset */
#define AEM13920_BST1CFG_EN_Msk			(0x01)	/**< EN field mask */
#define AEM13920_BST1CFG_EN_Pos			(0x00)	/**< EN field offset */
#define AEM13920_BST1CFG_HPEN_Msk		(0x02)	/**< HPEN field mask */
#define AEM13920_BST1CFG_HPEN_Pos		(0x01)	/**< HPEN field offset */
#define AEM13920_BST1CFG_TMULT_Msk		(0x1C)	/**< TMULT field mask */
#define AEM13920_BST1CFG_TMULT_Pos		(0x02)	/**< TMULT field offset */

#define AEM13920_BST2CFG_OFFSET			(0x09)	/**< BST2CFG register offset */
#define AEM13920_BST2CFG_EN_Msk			(0x01)	/**< EN field mask */
#define AEM13920_BST2CFG_EN_Pos			(0x00)	/**< EN field offset */
#define AEM13920_BST2CFG_HPEN_Msk		(0x02)	/**< HPEN field mask */
#define AEM13920_BST2CFG_HPEN_Pos		(0x01)	/**< HPEN field offset */
#define AEM13920_BST2CFG_TMULT_Msk		(0x1C)	/**< TMULT field mask */
#define AEM13920_BST2CFG_TMULT_Pos		(0x02)	/**< TMULT field offset */

#define AEM13920_BUCKCFG_OFFSET			(0x0A)	/**< BUCKCFG register offset */
#define AEM13920_BUCKCFG_VOUT_Msk		(0x07)	/**< VOUT field mask */
#define AEM13920_BUCKCFG_VOUT_Pos		(0x00)	/**< VOUT field offset */
#define AEM13920_BUCKCFG_TMULT_Msk		(0x38)	/**< TMULT field mask */
#define AEM13920_BUCKCFG_TMULT_Pos		(0x03)	/**< TMULT field offset */

#define AEM13920_TEMPCOLDCH_OFFSET		(0x0B) 	/**< TEMPCOLDCH register offset */
#define AEM13920_TEMPHOTCH_OFFSET		(0x0C) 	/**< TEMPHOTCH register offset */
#define AEM13920_TEMPCOLDDIS_OFFSET		(0x0D) 	/**< TEMPCOLDDIS register offset */
#define AEM13920_TEMPHOTDIS_OFFSET		(0x0E) 	/**< TEMPHOT register offset */
#define AEM13920_TEMPX_THRESH_Msk		(0xFF) 	/**< THRESH field mask */
#define AEM13920_TEMPX_THRESH_Pos		(0x00)	/**< THRESH field offset */
#define AEM13920_TEMPCOLDCH_THRESH_Msk		(AEM13920_TEMPX_THRESH_Msk)
#define AEM13920_TEMPHOTCH_THRESH_Msk		(AEM13920_TEMPX_THRESH_Msk)
#define AEM13920_TEMPCOLDDIS_THRESH_Msk		(AEM13920_TEMPX_THRESH_Msk)
#define AEM13920_TEMPHOTDIS_THRESH_Msk		(AEM13920_TEMPX_THRESH_Msk)
#define AEM13920_TEMPCOLDCH_THRESH_Pos		(AEM13920_TEMPX_THRESH_Pos)
#define AEM13920_TEMPHOTCH_THRESH_Pos		(AEM13920_TEMPX_THRESH_Pos)
#define AEM13920_TEMPCOLDDIS_THRESH_Pos		(AEM13920_TEMPX_THRESH_Pos)
#define AEM13920_TEMPHOTDIS_THRESH_Pos		(AEM13920_TEMPX_THRESH_Pos)

#define AEM13920_TMON_OFFSET			(0x0F)	/**< TMON register offset */
#define AEM13920_TMON_EN_Msk			(0x01)	/**< EN field mask */
#define AEM13920_TMON_EN_Pos			(0x00)	/**< EN field offset */

#define AEM13920_SLEEP_OFFSET			(0x10)	/**< SLEEP register offset */
#define AEM13920_SLEEP_SRC1THRESH_Msk		(0x07)	/**< SRC1THRESH field mask */
#define AEM13920_SLEEP_SRC1THRESH_Pos		(0x00)	/**< SRC1THRESH field offset */
#define AEM13920_SLEEP_SRC2THRESH_Msk		(0x38)	/**< SRC2THRESH field mask */
#define AEM13920_SLEEP_SRC2THRESH_Pos		(0x03)	/**< SRC2THRESH field offset */

#define AEM13920_APM_OFFSET			(0x11)	/**< APM register offset */
#define AEM13920_APM_SRC1EN_Msk			(0x01)	/**< SRC1EN field mask */
#define AEM13920_APM_SRC1EN_Pos			(0x00)	/**< SRC1EN field offset */
#define AEM13920_APM_SRC2EN_Msk			(0x02)	/**< SRC2EN field mask */
#define AEM13920_APM_SRC2EN_Pos			(0x01)	/**< SRC2EN field offset */
#define AEM13920_APM_BUCKEN_Msk			(0x04)	/**< BUCKEN field mask */
#define AEM13920_APM_BUCKEN_Pos			(0x02)	/**< BUCKEN field offset */
#define AEM13920_APM_MODE_Msk			(0x08)	/**< MODE field mask */
#define AEM13920_APM_MODE_Pos			(0x03)	/**< MODE field offset */
#define AEM13920_APM_WINDOW_Msk			(0x10)	/**< WINDOW field mask */
#define AEM13920_APM_WINDOW_Pos			(0x04)	/**< WINDOW field offset */

#define AEM13920_IRQEN0_OFFSET			(0x12)	/**< IRQEN0 register offset */
#define AEM13920_IRQEN0_ALL_Msk			(0x7F)	/**< All fields mask */
#define AEM13920_IRQEN0_I2CRDY_Msk		(0x01)	/**< I2CRDY field mask */
#define AEM13920_IRQEN0_I2CRDY_Pos		(0x00)	/**< I2CRDY field offset */
#define AEM13920_IRQEN0_VOVDIS_Msk		(0x02)	/**< VOVDIS field mask */
#define AEM13920_IRQEN0_VOVDIS_Pos		(0x01)	/**< VOVDIS field offset */
#define AEM13920_IRQEN0_VCHRDY_Msk		(0x04)	/**< VCHRDY field mask */
#define AEM13920_IRQEN0_VCHRDY_Pos		(0x02)	/**< VCHRDY field offset */
#define AEM13920_IRQEN0_VOVCH_Msk		(0x08)	/**< VOVCH field mask */
#define AEM13920_IRQEN0_VOVCH_Pos		(0x03)	/**< VOVCH field offset */
#define AEM13920_IRQEN0_SLEEP_Msk		(0x10)	/**< SLEEP field mask */
#define AEM13920_IRQEN0_SLEEP_Pos		(0x04)	/**< SLEEP field offset */
#define AEM13920_IRQEN0_TEMPCH_Msk		(0x20)	/**< TEMPCH field mask */
#define AEM13920_IRQEN0_TEMPCH_Pos		(0x05)	/**< TEMPCH field offset */
#define AEM13920_IRQEN0_TEMPDIS_Msk		(0x40)	/**< TEMPDIS field mask */
#define AEM13920_IRQEN0_TEMPDIS_Pos		(0x06)	/**< TEMPDIS field offset */

#define AEM13920_IRQEN1_OFFSET			(0x13)	/**< IRQEN1 register offset */
#define AEM13920_IRQEN1_ALL_Msk			(0xFF)	/**< All fields mask */
#define AEM13920_IRQEN1_SRC1MPPTSTART_Msk	(0x01)	/**< SRC1MPPTSTART field mask */
#define AEM13920_IRQEN1_SRC1MPPTSTART_Pos	(0x00)	/**< SRC1MPPTSTART field offset */
#define AEM13920_IRQEN1_SRC1MPPTDONE_Msk	(0x02)	/**< SRC1MPPTDONE field mask */
#define AEM13920_IRQEN1_SRC1MPPTDONE_Pos	(0x01)	/**< SRC1MPPTDONE field offset */
#define AEM13920_IRQEN1_SRC2MPPTSTART_Msk	(0x04)	/**< SRC2MPPTSTART field mask */
#define AEM13920_IRQEN1_SRC2MPPTSTART_Pos	(0x02)	/**< SRC2MPPTSTART field offset */
#define AEM13920_IRQEN1_SRC2MPPTDONE_Msk	(0x08)	/**< SRC2MPPTDONE field mask */
#define AEM13920_IRQEN1_SRC2MPPTDONE_Pos	(0x03)	/**< SRC2MPPTDONE field offset */
#define AEM13920_IRQEN1_STODONE_Msk		(0x10)	/**< STODONE field mask */
#define AEM13920_IRQEN1_STODONE_Pos		(0x04)	/**< STODONE field offset */
#define AEM13920_IRQEN1_TEMPDONE_Msk		(0x20)	/**< TEMP field mask */
#define AEM13920_IRQEN1_TEMPDONE_Pos		(0x05)	/**< TEMP field offset */
#define AEM13920_IRQEN1_APMDONE_Msk		(0x40)	/**< APMDONE field mask */
#define AEM13920_IRQEN1_APMDONE_Pos		(0x06)	/**< APMDONE field offset */
#define AEM13920_IRQEN1_APMERR_Msk		(0x80)	/**< APMERR field mask */
#define AEM13920_IRQEN1_APMERR_Pos		(0x07)	/**< APMERR field offset */

#define AEM13920_CTRL_OFFSET			(0x14)	/**< CTRL register offset */
#define AEM13920_CTRL_UPDATE_Msk		(0x01)	/**< UPDATE field mask */
#define AEM13920_CTRL_UPDATE_Pos		(0x00)	/**< UPDATE field offset */
#define AEM13920_CTRL_SYNCBUSY_Msk		(0x04)	/**< SYNCBUSY field mask */
#define AEM13920_CTRL_SYNCBUSY_Pos		(0x02)	/**< SYNCBUSY field offset */

#define AEM13920_IRQFLG0_OFFSET			(0x15)	/**< IRQFLG0 register offset */
#define AEM13920_IRQFLG0_ALL_Msk		(0x7F)	/**< All fields mask */
#define AEM13920_IRQFLG0_I2CRDY_Msk		(0x01)	/**< I2CRDY field mask */
#define AEM13920_IRQFLG0_I2CRDY_Pos		(0x00)	/**< I2CRDY field offset */
#define AEM13920_IRQFLG0_VOVDIS_Msk		(0x02)	/**< VOVDIS field mask */
#define AEM13920_IRQFLG0_VOVDIS_Pos		(0x01)	/**< VOVDIS field offset */
#define AEM13920_IRQFLG0_VCHRDY_Msk		(0x04)	/**< VCHRDY field mask */
#define AEM13920_IRQFLG0_VCHRDY_Pos		(0x02)	/**< VCHRDY field offset */
#define AEM13920_IRQFLG0_VOVCH_Msk		(0x08)	/**< VOVCH field mask */
#define AEM13920_IRQFLG0_VOVCH_Pos		(0x03)	/**< VOVCH field offset */
#define AEM13920_IRQFLG0_SLEEP_Msk		(0x10)	/**< SLEEP field mask */
#define AEM13920_IRQFLG0_SLEEP_Pos		(0x04)	/**< SLEEP field offset */
#define AEM13920_IRQFLG0_TEMPCH_Msk		(0x20)	/**< TEMPCH field mask */
#define AEM13920_IRQFLG0_TEMPCH_Pos		(0x05)	/**< TEMPCH field offset */
#define AEM13920_IRQFLG0_TEMPDIS_Msk		(0x40)	/**< TEMPDIS field mask */
#define AEM13920_IRQFLG0_TEMPDIS_Pos		(0x06)	/**< TEMPDIS field offset */

#define AEM13920_IRQFLG1_OFFSET			(0x16)	/**< IRQFLG1 register offset */
#define AEM13920_IRQFLG1_ALL_Msk		(0xFF)	/**< All fields mask */
#define AEM13920_IRQFLG1_SRC1MPPTSTART_Msk	(0x01)	/**< SRC1MPPTSTART field mask */
#define AEM13920_IRQFLG1_SRC1MPPTSTART_Pos	(0x00)	/**< SRC1MPPTSTART field offset */
#define AEM13920_IRQFLG1_SRC1MPPTDONE_Msk	(0x02)	/**< SRC1MPPTDONE field mask */
#define AEM13920_IRQFLG1_SRC1MPPTDONE_Pos	(0x01)	/**< SRC1MPPTDONE field offset */
#define AEM13920_IRQFLG1_SRC2MPPTSTART_Msk	(0x04)	/**< SRC2MPPTSTART field mask */
#define AEM13920_IRQFLG1_SRC2MPPTSTART_Pos	(0x02)	/**< SRC2MPPTSTART field offset */
#define AEM13920_IRQFLG1_SRC2MPPTDONE_Msk	(0x08)	/**< SRC2MPPTDONE field mask */
#define AEM13920_IRQFLG1_SRC2MPPTDONE_Pos	(0x03)	/**< SRC2MPPTDONE field offset */
#define AEM13920_IRQFLG1_STODONE_Msk		(0x10)	/**< STODONE field mask */
#define AEM13920_IRQFLG1_STODONE_Pos		(0x04)	/**< STODONE field offset */
#define AEM13920_IRQFLG1_TEMPDONE_Msk		(0x20)	/**< TEMP field mask */
#define AEM13920_IRQFLG1_TEMPDONE_Pos		(0x05)	/**< TEMP field offset */
#define AEM13920_IRQFLG1_APMDONE_Msk		(0x40)	/**< APMDONE field mask */
#define AEM13920_IRQFLG1_APMDONE_Pos		(0x06)	/**< APMDONE field offset */
#define AEM13920_IRQFLG1_APMERR_Msk		(0x80)	/**< APMERR field mask */
#define AEM13920_IRQFLG1_APMERR_Pos		(0x07)	/**< APMERR field offset */

#define AEM13920_STATUS0_OFFSET			(0x17)	/**< STATUS0 register offset */
#define AEM13920_STATUS0_ALL_Msk		(0x1F)	/**< All fields mask */
#define AEM13920_STATUS0_VOVDIS_Msk		(0x01)	/**< VOVDIS field mask */
#define AEM13920_STATUS0_VOVDIS_Pos		(0x00)	/**< VOVDIS field offset */
#define AEM13920_STATUS0_VCHRDY_Msk		(0x02)	/**< VOVDIS field mask */
#define AEM13920_STATUS0_VCHRDY_Pos		(0x01)	/**< VOVDIS field offset */
#define AEM13920_STATUS0_VOVCH_Msk		(0x04)	/**< VOVCH field mask */
#define AEM13920_STATUS0_VOVCH_Pos		(0x02)	/**< VOVCH field offset */
#define AEM13920_STATUS0_SRC1SLEEP_Msk		(0x08)	/**< SRC1SLEEP field mask */
#define AEM13920_STATUS0_SRC1SLEEP_Pos		(0x03)	/**< SRC1SLEEP field offset */
#define AEM13920_STATUS0_SRC2SLEEP_Msk		(0x10)	/**< SRC2SLEEP field mask */
#define AEM13920_STATUS0_SRC2SLEEP_Pos		(0x04)	/**< SRC2SLEEP field offset */

#define AEM13920_STATUS1_OFFSET			(0x18)	/**< STATUS1 register offset */
#define AEM13920_STATUS1_ALL_Msk		(0x0F)	/**< All fields mask */
#define AEM13920_STATUS1_TEMPCOLDCH_Msk		(0x01)	/**< TEMPCOLDCH field mask */
#define AEM13920_STATUS1_TEMPCOLDCH_Pos		(0x00)	/**< TEMPCOLDCH field offset */
#define AEM13920_STATUS1_TEMPHOTCH_Msk		(0x02)	/**< TEMPHOTCH field mask */
#define AEM13920_STATUS1_TEMPHOTCH_Pos		(0x01)	/**< TEMPHOTCH field offset */
#define AEM13920_STATUS1_TEMPCOLDDIS_Msk	(0x04)	/**< TEMPCOLDDIS field mask */
#define AEM13920_STATUS1_TEMPCOLDDIS_Pos	(0x02)	/**< TEMPCOLDDIS field offset */
#define AEM13920_STATUS1_TEMPHOTDIS_Msk		(0x08)	/**< TEMPHOTDIS field mask */
#define AEM13920_STATUS1_TEMPHOTDIS_Pos		(0x03)	/**< TEMPHOTDIS field offset */

#define AEM13920_APM0SRC1_OFFSET		(0x19)	/**< APM0SRC1 register offset */
#define AEM13920_APM0SRC1_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM0SRC1_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM1SRC1_OFFSET		(0x1A)	/**< APM1SRC1 register's offset */
#define AEM13920_APM1SRC1_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM1SRC1_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM2SRC1_OFFSET		(0x1B)	/**< APM2SRC1 register's offset */
#define AEM13920_APM2SRC1_DATA_Msk		(0xFF)	/**< DATA field mask (pulse counter) */
#define AEM13920_APM2SRC1_DATA_Pos		(0x00)	/**< DATA field offset (pulse counter) */
#define AEM13920_APM2SRC1_PM_DATA_Msk		(0x07)	/**< DATA field mask (power meter) */
#define AEM13920_APM2SRC1_PM_SHIFT_Msk		(0x78)	/**< SHIFT field mask (power meter) */
#define AEM13920_APM2SRC1_PM_SHIFT_Pos		(0x03)	/**< SHIFT field offset (power meter) */

#define AEM13920_APM0SRC2_OFFSET		(0x1C)	/**< APM0SRC2 register offset */
#define AEM13920_APM0SRC2_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM0SRC2_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM1SRC2_OFFSET		(0x1D)	/**< APM1SRC2 register's offset */
#define AEM13920_APM1SRC2_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM1SRC2_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM2SRC2_OFFSET		(0x1E)	/**< APM2SRC2 register's offset */
#define AEM13920_APM2SRC2_DATA_Msk		(0xFF)	/**< DATA field mask (pulse counter) */
#define AEM13920_APM2SRC2_DATA_Pos		(0x00)	/**< DATA field offset (pulse counter) */
#define AEM13920_APM2SRC2_PM_DATA_Msk		(0x07)	/**< DATA field's mask (power meter) */
#define AEM13920_APM2SRC2_PM_SHIFT_Msk		(0x78)	/**< SHIFT field's mask (power meter) */
#define AEM13920_APM2SRC2_PM_SHIFT_Pos		(0x03)	/**< SHIFT field's offset (power meter) */

#define AEM13920_APM0BUCK_OFFSET		(0x1F)	/**< APM0BUCK register offset */
#define AEM13920_APM0BUCK_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM0BUCK_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM1BUCK_OFFSET		(0x20)	/**< APM1BUCK register's offset */
#define AEM13920_APM1BUCK_DATA_Msk		(0xFF)	/**< DATA field mask */
#define AEM13920_APM1BUCK_DATA_Pos		(0x00)	/**< DATA field offset */

#define AEM13920_APM2BUCK_OFFSET		(0x21)	/**< APM2BUCK register's offset */
#define AEM13920_APM2BUCK_DATA_Msk		(0xFF)	/**< DATA field mask (pulse counter) */
#define AEM13920_APM2BUCK_DATA_Pos		(0x00)	/**< DATA field offset (pulse counter) */
#define AEM13920_APM2BUCK_PM_DATA_Msk		(0x07)	/**< DATA field's mask (power meter) */
#define AEM13920_APM2BUCK_PM_SHIFT_Msk		(0x78)	/**< SHIFT field's mask (power meter) */
#define AEM13920_APM2BUCK_PM_SHIFT_Pos		(0x03)	/**< SHIFT field's offset (power meter) */

#define AEM13920_APMERR_OFFSET			(0x22)	/**< APMERR register offset */
#define AEM13920_APMERR_ALL_Msk			(0x3F)	/**< All fields mask */
#define AEM13920_APMERR_SRC1OV_Msk		(0x01)	/**< SRC1OV field mask */
#define AEM13920_APMERR_SRC1OV_Pos		(0x00)	/**< SRC1OV field offset */
#define AEM13920_APMERR_SRC1NVLD_Msk		(0x02)	/**< SRC1NVLD field mask */
#define AEM13920_APMERR_SRC1NVLD_Pos		(0x01)	/**< SRC1NVLD field offset */
#define AEM13920_APMERR_SRC2OV_Msk		(0x04)	/**< SRC2OV field mask */
#define AEM13920_APMERR_SRC2OV_Pos		(0x02)	/**< SRC2OV field offset */
#define AEM13920_APMERR_SRC2NVLD_Msk		(0x08)	/**< SRC2NVLD field mask */
#define AEM13920_APMERR_SRC2NVLD_Pos		(0x03)	/**< SRC2NVLD field offset */
#define AEM13920_APMERR_BUCKOV_Msk		(0x10)	/**< BUCKOV field mask */
#define AEM13920_APMERR_BUCKOV_Pos		(0x04)	/**< BUCKOV field offset */
#define AEM13920_APMERR_BUCKNVLD_Msk		(0x20)	/**< BUCKNVLD field mask */
#define AEM13920_APMERR_BUCKNVLD_Pos		(0x05)	/**< BUCKNVLD field offset */

#define AEM13920_TEMP_OFFSET			(0x23)	/**< TEMP register's offset */
#define AEM13920_TEMP_DATA_Msk			(0xFF)	/**< DATA field's mask */
#define AEM13920_TEMP_DATA_Pos			(0x00)	/**< DATA field's offset */

#define AEM13920_STO_OFFSET			(0x24)	/**< STO register's offset */
#define AEM13920_STO_DATA_Msk			(0xFF)	/**< DATA field's mask */
#define AEM13920_STO_DATA_Pos			(0x00)	/**< DATA field's offset */

#define AEM13920_SRC1_OFFSET			(0x25)	/**< SRC register's offset */
#define AEM13920_SRC1_DATA_Msk			(0xFF)	/**< DATA field's mask */
#define AEM13920_SRC1_DATA_Pos			(0x00)	/**< DATA field's offset */

#define AEM13920_SRC2_OFFSET			(0x26)	/**< SRC register's offset */
#define AEM13920_SRC2_DATA_Msk			(0xFF)	/**< DATA field's mask */
#define AEM13920_SRC2_DATA_Pos			(0x00)	/**< DATA field's offset */

#define AEM13920_CFG_OFFSET			AEM13920_SRC1REGU0_OFFSET
#define AEM13920_CFG_LENGTH			(0x14)	/**< Length of the configuration area */

#ifdef __cplusplus
}
#endif

#endif /* AEM13920_REGISTERMAP_H_ */
