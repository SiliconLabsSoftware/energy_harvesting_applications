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
#ifndef AEM13920_REGISTERMAP_H_
#define AEM13920_REGISTERMAP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define AEM13920_VERSION_OFFSET			(0x00U)	/**< VERSION register offset */
#define AEM13920_VERSION_MINOR_Msk		(0x0FU)	/**< MINOR field's mask */
#define AEM13920_VERSION_MINOR_Pos		(0x00U)	/**< MINOR field's offset */
#define AEM13920_VERSION_MAJOR_Msk		(0xF0U)	/**< MAJOR field's mask */
#define AEM13920_VERSION_MAJOR_Pos		(0x04U)	/**< MAJOR field's offset */

#define AEM13920_SRC1REGU0_OFFSET		(0x01U)	/**< SRC1REGU0 register offset */
#define AEM13920_SRC2REGU0_OFFSET		(0x03U)	/**< SRC2REGU0 register offset */
#define AEM13920_SRCxREGU0_MODE_Msk		(0x01U)	/**< MODE field's mask */
#define AEM13920_SRCxREGU0_MODE_Pos		(0x00U)	/**< MODE field's offset */
#define AEM13920_SRCxREGU0_CFG0_Msk		(0x0EU)	/**< CFG0 field's mask */
#define AEM13920_SRCxREGU0_CFG0_Pos		(0x01U)	/**< CFG0 field's offset */

#define AEM13920_SRC1REGU1_OFFSET		(0x02U)	/**< SRC1REGU1 register offset */
#define AEM13920_SRC2REGU1_OFFSET		(0x04U)	/**< SRC2REGU1 register offset */
#define AEM13920_SRCxREGU1_CFG1_Msk		(0x07U)	/**< CFG1 field's mask */
#define AEM13920_SRCxREGU1_CFG1_Pos		(0x00U)	/**< CFG1 field's offset */
#define AEM13920_SRCxREGU1_CFG2_Msk		(0x38U)	/**< CFG1 field's mask */
#define AEM13920_SRCxREGU1_CFG2_Pos		(0x03U)	/**< CFG1 field's offset */

#define AEM13920_VOVDIS_OFFSET			(0x05U)	/**< VOVDIS register offset */
#define AEM13920_VOVDIS_THRESH_Msk		(0x3FU)	/**< THRESH field's mask */
#define AEM13920_VOVDIS_THRESH_Pos		(0x00U)	/**< THRESH field's offset */

#define AEM13920_VCHRDY_OFFSET			(0x06U)	/**< VCHRDY register offset */
#define AEM13920_VCHRDY_THRESH_Msk		(0x3FU)	/**< THRESH field's mask */
#define AEM13920_VCHRDY_THRESH_Pos		(0x00U)	/**< THRESH field's offset */

#define AEM13920_VOVCH_OFFSET			(0x07U)	/**< VOVCH register offset */
#define AEM13920_VOVCH_THRESH_Msk		(0x7FU)	/**< THRESH field's mask */
#define AEM13920_VOVCH_THRESH_Pos		(0x00U)	/**< THRESH field's offset */

#define AEM13920_BST1CFG_OFFSET			(0x08U)	/**< BST1CFG register offset */
#define AEM13920_BST2CFG_OFFSET			(0x09U)	/**< BST2CFG register offset */
#define AEM13920_BSTxCFG_EN_Msk			(0x01U)	/**< EN field's mask */
#define AEM13920_BSTxCFG_EN_Pos			(0x00U)	/**< EN field's offset */
#define AEM13920_BSTxCFG_HPEN_Msk		(0x02U)	/**< HPEN field's mask */
#define AEM13920_BSTxCFG_HPEN_Pos		(0x01U)	/**< HPEN field's offset */
#define AEM13920_BSTxCFG_TMULT_Msk		(0x1CU)	/**< TMULT field's mask */
#define AEM13920_BSTxCFG_TMULT_Pos		(0x02U)	/**< TMULT field's offset */
#define AEM13920_BSTxCFG_ALL_Msk		( AEM13920_BSTxCFG_EN_Msk	\
						| AEM13920_BSTxCFG_HPEN_Msk	\
						| AEM13920_BSTxCFG_TMULT_Msk )

