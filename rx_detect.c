#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>

#include "rx_detect_define.h"
#include "gpio_i2c.h"

const struct bt_timings bt_timings_presets[] = {
	V4L2_DV_BT_CEA_640X480P59_94,
	V4L2_DV_BT_CEA_720X480I59_94,
	V4L2_DV_BT_CEA_720X480P59_94,
	V4L2_DV_BT_CEA_720X576I50,
	V4L2_DV_BT_CEA_720X576P50,
	V4L2_DV_BT_CEA_1280X720P24,
	V4L2_DV_BT_CEA_1280X720P25,
	V4L2_DV_BT_CEA_1280X720P30,
	V4L2_DV_BT_CEA_1280X720P50,
	V4L2_DV_BT_CEA_1280X720P60,
	V4L2_DV_BT_CEA_1920X1080P24,
	V4L2_DV_BT_CEA_1920X1080P25,
	V4L2_DV_BT_CEA_1920X1080P30,
	V4L2_DV_BT_CEA_1920X1080I50,
	V4L2_DV_BT_CEA_1920X1080P50,
	V4L2_DV_BT_CEA_1920X1080I60,
	V4L2_DV_BT_CEA_1920X1080P60,
	V4L2_DV_BT_DMT_640X350P85,
	V4L2_DV_BT_DMT_640X400P85,
	V4L2_DV_BT_DMT_720X400P85,
	V4L2_DV_BT_DMT_640X480P72,
	V4L2_DV_BT_DMT_640X480P75,
	V4L2_DV_BT_DMT_640X480P85,
	V4L2_DV_BT_DMT_800X600P56,
	V4L2_DV_BT_DMT_800X600P60,
	V4L2_DV_BT_DMT_800X600P72,
	V4L2_DV_BT_DMT_800X600P75,
	V4L2_DV_BT_DMT_800X600P85,
	V4L2_DV_BT_DMT_800X600P120_RB,
	V4L2_DV_BT_DMT_848X480P60,
	V4L2_DV_BT_DMT_1024X768I43,
	V4L2_DV_BT_DMT_1024X768P60,
	V4L2_DV_BT_DMT_1024X768P70,
	V4L2_DV_BT_DMT_1024X768P75,
	V4L2_DV_BT_DMT_1024X768P85,
	V4L2_DV_BT_DMT_1024X768P120_RB,
	V4L2_DV_BT_DMT_1152X864P75,
	V4L2_DV_BT_DMT_1280X768P60_RB,
	V4L2_DV_BT_DMT_1280X768P60,
	V4L2_DV_BT_DMT_1280X768P75,
	V4L2_DV_BT_DMT_1280X768P85,
	V4L2_DV_BT_DMT_1280X768P120_RB,
	V4L2_DV_BT_DMT_1280X800P60_RB,
	V4L2_DV_BT_DMT_1280X800P60,
	V4L2_DV_BT_DMT_1280X800P75,
	V4L2_DV_BT_DMT_1280X800P85,
	V4L2_DV_BT_DMT_1280X800P120_RB,
	V4L2_DV_BT_DMT_1280X960P60,
	V4L2_DV_BT_DMT_1280X960P85,
	V4L2_DV_BT_DMT_1280X960P120_RB,
	V4L2_DV_BT_DMT_1280X1024P60,
	V4L2_DV_BT_DMT_1280X1024P75,
	V4L2_DV_BT_DMT_1280X1024P85,
	V4L2_DV_BT_DMT_1280X1024P120_RB,
	V4L2_DV_BT_DMT_1360X768P60,
	V4L2_DV_BT_DMT_1360X768P120_RB,
	V4L2_DV_BT_DMT_1366X768P60,
	V4L2_DV_BT_DMT_1366X768P60_RB,
	V4L2_DV_BT_DMT_1400X1050P60_RB,
	V4L2_DV_BT_DMT_1400X1050P60,
	V4L2_DV_BT_DMT_1400X1050P75,
	V4L2_DV_BT_DMT_1400X1050P85,
	V4L2_DV_BT_DMT_1400X1050P120_RB,
	V4L2_DV_BT_DMT_1440X900P60_RB,
	V4L2_DV_BT_DMT_1440X900P60,
	V4L2_DV_BT_DMT_1440X900P75,
	V4L2_DV_BT_DMT_1440X900P85,
	V4L2_DV_BT_DMT_1440X900P120_RB,
	V4L2_DV_BT_DMT_1600X900P60_RB,
	V4L2_DV_BT_DMT_1600X1200P60,
	V4L2_DV_BT_DMT_1600X1200P65,
	V4L2_DV_BT_DMT_1600X1200P70,
	V4L2_DV_BT_DMT_1600X1200P75,
	V4L2_DV_BT_DMT_1600X1200P85,
	V4L2_DV_BT_DMT_1600X1200P120_RB,
	V4L2_DV_BT_DMT_1680X1050P60_RB,
	V4L2_DV_BT_DMT_1680X1050P60,
	V4L2_DV_BT_DMT_1680X1050P75,
	V4L2_DV_BT_DMT_1680X1050P85,
	V4L2_DV_BT_DMT_1680X1050P120_RB,
	V4L2_DV_BT_DMT_1792X1344P60,
	V4L2_DV_BT_DMT_1792X1344P75,
	V4L2_DV_BT_DMT_1792X1344P120_RB,
	V4L2_DV_BT_DMT_1856X1392P60,
	V4L2_DV_BT_DMT_1856X1392P75,
	V4L2_DV_BT_DMT_1856X1392P120_RB,
	V4L2_DV_BT_DMT_1920X1200P60_RB,
	V4L2_DV_BT_DMT_1920X1200P60,
	V4L2_DV_BT_DMT_1920X1200P75,
	V4L2_DV_BT_DMT_1920X1200P85,
	V4L2_DV_BT_DMT_1920X1200P120_RB,
	V4L2_DV_BT_DMT_1920X1440P60,
	V4L2_DV_BT_DMT_1920X1440P75,
	V4L2_DV_BT_DMT_1920X1440P120_RB,
	V4L2_DV_BT_DMT_2048X1152P60_RB,
	V4L2_DV_BT_DMT_2560X1600P60_RB,
	V4L2_DV_BT_DMT_2560X1600P60,
	V4L2_DV_BT_DMT_2560X1600P75,
	V4L2_DV_BT_DMT_2560X1600P85,
	V4L2_DV_BT_DMT_2560X1600P120_RB,
	V4L2_DV_BT_CEA_3840X2160P24,
	V4L2_DV_BT_CEA_3840X2160P25,
	V4L2_DV_BT_CEA_3840X2160P30,
	V4L2_DV_BT_CEA_3840X2160P50,
	V4L2_DV_BT_CEA_3840X2160P60,
	V4L2_DV_BT_CEA_4096X2160P24,
	V4L2_DV_BT_CEA_4096X2160P25,
	V4L2_DV_BT_CEA_4096X2160P30,
	V4L2_DV_BT_CEA_4096X2160P50,
	V4L2_DV_BT_DMT_4096X2160P59_94_RB,
	V4L2_DV_BT_CEA_4096X2160P60,
	{}};

