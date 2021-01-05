/*******************************************************************************
* File Name: TXgate.h  
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

#if !defined(CY_PINS_TXgate_H) /* Pins TXgate_H */
#define CY_PINS_TXgate_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TXgate_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TXgate__PORT == 15 && ((TXgate__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TXgate_Write(uint8 value);
void    TXgate_SetDriveMode(uint8 mode);
uint8   TXgate_ReadDataReg(void);
uint8   TXgate_Read(void);
void    TXgate_SetInterruptMode(uint16 position, uint16 mode);
uint8   TXgate_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TXgate_SetDriveMode() function.
     *  @{
     */
        #define TXgate_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TXgate_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TXgate_DM_RES_UP          PIN_DM_RES_UP
        #define TXgate_DM_RES_DWN         PIN_DM_RES_DWN
        #define TXgate_DM_OD_LO           PIN_DM_OD_LO
        #define TXgate_DM_OD_HI           PIN_DM_OD_HI
        #define TXgate_DM_STRONG          PIN_DM_STRONG
        #define TXgate_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TXgate_MASK               TXgate__MASK
#define TXgate_SHIFT              TXgate__SHIFT
#define TXgate_WIDTH              1u

/* Interrupt constants */
#if defined(TXgate__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TXgate_SetInterruptMode() function.
     *  @{
     */
        #define TXgate_INTR_NONE      (uint16)(0x0000u)
        #define TXgate_INTR_RISING    (uint16)(0x0001u)
        #define TXgate_INTR_FALLING   (uint16)(0x0002u)
        #define TXgate_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TXgate_INTR_MASK      (0x01u) 
#endif /* (TXgate__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TXgate_PS                     (* (reg8 *) TXgate__PS)
/* Data Register */
#define TXgate_DR                     (* (reg8 *) TXgate__DR)
/* Port Number */
#define TXgate_PRT_NUM                (* (reg8 *) TXgate__PRT) 
/* Connect to Analog Globals */                                                  
#define TXgate_AG                     (* (reg8 *) TXgate__AG)                       
/* Analog MUX bux enable */
#define TXgate_AMUX                   (* (reg8 *) TXgate__AMUX) 
/* Bidirectional Enable */                                                        
#define TXgate_BIE                    (* (reg8 *) TXgate__BIE)
/* Bit-mask for Aliased Register Access */
#define TXgate_BIT_MASK               (* (reg8 *) TXgate__BIT_MASK)
/* Bypass Enable */
#define TXgate_BYP                    (* (reg8 *) TXgate__BYP)
/* Port wide control signals */                                                   
#define TXgate_CTL                    (* (reg8 *) TXgate__CTL)
/* Drive Modes */
#define TXgate_DM0                    (* (reg8 *) TXgate__DM0) 
#define TXgate_DM1                    (* (reg8 *) TXgate__DM1)
#define TXgate_DM2                    (* (reg8 *) TXgate__DM2) 
/* Input Buffer Disable Override */
#define TXgate_INP_DIS                (* (reg8 *) TXgate__INP_DIS)
/* LCD Common or Segment Drive */
#define TXgate_LCD_COM_SEG            (* (reg8 *) TXgate__LCD_COM_SEG)
/* Enable Segment LCD */
#define TXgate_LCD_EN                 (* (reg8 *) TXgate__LCD_EN)
/* Slew Rate Control */
#define TXgate_SLW                    (* (reg8 *) TXgate__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TXgate_PRTDSI__CAPS_SEL       (* (reg8 *) TXgate__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TXgate_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TXgate__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TXgate_PRTDSI__OE_SEL0        (* (reg8 *) TXgate__PRTDSI__OE_SEL0) 
#define TXgate_PRTDSI__OE_SEL1        (* (reg8 *) TXgate__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TXgate_PRTDSI__OUT_SEL0       (* (reg8 *) TXgate__PRTDSI__OUT_SEL0) 
#define TXgate_PRTDSI__OUT_SEL1       (* (reg8 *) TXgate__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TXgate_PRTDSI__SYNC_OUT       (* (reg8 *) TXgate__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TXgate__SIO_CFG)
    #define TXgate_SIO_HYST_EN        (* (reg8 *) TXgate__SIO_HYST_EN)
    #define TXgate_SIO_REG_HIFREQ     (* (reg8 *) TXgate__SIO_REG_HIFREQ)
    #define TXgate_SIO_CFG            (* (reg8 *) TXgate__SIO_CFG)
    #define TXgate_SIO_DIFF           (* (reg8 *) TXgate__SIO_DIFF)
#endif /* (TXgate__SIO_CFG) */

/* Interrupt Registers */
#if defined(TXgate__INTSTAT)
    #define TXgate_INTSTAT            (* (reg8 *) TXgate__INTSTAT)
    #define TXgate_SNAP               (* (reg8 *) TXgate__SNAP)
    
	#define TXgate_0_INTTYPE_REG 		(* (reg8 *) TXgate__0__INTTYPE)
#endif /* (TXgate__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TXgate_H */


/* [] END OF FILE */
