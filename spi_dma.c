#include "project.h"
#include "spi_dma.h"

#define TD_POOL_SIZE    (10U)
// #define DBG_SPI_DMA

static const uint8_t DMA_REQUEST_PER_BURST = 1;
static const uint8_t DMA_BYTES_PER_BURST = 1;
static const size_t MAX_BYTES_PER_TD = 4095;

static uint8_t DMA_Tx_TD_pool[TD_POOL_SIZE] = {0};
static uint8_t DMA_Tx_Chan = 0;

static volatile spi_xfer_status_t spi_xfer_status = SPI_XFER_IDLE;

/* Keep track of the number of allocated TDs */
static volatile size_t g_tds_needed = 0;

void spi_initialize_dma(void)
{
    DMA_Tx_Chan = DMA_Tx_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST,
        HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
}

/* Transfer data in blocking mode */
void send_spi(const uint8_t *tx_data, const size_t size)
{
    uint8_t dummy = 0;
    
    spi_xfer_status = SPI_XFER_ONGOING;
    
    SS_Write(0);
    
    for (size_t idx = 0; idx < size; idx++) {
        SPI_WriteTxData(tx_data[idx]);
        
        while(!(SPI_ReadTxStatus() & SPI_STS_BYTE_COMPLETE)) {
        }
        
        /* Read the data available on the Rx FIFO so it doesn't trigger an
         * overrun interrupt */
        dummy = SPI_ReadRxData();
    }
    
    SS_Write(1);
    
    spi_xfer_status = SPI_XFER_IDLE;
}

/* Called when the data transfer is done.
 * We need to disable the DMA channel and free the allocated TDs.
 *
 * Interesting links:
 * https://community.cypress.com/thread/31826
 * https://community.cypress.com/thread/14232 */
void spi_xfer_on_completion(void)
{
    spi_xfer_set_status(SPI_XFER_IDLE);
    
    cystatus ch_disable_sts = CyDmaChDisable(DMA_Tx_Chan);
    if (CYRET_SUCCESS != ch_disable_sts) {
        UART_PutString("Error trying to disable the DMA channel");
        
        while (1) {
            
        }
    }

#ifdef DBG_SPI_DMA
    UART_PutString("TDs used: ");
    UART_PutHexByte(g_tds_needed);
    UART_PutCRLF();
    uint8_t free_tds = CyDmaTdFreeCount();
    UART_PutString("Free TDs: ");
    UART_PutHexByte(free_tds);
#endif

    /* Free all used TDs */
    for (size_t idx = 0; idx < g_tds_needed; idx++) {
#ifdef DBG_SPI_DMA
        UART_PutCRLF();
        UART_PutString("Freeing TD handle ");
        UART_PutHexByte(DMA_Tx_TD_pool[idx]);
        UART_PutCRLF();
#endif
        CyDmaTdFree(DMA_Tx_TD_pool[idx]);
    }

#ifdef DBG_SPI_DMA
    free_tds = CyDmaTdFreeCount();
    UART_PutChar('.');
    UART_PutHexByte(free_tds);
    UART_PutCRLF();
#endif
}

uint8_t spi_xfer_is_ongoing(void)
{
    return (uint8_t) SPI_XFER_IDLE != spi_xfer_status;
}

void spi_xfer_set_status(spi_xfer_status_t status)
{
    spi_xfer_status = status;
}

spi_xfer_status_t spi_xfer_get_status(void)
{
    return spi_xfer_status;
}

static size_t calculate_needed_tds(size_t bytes_to_transfer)
{
    /* We only need one td for transfers <= MAX_BYTES_PER_TD (4096) bytes */
    size_t retval = 1;
    
    /* Keep track of the needed TDs to transfer bytes_to_transfer bytes */
    if (MAX_BYTES_PER_TD < bytes_to_transfer) {
        retval = (bytes_to_transfer + MAX_BYTES_PER_TD) / MAX_BYTES_PER_TD;
    }

#ifdef DBG_SPI_DMA
    UART_PutString("Needed TDs: ");
    UART_PutHexInt((uint16_t) retval);
    UART_PutCRLF();
#endif

    return retval;
}

