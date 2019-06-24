#ifndef PRIVATE_I2C_H_
#define PRIVATE_I2C_H_

#define GPIO_BASE 0x200f0000
#define GPIO_18_BASE 0x20270000
#define GPIO_DIR      IO_ADDRESS(GPIO_18_BASE + 0x400)

#define SCL_MUTIPLEX	IO_ADDRESS(GPIO_BASE + 0x254) //0
#define SDA_MUTIPLEX	IO_ADDRESS(GPIO_BASE + 0x258) //0

#define SCL                 (1 << 3)    /* GPIO18_3 */
#define SDA                 (1 << 4)    /* GPIO18_4 */
#define GPIO_I2C_SCL_REG    IO_ADDRESS(GPIO_18_BASE + (SCL << 2))
#define GPIO_I2C_SDA_REG    IO_ADDRESS(GPIO_18_BASE + (SDA << 2))

#define HW_REG(reg)         *((volatile unsigned int *)(reg))
#define DELAY(us)           udelay(us*10)

void i2c_clr(unsigned char whichline) {
	unsigned char regvalue;

	if (whichline == SCL) {
		regvalue = HW_REG(GPIO_DIR);
		regvalue |= SCL;
		HW_REG(GPIO_DIR) = regvalue;

		HW_REG(GPIO_I2C_SCL_REG) = 0;
		return;
	} else if (whichline == SDA) {
		regvalue = HW_REG(GPIO_DIR);
		regvalue |= SDA;
		HW_REG(GPIO_DIR) = regvalue;

		HW_REG(GPIO_I2C_SDA_REG) = 0;
		return;
	} else {
		printk("Error input.\n");
		return;
	}

}

void i2c_set(unsigned char whichline) {
	unsigned char regvalue;

	if (whichline == SCL) {
		regvalue = HW_REG(GPIO_DIR);
		regvalue |= SCL;
		HW_REG(GPIO_DIR) = regvalue;

		HW_REG(GPIO_I2C_SCL_REG) = SCL;
		return;
	} else if (whichline == SDA) {
		regvalue = HW_REG(GPIO_DIR);
		regvalue |= SDA;
		HW_REG(GPIO_DIR) = regvalue;

		HW_REG(GPIO_I2C_SDA_REG) = SDA;
		return;
	} else {
		printk("Error input.\n");
		return;
	}
}

unsigned char i2c_data_read(void) {
	unsigned char regvalue;

	regvalue = HW_REG(GPIO_DIR);
	regvalue &= (~SDA);
	HW_REG(GPIO_DIR) = regvalue;
	DELAY(1);

	regvalue = HW_REG(GPIO_I2C_SDA_REG);
	if ((regvalue & SDA) != 0)
		return 1;
	else
		return 0;
}

void i2c_start_bit(void) {
	DELAY(1);
	i2c_set(SDA);
	i2c_set(SCL);
	DELAY(1);
	i2c_clr(SDA);
	DELAY(1);
}

void i2c_stop_bit(void) {
	/* clock the ack */
	DELAY(1);
	i2c_set(SCL);
	DELAY(1);
	i2c_clr(SCL);

	/* actual stop bit */
	DELAY(1);
	i2c_clr(SDA);
	DELAY(1);
	i2c_set(SCL);
	DELAY(1);
	i2c_set(SDA);
	DELAY(1);
}

void i2c_send_byte(unsigned char c) {
	int i;
	for (i = 0; i < 8; i++) {
		DELAY(1);
		i2c_clr(SCL);
		DELAY(1);

		if (c & (1 << (7 - i)))
			i2c_set(SDA);
		else
			i2c_clr(SDA);

		DELAY(1);
		i2c_set(SCL);
		DELAY(1);
		i2c_clr(SCL);
	}
	DELAY(1);
	// i2c_set(SDA);
}

unsigned char i2c_receive_byte(void) {
	int j = 0;
	int i;
	unsigned char regvalue;

	for (i = 0; i < 8; i++) {
		DELAY(1);
		i2c_clr(SCL);
		DELAY(1);
		i2c_set(SCL);

		regvalue = HW_REG(GPIO_DIR);
		regvalue &= (~SDA);
		HW_REG(GPIO_DIR) = regvalue;
		DELAY(1);

		if (i2c_data_read())
			j += (1 << (7 - i));

		DELAY(1);
		i2c_clr(SCL);
	}
	DELAY(1);
	// i2c_clr(SDA);
	// DELAY(1);

	return j;
}

int i2c_receive_ack(void) {
	int nack;
	unsigned char regvalue;

	DELAY(1);

	regvalue = HW_REG(GPIO_DIR);
	regvalue &= (~SDA);
	HW_REG(GPIO_DIR) = regvalue;

	DELAY(1);
	i2c_clr(SCL);
	DELAY(1);
	i2c_set(SCL);
	DELAY(1);

	nack = i2c_data_read();

	DELAY(1);
	i2c_clr(SCL);
	DELAY(1);
	//  i2c_set(SDA);
	//  DELAY(1);

	if (nack == 0)
		return 1;

	return 0;
}

void i2c_send_ack(void) {
	DELAY(1);
	i2c_clr(SCL);
	DELAY(1);
	i2c_set(SDA);
	DELAY(1);
	i2c_set(SCL);
	DELAY(1);
	i2c_clr(SCL);
	DELAY(1);
	i2c_clr(SDA);
	DELAY(1);
}

static unsigned char gpio_i2c_read_adv7842(unsigned char devaddress,unsigned char address) {
	int rxdata;

	i2c_start_bit();
	i2c_send_byte((unsigned char) (devaddress));
	i2c_receive_ack();
	i2c_send_byte(address);
	i2c_receive_ack();
	i2c_start_bit();
	i2c_send_byte((unsigned char) (devaddress) | 1);
	i2c_receive_ack();
	rxdata = i2c_receive_byte();
	//i2c_send_ack();
	i2c_stop_bit();

	return rxdata;
}

static void gpio_i2c_write_adv7842(unsigned char devaddress, unsigned char address,
		unsigned char data) {
	i2c_start_bit();
	i2c_send_byte((unsigned char) (devaddress));
	i2c_receive_ack();
	i2c_send_byte(address);
	i2c_receive_ack();
	i2c_send_byte(data);
	// i2c_receive_ack();//add by hyping for tw2815
	i2c_stop_bit();
}

void i2c_bus_initialization(void){
	HW_REG(SCL_MUTIPLEX)=0;
	HW_REG(SDA_MUTIPLEX)=0;
}

#endif /* PRIVATE_I2C_H_ */
