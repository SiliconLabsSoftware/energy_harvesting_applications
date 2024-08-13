/**
  ******************************************************************************
  * \file	AEM_I2C.h
  * \copyright	(c) e-peas S.A.  All rights reserved
  * \license	"e-peas_AEM_End_User_License_Agreement.txt"
  * \support 	support\@e-peas.com
  * 
  * \defgroup	AEM_Driver_Comm	I2C Communication interface
  * @{
  * 
  * \brief	I2C Communication interface's API
  * \details
  * 
  * This library provide the API of a generic communication interface that
  * can be used to communicate with an AEM.
  * 
  * It consists in four functions, that need to be implemented by the user
  * depending on their platform. The AEM_I2C_Template.c template file 
  * can be used in doing so. Or, if your device comes with CMSIS drivers,
  * the AEM_I2C_CMSIS.c file can be used instead, which uses the CMSIS_I2C 
  * driver to offer a portable implementation of the interface.
  * 
  * These functions all take a \ref AEM_i2c_cfg "data structure" as a 
  * mandatory argument. This data structure holds required information about
  * the I2C communication.
  * 
  * An example of initialization of such a data structure would look as follows: 
  * \code
  * AEM_i2c_cfg my_aem_i2c_config;
  * 
  * my_aem_i2c_config.usr_callback = my_callback;
  * my_aem_i2c_config.interface = 0;
  * my_aem_i2c_config.slaveAddress = 0x41;
  * \endcode
  * ... with \c my_callback a function the user wants to be called on driver 
  * events (use NULL when there is no need for such a callback)
  * 
  * When the AEM_I2C_CMSIS.c template is used, the \c interface
  * field shall be set with the address of the driver's access structure to be 
  * used:
  * \code
  * my_aem_i2c_config.interface = (uint32_t) &Driver_I2C0;
  * \endcode
  * 
  * \version	1.0.0
  * 	- Initial release
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
#ifndef AEM_I2C_H_
#define AEM_I2C_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @defgroup	AEM_I2C_ERR_CODE	Status/Error code
  * @{
  */
/**
  * @brief 	Operation succeeded
  * 
  * @details 	The function execution completed without error
  */
#define AEM_I2C_OK 			(0)
/**
  * @brief 	Unspecified I2C error
  * 
  * @details 	An unspecified error occurred during I2C transaction
  */
#define AEM_I2C_ERR_UNSPECIFIED		(-100)
/**
  * @brief 	Slave address error
  * 
  * @details 	An error occurred because of an invalid slave address
  */
#define AEM_I2C_ERR_SLV_ADDR 		(-101)
/**
  * @brief 	I2C not ready error
  * 
  * @details	An error occurred because the AEM was in reset state
  * 		and therefore was not ready to initiate an I2C transaction
  */
#define AEM_I2C_ERR_NOT_READY		(-102)
/**
  * @brief 	I2C configuration missing error
  * 
  * @details	An error occurred because no valid configuration has been
  * 		passed to the function
  */
#define AEM_I2C_ERR_MISSING_CFG		(-103)
/**
  * @brief 	I2C interface invalid
  * 
  * @details	An error occurred because the specified I2C interface is not
  * 		supported
  */
#define AEM_I2C_ERR_INTERFACE		(-104)
/** @} *//* endgroup AEM_I2C_ERR_CODE */


/**
  * @struct 	AEM_i2c_cfg
  * @brief 	Communication interface's required information
  */
typedef struct _AEM_i2c_cfg {
	/** 
	 * @brief 	A function callback
	 * 
	 * @note 	Use NULL if no callback should be used
	 */
	void (*usr_callback) (int32_t event);
	/** 
	 * @brief 	The index of the interface to be used 
	 * 
	 * When multiple I2C interfaces are available, set this field with the 
	 * index of the interface to be used 
	 */
	uint32_t interface;
	/**
	  * @brief 	The AEM's slave address
	  * 
	  * Set this field with the device's slave address
	  */
	uint32_t slaveAddress;
} AEM_i2c_cfg;

/**
  * @brief 	Initialize the I2C communication interface
  * 
  * @param 	commInfo	Communication interface's required information 
  * @return 	int32_t 	\ref AEM_I2C_ERR_CODE
  * 
  * @pre	\p commInfo is not NULL
  * @pre	\p commInfo->interface refers to a valid communication interface
  * @pre	\p commInfo->slaveAddress refers to a valid slave address
  * @post	The communication interface is initialized and ready for transfers
  */
int32_t AEM_I2C_Initialize(const AEM_i2c_cfg *commInfo);

/**
  * @brief 	Read the content of a number of consecutive registers from the AEM	
  * 
  * @param 	commInfo	Communication interface's required information 
  * @param 	offset 		Address of the first register to read
  * @param 	len 		Number of consecutive registers to read
  * @param 	buffer 		Pointer to a buffer to store the read registers' 
  * 				values
  * @return 	int32_t 	\ref AEM_I2C_ERR_CODE
  * 
  * @pre	\p commInfo is not NULL
  * @pre	\p commInfo->interface refers to a valid communication interface
  * @pre	\p commInfo->slaveAddress refers to a valid slave address
  * @pre	\p offset corresponds to the offset of a readable register
  * @pre	\p buffer is not NULL and can hold \p len bytes
  * @post	\p buffer holds the values of n consecutive readable 
  * 		registers, with 0 < n <= x <= \p len and x the limit of the 
  * 		readable area starting from \p offset
  * @note	See the datasheet, section 8.5. for more information about
  * 		the specification of a read register operation
  */
int32_t AEM_I2C_ReadRegisters(	const AEM_i2c_cfg *commInfo, 	\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer);

/**
  * @brief 	Write a number of consecutive registers from the AEM
  * 
  * @param 	commInfo	Communication interface's required information 
  * @param 	offset 		Address of the first register to write
  * @param 	len 		Number of consecutive registers to write
  * @param 	buffer 		Pointer to a buffer holding the values to be 
  * 				written
  * @return	int32_t 	\ref AEM_I2C_ERR_CODE
  * 
  * @pre	\p commInfo is not NULL
  * @pre	\p commInfo->interface refers to a valid communication interface
  * @pre	\p commInfo->slaveAddress refers to a valid slave address
  * @pre	\p offset corresponds to the offset of a writable register
  * @pre	\p buffer is not NULL and holds \p len bytes
  * @post	The AEM's registers hold the values set in \p buffer
  * @post	The registers \p offset to \p offset + n are set with the 
  * 		values stored in \p buffer , with 0 < n <= x <= \p len and x the 
  * 		limit of the writable area starting from \p offset
  * @note	See the datasheet, section 8.5. for more information about
  * 		the specification of a write register operation
  */
int32_t AEM_I2C_WriteRegisters(	const AEM_i2c_cfg *commInfo, 	\
					uint32_t offset, uint32_t len, 		\
					const uint8_t *buffer);

/**
  * @brief 	De initialize the I2C communication interface
  * 
  * @param 	commInfo	Communication interface's required information 
  * @return 	int32_t 	\ref AEM_I2C_ERR_CODE
  * 
  * @pre	\p commInfo is not NULL
  * @pre	\p commInfo->interface refers to a valid communication interface
  * @pre	\p commInfo->slaveAddress refers to a valid slave address
  * @post	All the resources allocated to the communication interface
  * 		are released
  */
int32_t AEM_I2C_Deinitialize(const AEM_i2c_cfg *commInfo);

#ifdef __cplusplus
}
#endif

/** @} *//* endgroup AEM_Driver_Comm */
#endif /* AEM_Driver_Comm */
