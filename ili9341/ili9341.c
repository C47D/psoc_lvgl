/**
 * @file ili9341.c
 */

/*********************
 *      INCLUDES
 *********************/
#include "ili9341.h"
#include "disp_spi.h"

/**********************
 *      TYPEDEFS
 **********************/

/* The LCD needs a bunch of command/argument values to be initialized.
 * They are stored in this struct. */
typedef struct {
    uint8_t cmd;
    uint8_t params[16];
    /* Number of parameters in parameters; bit 7 (0x80) = delay after set; 0xFF = end of cmds. */
    uint8_t num_params;
} lcd_init_cmd_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static inline void ili9341_set_mode(lv_disp_drv_t *drv, data_mode_t mode);

static void ili9341_set_orientation(lv_disp_drv_t *drv, uint8_t orientation);

static void ili9341_send_cmd(lv_disp_drv_t *drv, uint8_t cmd);
static void ili9341_send_data(lv_disp_drv_t *drv, void *data, uint16_t length);

/* Sending color data to the display can be both blocking and async, but is
 * better to use async. When we're done sending color data we need to call
 * lv_disp_flush_ready(&disp->driver) */
static void ili9341_send_color(lv_disp_drv_t *drv, void *data, uint16_t length);

/**********************
 *  STATIC VARIABLES
 **********************/

/*********************
 *      DEFINES
 *********************/
#define DRIVER_NAME             "ILI9341"

#define DELAY_AFTER_SET_MS      (100U)
#define END_OF_CMD_DESCRIPTOR   (0xFF)
#define DELAY_AFTER_CMD         (0x80)

/**********************
 *      MACROS
 **********************/
