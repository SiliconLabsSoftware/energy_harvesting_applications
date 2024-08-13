#include <stdint.h>
#include "AEM_I2C.h"

int32_t AEM_I2C_Initialize(const AEM_i2c_cfg *commInfo)
{
	/// \todo To be implemented depending on the user's platform
}

int32_t AEM_I2C_ReadRegisters(	const AEM_i2c_cfg *commInfo, 			\
				uint32_t offset, uint32_t len, uint8_t *buffer)
{
	/// \todo To be implemented depending on the user's platform
}

int32_t AEM_I2C_WriteRegisters(	const AEM_i2c_cfg *commInfo, 			\
				uint32_t offset, uint32_t len, 			\
				const uint8_t *buffer)
{
	/// \todo To be implemented depending on the user's platform
}

int32_t AEM_I2C_Deinitialize(const AEM_i2c_cfg *commInfo)
{
	/// \todo To be implemented depending on the user's platform
}
