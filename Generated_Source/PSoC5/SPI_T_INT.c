/*******************************************************************************
* File Name: SPI_T_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_T_PVT.h"
#include "cyapicallbacks.h"

/* User code required at start of ISR */
/* `#START SPI_T_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: SPI_T_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_T_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPI_T_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SPI_T_txBuffer[SPI_T_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_T_TX_ISR)
{
    #if(SPI_T_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SPI_T_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_T_TX_ISR_ENTRY_CALLBACK
        SPI_T_TX_ISR_EntryCallback();
    #endif /* SPI_T_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_T_TX_ISR_START` */

    /* `#END` */
    
    #if(SPI_T_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SPI_T_txBufferRead != SPI_T_txBufferWrite)
        {
            tmpStatus = SPI_T_GET_STATUS_TX(SPI_T_swStatusTx);
            SPI_T_swStatusTx = tmpStatus;

            if(0u != (SPI_T_swStatusTx & SPI_T_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SPI_T_txBufferFull)
                {
                   SPI_T_txBufferRead++;

                    if(SPI_T_txBufferRead >= SPI_T_TX_BUFFER_SIZE)
                    {
                        SPI_T_txBufferRead = 0u;
                    }
                }
                else
                {
                    SPI_T_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(SPI_T_TXDATA_PTR, 
                                             SPI_T_txBuffer[SPI_T_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SPI_T_txBufferRead == SPI_T_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SPI_T_TX_STATUS_MASK_REG &= ((uint8) ~SPI_T_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SPI_T_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_T_TX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_T_TX_ISR_EXIT_CALLBACK
        SPI_T_TX_ISR_ExitCallback();
    #endif /* SPI_T_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: SPI_T_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_T_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SPI_T_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SPI_T_rxBuffer[SPI_T_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_T_RX_ISR)
{
    #if(SPI_T_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (SPI_T_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_T_RX_ISR_ENTRY_CALLBACK
        SPI_T_RX_ISR_EntryCallback();
    #endif /* SPI_T_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_T_RX_ISR_START` */

    /* `#END` */
    
    #if(SPI_T_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SPI_T_GET_STATUS_RX(SPI_T_swStatusRx);
        SPI_T_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SPI_T_swStatusRx & SPI_T_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(SPI_T_RXDATA_PTR);

            /* Set next pointer. */
            SPI_T_rxBufferWrite++;
            if(SPI_T_rxBufferWrite >= SPI_T_RX_BUFFER_SIZE)
            {
                SPI_T_rxBufferWrite = 0u;
            }

            if(SPI_T_rxBufferWrite == SPI_T_rxBufferRead)
            {
                SPI_T_rxBufferRead++;
                if(SPI_T_rxBufferRead >= SPI_T_RX_BUFFER_SIZE)
                {
                    SPI_T_rxBufferRead = 0u;
                }

                SPI_T_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SPI_T_rxBuffer[SPI_T_rxBufferWrite] = rxData;

            tmpStatus = SPI_T_GET_STATUS_RX(SPI_T_swStatusRx);
            SPI_T_swStatusRx = tmpStatus;
        }

    #endif /* (SPI_T_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_T_RX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_T_RX_ISR_EXIT_CALLBACK
        SPI_T_RX_ISR_ExitCallback();
    #endif /* SPI_T_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
