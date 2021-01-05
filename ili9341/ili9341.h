/**
 * @file ILI9341.h
 */

#ifndef ILI9341_H
#define ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/* Maybe rename to lv_driver_generic */
#include "driver_generic.h"

/*********************
 *      DEFINES
 *********************/

/* Level 1 Command */
#define ILI9341_CMD_NOP     (0x00U)
#define ILI9341_CMD_SWRESET (0x01U)
#define ILI9341_CMD_RDDIDIF (0x04U)
#define ILI9341_CMD_RDDST   (0x09U)
#define ILI9341_CMD_RDDPM   (0x0AU)
#define ILI9341_CMD_RDDMADCTL   (0x0BU)
#define ILI9341_CMD_RDDCOLMOD   (0x0CU)
#define ILI9341_CMD_RDDIM   (0x0DU)
#define ILI9341_CMD_RDDSM   (0x0EU)
#define ILI9341_CMD_RDDSDR  (0x0FU)
#define ILI9341_CMD_SLPIN   (0x10U)
#define ILI9341_CMD_SLPOUT  (0x11U)
#define ILI9341_CMD_PTLON   (0x12U)
#define ILI9341_CMD_NORON   (0x13U)
#define ILI9341_CMD_DINVOFF (0x20U)
#define ILI9341_CMD_DINVON  (0x21U)
#define ILI9341_CMD_GAMSET  (0x26U)
#define ILI9341_CMD_DISPOFF (0x28U)
#define ILI9341_CMD_DISPON  (0x29U)
#define ILI9341_CMD_CASET   (0x2AU)
#define ILI9341_CMD_PASET   (0x2BU)
#define ILI9341_CMD_RAMWR   (0x2CU)
#define ILI9341_CMD_RGBSET  (0x2DU)
#define ILI9341_CMD_RAMRD   (0x2EU)
#define ILI9341_CMD_PLTAR   (0x30U)
#define ILI9341_CMD_VSCRDEF (0x33U)
#define ILI9341_CMD_TEOFF   (0x34U)
#define ILI9341_CMD_TEON    (0x35U)
#define ILI9341_CMD_MADCTL  (0x36U)
#define ILI9341_CMD_VSCRSADD    (0x37U)
#define ILI9341_CMD_IDMOFF  (0x38U)
#define ILI9341_CMD_IDMON   (0x39U)
#define ILI9341_CMD_PIXSET  (0x3AU)
#define ILI9341_CMD_WRITE_MEMORY_CONTINUE   (0x3CU)
#define ILI9341_CMD_READ_MEMORY_CONTINUE    (0x3EU)
#define ILI9341_CMD_SET_TEAR_SCANLINE   (0x44U)
#define ILI9341_CMD_GET_SCANLINE    (0x45U)
#define ILI9341_CMD_WRDISBV (0x51U)
#define ILI9341_CMD_RDDISBV (0x52U)
#define ILI9341_CMD_WRCTRLD (0x53U)
#define ILI9341_CMD_RDCTRLD (0x54U)
#define ILI9341_CMD_WRCABC  (0x55U)
#define ILI9341_CMD_RDCABC  (0x56U)
#define ILI9341_CMD_WRITE_BACKLIGHT_CONTROL_1   (0x5EU)
#define ILI9341_CMD_READ_BACKLIGHT_CONTROL_1    (0x5FU)
#define ILI9341_CMD_RDID1   (0xDAU)
#define ILI9341_CMD_RDID2   (0xDBU)
#define ILI9341_CMD_RDID3   (0xDCU)

/* Level 2 Command */

#define ILI9341_CMD_IFMODE  (0xB0U)
#define ILI9341_CMD_FRMCTR1 (0xB1U)
#define ILI9341_CMD_FRMCTR2 (0xB2U)
#define ILI9341_CMD_FRMCTR3 (0xB3U)
#define ILI9341_CMD_INVTR   (0xB4U)
#define ILI9341_CMD_PRCTR   (0xB5U)
#define ILI9341_CMD_DISCTRL (0xB6U)
#define ILI9341_CMD_ETMOD   (0xB7U)
#define ILI9341_CMD_BACKLIGHT_CONTROL_1 (0xB8U)
#define ILI9341_CMD_BACKLIGHT_CONTROL_2 (0xB9U)
#define ILI9341_CMD_BACKLIGHT_CONTROL_3 (0xBAU)
#define ILI9341_CMD_BACKLIGHT_CONTROL_4 (0xBBU)
#define ILI9341_CMD_BACKLIGHT_CONTROL_5 (0xBCU)
#define ILI9341_CMD_BACKLIGHT_CONTROL_7 (0xBEU)
#define ILI9341_CMD_BACKLIGHT_CONTROL_8 (0xBFU)
#define ILI9341_CMD_PWCTRL1 (0xC0U)
#define ILI9341_CMD_PWCTRL2 (0xC1U)
#define ILI9341_CMD_VMCTRL1 (0xC5U)
#define ILI9341_CMD_VMCTRL2 (0xC7U)
#define ILI9341_CMD_NVMWR   (0xD0U)
#define ILI9341_CMD_NVMPKEY (0xD1U)
#define ILI9341_CMD_RDNVM   (0xD2U)
#define ILI9341_CMD_RDID4   (0xD4U)
#define ILI9341_CMD_PGAMCTRL    (0xE0U)
#define ILI9341_CMD_NGAMCTRL    (0xE1U)
#define ILI9341_CMD_DGAMCTRL_1  (0xE2U)
#define ILI9341_CMD_DGAMCTRL_2  (0xE3U)
#define ILI9341_CMD_IFCTL   (0xF6U)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void ili9341_init(lv_disp_drv_t *drv, void *user_descriptor);
void ili9341_flush(lv_disp_drv_t *drv, const lv_area_t * area, lv_color_t * color_map);
void ili9341_enable_backlight(lv_disp_drv_t *drv, display_backlight_control_t backlight_en);
void ili9341_sleep_in(lv_disp_drv_t *drv);
void ili9341_sleep_out(lv_disp_drv_t *drv);
void ili9341_reset(lv_disp_drv_t *drv);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ILI9341_H*/
