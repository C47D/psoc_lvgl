#ifndef SPI_DMA_H
#define SPI_DMA_H

#include <stdint.h>
#include <stddef.h>

/* TODO: Find a better name for the type because we also use this flag
 * on blocking spi transfers. */
typedef enum {
    SPI_XFER_IDLE,
    SPI_XFER_DONE,
    SPI_XFER_ONGOING,
} spi_xfer_status_t;

void spi_initialize_dma(void);
void spi_xfer_on_completion(void);

uint8_t spi_xfer_is_ongoing(void);
void spi_xfer_set_status(spi_xfer_status_t status);
spi_xfer_status_t spi_xfer_get_status(void);

void send_spi_dma(const uint8_t *tx_data, uint8_t *rx_data, const size_t size);
void send_spi(const uint8_t *tx_data, const size_t size);

#endif

/* [] END OF FILE */