// transfer_count parameter is limited to 4095 bytes.
void send_spi_dma(const uint8_t *tx_data, uint8_t *rx_data, const size_t size)
{    
    (void) rx_data;
    
    if (0 == size) {
        return;
    }
    
    cystatus tx_td_sts;
    size_t tx_addr_offset = 0;
    
    SPI_ClearTxBuffer();
    
    /* Calculate neccesary TDs */
    g_tds_needed = calculate_needed_tds(size);
    
#ifdef DBG_SPI_DMA
    UART_PutString("Allocating ");
    UART_PutHexByte(g_tds_needed);
    UART_PutString(" TDs\r\n");
#endif
    
    /* Allocate a pool of TD handles */
    for (size_t idx = 0; idx < g_tds_needed; idx++) {
        uint8_t td_idx = CyDmaTdAllocate();
        
        if (CY_DMA_INVALID_TD != td_idx) {
#ifdef DBG_SPI_DMA
            UART_PutString("Allocated\r\n");
#endif
            DMA_Tx_TD_pool[idx] = td_idx;
        } else {
            UART_PutString("No free TDs available");
            
            while (1) {
            
            }
        }
    }
    
    /* First TD on the pool is always the initial TD of the DMA channel */
    cystatus td_sts = CyDmaChSetInitialTd(DMA_Tx_Chan, DMA_Tx_TD_pool[0]);
    if (CYRET_SUCCESS != td_sts) {
        UART_PutString("Invalid first TD");
        
        while (1) {
            
        }
    }
    
    /* Number of TDs with MAX_BYTES_PE_TD bytes to send */
    size_t td_max_bytes = size / MAX_BYTES_PER_TD;
    
    /* Los bytes a enviar por el ultimo TD se calculan restandole los bytes
     * enviados por los TDs previos, en caso de usar mas de 1 TD todos los TDs
     * excepto el ultimo envian MAX_BYTES_PER_TD. */
    uint16_t bytes_for_last_td = size - (td_max_bytes * MAX_BYTES_PER_TD);;

#ifdef DBG_SPI_DMA
    UART_PutString("TDs with MAX_BYTES_PER_TD: ");
    UART_PutHexInt((uint16_t) td_max_bytes);
    UART_PutCRLF();
    UART_PutString("Bytes for last TD: ");
    UART_PutHexInt((uint16_t) bytes_for_last_td);
    UART_PutCRLF();
#endif

    /* Configure all needed TDs */    
    for (size_t td_idx = 0; td_idx < g_tds_needed; td_idx++) {
        
        uint8_t is_last_td_to_configure = td_idx == (g_tds_needed - 1);
        
        if (is_last_td_to_configure) {
#ifdef DBG_SPI_DMA
            UART_PutString("Configuring last TD...\r\nTD idx: ");
            UART_PutHexByte(td_idx);
            UART_PutString(" - TD handle: ");
            UART_PutHexByte(DMA_Tx_TD_pool[td_idx]);
            UART_PutCRLF();
#endif

            /* The last TD should generate a pulse on the nrq signal to let us know it
             * is done transfering data */
            tx_td_sts = CyDmaTdSetConfiguration(DMA_Tx_TD_pool[td_idx],
                bytes_for_last_td,
                CY_DMA_DISABLE_TD, /* Disable this DMA channel after this TD is done */
                CY_DMA_TD_INC_SRC_ADR | DMA_Tx__TD_TERMOUT_EN);
            
            if (CYRET_SUCCESS != tx_td_sts) {
                UART_PutString("Invalid TD configuration");
                
                while (1) {
            
                }
            }

#ifdef DBG_SPI_DMA
            UART_PutString("source array offset: ");
            UART_PutHexInt(tx_addr_offset);
            UART_PutCRLF();
            
            UART_PutString("Configuring TD addresses\r\n");
#endif

            tx_td_sts = CyDmaTdSetAddress(DMA_Tx_TD_pool[td_idx],
                LO16((uint32_t) &tx_data[tx_addr_offset]),
                LO16((uint32_t) SPI_TXDATA_PTR));
            
            if (CYRET_SUCCESS != tx_td_sts) {
                UART_PutString("Invalid TD address");
                UART_PutHexByte(tx_td_sts);
                
                while (1) {
            
                }
            }
            
            break;
        } else {
#ifdef DBG_SPI_DMA
            UART_PutString("Configuring TD...\r\nTD idx: ");
            UART_PutHexByte(td_idx);
            UART_PutCRLF();
#endif

            tx_td_sts = CyDmaTdSetConfiguration(DMA_Tx_TD_pool[td_idx],
                (uint16_t) MAX_BYTES_PER_TD,
                DMA_Tx_TD_pool[td_idx + 1] /* Next TD */,
                CY_DMA_TD_INC_SRC_ADR);
            
            if (CYRET_SUCCESS != tx_td_sts) {
                UART_PutString("Invalid TD configuration");
                while (1) {
            
                }
            }

#ifdef DBG_SPI_DMA
            UART_PutString("source array offset:");
            UART_PutHexInt(tx_addr_offset);
            UART_PutCRLF();
#endif

            tx_td_sts = CyDmaTdSetAddress(DMA_Tx_TD_pool[td_idx],
                LO16((uint32_t) &tx_data[tx_addr_offset]),
                LO16((uint32_t) SPI_TXDATA_PTR));
            
            if (CYRET_SUCCESS != tx_td_sts) {
                UART_PutString("Invalid TD address");
                
                while (1) {
            
                }
            }
            
            tx_addr_offset += MAX_BYTES_PER_TD;
        }
    }
    
#ifdef DBG_SPI_DMA
    UART_PutString("Done configuring\r\n");
#endif

    spi_xfer_status = SPI_XFER_ONGOING;
    CyDmaChEnable(DMA_Tx_Chan, 1 /* Restore TDs configuration when done */);
    
    SS_Write(0);
    
    /* let the spi interrupt tx fifo not full trigger the dma request */
    SPI_SetTxInterruptMode(1u << SPI_STS_TX_FIFO_NOT_FULL_SHIFT);
}

/* [] END OF FILE */
