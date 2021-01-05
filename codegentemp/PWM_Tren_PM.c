/*******************************************************************************
* File Name: PWM_Tren_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_Tren.h"

static PWM_Tren_backupStruct PWM_Tren_backup;


/*******************************************************************************
* Function Name: PWM_Tren_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Tren_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Tren_SaveConfig(void) 
{

    #if(!PWM_Tren_UsingFixedFunction)
        #if(!PWM_Tren_PWMModeIsCenterAligned)
            PWM_Tren_backup.PWMPeriod = PWM_Tren_ReadPeriod();
        #endif /* (!PWM_Tren_PWMModeIsCenterAligned) */
        PWM_Tren_backup.PWMUdb = PWM_Tren_ReadCounter();
        #if (PWM_Tren_UseStatus)
            PWM_Tren_backup.InterruptMaskValue = PWM_Tren_STATUS_MASK;
        #endif /* (PWM_Tren_UseStatus) */

        #if(PWM_Tren_DeadBandMode == PWM_Tren__B_PWM__DBM_256_CLOCKS || \
            PWM_Tren_DeadBandMode == PWM_Tren__B_PWM__DBM_2_4_CLOCKS)
            PWM_Tren_backup.PWMdeadBandValue = PWM_Tren_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Tren_KillModeMinTime)
             PWM_Tren_backup.PWMKillCounterPeriod = PWM_Tren_ReadKillTime();
        #endif /* (PWM_Tren_KillModeMinTime) */

        #if(PWM_Tren_UseControl)
            PWM_Tren_backup.PWMControlRegister = PWM_Tren_ReadControlRegister();
        #endif /* (PWM_Tren_UseControl) */
    #endif  /* (!PWM_Tren_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Tren_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Tren_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Tren_RestoreConfig(void) 
{
        #if(!PWM_Tren_UsingFixedFunction)
            #if(!PWM_Tren_PWMModeIsCenterAligned)
                PWM_Tren_WritePeriod(PWM_Tren_backup.PWMPeriod);
            #endif /* (!PWM_Tren_PWMModeIsCenterAligned) */

            PWM_Tren_WriteCounter(PWM_Tren_backup.PWMUdb);

            #if (PWM_Tren_UseStatus)
                PWM_Tren_STATUS_MASK = PWM_Tren_backup.InterruptMaskValue;
            #endif /* (PWM_Tren_UseStatus) */

            #if(PWM_Tren_DeadBandMode == PWM_Tren__B_PWM__DBM_256_CLOCKS || \
                PWM_Tren_DeadBandMode == PWM_Tren__B_PWM__DBM_2_4_CLOCKS)
                PWM_Tren_WriteDeadTime(PWM_Tren_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Tren_KillModeMinTime)
                PWM_Tren_WriteKillTime(PWM_Tren_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Tren_KillModeMinTime) */

            #if(PWM_Tren_UseControl)
                PWM_Tren_WriteControlRegister(PWM_Tren_backup.PWMControlRegister);
            #endif /* (PWM_Tren_UseControl) */
        #endif  /* (!PWM_Tren_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Tren_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Tren_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Tren_Sleep(void) 
{
    #if(PWM_Tren_UseControl)
        if(PWM_Tren_CTRL_ENABLE == (PWM_Tren_CONTROL & PWM_Tren_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Tren_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Tren_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Tren_UseControl) */

    /* Stop component */
    PWM_Tren_Stop();

    /* Save registers configuration */
    PWM_Tren_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Tren_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Tren_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Tren_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Tren_RestoreConfig();

    if(PWM_Tren_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Tren_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