static inline unsigned hblanking(const struct bt_timings *t)
{
	return V4L2_DV_BT_BLANKING_WIDTH(t);
}

static inline unsigned htotal(const struct bt_timings *t)
{
	return V4L2_DV_BT_FRAME_WIDTH(t);
}

static inline unsigned vblanking(const struct bt_timings *t)
{
	return V4L2_DV_BT_BLANKING_HEIGHT(t);
}

static inline unsigned vtotal(const struct bt_timings *t)
{
	return V4L2_DV_BT_FRAME_HEIGHT(t);
}

/** detect_cvt - detect if the given timings follow the CVT standard
* @frame_height - the total height of the frame (including blanking) in lines.
* @hfreq - the horizontal frequency in Hz.
* @vsync - the height of the vertical sync in lines.
* @polarities - the horizontal and vertical polarities (same as struct
*		v4l2_bt_timings polarities).
* @fmt - the resulting timings.
*
* This function will attempt to detect if the given values correspond to a
* valid CVT format. If so, then it will return true, and fmt will be filled
* in with the found CVT timings.
*
* TODO: VESA defined a new version 2 of their reduced blanking
* formula. Support for that is currently missing in this CVT
* detection function.
*/
bool detect_cvt(unsigned frame_height, unsigned hfreq, unsigned vsync,
				u32 polarities, struct bt_timings *bt)
{
	int v_fp, v_bp, h_fp, h_bp, hsync;
	int frame_width, image_height, image_width;
	bool reduced_blanking;
	unsigned pix_clk;

	if (vsync < 4 || vsync > 7)
		return false;

	if (polarities == V4L2_DV_VSYNC_POS_POL)
		reduced_blanking = false;
	else if (polarities == V4L2_DV_HSYNC_POS_POL)
		reduced_blanking = true;
	else
		return false;

	/* Vertical */
	if (reduced_blanking)
	{
		v_fp = CVT_RB_V_FPORCH;
		v_bp = (CVT_RB_MIN_V_BLANK * hfreq + 1999999) / 1000000;
		v_bp -= vsync + v_fp;

		if (v_bp < CVT_RB_MIN_V_BPORCH)
			v_bp = CVT_RB_MIN_V_BPORCH;
	}
	else
	{
		v_fp = CVT_MIN_V_PORCH_RND;
		v_bp = (CVT_MIN_VSYNC_BP * hfreq + 1999999) / 1000000 - vsync;

		if (v_bp < CVT_MIN_V_BPORCH)
			v_bp = CVT_MIN_V_BPORCH;
	}
	image_height = (frame_height - v_fp - vsync - v_bp + 1) & ~0x1;

	/* Aspect ratio based on vsync */
	switch (vsync)
	{
	case 4:
		image_width = (image_height * 4) / 3;
		break;
	case 5:
		image_width = (image_height * 16) / 9;
		break;
	case 6:
		image_width = (image_height * 16) / 10;
		break;
	case 7:
		/* special case */
		if (image_height == 1024)
			image_width = (image_height * 5) / 4;
		else if (image_height == 768)
			image_width = (image_height * 15) / 9;
		else
			return false;
		break;
	default:
		return false;
	}

	image_width = image_width & ~7;

	/* Horizontal */
	if (reduced_blanking)
	{
		pix_clk = (image_width + CVT_RB_H_BLANK) * hfreq;
		pix_clk = (pix_clk / CVT_PXL_CLK_GRAN) * CVT_PXL_CLK_GRAN;

		h_bp = CVT_RB_H_BPORCH;
		hsync = CVT_RB_H_SYNC;
		h_fp = CVT_RB_H_BLANK - h_bp - hsync;

		frame_width = image_width + CVT_RB_H_BLANK;
	}
	else
	{
		unsigned ideal_duty_cycle_per_myriad =
			100 * CVT_C_PRIME - (CVT_M_PRIME * 100000) / hfreq;
		int h_blank;

		if (ideal_duty_cycle_per_myriad < 2000)
			ideal_duty_cycle_per_myriad = 2000;

		h_blank = image_width * ideal_duty_cycle_per_myriad /
				  (10000 - ideal_duty_cycle_per_myriad);
		h_blank = (h_blank / (2 * CVT_CELL_GRAN)) * 2 * CVT_CELL_GRAN;

		pix_clk = (image_width + h_blank) * hfreq;
		pix_clk = (pix_clk / CVT_PXL_CLK_GRAN) * CVT_PXL_CLK_GRAN;

		h_bp = h_blank / 2;
		frame_width = image_width + h_blank;

		hsync = (frame_width * 8 + 50) / 100;
		hsync = hsync - hsync % CVT_CELL_GRAN;
		h_fp = h_blank - hsync - h_bp;
	}

	bt->polarities = polarities;
	bt->width = image_width;
	bt->height = image_height;
	bt->hfrontporch = h_fp;
	bt->vfrontporch = v_fp;
	bt->hsync = hsync;
	bt->vsync = vsync;
	bt->hbackporch = frame_width - image_width - h_fp - hsync;
	bt->vbackporch = frame_height - image_height - v_fp - vsync;
	bt->pixelclock = pix_clk;
	bt->standards = V4L2_DV_BT_STD_CVT;
	if (reduced_blanking)
		bt->flags |= V4L2_DV_FL_REDUCED_BLANKING;
	return true;
}

