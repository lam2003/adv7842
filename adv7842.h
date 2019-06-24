#ifndef ADV7842_H_
#define ADV7842_H_

typedef enum ADV7842_CTRL_CMD_E
{
	CMD_DEVICE_INIT,
	CMD_QUERY_TIMINGS,
} ADV7842_CTRL_CMD;

typedef enum ADV7842_CMODE_E
{
	MODE_HDMI,
	MODE_VGA
} ADV7842_MODE;

typedef enum ADV7842_HDMI_PORT_E
{
	HDMI_PORT_A,
	HDMI_PORT_B
} ADV7842_HDMI_PORT;

/** struct bt_timings - BT.656/BT.1120 timing data
* @width:	total width of the active video in pixels
* @height:	total height of the active video in lines
* @interlaced:	Interlaced or progressive
* @polarities:	Positive or negative polarities
* @pixelclock:	Pixel clock in HZ. Ex. 74.25MHz->74250000
* @hfrontporch:Horizontal front porch in pixels
* @hsync:	Horizontal Sync length in pixels
* @hbackporch:	Horizontal back porch in pixels
* @vfrontporch:Vertical front porch in lines
* @vsync:	Vertical Sync length in lines
* @vbackporch:	Vertical back porch in lines
* @il_vfrontporch:Vertical front porch for the even field
*		(aka field 2) of interlaced field formats
* @il_vsync:	Vertical Sync length for the even field
*		(aka field 2) of interlaced field formats
* @il_vbackporch:Vertical back porch for the even field
*		(aka field 2) of interlaced field formats
* @standards:	Standards the timing belongs to
*
* A note regarding vertical interlaced timings: height refers to the total
* height of the active video frame (= two fields). The blanking timings refer
* to the blanking of each field. So the height of the total frame is
* calculated as follows:
*
* tot_height = height + vfrontporch + vsync + vbackporch +
*                       il_vfrontporch + il_vsync + il_vbackporch
*
* The active height of each field is height / 2.
*/
struct bt_timings
{
	unsigned int width;
	unsigned int height;
	unsigned int interlaced;
	unsigned int polarities;
	unsigned int pixelclock;
	unsigned int hfrontporch;
	unsigned int hsync;
	unsigned int hbackporch;
	unsigned int vfrontporch;
	unsigned int vsync;
	unsigned int vbackporch;
	unsigned int il_vfrontporch;
	unsigned int il_vsync;
	unsigned int il_vbackporch;
	unsigned int standards;
	unsigned int flags;
	unsigned int hdmi_colorspace;
};

#define SYSTEM_CLOCK_PERIOD 28636363

#endif /* ADV7842_H_ */
