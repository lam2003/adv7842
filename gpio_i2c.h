
#ifndef _GPIO_I2C_H
#define _GPIO_I2C_H

#include "adv7842_3routes_i2c.h"

#define GPIO_I2C_READ   0x01
#define GPIO_I2C_WRITE  0x03
typedef unsigned char		byte;

unsigned char gpio_i2c_read(unsigned char devaddress, unsigned char address);
void gpio_i2c_write(unsigned char devaddress, unsigned char address, unsigned char value);
byte siiReadSegmentBlockEDID(byte SlaveAddr, byte Segment, byte Offset, byte *Buffer, byte Length);

#define gpio_i2c_write_adv7842 gpio_i2c_write_adv7842_2
#define gpio_i2c_read_adv7842 gpio_i2c_read_adv7842_2


#endif

