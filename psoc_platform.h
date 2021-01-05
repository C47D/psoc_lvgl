#ifndef PSOC_PLATFORM_H
#define PSOC_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "driver_generic.h"

void psoc_display_backlight_control(lv_disp_drv_t *drv, display_backlight_control_t backlight_control);
void psoc_data_mode(lv_disp_drv_t *drv, data_mode_t mode);
void psoc_delay_ms(lv_disp_drv_t *drv, uint32_t ms);
void psoc_write_blocking(const uint8_t *tx_data, const size_t size);
void psoc_write_async(const uint8_t *tx_data, uint8_t *rx_data, const size_t size);
void psoc_hw_reset(lv_disp_drv_t *drv);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PSOC_PLATFORM_H */