/** detect_gtf - detect if the given timings follow the GTF standard
* @frame_height - the total height of the frame (including blanking) in lines.
* @hfreq - the horizontal frequency in Hz.
* @vsync - the height of the vertical sync in lines.
* @polarities - the horizontal and vertical polarities (same as struct
*		v4l2_bt_timings polarities).
* @aspect - preferred aspect ratio. GTF has no method of determining the
*		aspect ratio in order to derive the image width from the
*		image height, so it has to be passed explicitly. Usually
*		the native screen aspect ratio is used for this. If it
*		is not filled in correctly, then 16:9 will be assumed.
* @fmt - the resulting timings.
*
* This function will attempt to detect if the given values correspond to a
* valid GTF format. If so, then it will return true, and fmt will be filled
* in with the found GTF timings.
*/
bool detect_gtf(unsigned frame_height,
				unsigned hfreq,
				unsigned vsync,
				u32 polarities,
				struct fract aspect,
				struct bt_timings *bt)
{
	int pix_clk;
	int v_fp, v_bp, h_fp, hsync;
	int frame_width, image_height, image_width;
	bool default_gtf;
	int h_blank;

	if (vsync != 3)
		return false;

	if (polarities == V4L2_DV_VSYNC_POS_POL)
		default_gtf = true;
	else if (polarities == V4L2_DV_HSYNC_POS_POL)
		default_gtf = false;
	else
		return false;

