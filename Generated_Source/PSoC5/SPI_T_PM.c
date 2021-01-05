/*******************************************************************************
* File Name: SPI_T_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_T_PVT.h"

static SPI_T_BACKUP_STRUCT SPI_T_backup =
{
    SPI_T_DISABLED,
    SPI_T_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_T_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_T_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_T_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_T_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_T_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_T_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_T_Sleep(void) 
{
    /* Save components enable state */
    SPI_T_backup.enableState = ((uint8) SPI_T_IS_ENABLED);

    SPI_T_Stop();
}


/*******************************************************************************
* Function Name: SPI_T_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_T_backup - used when non-retention registers are restored.
*  SPI_T_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_T_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_T_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_T_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_T_Wakeup(void) 
{
    #if(SPI_T_RX_SOFTWARE_BUF_ENABLED)
        SPI_T_rxBufferFull  = 0u;
        SPI_T_rxBufferRead  = 0u;
        SPI_T_rxBufferWrite = 0u;
    #endif /* (SPI_T_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_T_TX_SOFTWARE_BUF_ENABLED)
        SPI_T_txBufferFull  = 0u;
        SPI_T_txBufferRead  = 0u;
        SPI_T_txBufferWrite = 0u;
    #endif /* (SPI_T_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_T_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_T_backup.enableState)
    {
        SPI_T_Enable();
    }
}


/* [] END OF FILE */
