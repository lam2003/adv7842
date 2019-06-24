/*
 * adv7842.c
 *
 *  Created on: 2014-6-6
 *      Author: eternal
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>

#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>

#include "adv7842_define.h"
#include "gpio_i2c.h"
#include "adv7842.h"
#include "rx_detect.h"

static void adv7842_i2c_initialization(void)
{
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xff, 0x80);
	mdelay(500);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xfa, EDID);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xfe, VDP);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf2, SDPIO_MAP);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf3, AVLINK);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf4, CEC);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf5, INFOFRAME);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf8, AFE);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf1, SDP_MAP);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xf9, KSV);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xfb, HDMI);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0xfd, CP);
}

static void adv7842_HDMI_input_1920x1080p60(void)
{
	adv7842_i2c_initialization();

	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x00, 0x1f);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x01, 0x05);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x02, 0x0);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x03, 0x80);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x04, 0x62);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x05, 0x2c);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x06, 0xa7);

	gpio_i2c_write_adv7842(HDMI, 0xc1, 0xff);
	gpio_i2c_write_adv7842(HDMI, 0xc2, 0xff);
	gpio_i2c_write_adv7842(HDMI, 0xc3, 0xff);
	gpio_i2c_write_adv7842(HDMI, 0xc4, 0xff);
	gpio_i2c_write_adv7842(HDMI, 0xc5, 0x00);
	gpio_i2c_write_adv7842(HDMI, 0xc6, 0x00);
	gpio_i2c_write_adv7842(HDMI, 0xc0, 0xff);

	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x0c, 0x40);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x14, 0x3f);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x15, 0x80);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x19, 0x83);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x33, 0x43);

	gpio_i2c_write_adv7842(CP, 0xba, 0x01);
	gpio_i2c_write_adv7842(CP, 0x3e, 0x00);
	gpio_i2c_write_adv7842(CP, 0x6c, 0x00);

	gpio_i2c_write_adv7842(AFE, 0x00, 0xff);
	gpio_i2c_write_adv7842(AFE, 0x01, 0xfe);
	gpio_i2c_write_adv7842(AFE, 0xb5, 0x01);

	gpio_i2c_write_adv7842(HDMI, 0x00, 0x32);
	gpio_i2c_write_adv7842(HDMI, 0x01, 0x18);
	gpio_i2c_write_adv7842(HDMI, 0x0d, 0x34);
	gpio_i2c_write_adv7842(HDMI, 0x1a, 0x8a);
	gpio_i2c_write_adv7842(HDMI, 0x3d, 0x10);
	gpio_i2c_write_adv7842(HDMI, 0x44, 0x85);
	gpio_i2c_write_adv7842(HDMI, 0x46, 0x1f);
	gpio_i2c_write_adv7842(HDMI, 0x60, 0x88);
	gpio_i2c_write_adv7842(HDMI, 0x61, 0x88);
	gpio_i2c_write_adv7842(HDMI, 0x6c, 0x18);
	gpio_i2c_write_adv7842(HDMI, 0x57, 0xb6);
	gpio_i2c_write_adv7842(HDMI, 0x58, 0x03);
	gpio_i2c_write_adv7842(HDMI, 0x67, 0x20);
	gpio_i2c_write_adv7842(HDMI, 0x75, 0x10);
	gpio_i2c_write_adv7842(HDMI, 0x85, 0x1f);
	gpio_i2c_write_adv7842(HDMI, 0x87, 0x70);
	gpio_i2c_write_adv7842(HDMI, 0x89, 0x04);
	gpio_i2c_write_adv7842(HDMI, 0x8a, 0x1e);
	gpio_i2c_write_adv7842(HDMI, 0x93, 0x04);
	gpio_i2c_write_adv7842(HDMI, 0x94, 0x1e);
	gpio_i2c_write_adv7842(HDMI, 0x9d, 0x02);
	gpio_i2c_write_adv7842(HDMI, 0x99, 0xa1);
	gpio_i2c_write_adv7842(HDMI, 0x9b, 0x09);
	gpio_i2c_write_adv7842(HDMI, 0xc9, 0x01);
}

static void adv7842_VGA_input_1920x1080p60(void)
{
	adv7842_i2c_initialization();

	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x00, 0x1e);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x01, 0x81);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x02, 0x10);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x03, 0x80);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x04, 0x62);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x05, 0x2c);

	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x0c, 0x40);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x14, 0x3f);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x15, 0xb0);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x19, 0x81);
	gpio_i2c_write_adv7842(DEVICE_ADDR, 0x33, 0x40);
	gpio_i2c_write_adv7842(AFE, 0x05, 0x0f);
	gpio_i2c_write_adv7842(AFE, 0x06, 0x20);
	gpio_i2c_write_adv7842(AFE, 0x07, 0xff);
	gpio_i2c_write_adv7842(CP, 0x73, 0xea);
	gpio_i2c_write_adv7842(CP, 0x74, 0x8a);
	gpio_i2c_write_adv7842(CP, 0x75, 0xa2);
	gpio_i2c_write_adv7842(CP, 0x76, 0xa8);
	gpio_i2c_write_adv7842(CP, 0x85, 0x0b);
	gpio_i2c_write_adv7842(CP, 0xc3, 0x39);
	gpio_i2c_write_adv7842(AFE, 0x0c, 0x1f);
	gpio_i2c_write_adv7842(AFE, 0x12, 0x63);
	gpio_i2c_write_adv7842(AFE, 0x00, 0x80);
	gpio_i2c_write_adv7842(AFE, 0x02, 0x00);
	gpio_i2c_write_adv7842(AFE, 0xc8, 0x35);
}

static void adv7842_set_HDMI_port(ADV7842_HDMI_PORT port)
{
	if (port == HDMI_PORT_A)
	{
		gpio_i2c_write_adv7842(HDMI, 0x00, 0x32);
	}
	else if (port == HDMI_PORT_B)
	{
		gpio_i2c_write_adv7842(HDMI, 0x00, 0x33);
	}
}

static void adv7842_set_output_format(ADV7842_MODE mode)
{
	if (mode == MODE_VGA)
	{
		adv7842_VGA_input_1920x1080p60();
	}
	else if (mode == MODE_HDMI)
	{
		adv7842_HDMI_input_1920x1080p60();
		adv7842_set_HDMI_port(HDMI_PORT_A);
	}
}

static long adv7842_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{
	switch (cmd)
	{
	case CMD_DEVICE_INIT:
	{
		ADV7842_MODE mode = (ADV7842_MODE)data;
		adv7842_set_output_format(mode);
		break;
	}
	case CMD_QUERY_TIMINGS:
	{
		struct bt_timings *bt = (struct bt_timings *)data;
		adv7842_query_timings(bt);
		break;
	}
	default:
	{
		printk(KERN_ALERT "adv7842_ioctl cmd analysis failed! not find this cmd 0x%x!\n", cmd);
		break;
	}
	}
	return 0;
}
static int adv7842_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int adv7842_close(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations adv7842_fops = {
	.owner = THIS_MODULE,
	.open = adv7842_open,
	.release = adv7842_close,
	.unlocked_ioctl = adv7842_ioctl,
};

static struct miscdevice adv7842_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "adv7842",
	.fops = &adv7842_fops,
};

static int __init adv7842_init(void)
{
	int ret;
	ret = misc_register(&adv7842_dev);
	if (0 != ret)
	{
		return -1;
	}
	return 0;
}

static void __exit adv7842_exit(void)
{
	misc_deregister(&adv7842_dev);
}

module_init(adv7842_init);
module_exit(adv7842_exit);

MODULE_LICENSE("GPL");