#define AEM13920_BUCKCFG_OFFSET			(0x0AU)	/**< BUCKCFG register offset */
#define AEM13920_BUCKCFG_VOUT_Msk		(0x07U)	/**< VOUT field's mask */
#define AEM13920_BUCKCFG_VOUT_Pos		(0x00U)	/**< VOUT field's offset */
#define AEM13920_BUCKCFG_TMULT_Msk		(0x38U)	/**< TMULT field's mask */
#define AEM13920_BUCKCFG_TMULT_Pos		(0x03U)	/**< TMULT field's offset */
#define AEM13920_BUCKCFG_ALL_Msk		( AEM13920_BUCKCFG_VOUT_Msk	\
						| AEM13920_BUCKCFG_TMULT_Msk )

#define AEM13920_TEMPCOLDCH_OFFSET		(0x0BU) /**< TEMPCOLDCH register offset */
#define AEM13920_TEMPHOTCH_OFFSET		(0x0CU) /**< TEMPHOTCH register offset */
#define AEM13920_TEMPCOLDDIS_OFFSET		(0x0DU) /**< TEMPCOLDDIS register offset */
#define AEM13920_TEMPHOTDIS_OFFSET		(0x0EU) /**< TEMPHOT register offset */
#define AEM13920_TEMPx_THRESH_Msk		(0xFFU) /**< THRESH field's mask */
#define AEM13920_TEMPx_THRESH_Pos		(0x00U)	/**< THRESH field's offset */

#define AEM13920_TMON_OFFSET			(0x0FU)	/**< TMON register offset */
#define AEM13920_TMON_EN_Msk			(0x01U)	/**< EN field's mask */
#define AEM13920_TMON_EN_Pos			(0x00U)	/**< EN field's offset */

#define AEM13920_SRCLOW_OFFSET			(0x10U)	/**< SRCLOW register offset */
#define AEM13920_SRCLOW_SRC1THRESH_Msk		(0x07U)	/**< SRC1THRESH field's mask */
#define AEM13920_SRCLOW_SRC1THRESH_Pos		(0x00U)	/**< SRC1THRESH field's offset */
#define AEM13920_SRCLOW_SRC2THRESH_Msk		(0x38U)	/**< SRC2THRESH field's mask */
#define AEM13920_SRCLOW_SRC2THRESH_Pos		(0x03U)	/**< SRC2THRESH field's offset */
#define AEM13920_SRCLOW_ALL_Msk			( AEM13920_SRCLOW_SRC1THRESH_Msk	\
						| AEM13920_SRCLOW_SRC2THRESH_Msk )

#define AEM13920_APM_OFFSET			(0x11U)	/**< APM register offset */
#define AEM13920_APM_SRC1EN_Msk			(0x01U)	/**< SRC1EN field's mask */
#define AEM13920_APM_SRC1EN_Pos			(0x00U)	/**< SRC1EN field's offset */
#define AEM13920_APM_SRC2EN_Msk			(0x02U)	/**< SRC2EN field's mask */
#define AEM13920_APM_SRC2EN_Pos			(0x01U)	/**< SRC2EN field's offset */
#define AEM13920_APM_BUCKEN_Msk			(0x04U)	/**< BUCKEN field's mask */
#define AEM13920_APM_BUCKEN_Pos			(0x02U)	/**< BUCKEN field's offset */
#define AEM13920_APM_MODE_Msk			(0x08U)	/**< MODE field's mask */
#define AEM13920_APM_MODE_Pos			(0x03U)	/**< MODE field's offset */
#define AEM13920_APM_WINDOW_Msk			(0x10U)	/**< WINDOW field's mask */
#define AEM13920_APM_WINDOW_Pos			(0x04U)	/**< WINDOW field's offset */
#define AEM13920_APM_ALL_Msk			( AEM13920_APM_SRC1EN_Msk	\
						| AEM13920_APM_SRC2EN_Msk	\
						| AEM13920_APM_BUCKEN_Msk	\
						| AEM13920_APM_MODE_Msk		\
						| AEM13920_APM_WINDOW_Msk )

