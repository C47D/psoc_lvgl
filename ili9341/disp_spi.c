/**
 * @file disp_spi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <string.h>

#include "lvgl/lvgl.h"

#include "disp_spi.h"
#include "spi_dma.h"

/*********************
 *      DEFINES
 *********************/
#define TAG "DISPLAY_TL"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static volatile disp_spi_send_flag_t spi_flags = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void display_spi_transaction(const uint8_t *data, uint16_t length,
    disp_spi_send_flag_t flags, disp_spi_read_data *out,
    uint64_t addr)
{
    (void) addr;
    (void) out;
    
    if (0 == length) {
        return;
    }

    spi_flags = flags;

    /* Wait for previous pending transaction results */
    display_tl_poll_is_busy();

    if (flags & DISP_SPI_RECEIVE) {

    }

    if (flags & DISP_SPI_ADDRESS_24) {

    }

    /* Poll/Complete/Queue transaction */
    if (flags & DISP_SPI_SEND_POLLING) {
        // NOTE: Platform dependent
        send_spi(data, (size_t) length);
    } else if (flags & DISP_SPI_SEND_SYNCHRONOUS) {

    } else {
        // NOTE: Platform dependent
        send_spi_dma(data, NULL, (size_t) length);
    }
}

void display_tl_poll_is_busy(void)
{
    while (spi_xfer_is_ongoing()) {
        
    }
}

void display_spi_acquire(void)
{

}

void display_spi_release(void)
{

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Called on the spi complete callback */
void display_colors_sent_notification(void)
{
    if (spi_flags & DISP_SPI_SIGNAL_FLUSH) {
        lv_disp_t * disp = NULL;

#if (LVGL_VERSION_MAJOR >= 7)
        disp = _lv_refr_get_disp_refreshing();
#else /* Before v7 */
        disp = lv_refr_get_disp_refreshing();
#endif

        lv_disp_flush_ready(&disp->driver);
    }
}
