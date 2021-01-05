/*******************************************************************************
* File Name: LV_TICK.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LV_TICK_H) /* Pins LV_TICK_H */
#define CY_PINS_LV_TICK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LV_TICK_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LV_TICK__PORT == 15 && ((LV_TICK__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    LV_TICK_Write(uint8 value);
void    LV_TICK_SetDriveMode(uint8 mode);
uint8   LV_TICK_ReadDataReg(void);
uint8   LV_TICK_Read(void);
void    LV_TICK_SetInterruptMode(uint16 position, uint16 mode);
uint8   LV_TICK_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LV_TICK_SetDriveMode() function.
     *  @{
     */
        #define LV_TICK_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LV_TICK_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LV_TICK_DM_RES_UP          PIN_DM_RES_UP
        #define LV_TICK_DM_RES_DWN         PIN_DM_RES_DWN
        #define LV_TICK_DM_OD_LO           PIN_DM_OD_LO
        #define LV_TICK_DM_OD_HI           PIN_DM_OD_HI
        #define LV_TICK_DM_STRONG          PIN_DM_STRONG
        #define LV_TICK_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define LV_TICK_MASK               LV_TICK__MASK
#define LV_TICK_SHIFT              LV_TICK__SHIFT
#define LV_TICK_WIDTH              1u

/* Interrupt constants */
#if defined(LV_TICK__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LV_TICK_SetInterruptMode() function.
     *  @{
     */
        #define LV_TICK_INTR_NONE      (uint16)(0x0000u)
        #define LV_TICK_INTR_RISING    (uint16)(0x0001u)
        #define LV_TICK_INTR_FALLING   (uint16)(0x0002u)
        #define LV_TICK_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define LV_TICK_INTR_MASK      (0x01u) 
#endif /* (LV_TICK__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LV_TICK_PS                     (* (reg8 *) LV_TICK__PS)
/* Data Register */
#define LV_TICK_DR                     (* (reg8 *) LV_TICK__DR)
/* Port Number */
#define LV_TICK_PRT_NUM                (* (reg8 *) LV_TICK__PRT) 
/* Connect to Analog Globals */                                                  
#define LV_TICK_AG                     (* (reg8 *) LV_TICK__AG)                       
/* Analog MUX bux enable */
#define LV_TICK_AMUX                   (* (reg8 *) LV_TICK__AMUX) 
/* Bidirectional Enable */                                                        
#define LV_TICK_BIE                    (* (reg8 *) LV_TICK__BIE)
/* Bit-mask for Aliased Register Access */
#define LV_TICK_BIT_MASK               (* (reg8 *) LV_TICK__BIT_MASK)
/* Bypass Enable */
#define LV_TICK_BYP                    (* (reg8 *) LV_TICK__BYP)
/* Port wide control signals */                                                   
#define LV_TICK_CTL                    (* (reg8 *) LV_TICK__CTL)
/* Drive Modes */
#define LV_TICK_DM0                    (* (reg8 *) LV_TICK__DM0) 
#define LV_TICK_DM1                    (* (reg8 *) LV_TICK__DM1)
#define LV_TICK_DM2                    (* (reg8 *) LV_TICK__DM2) 
/* Input Buffer Disable Override */
#define LV_TICK_INP_DIS                (* (reg8 *) LV_TICK__INP_DIS)
/* LCD Common or Segment Drive */
#define LV_TICK_LCD_COM_SEG            (* (reg8 *) LV_TICK__LCD_COM_SEG)
/* Enable Segment LCD */
#define LV_TICK_LCD_EN                 (* (reg8 *) LV_TICK__LCD_EN)
/* Slew Rate Control */
#define LV_TICK_SLW                    (* (reg8 *) LV_TICK__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LV_TICK_PRTDSI__CAPS_SEL       (* (reg8 *) LV_TICK__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LV_TICK_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LV_TICK__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LV_TICK_PRTDSI__OE_SEL0        (* (reg8 *) LV_TICK__PRTDSI__OE_SEL0) 
#define LV_TICK_PRTDSI__OE_SEL1        (* (reg8 *) LV_TICK__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LV_TICK_PRTDSI__OUT_SEL0       (* (reg8 *) LV_TICK__PRTDSI__OUT_SEL0) 
#define LV_TICK_PRTDSI__OUT_SEL1       (* (reg8 *) LV_TICK__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LV_TICK_PRTDSI__SYNC_OUT       (* (reg8 *) LV_TICK__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(LV_TICK__SIO_CFG)
    #define LV_TICK_SIO_HYST_EN        (* (reg8 *) LV_TICK__SIO_HYST_EN)
    #define LV_TICK_SIO_REG_HIFREQ     (* (reg8 *) LV_TICK__SIO_REG_HIFREQ)
    #define LV_TICK_SIO_CFG            (* (reg8 *) LV_TICK__SIO_CFG)
    #define LV_TICK_SIO_DIFF           (* (reg8 *) LV_TICK__SIO_DIFF)
#endif /* (LV_TICK__SIO_CFG) */

/* Interrupt Registers */
#if defined(LV_TICK__INTSTAT)
    #define LV_TICK_INTSTAT            (* (reg8 *) LV_TICK__INTSTAT)
    #define LV_TICK_SNAP               (* (reg8 *) LV_TICK__SNAP)
    
	#define LV_TICK_0_INTTYPE_REG 		(* (reg8 *) LV_TICK__0__INTTYPE)
#endif /* (LV_TICK__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LV_TICK_H */


/* [] END OF FILE */