#define AEM13920_IRQEN0_OFFSET			(0x12U)	/**< IRQEN0 register offset */
#define AEM13920_IRQEN0_I2CRDY_Msk		(0x01U)	/**< I2CRDY field's mask */
#define AEM13920_IRQEN0_I2CRDY_Pos		(0x00U)	/**< I2CRDY field's offset */
#define AEM13920_IRQEN0_VOVDIS_Msk		(0x02U)	/**< VOVDIS field's mask */
#define AEM13920_IRQEN0_VOVDIS_Pos		(0x01U)	/**< VOVDIS field's offset */
#define AEM13920_IRQEN0_VCHRDY_Msk		(0x04U)	/**< VCHRDY field's mask */
#define AEM13920_IRQEN0_VCHRDY_Pos		(0x02U)	/**< VCHRDY field's offset */
#define AEM13920_IRQEN0_VOVCH_Msk		(0x08U)	/**< VOVCH field's mask */
#define AEM13920_IRQEN0_VOVCH_Pos		(0x03U)	/**< VOVCH field's offset */
#define AEM13920_IRQEN0_SRCLOW_Msk		(0x10U)	/**< SRCLOW field's mask */
#define AEM13920_IRQEN0_SRCLOW_Pos		(0x04U)	/**< SRCLOW field's offset */
#define AEM13920_IRQEN0_TEMPCH_Msk		(0x20U)	/**< TEMPCH field's mask */
#define AEM13920_IRQEN0_TEMPCH_Pos		(0x05U)	/**< TEMPCH field's offset */
#define AEM13920_IRQEN0_TEMPDIS_Msk		(0x40U)	/**< TEMPDIS field's mask */
#define AEM13920_IRQEN0_TEMPDIS_Pos		(0x06U)	/**< TEMPDIS field's offset */
#define AEM13920_IRQEN0_ALL_Msk			( AEM13920_IRQEN0_I2CRDY_Msk	\
						| AEM13920_IRQEN0_VOVDIS_Msk	\
						| AEM13920_IRQEN0_VCHRDY_Msk	\
						| AEM13920_IRQEN0_VOVCH_Msk	\
						| AEM13920_IRQEN0_SRCLOW_Msk	\
						| AEM13920_IRQEN0_TEMPCH_Msk	\
						| AEM13920_IRQEN0_TEMPDIS_Msk )

#define AEM13920_IRQEN1_OFFSET			(0x13U)	/**< IRQEN1 register offset */
#define AEM13920_IRQEN1_SRC1MPPTSTART_Msk	(0x01U)	/**< SRC1MPPTSTART field's mask */
#define AEM13920_IRQEN1_SRC1MPPTSTART_Pos	(0x00U)	/**< SRC1MPPTSTART field's offset */
#define AEM13920_IRQEN1_SRC1MPPTDONE_Msk	(0x02U)	/**< SRC1MPPTDONE field's mask */
#define AEM13920_IRQEN1_SRC1MPPTDONE_Pos	(0x01U)	/**< SRC1MPPTDONE field's offset */
#define AEM13920_IRQEN1_SRC2MPPTSTART_Msk	(0x04U)	/**< SRC2MPPTSTART field's mask */
#define AEM13920_IRQEN1_SRC2MPPTSTART_Pos	(0x02U)	/**< SRC2MPPTSTART field's offset */
#define AEM13920_IRQEN1_SRC2MPPTDONE_Msk	(0x08U)	/**< SRC2MPPTDONE field's mask */
#define AEM13920_IRQEN1_SRC2MPPTDONE_Pos	(0x03U)	/**< SRC2MPPTDONE field's offset */
#define AEM13920_IRQEN1_STODONE_Msk		(0x10U)	/**< STODONE field's mask */
#define AEM13920_IRQEN1_STODONE_Pos		(0x04U)	/**< STODONE field's offset */
#define AEM13920_IRQEN1_TEMPDONE_Msk		(0x20U)	/**< TEMP field's mask */
#define AEM13920_IRQEN1_TEMPDONE_Pos		(0x05U)	/**< TEMP field's offset */
#define AEM13920_IRQEN1_APMDONE_Msk		(0x40U)	/**< APMDONE field's mask */
#define AEM13920_IRQEN1_APMDONE_Pos		(0x06U)	/**< APMDONE field's offset */
#define AEM13920_IRQEN1_APMERR_Msk		(0x80U)	/**< APMERR field's mask */
#define AEM13920_IRQEN1_APMERR_Pos		(0x07U)	/**< APMERR field's offset */
#define AEM13920_IRQEN1_ALL_Msk			( AEM13920_IRQEN1_SRC1MPPTSTART_Msk	\
						| AEM13920_IRQEN1_SRC1MPPTDONE_Msk	\
						| AEM13920_IRQEN1_SRC2MPPTSTART_Msk	\
						| AEM13920_IRQEN1_SRC2MPPTDONE_Msk	\
						| AEM13920_IRQEN1_STODONE_Msk		\
						| AEM13920_IRQEN1_TEMPDONE_Msk		\
						| AEM13920_IRQEN1_APMDONE_Msk		\
						| AEM13920_IRQEN1_APMERR_Msk )

