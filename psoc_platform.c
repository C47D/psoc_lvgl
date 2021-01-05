#include "driver_generic.h"

#include "DISP_DC.h"
#include "DISP_RST.h"
#include "DISP_BCKL.h"
#include "UART.h"
#include "CyLib.h"

void psoc_display_backlight_control(lv_disp_drv_t *drv, display_backlight_control_t backlight_control)
{
    (void) drv;
    
    DISP_BCKL_Write(DISPLAY_BACKLIGHT_CONTROL_ENABLE == backlight_control ? 1 : 0);
}

void psoc_data_mode(lv_disp_drv_t *drv, data_mode_t mode)
{
    (void) drv;
    
    if (DATA_MODE_CMD == mode) {
        DISP_DC_Write(0);
    } else {
        DISP_DC_Write(1);
    }
}

void psoc_delay_ms(lv_disp_drv_t *drv, uint32_t ms)
{
    (void) drv;
    
    CyDelay(ms);
}

void psoc_write_blocking(const uint8_t *tx_data, const size_t size)
{
    
}

void psoc_write_async(const uint8_t *tx_data, uint8_t *rx_data, const size_t size)
{
    
}

void psoc_hw_reset(lv_disp_drv_t *drv)
{
    (void) drv;
    
    DISP_RST_Write(0);
    CyDelay(100);
    DISP_RST_Write(1);
    CyDelay(100);
}

/* [] END OF FILE */
