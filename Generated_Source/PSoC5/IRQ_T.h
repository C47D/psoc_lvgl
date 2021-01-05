/*******************************************************************************
* File Name: IRQ_T.h  
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

#if !defined(CY_PINS_IRQ_T_H) /* Pins IRQ_T_H */
#define CY_PINS_IRQ_T_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IRQ_T_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 IRQ_T__PORT == 15 && ((IRQ_T__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    IRQ_T_Write(uint8 value);
void    IRQ_T_SetDriveMode(uint8 mode);
uint8   IRQ_T_ReadDataReg(void);
uint8   IRQ_T_Read(void);
void    IRQ_T_SetInterruptMode(uint16 position, uint16 mode);
uint8   IRQ_T_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the IRQ_T_SetDriveMode() function.
     *  @{
     */
        #define IRQ_T_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define IRQ_T_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define IRQ_T_DM_RES_UP          PIN_DM_RES_UP
        #define IRQ_T_DM_RES_DWN         PIN_DM_RES_DWN
        #define IRQ_T_DM_OD_LO           PIN_DM_OD_LO
        #define IRQ_T_DM_OD_HI           PIN_DM_OD_HI
        #define IRQ_T_DM_STRONG          PIN_DM_STRONG
        #define IRQ_T_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define IRQ_T_MASK               IRQ_T__MASK
#define IRQ_T_SHIFT              IRQ_T__SHIFT
#define IRQ_T_WIDTH              1u

/* Interrupt constants */
#if defined(IRQ_T__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IRQ_T_SetInterruptMode() function.
     *  @{
     */
        #define IRQ_T_INTR_NONE      (uint16)(0x0000u)
        #define IRQ_T_INTR_RISING    (uint16)(0x0001u)
        #define IRQ_T_INTR_FALLING   (uint16)(0x0002u)
        #define IRQ_T_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define IRQ_T_INTR_MASK      (0x01u) 
#endif /* (IRQ_T__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IRQ_T_PS                     (* (reg8 *) IRQ_T__PS)
/* Data Register */
#define IRQ_T_DR                     (* (reg8 *) IRQ_T__DR)
/* Port Number */
#define IRQ_T_PRT_NUM                (* (reg8 *) IRQ_T__PRT) 
/* Connect to Analog Globals */                                                  
#define IRQ_T_AG                     (* (reg8 *) IRQ_T__AG)                       
/* Analog MUX bux enable */
#define IRQ_T_AMUX                   (* (reg8 *) IRQ_T__AMUX) 
/* Bidirectional Enable */                                                        
#define IRQ_T_BIE                    (* (reg8 *) IRQ_T__BIE)
/* Bit-mask for Aliased Register Access */
#define IRQ_T_BIT_MASK               (* (reg8 *) IRQ_T__BIT_MASK)
/* Bypass Enable */
#define IRQ_T_BYP                    (* (reg8 *) IRQ_T__BYP)
/* Port wide control signals */                                                   
#define IRQ_T_CTL                    (* (reg8 *) IRQ_T__CTL)
/* Drive Modes */
#define IRQ_T_DM0                    (* (reg8 *) IRQ_T__DM0) 
#define IRQ_T_DM1                    (* (reg8 *) IRQ_T__DM1)
#define IRQ_T_DM2                    (* (reg8 *) IRQ_T__DM2) 
/* Input Buffer Disable Override */
#define IRQ_T_INP_DIS                (* (reg8 *) IRQ_T__INP_DIS)
/* LCD Common or Segment Drive */
#define IRQ_T_LCD_COM_SEG            (* (reg8 *) IRQ_T__LCD_COM_SEG)
/* Enable Segment LCD */
#define IRQ_T_LCD_EN                 (* (reg8 *) IRQ_T__LCD_EN)
/* Slew Rate Control */
#define IRQ_T_SLW                    (* (reg8 *) IRQ_T__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IRQ_T_PRTDSI__CAPS_SEL       (* (reg8 *) IRQ_T__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IRQ_T_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IRQ_T__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IRQ_T_PRTDSI__OE_SEL0        (* (reg8 *) IRQ_T__PRTDSI__OE_SEL0) 
#define IRQ_T_PRTDSI__OE_SEL1        (* (reg8 *) IRQ_T__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IRQ_T_PRTDSI__OUT_SEL0       (* (reg8 *) IRQ_T__PRTDSI__OUT_SEL0) 
#define IRQ_T_PRTDSI__OUT_SEL1       (* (reg8 *) IRQ_T__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IRQ_T_PRTDSI__SYNC_OUT       (* (reg8 *) IRQ_T__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(IRQ_T__SIO_CFG)
    #define IRQ_T_SIO_HYST_EN        (* (reg8 *) IRQ_T__SIO_HYST_EN)
    #define IRQ_T_SIO_REG_HIFREQ     (* (reg8 *) IRQ_T__SIO_REG_HIFREQ)
    #define IRQ_T_SIO_CFG            (* (reg8 *) IRQ_T__SIO_CFG)
    #define IRQ_T_SIO_DIFF           (* (reg8 *) IRQ_T__SIO_DIFF)
#endif /* (IRQ_T__SIO_CFG) */

/* Interrupt Registers */
#if defined(IRQ_T__INTSTAT)
    #define IRQ_T_INTSTAT            (* (reg8 *) IRQ_T__INTSTAT)
    #define IRQ_T_SNAP               (* (reg8 *) IRQ_T__SNAP)
    
	#define IRQ_T_0_INTTYPE_REG 		(* (reg8 *) IRQ_T__0__INTTYPE)
#endif /* (IRQ_T__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_IRQ_T_H */


/* [] END OF FILE */