#define AEM13920_CTRL_OFFSET			(0x14U)	/**< CTRL register offset */
#define AEM13920_CTRL_UPDATE_Msk		(0x01U)	/**< UPDATE field's mask */
#define AEM13920_CTRL_UPDATE_Pos		(0x00U)	/**< UPDATE field's offset */
#define AEM13920_CTRL_SYNCBUSY_Msk		(0x04U)	/**< SYNCBUSY field's mask */
#define AEM13920_CTRL_SYNCBUSY_Pos		(0x02U)	/**< SYNCBUSY field's offset */

#define AEM13920_IRQFLG0_OFFSET			(0x15U)	/**< IRQFLG0 register offset */
#define AEM13920_IRQFLG0_I2CRDY_Msk		(0x01U)	/**< I2CRDY field's mask */
#define AEM13920_IRQFLG0_I2CRDY_Pos		(0x00U)	/**< I2CRDY field's offset */
#define AEM13920_IRQFLG0_VOVDIS_Msk		(0x02U)	/**< VOVDIS field's mask */
#define AEM13920_IRQFLG0_VOVDIS_Pos		(0x01U)	/**< VOVDIS field's offset */
#define AEM13920_IRQFLG0_VCHRDY_Msk		(0x04U)	/**< VCHRDY field's mask */
#define AEM13920_IRQFLG0_VCHRDY_Pos		(0x02U)	/**< VCHRDY field's offset */
#define AEM13920_IRQFLG0_VOVCH_Msk		(0x08U)	/**< VOVCH field's mask */
#define AEM13920_IRQFLG0_VOVCH_Pos		(0x03U)	/**< VOVCH field's offset */
#define AEM13920_IRQFLG0_SRCLOW_Msk		(0x10U)	/**< SRCLOW field's mask */
#define AEM13920_IRQFLG0_SRCLOW_Pos		(0x04U)	/**< SRCLOW field's offset */
#define AEM13920_IRQFLG0_TEMPCH_Msk		(0x20U)	/**< TEMPCH field's mask */
#define AEM13920_IRQFLG0_TEMPCH_Pos		(0x05U)	/**< TEMPCH field's offset */
#define AEM13920_IRQFLG0_TEMPDIS_Msk		(0x40U)	/**< TEMPDIS field's mask */
#define AEM13920_IRQFLG0_TEMPDIS_Pos		(0x06U)	/**< TEMPDIS field's offset */
#define AEM13920_IRQFLG0_ALL_Msk		( AEM13920_IRQFLG0_I2CRDY_Msk	\
						| AEM13920_IRQFLG0_VOVDIS_Msk	\
						| AEM13920_IRQFLG0_VCHRDY_Msk	\
						| AEM13920_IRQFLG0_VOVCH_Msk	\
						| AEM13920_IRQFLG0_SRCLOW_Msk	\
						| AEM13920_IRQFLG0_TEMPCH_Msk	\
						| AEM13920_IRQFLG0_TEMPDIS_Msk )

