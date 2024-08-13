#include <stdint.h>
#include "AEM_I2C.h"
#include "CMSIS_I2C.h"

int32_t AEM_I2C_Initialize(const AEM_i2c_cfg *commInfo)
{
	int32_t errno;
	ARM_DRIVER_I2C *master = (ARM_DRIVER_I2C*) commInfo->interface;

	errno = master->Initialize((void (*) (uint32_t)) commInfo->usr_callback);

	if (errno == ARM_DRIVER_OK) {
		errno = master->PowerControl(ARM_POWER_FULL);
	}
	if (errno == ARM_DRIVER_OK) {
		errno = master->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
	}
	if (errno == ARM_DRIVER_OK) {
		errno = master->Control(ARM_I2C_BUS_CLEAR, 0);
	}
	if (errno != ARM_DRIVER_OK) {
		return AEM_I2C_ERR_UNSPECIFIED;
	}
	return AEM_I2C_OK;
}

int32_t AEM_I2C_Deinitialize(const AEM_i2c_cfg *commInfo)
{
	int32_t errno;
	ARM_DRIVER_I2C *master = (ARM_DRIVER_I2C*) commInfo->interface;

	errno = master->PowerControl(ARM_POWER_OFF);
	if (errno == ARM_DRIVER_OK) {
		errno = master->Uninitialize();
	}
	if (errno != ARM_DRIVER_OK) {
		return AEM_I2C_ERR_UNSPECIFIED;
	}
	return AEM_I2C_OK;
}

int32_t AEM_I2C_ReadRegisters(	const AEM_i2c_cfg *commInfo, 	\
					uint32_t offset, uint32_t len, 		\
					uint8_t *buffer)
{
	int32_t errno;
	ARM_DRIVER_I2C *master = (ARM_DRIVER_I2C*) commInfo->interface;

	errno = master->MasterTransmit(	commInfo->slaveAddress,			\
					((uint8_t*) &offset), 1, true);
	while (master->GetStatus().busy);

	if (errno == ARM_DRIVER_OK) {
		errno = master->MasterReceive(	commInfo->slaveAddress, buffer, \
						len, false);
		while (master->GetStatus().busy);
	}

	if (errno != ARM_DRIVER_OK) {
		return AEM_I2C_ERR_UNSPECIFIED;
	}
	return AEM_I2C_OK;
}

int32_t AEM_I2C_WriteRegisters(	const AEM_i2c_cfg *commInfo, 			\
				uint32_t offset, uint32_t len, 			\
				const uint8_t *buffer)
{
	int32_t errno = ARM_DRIVER_OK;
	ARM_DRIVER_I2C *master = (ARM_DRIVER_I2C*) commInfo->interface;
	uint32_t i = 0;
	uint8_t __buffer[len + 1];

	__buffer[0] = (uint8_t) offset;
	for (i = 0; i < len; i++) {
		__buffer[i+1] = buffer[i];
	}

	errno = master->MasterTransmit(	commInfo->slaveAddress, __buffer, 	\
					(len + 1U), false);
	while (master->GetStatus().busy);

	if (errno != ARM_DRIVER_OK) {
		return AEM_I2C_ERR_UNSPECIFIED;
	}
	return AEM_I2C_OK;
}
