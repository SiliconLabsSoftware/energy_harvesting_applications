#include <stdint.h>
#include <string.h>
#include "AEM_I2C.h"
#include "sl_i2cspm.h"

int32_t AEM_I2C_Initialize(const AEM_i2c_cfg *commInfo)
{
  (void) commInfo;
  return AEM_I2C_OK;
}

int32_t AEM_I2C_ReadRegisters(const AEM_i2c_cfg *commInfo,
                              uint32_t offset,
                              uint32_t len,
                              uint8_t *buffer)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t cmd = (uint8_t)offset;
  sl_i2cspm_t *sl_i2cspm_instance = (sl_i2cspm_t *) commInfo->interface;

  seq.addr = commInfo->slaveAddress << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  /* Select command to issue */
  seq.buf[0].data = (uint8_t *)&cmd;
  seq.buf[0].len = 1;

  /* Select location/length of data to be read */
  seq.buf[1].data = buffer;
  seq.buf[1].len = (uint16_t)len;

  if (I2CSPM_Transfer(sl_i2cspm_instance, &seq) != i2cTransferDone) {
    return AEM_I2C_ERR_UNSPECIFIED;
  }

  return AEM_I2C_OK;
}

int32_t AEM_I2C_WriteRegisters(const AEM_i2c_cfg *commInfo,
                               uint32_t offset,
                               uint32_t len,
                               const uint8_t *buffer)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t payload[len + 1]; // buffer + cmd offset
  sl_i2cspm_t *sl_i2cspm_instance = (sl_i2cspm_t *) commInfo->interface;

  payload[0] = (uint8_t)offset;
  memcpy(&payload[1], buffer, len);

  seq.addr = commInfo->slaveAddress << 1;
  seq.flags = I2C_FLAG_WRITE;

  /* Select command to issue */
  seq.buf[0].data = payload;
  seq.buf[0].len = (uint16_t)len + 1;

  if (I2CSPM_Transfer(sl_i2cspm_instance, &seq) != i2cTransferDone) {
    return AEM_I2C_ERR_UNSPECIFIED;
  }

  return AEM_I2C_OK;
}

int32_t AEM_I2C_Deinitialize(const AEM_i2c_cfg *commInfo)
{
  (void) commInfo;
  return AEM_I2C_OK;
}