#define AEM13920_IRQFLG1_OFFSET			(0x16U)	/**< IRQFLG1 register offset */
#define AEM13920_IRQFLG1_SRC1MPPTSTART_Msk	(0x01U)	/**< SRC1MPPTSTART field's mask */
#define AEM13920_IRQFLG1_SRC1MPPTSTART_Pos	(0x00U)	/**< SRC1MPPTSTART field's offset */
#define AEM13920_IRQFLG1_SRC1MPPTDONE_Msk	(0x02U)	/**< SRC1MPPTDONE field's mask */
#define AEM13920_IRQFLG1_SRC1MPPTDONE_Pos	(0x01U)	/**< SRC1MPPTDONE field's offset */
#define AEM13920_IRQFLG1_SRC2MPPTSTART_Msk	(0x04U)	/**< SRC2MPPTSTART field's mask */
#define AEM13920_IRQFLG1_SRC2MPPTSTART_Pos	(0x02U)	/**< SRC2MPPTSTART field's offset */
#define AEM13920_IRQFLG1_SRC2MPPTDONE_Msk	(0x08U)	/**< SRC2MPPTDONE field's mask */
#define AEM13920_IRQFLG1_SRC2MPPTDONE_Pos	(0x03U)	/**< SRC2MPPTDONE field's offset */
#define AEM13920_IRQFLG1_STODONE_Msk		(0x10U)	/**< STODONE field's mask */
#define AEM13920_IRQFLG1_STODONE_Pos		(0x04U)	/**< STODONE field's offset */
#define AEM13920_IRQFLG1_TEMPDONE_Msk		(0x20U)	/**< TEMP field's mask */
#define AEM13920_IRQFLG1_TEMPDONE_Pos		(0x05U)	/**< TEMP field's offset */
#define AEM13920_IRQFLG1_APMDONE_Msk		(0x40U)	/**< APMDONE field's mask */
#define AEM13920_IRQFLG1_APMDONE_Pos		(0x06U)	/**< APMDONE field's offset */
#define AEM13920_IRQFLG1_APMERR_Msk		(0x80U)	/**< APMERR field's mask */
#define AEM13920_IRQFLG1_APMERR_Pos		(0x07U)	/**< APMERR field's offset */
#define AEM13920_IRQFLG1_ALL_Msk		( AEM13920_IRQFLG1_SRC1MPPTSTART_Msk	\
						| AEM13920_IRQFLG1_SRC1MPPTDONE_Msk	\
						| AEM13920_IRQFLG1_SRC2MPPTSTART_Msk	\
						| AEM13920_IRQFLG1_SRC2MPPTDONE_Msk	\
						| AEM13920_IRQFLG1_STODONE_Msk		\
						| AEM13920_IRQFLG1_TEMPDONE_Msk		\
						| AEM13920_IRQFLG1_APMDONE_Msk		\
						| AEM13920_IRQFLG1_APMERR_Msk )

