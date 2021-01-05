#ifndef DRIVER_GENERIC_H
#define DRIVER_GENERIC_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

typedef enum {
    DATA_MODE_CMD,
    DATA_MODE_DATA,
} data_mode_t;

typedef enum {
    DISPLAY_BACKLIGHT_CONTROL_DISABLE,
    DISPLAY_BACKLIGHT_CONTROL_ENABLE,
} display_backlight_control_t;

typedef struct {
    void (*display_backlight_control)(lv_disp_drv_t *drv, display_backlight_control_t backlight_control);
    void (*data_mode)(lv_disp_drv_t *drv, data_mode_t mode);
    void (*delay_ms)(lv_disp_drv_t *drv, uint32_t ms);
    void (*write_blocking)(const uint8_t *tx_data, const size_t size);
    void (*write_async)(const uint8_t *tx_data, uint8_t *rx_data, const size_t size);
    void (*hw_reset)(lv_disp_drv_t *drv);
} driver_generic_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DRIVER_GENERIC_H */