	/* Vertical */
	v_fp = GTF_V_FP;
	v_bp = (GTF_MIN_VSYNC_BP * hfreq + 999999) / 1000000 - vsync;
	image_height = (frame_height - v_fp - vsync - v_bp + 1) & ~0x1;

	if (aspect.numerator == 0 || aspect.denominator == 0)
	{
		aspect.numerator = 16;
		aspect.denominator = 9;
	}
	image_width = ((image_height * aspect.numerator) / aspect.denominator);
	image_width = (image_width + GTF_CELL_GRAN / 2) & ~(GTF_CELL_GRAN - 1);

	/* Horizontal */
	if (default_gtf)
		h_blank = ((image_width * GTF_D_C_PRIME * hfreq) -
				   (image_width * GTF_D_M_PRIME * 1000) +
				   (hfreq * (100 - GTF_D_C_PRIME) + GTF_D_M_PRIME * 1000) / 2) /
				  (hfreq * (100 - GTF_D_C_PRIME) + GTF_D_M_PRIME * 1000);
	else
		h_blank = ((image_width * GTF_S_C_PRIME * hfreq) -
				   (image_width * GTF_S_M_PRIME * 1000) +
				   (hfreq * (100 - GTF_S_C_PRIME) + GTF_S_M_PRIME * 1000) / 2) /
				  (hfreq * (100 - GTF_S_C_PRIME) + GTF_S_M_PRIME * 1000);

	h_blank = h_blank - h_blank % (2 * GTF_CELL_GRAN);
	frame_width = image_width + h_blank;

	pix_clk = (image_width + h_blank) * hfreq;
	pix_clk = pix_clk / GTF_PXL_CLK_GRAN * GTF_PXL_CLK_GRAN;

	hsync = (frame_width * 8 + 50) / 100;
	hsync = hsync - hsync % GTF_CELL_GRAN;

	h_fp = h_blank / 2 - hsync;

	bt->polarities = polarities;
	bt->width = image_width;
	bt->height = image_height;
	bt->hfrontporch = h_fp;
	bt->vfrontporch = v_fp;
	bt->hsync = hsync;
	bt->vsync = vsync;
	bt->hbackporch = frame_width - image_width - h_fp - hsync;
	bt->vbackporch = frame_height - image_height - v_fp - vsync;
	bt->pixelclock = pix_clk;
	bt->standards = V4L2_DV_BT_STD_GTF;
	if (!default_gtf)
		bt->flags |= V4L2_DV_FL_REDUCED_BLANKING;

	return true;
}

/* ADV7842 system clock frequency */

static inline int hdmi_read(u8 reg)
{
	return gpio_i2c_read_adv7842_2(0x68, reg);
}

static inline int cp_read(u8 reg)
{
	return gpio_i2c_read_adv7842_2(0x44, reg);
}


static int stdi2bt_timings(struct stdi_readback *stdi, struct bt_timings *timings)
{
	u32 hfreq = (SYSTEM_CLOCK_PERIOD * 8) / stdi->bl;
	u32 pix_clk;
	int i;
	struct fract aspect_ratio;

	for (i = 0; bt_timings_presets[i].width; i++)
	{
		const struct bt_timings *bt = &bt_timings_presets[i];

		if (vtotal(bt) != stdi->lcf + 1)
			continue;
		if (bt->vsync != stdi->lcvs)
			continue;

		pix_clk = hfreq * htotal(bt);

		if ((pix_clk < bt->pixelclock + 1000000) &&
			(pix_clk > bt->pixelclock - 1000000))
		{
			*timings = bt_timings_presets[i];
			return 0;
		}
	}

	if (detect_cvt(stdi->lcf + 1, hfreq, stdi->lcvs,
				   (stdi->hs_pol == '+' ? V4L2_DV_HSYNC_POS_POL : 0) |
					   (stdi->vs_pol == '+' ? V4L2_DV_VSYNC_POS_POL : 0),
				   timings))
		return 0;

	aspect_ratio.numerator = 16;
	aspect_ratio.denominator = 9;
	if (detect_gtf(stdi->lcf + 1, hfreq, stdi->lcvs,
				   (stdi->hs_pol == '+' ? V4L2_DV_HSYNC_POS_POL : 0) |
					   (stdi->vs_pol == '+' ? V4L2_DV_VSYNC_POS_POL : 0),
				   aspect_ratio, timings))
		return 0;

	aspect_ratio.numerator = 4;
	aspect_ratio.denominator = 3;
	if (detect_gtf(stdi->lcf + 1, hfreq, stdi->lcvs,
				   (stdi->hs_pol == '+' ? V4L2_DV_HSYNC_POS_POL : 0) |
					   (stdi->vs_pol == '+' ? V4L2_DV_VSYNC_POS_POL : 0),
				   aspect_ratio, timings))
		return 0;
	return -1;
}

static inline bool is_hdmi(void)
{
	return hdmi_read(0x05) & 0x80;
}

static const char *const input_color_space_txt[16] = {
	"RGB limited range (16-235)", "RGB full range (0-255)",
	"YCbCr Bt.601 (16-235)", "YCbCr Bt.709 (16-235)",
	"xvYCC Bt.601", "xvYCC Bt.709",
	"YCbCr Bt.601 (0-255)", "YCbCr Bt.709 (0-255)",
	"invalid", "invalid", "invalid", "invalid", "invalid",
	"invalid", "invalid", "automatic"};

void print_bt(struct bt_timings *bt)
{
	u32 htot, vtot, fps;
	htot = bt->hfrontporch + bt->hsync + bt->hbackporch + bt->width;
	vtot = bt->vfrontporch + bt->vsync + bt->vbackporch + bt->height;

	if (htot * vtot != 0)
	{
		fps = bt->pixelclock / (htot * vtot);
	}
	else
	{
		fps = 0;
	}

	printk("%s: %dx%d@%d%s (%dx%d) %s\n", __func__, bt->width, bt->height, fps, bt->interlaced ? "i" : "p", htot, vtot, input_color_space_txt[bt->hdmi_colorspace]);
}

static int read_stdi(struct stdi_readback *stdi)
{

	stdi->bl = ((cp_read(0xb1) & 0x3f) << 8) | cp_read(0xb2);
	stdi->lcf = ((cp_read(0xb3) & 0x7) << 8) | cp_read(0xb4);
	stdi->lcvs = cp_read(0xb3) >> 3;
	stdi->fcl = ((cp_read(0xb8) & 0x1f) << 8) | cp_read(0xb9);

	if ((cp_read(0xb5) & 0x80) && ((cp_read(0xb5) & 0x03) == 0x01))
	{
		stdi->hs_pol = ((cp_read(0xb5) & 0x10) ? ((cp_read(0xb5) & 0x08) ? '+' : '-') : 'x');
		stdi->vs_pol = ((cp_read(0xb5) & 0x40) ? ((cp_read(0xb5) & 0x20) ? '+' : '-') : 'x');
	}
	else
	{
		stdi->hs_pol = 'x';
		stdi->vs_pol = 'x';
	}
	stdi->interlaced = (cp_read(0xb1) & 0x40) ? true : false;

	if (stdi->lcf < 239 || stdi->bl < 8 || stdi->bl == 0x3fff)
	{
		return -1;
	}

	return 0;
}

static inline bool is_digital_input(void)
{
	return false;
}

int adv7842_query_timings(struct bt_timings *bt)
{
	struct stdi_readback stdi;
	uint32_t freq;

	if (read_stdi(&stdi))
	{
		return -1;
	}

	if (is_digital_input())
	{
		bt->interlaced = stdi.interlaced ? 1 : 0;

		bt->width = (hdmi_read(0x07) & 0x0f) * 256 + hdmi_read(0x08);
		bt->height = (hdmi_read(0x09) & 0x0f) * 256 + hdmi_read(0x0a);
		freq = ((hdmi_read(0x51) << 1) + (hdmi_read(0x52) >> 7)) * 1000000;
		freq += ((hdmi_read(0x52) & 0x7f) * 7813);

		if (is_hdmi())
		{
			/* adjust for deep color mode */
			freq = freq * 8 / (((hdmi_read(0x0b) & 0xc0) >> 6) * 2 + 8);
		}

		bt->hdmi_colorspace = (hdmi_read(0x53) & 0x0f);
		bt->pixelclock = freq;

		bt->hfrontporch = (hdmi_read(0x20) & 0x03) * 256 + hdmi_read(0x21);
		bt->hsync = (hdmi_read(0x22) & 0x03) * 256 + hdmi_read(0x23);
		bt->hbackporch = (hdmi_read(0x24) & 0x03) * 256 + hdmi_read(0x25);

		bt->vfrontporch = ((hdmi_read(0x2a) & 0x1f) * 256 + hdmi_read(0x2b)) / 2;
		bt->vsync = ((hdmi_read(0x2e) & 0x1f) * 256 + hdmi_read(0x2f)) / 2;
		bt->vbackporch = ((hdmi_read(0x32) & 0x1f) * 256 + hdmi_read(0x33)) / 2;

		bt->polarities = ((hdmi_read(0x05) & 0x10) ? 1 : 0) | ((hdmi_read(0x05) & 0x20) ? 1 : 0);

		if (bt->interlaced == 1)
		{
			bt->height += (hdmi_read(0x0b) & 0x0f) * 256 + hdmi_read(0x0c);
			bt->il_vfrontporch = ((hdmi_read(0x2c) & 0x1f) * 256 + hdmi_read(0x2d)) / 2;
			bt->il_vsync = ((hdmi_read(0x30) & 0x1f) * 256 + hdmi_read(0x31)) / 2;
			bt->vbackporch = ((hdmi_read(0x34) & 0x1f) * 256 + hdmi_read(0x35)) / 2;
		}
	}
	else
	{
		if (!stdi2bt_timings(&stdi, bt))
			goto found;

		stdi.lcvs += 1;
		if (!stdi2bt_timings(&stdi, bt))
			goto found;

		stdi.lcvs -= 2;
		if (stdi2bt_timings(&stdi, bt))
		{
			return -1;
		}
	}

found:
	return 0;
}
