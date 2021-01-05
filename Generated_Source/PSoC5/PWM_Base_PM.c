/*******************************************************************************
* File Name: PWM_Base_PM.c
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

#include "PWM_Base.h"

static PWM_Base_backupStruct PWM_Base_backup;


/*******************************************************************************
* Function Name: PWM_Base_SaveConfig
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
*  PWM_Base_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Base_SaveConfig(void) 
{

    #if(!PWM_Base_UsingFixedFunction)
        #if(!PWM_Base_PWMModeIsCenterAligned)
            PWM_Base_backup.PWMPeriod = PWM_Base_ReadPeriod();
        #endif /* (!PWM_Base_PWMModeIsCenterAligned) */
        PWM_Base_backup.PWMUdb = PWM_Base_ReadCounter();
        #if (PWM_Base_UseStatus)
            PWM_Base_backup.InterruptMaskValue = PWM_Base_STATUS_MASK;
        #endif /* (PWM_Base_UseStatus) */

        #if(PWM_Base_DeadBandMode == PWM_Base__B_PWM__DBM_256_CLOCKS || \
            PWM_Base_DeadBandMode == PWM_Base__B_PWM__DBM_2_4_CLOCKS)
            PWM_Base_backup.PWMdeadBandValue = PWM_Base_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Base_KillModeMinTime)
             PWM_Base_backup.PWMKillCounterPeriod = PWM_Base_ReadKillTime();
        #endif /* (PWM_Base_KillModeMinTime) */

        #if(PWM_Base_UseControl)
            PWM_Base_backup.PWMControlRegister = PWM_Base_ReadControlRegister();
        #endif /* (PWM_Base_UseControl) */
    #endif  /* (!PWM_Base_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Base_RestoreConfig
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
*  PWM_Base_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Base_RestoreConfig(void) 
{
        #if(!PWM_Base_UsingFixedFunction)
            #if(!PWM_Base_PWMModeIsCenterAligned)
                PWM_Base_WritePeriod(PWM_Base_backup.PWMPeriod);
            #endif /* (!PWM_Base_PWMModeIsCenterAligned) */

            PWM_Base_WriteCounter(PWM_Base_backup.PWMUdb);

            #if (PWM_Base_UseStatus)
                PWM_Base_STATUS_MASK = PWM_Base_backup.InterruptMaskValue;
            #endif /* (PWM_Base_UseStatus) */

            #if(PWM_Base_DeadBandMode == PWM_Base__B_PWM__DBM_256_CLOCKS || \
                PWM_Base_DeadBandMode == PWM_Base__B_PWM__DBM_2_4_CLOCKS)
                PWM_Base_WriteDeadTime(PWM_Base_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Base_KillModeMinTime)
                PWM_Base_WriteKillTime(PWM_Base_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Base_KillModeMinTime) */

            #if(PWM_Base_UseControl)
                PWM_Base_WriteControlRegister(PWM_Base_backup.PWMControlRegister);
            #endif /* (PWM_Base_UseControl) */
        #endif  /* (!PWM_Base_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Base_Sleep
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
*  PWM_Base_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Base_Sleep(void) 
{
    #if(PWM_Base_UseControl)
        if(PWM_Base_CTRL_ENABLE == (PWM_Base_CONTROL & PWM_Base_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Base_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Base_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Base_UseControl) */

    /* Stop component */
    PWM_Base_Stop();

    /* Save registers configuration */
    PWM_Base_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Base_Wakeup
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
*  PWM_Base_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Base_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Base_RestoreConfig();

    if(PWM_Base_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Base_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
