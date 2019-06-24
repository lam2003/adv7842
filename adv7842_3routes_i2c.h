/*
 * adv7842_3routes_i2c.h
 *
 *  Created on: 2015-3-1
 *      Author: eternal
 */

#ifndef adv7842_3routes_I2C_H_
#define adv7842_3routes_I2C_H_

struct adv7842_3routes_i2c_transmission{
	unsigned char slaveaddress;
	unsigned char group;
};


void gpio_i2c_write_adv7842_1(unsigned char devaddress, unsigned char address, unsigned char data);
unsigned char gpio_i2c_read_adv7842_1(unsigned char devaddress, unsigned char address);

void gpio_i2c_write_adv7842_2(unsigned char devaddress, unsigned char address, unsigned char data);
unsigned char gpio_i2c_read_adv7842_2(unsigned char devaddress, unsigned char address);


#endif /* adv7842_3routes_I2C_H_ */
