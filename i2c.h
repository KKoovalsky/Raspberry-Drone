#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include <inttypes.h>
	
void i2cGetXYZ(int fd, int regAddr, uint8_t *buf);
void i2cGetXYZInv(int fd, int regAddr, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */

