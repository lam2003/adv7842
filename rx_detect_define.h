/*
 * rx_detect_define.h
 *
 *  Created on: 2016-8-10
 *      Author: eternal
 */

#ifndef RX_DETECT_DEFINE_H_
#define RX_DETECT_DEFINE_H_


#include "v4l2-dv-timings.h"
#include "rx_detect.h"


/* Polarities. If bit is not set, it is assumed to be negative polarity */
#define V4L2_DV_VSYNC_POS_POL	0x00000001
#define V4L2_DV_HSYNC_POS_POL	0x00000002

/* Timings standards */
#define V4L2_DV_BT_STD_CEA861	(1 << 0)  /* CEA-861 Digital TV Profile */
#define V4L2_DV_BT_STD_DMT	(1 << 1)  /* VESA Discrete Monitor Timings */
#define V4L2_DV_BT_STD_CVT	(1 << 2)  /* VESA Coordinated Video Timings */
#define V4L2_DV_BT_STD_GTF	(1 << 3)  /* VESA Generalized Timings Formula */

/* CVT/GTF specific: timing uses reduced blanking (CVT) or the 'Secondary
GTF' curve (GTF). In both cases the horizontal and/or vertical blanking
intervals are reduced, allowing a higher resolution over the same
bandwidth. This is a read-only flag. */
#define V4L2_DV_FL_REDUCED_BLANKING		(1 << 0)
/* CEA-861 specific: set for CEA-861 formats with a framerate of a multiple
of six. These formats can be optionally played at 1 / 1.001 speed.
This is a read-only flag. */
#define V4L2_DV_FL_CAN_REDUCE_FPS		(1 << 1)
/* CEA-861 specific: only valid for video transmitters, the flag is cleared
by receivers.
If the framerate of the format is a multiple of six, then the pixelclock
used to set up the transmitter is divided by 1.001 to make it compatible
with 60 Hz based standards such as NTSC and PAL-M that use a framerate of
29.97 Hz. Otherwise this flag is cleared. If the transmitter can't generate
such frequencies, then the flag will also be cleared. */
#define V4L2_DV_FL_REDUCED_FPS			(1 << 2)
/* Specific to interlaced formats: if set, then field 1 is really one half-line
longer and field 2 is really one half-line shorter, so each field has
exactly the same number of half-lines. Whether half-lines can be detected
or used depends on the hardware. */
#define V4L2_DV_FL_HALF_LINE			(1 << 3)
/* If set, then this is a Consumer Electronics (CE) video format. Such formats
* differ from other formats (commonly called IT formats) in that if RGB
* encoding is used then by default the RGB values use limited range (i.e.
* use the range 16-235) as opposed to 0-255. All formats defined in CEA-861
* except for the 640x480 format are CE formats. */
#define V4L2_DV_FL_IS_CE_VIDEO			(1 << 4)

/* A few useful defines to calculate the total blanking and frame sizes */
#define V4L2_DV_BT_BLANKING_WIDTH(bt) \
	((bt)->hfrontporch + (bt)->hsync + (bt)->hbackporch)
#define V4L2_DV_BT_FRAME_WIDTH(bt) \
	((bt)->width + V4L2_DV_BT_BLANKING_WIDTH(bt))
#define V4L2_DV_BT_BLANKING_HEIGHT(bt) \
	((bt)->vfrontporch + (bt)->vsync + (bt)->vbackporch + \
	 (bt)->il_vfrontporch + (bt)->il_vsync + (bt)->il_vbackporch)
#define V4L2_DV_BT_FRAME_HEIGHT(bt) \
	((bt)->height + V4L2_DV_BT_BLANKING_HEIGHT(bt))

/*
* CVT defines
* Based on Coordinated Video Timings Standard
* version 1.1 September 10, 2003
*/

#define CVT_PXL_CLK_GRAN	250000	/* pixel clock granularity */

/* Normal blanking */
#define CVT_MIN_V_BPORCH	7	/* lines */
#define CVT_MIN_V_PORCH_RND	3	/* lines */
#define CVT_MIN_VSYNC_BP	550	/* min time of vsync + back porch (us) */

/* Normal blanking for CVT uses GTF to calculate horizontal blanking */
#define CVT_CELL_GRAN		8	/* character cell granularity */
#define CVT_M			600	/* blanking formula gradient */
#define CVT_C			40	/* blanking formula offset */
#define CVT_K			128	/* blanking formula scaling factor */
#define CVT_J			20	/* blanking formula scaling factor */
#define CVT_C_PRIME (((CVT_C - CVT_J) * CVT_K / 256) + CVT_J)
#define CVT_M_PRIME (CVT_K * CVT_M / 256)

/* Reduced Blanking */
#define CVT_RB_MIN_V_BPORCH    7       /* lines  */
#define CVT_RB_V_FPORCH        3       /* lines  */
#define CVT_RB_MIN_V_BLANK   460     /* us     */
#define CVT_RB_H_SYNC         32       /* pixels */
#define CVT_RB_H_BPORCH       80       /* pixels */
#define CVT_RB_H_BLANK       160       /* pixels */

/*
* GTF defines
* Based on Generalized Timing Formula Standard
* Version 1.1 September 2, 1999
*/

#define GTF_PXL_CLK_GRAN	250000	/* pixel clock granularity */

#define GTF_MIN_VSYNC_BP	550	/* min time of vsync + back porch (us) */
#define GTF_V_FP		1	/* vertical front porch (lines) */
#define GTF_CELL_GRAN		8	/* character cell granularity */

/* Default */
#define GTF_D_M			600	/* blanking formula gradient */
#define GTF_D_C			40	/* blanking formula offset */
#define GTF_D_K			128	/* blanking formula scaling factor */
#define GTF_D_J			20	/* blanking formula scaling factor */
#define GTF_D_C_PRIME ((((GTF_D_C - GTF_D_J) * GTF_D_K) / 256) + GTF_D_J)
#define GTF_D_M_PRIME ((GTF_D_K * GTF_D_M) / 256)

/* Secondary */
#define GTF_S_M			3600	/* blanking formula gradient */
#define GTF_S_C			40	/* blanking formula offset */
#define GTF_S_K			128	/* blanking formula scaling factor */
#define GTF_S_J			35	/* blanking formula scaling factor */
#define GTF_S_C_PRIME ((((GTF_S_C - GTF_S_J) * GTF_S_K) / 256) + GTF_S_J)
#define GTF_S_M_PRIME ((GTF_S_K * GTF_S_M) / 256)


struct fract {
	__u32   numerator;
	__u32   denominator;
};

struct stdi_readback {
	u16 bl, lcf, lcvs, fcl;
	u8 hs_pol, vs_pol;
	bool interlaced;
};

#endif /* RX_DETECT_DEFINE_H_ */