#define AEM13920_STATUS0_OFFSET			(0x17U)	/**< STATUS0 register offset */
#define AEM13920_STATUS0_VOVDIS_Msk		(0x01U)	/**< VOVDIS field's mask */
#define AEM13920_STATUS0_VOVDIS_Pos		(0x00U)	/**< VOVDIS field's offset */
#define AEM13920_STATUS0_VCHRDY_Msk		(0x02U)	/**< VOVDIS field's mask */
#define AEM13920_STATUS0_VCHRDY_Pos		(0x01U)	/**< VOVDIS field's offset */
#define AEM13920_STATUS0_VOVCH_Msk		(0x04U)	/**< VOVCH field's mask */
#define AEM13920_STATUS0_VOVCH_Pos		(0x02U)	/**< VOVCH field's offset */
#define AEM13920_STATUS0_SRC1SRCLOW_Msk		(0x08U)	/**< SRC1SRCLOW field's mask */
#define AEM13920_STATUS0_SRC1SRCLOW_Pos		(0x03U)	/**< SRC1SRCLOW field's offset */
#define AEM13920_STATUS0_SRC2SRCLOW_Msk		(0x10U)	/**< SRC2SRCLOW field's mask */
#define AEM13920_STATUS0_SRC2SRCLOW_Pos		(0x04U)	/**< SRC2SRCLOW field's offset */
#define AEM13920_STATUS0_ALL_Msk		( AEM13920_STATUS0_VOVDIS_Msk	\
						| AEM13920_STATUS0_VCHRDY_Msk	\
						| AEM13920_STATUS0_VOVCH_Msk	\
						| AEM13920_STATUS0_SRC1SRCLOW_Msk\
						| AEM13920_STATUS0_SRC2SRCLOW_Msk)

#define AEM13920_STATUS1_OFFSET			(0x18U)	/**< STATUS1 register offset */
#define AEM13920_STATUS1_TEMPCOLDCH_Msk		(0x01U)	/**< TEMPCOLDCH field's mask */
#define AEM13920_STATUS1_TEMPCOLDCH_Pos		(0x00U)	/**< TEMPCOLDCH field's offset */
#define AEM13920_STATUS1_TEMPHOTCH_Msk		(0x02U)	/**< TEMPHOTCH field's mask */
#define AEM13920_STATUS1_TEMPHOTCH_Pos		(0x01U)	/**< TEMPHOTCH field's offset */
#define AEM13920_STATUS1_TEMPCOLDDIS_Msk	(0x04U)	/**< TEMPCOLDDIS field's mask */
#define AEM13920_STATUS1_TEMPCOLDDIS_Pos	(0x02U)	/**< TEMPCOLDDIS field's offset */
#define AEM13920_STATUS1_TEMPHOTDIS_Msk		(0x08U)	/**< TEMPHOTDIS field's mask */
#define AEM13920_STATUS1_TEMPHOTDIS_Pos		(0x03U)	/**< TEMPHOTDIS field's offset */
#define AEM13920_STATUS1_ALL_Msk		( AEM13920_STATUS1_TEMPCOLDCH_Msk	\
						| AEM13920_STATUS1_TEMPHOTCH_Msk	\
						| AEM13920_STATUS1_TEMPCOLDDIS_Msk	\
						| AEM13920_STATUS1_TEMPHOTDIS_Msk )

#define AEM13920_APM0SRC1_OFFSET		(0x19U)	/**< APM0SRC1 register offset */
#define AEM13920_APM0SRC2_OFFSET		(0x1CU)	/**< APM0SRC2 register offset */
#define AEM13920_APM0BUCK_OFFSET		(0x1FU)	/**< APM0BUCK register offset */
#define AEM13920_APM0x_DATA_Msk			(0xFFU)	/**< DATA field's mask */
#define AEM13920_APM0x_DATA_Pos			(0x00U)	/**< DATA field's offset */

#define AEM13920_APM1SRC1_OFFSET		(0x1AU)	/**< APM1SRC1 register's offset */
#define AEM13920_APM1SRC2_OFFSET		(0x1DU)	/**< APM1SRC2 register's offset */
#define AEM13920_APM1BUCK_OFFSET		(0x20U)	/**< APM1BUCK register's offset */
#define AEM13920_APM1x_DATA_Msk			(0xFFU)	/**< DATA field's mask */
#define AEM13920_APM1x_DATA_Pos			(0x00U)	/**< DATA field's offset */