#define GET_BYTES_IN_PARAMS(params)   (params & 0x1F)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ili9341_init(lv_disp_drv_t *drv, void *user_descriptor)
{
    /* Default display initialization descriptor */
	lcd_init_cmd_t default_init_cmds[] = {
		{0xCF, {0x00, 0x83, 0X30}, 3},
		{0xED, {0x64, 0x03, 0X12, 0X81}, 4},
		{0xE8, {0x85, 0x01, 0x79}, 3},
		{0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
		{0xF7, {0x20}, 1},
		{0xEA, {0x00, 0x00}, 2},
		{ILI9341_CMD_PWCTRL1, {0x26}, 1},
		{ILI9341_CMD_PWCTRL2, {0x11}, 1},
		{ILI9341_CMD_VMCTRL1, {0x35, 0x3E}, 2},
		{ILI9341_CMD_VMCTRL2, {0xBE}, 1},
		{ILI9341_CMD_MADCTL, {0x28}, 1},
		{ILI9341_CMD_PIXSET, {0x55}, 1},
		{ILI9341_CMD_FRMCTR1, {0x00, 0x1B}, 2},
		{0xF2, {0x08}, 1},
		{ILI9341_CMD_GAMSET, {0x01}, 1},
		{ILI9341_CMD_PGAMCTRL, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
		{ILI9341_CMD_NGAMCTRL, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
		{ILI9341_CMD_CASET, {0x00, 0x00, 0x00, 0xEF}, 4},
		{ILI9341_CMD_PASET, {0x00, 0x00, 0x01, 0x3f}, 4},
		{ILI9341_CMD_RAMWR, {0}, 0},
		{ILI9341_CMD_ETMOD, {0x07}, 1},
		{ILI9341_CMD_DISCTRL, {0x0A, 0x82, 0x27, 0x00}, 4},
		{ILI9341_CMD_SLPOUT, {0}, DELAY_AFTER_CMD},
		{ILI9341_CMD_DISPON, {0}, DELAY_AFTER_CMD},
		{0, {0}, END_OF_CMD_DESCRIPTOR},
	};
    
    driver_generic_t *driver = (driver_generic_t *) drv->user_data;
    lcd_init_cmd_t *init_descriptor = default_init_cmds;
    
    if (user_descriptor) {
        init_descriptor = user_descriptor;
    }
    
    ili9341_reset(drv);

	/* Send the display initialization descriptor */
	uint16_t cmd_idx = 0;
    
	while (init_descriptor[cmd_idx].num_params != END_OF_CMD_DESCRIPTOR) {
		ili9341_send_cmd(drv, init_descriptor[cmd_idx].cmd);
		ili9341_send_data(drv, init_descriptor[cmd_idx].params, init_descriptor[cmd_idx].num_params & 0x1F);
		
        if (init_descriptor[cmd_idx].num_params & DELAY_AFTER_CMD) {
            driver->delay_ms(drv, DELAY_AFTER_SET_MS);
		}
        
		cmd_idx++;
	}

    /* TODO: Move to module layer */
	ili9341_enable_backlight(drv, DISPLAY_BACKLIGHT_CONTROL_ENABLE);

    /* Set display orientation
     * NOTE: It doesn't change LVGL orientation */
    ili9341_set_orientation(drv, 2 /* Landscape */);

	ili9341_send_cmd(drv, ILI9341_CMD_DINVOFF);
}

void ili9341_reset(lv_disp_drv_t *drv)
{
    driver_generic_t *driver = (driver_generic_t *) drv->user_data;
    
	/* Reset the display */
    if (driver->hw_reset) {
        driver->hw_reset(drv);
    } else {
        /* Software reset when no hardware reset is available */
        ili9341_send_cmd(drv, ILI9341_CMD_SWRESET);
        driver->delay_ms(drv, 5);
    }
}

/* Flush function for RGB565 color depth */
void ili9341_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    uint8_t data[4] = {0};
    
    uint32_t pixels_to_update = lv_area_get_width(area) * lv_area_get_height(area);
    /* times two because each pixel is 2 bytes when LV_COLOR_DEPTH is set to RGB565 */
    uint32_t bytes_to_send = pixels_to_update * 2;

	/*Column addresses*/
	data[0] = (area->x1 >> 8) & 0xFF;
	data[1] = area->x1 & 0xFF;
	data[2] = (area->x2 >> 8) & 0xFF;
	data[3] = area->x2 & 0xFF;
    
    ili9341_send_cmd(drv, ILI9341_CMD_CASET);
	ili9341_send_data(drv, data, sizeof data);

	/*Page addresses*/
	data[0] = (area->y1 >> 8) & 0xFF;
	data[1] = area->y1 & 0xFF;
	data[2] = (area->y2 >> 8) & 0xFF;
	data[3] = area->y2 & 0xFF;
    
    ili9341_send_cmd(drv, ILI9341_CMD_PASET);
	ili9341_send_data(drv, data, sizeof data);

	/*Memory write*/
    ili9341_send_cmd(drv, ILI9341_CMD_RAMWR);
	ili9341_send_color(drv, (void *) color_map, bytes_to_send);
}

/* TODO: How we can check if the driver_generic_t have an display_backlight_control
 * callback registered. */
void ili9341_enable_backlight(lv_disp_drv_t *drv, display_backlight_control_t backlight_en)
{
    driver_generic_t *driver = (driver_generic_t *) drv->user_data;
    
    driver->display_backlight_control(drv, backlight_en);
}

void ili9341_sleep_in(lv_disp_drv_t *drv)
{
	uint8_t data[] = {0x08};
    
	ili9341_send_cmd(drv, ILI9341_CMD_SLPIN);
	ili9341_send_data(drv, &data, sizeof data);
}

void ili9341_sleep_out(lv_disp_drv_t *drv)
{
	uint8_t data[] = {0x08};
    
	ili9341_send_cmd(drv, ILI9341_CMD_SLPOUT);
	ili9341_send_data(drv, &data, sizeof data);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* NOTE:
 * This is a blocking function, so maybe we could rename display_tl_send_data to
 * display_tl_send_blocking. */
static void ili9341_send_cmd(lv_disp_drv_t *drv, uint8_t cmd)
{
    display_tl_poll_is_busy();
    
    ili9341_set_mode(drv, DATA_MODE_CMD);
    display_tl_send_data(&cmd, 1);
}

/* NOTE:
 * This is a blocking function, so maybe we could rename display_tl_send_data to
 * display_tl_send_blocking. */
static void ili9341_send_data(lv_disp_drv_t *drv, void * data, uint16_t length)
{
    display_tl_poll_is_busy();
    
    ili9341_set_mode(drv, DATA_MODE_DATA);
    display_tl_send_data(data, length);
}

/* NOTE:
 * In this case it's better to send the data in async mode (because we send the
 * pixel color information), so maybe we could rename display_tl_send_colors to
 * display_tl_send_async. */
static void ili9341_send_color(lv_disp_drv_t *drv, void *data, uint16_t length)
{    
    display_tl_poll_is_busy();
    
    ili9341_set_mode(drv, DATA_MODE_DATA);
    display_tl_send_colors(data, length);
}

static void ili9341_set_orientation(lv_disp_drv_t *drv, uint8_t orientation)
{
    (void) drv;
    
    uint8_t orientation_code[] = {
        0x48,   /* Portrait */
        0x88,   /* Portrait inverted */
        0x28,   /* Landscape */
        0xE8    /* Landscape inverted */
    };

    ili9341_send_cmd(drv, ILI9341_CMD_MADCTL);
    ili9341_send_data(drv, (void *) &orientation_code[orientation], 1);
}

static inline void ili9341_set_mode(lv_disp_drv_t *drv, data_mode_t mode)
{
    driver_generic_t *driver = (driver_generic_t *) drv->user_data;
    
    driver->data_mode(drv, mode);
}