#define AEM13920_APM2SRC1_OFFSET		(0x1BU)	/**< APM2SRC1 register's offset */
#define AEM13920_APM2SRC2_OFFSET		(0x1EU)	/**< APM2SRC2 register's offset */
#define AEM13920_APM2BUCK_OFFSET		(0x21U)	/**< APM2BUCK register's offset */
#define AEM13920_APM2x_DATA_Msk			(0x7FU)	/**< DATA field's mask (pulse counter) */
#define AEM13920_APM2x_DATA_Pos			(0x00U)	/**< DATA field's offset (pulse counter) */
#define AEM13920_APM2x_PC_Msk			(0x3FU)	/**< PC field's mask (pulse counter) */
#define AEM13920_APM2x_PM_DATA_Msk		(0x07U)	/**< DATA field's mask (power meter) */
#define AEM13920_APM2x_PM_SHIFT_Msk		(0x78U)	/**< SHIFT field's mask (power meter) */
#define AEM13920_APM2x_PM_SHIFT_Pos		(0x03U)	/**< SHIFT field's offset (power meter) */

#define AEM13920_APMERR_OFFSET			(0x22U)	/**< APMERR register offset */
#define AEM13920_APMERR_SRC1OV_Msk		(0x01U)	/**< SRC1OV field's mask */
#define AEM13920_APMERR_SRC1OV_Pos		(0x00U)	/**< SRC1OV field's offset */
#define AEM13920_APMERR_SRC1NVLD_Msk		(0x02U)	/**< SRC1NVLD field's mask */
#define AEM13920_APMERR_SRC1NVLD_Pos		(0x01U)	/**< SRC1NVLD field's offset */
#define AEM13920_APMERR_SRC2OV_Msk		(0x04U)	/**< SRC2OV field's mask */
#define AEM13920_APMERR_SRC2OV_Pos		(0x02U)	/**< SRC2OV field's offset */
#define AEM13920_APMERR_SRC2NVLD_Msk		(0x08U)	/**< SRC2NVLD field's mask */
#define AEM13920_APMERR_SRC2NVLD_Pos		(0x03U)	/**< SRC2NVLD field's offset */
#define AEM13920_APMERR_BUCKOV_Msk		(0x10U)	/**< BUCKOV field's mask */
#define AEM13920_APMERR_BUCKOV_Pos		(0x04U)	/**< BUCKOV field's offset */
#define AEM13920_APMERR_BUCKNVLD_Msk		(0x20U)	/**< BUCKNVLD field's mask */
#define AEM13920_APMERR_BUCKNVLD_Pos		(0x05U)	/**< BUCKNVLD field's offset */
#define AEM13920_APMERR_ALL_Msk			( AEM13920_APMERR_SRC1OV_Msk	\
						| AEM13920_APMERR_SRC1NVLD_Msk	\
						| AEM13920_APMERR_SRC2OV_Msk	\
						| AEM13920_APMERR_SRC2NVLD_Msk	\
						| AEM13920_APMERR_BUCKOV_Msk	\
						| AEM13920_APMERR_BUCKNVLD_Msk )

#define AEM13920_TEMP_OFFSET			(0x23U)	/**< TEMP register's offset */
#define AEM13920_TEMP_DATA_Msk			(0xFFU)	/**< DATA field's mask */
#define AEM13920_TEMP_DATA_Pos			(0x00U)	/**< DATA field's offset */

#define AEM13920_STO_OFFSET			(0x24U)	/**< STO register's offset */
#define AEM13920_STO_DATA_Msk			(0xFFU)	/**< DATA field's mask */
#define AEM13920_STO_DATA_Pos			(0x00U)	/**< DATA field's offset */

#define AEM13920_SRC1_OFFSET			(0x25U)	/**< SRC register's offset */
#define AEM13920_SRC2_OFFSET			(0x26U)	/**< SRC register's offset */
#define AEM13920_SRCx_DATA_Msk			(0xFFU)	/**< DATA field's mask */
#define AEM13920_SRCx_DATA_Pos			(0x00U)	/**< DATA field's offset */

#define AEM13920_CFG_OFFSET			AEM13920_SRC1REGU0_OFFSET
#define AEM13920_CFG_LENGTH			(0x14U)	/**< Length of the configuration area */

#ifdef __cplusplus
}
#endif

#endif /* AEM13920_REGISTERMAP_H_ */
