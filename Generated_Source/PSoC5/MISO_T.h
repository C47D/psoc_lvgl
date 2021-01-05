/*******************************************************************************
* File Name: MISO_T.h  
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

#if !defined(CY_PINS_MISO_T_H) /* Pins MISO_T_H */
#define CY_PINS_MISO_T_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MISO_T_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MISO_T__PORT == 15 && ((MISO_T__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MISO_T_Write(uint8 value);
void    MISO_T_SetDriveMode(uint8 mode);
uint8   MISO_T_ReadDataReg(void);
uint8   MISO_T_Read(void);
void    MISO_T_SetInterruptMode(uint16 position, uint16 mode);
uint8   MISO_T_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MISO_T_SetDriveMode() function.
     *  @{
     */
        #define MISO_T_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MISO_T_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MISO_T_DM_RES_UP          PIN_DM_RES_UP
        #define MISO_T_DM_RES_DWN         PIN_DM_RES_DWN
        #define MISO_T_DM_OD_LO           PIN_DM_OD_LO
        #define MISO_T_DM_OD_HI           PIN_DM_OD_HI
        #define MISO_T_DM_STRONG          PIN_DM_STRONG
        #define MISO_T_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MISO_T_MASK               MISO_T__MASK
#define MISO_T_SHIFT              MISO_T__SHIFT
#define MISO_T_WIDTH              1u

/* Interrupt constants */
#if defined(MISO_T__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MISO_T_SetInterruptMode() function.
     *  @{
     */
        #define MISO_T_INTR_NONE      (uint16)(0x0000u)
        #define MISO_T_INTR_RISING    (uint16)(0x0001u)
        #define MISO_T_INTR_FALLING   (uint16)(0x0002u)
        #define MISO_T_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MISO_T_INTR_MASK      (0x01u) 
#endif /* (MISO_T__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MISO_T_PS                     (* (reg8 *) MISO_T__PS)
/* Data Register */
#define MISO_T_DR                     (* (reg8 *) MISO_T__DR)
/* Port Number */
#define MISO_T_PRT_NUM                (* (reg8 *) MISO_T__PRT) 
/* Connect to Analog Globals */                                                  
#define MISO_T_AG                     (* (reg8 *) MISO_T__AG)                       
/* Analog MUX bux enable */
#define MISO_T_AMUX                   (* (reg8 *) MISO_T__AMUX) 
/* Bidirectional Enable */                                                        
#define MISO_T_BIE                    (* (reg8 *) MISO_T__BIE)
/* Bit-mask for Aliased Register Access */
#define MISO_T_BIT_MASK               (* (reg8 *) MISO_T__BIT_MASK)
/* Bypass Enable */
#define MISO_T_BYP                    (* (reg8 *) MISO_T__BYP)
/* Port wide control signals */                                                   
#define MISO_T_CTL                    (* (reg8 *) MISO_T__CTL)
/* Drive Modes */
#define MISO_T_DM0                    (* (reg8 *) MISO_T__DM0) 
#define MISO_T_DM1                    (* (reg8 *) MISO_T__DM1)
#define MISO_T_DM2                    (* (reg8 *) MISO_T__DM2) 
/* Input Buffer Disable Override */
#define MISO_T_INP_DIS                (* (reg8 *) MISO_T__INP_DIS)
/* LCD Common or Segment Drive */
#define MISO_T_LCD_COM_SEG            (* (reg8 *) MISO_T__LCD_COM_SEG)
/* Enable Segment LCD */
#define MISO_T_LCD_EN                 (* (reg8 *) MISO_T__LCD_EN)
/* Slew Rate Control */
#define MISO_T_SLW                    (* (reg8 *) MISO_T__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MISO_T_PRTDSI__CAPS_SEL       (* (reg8 *) MISO_T__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MISO_T_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MISO_T__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MISO_T_PRTDSI__OE_SEL0        (* (reg8 *) MISO_T__PRTDSI__OE_SEL0) 
#define MISO_T_PRTDSI__OE_SEL1        (* (reg8 *) MISO_T__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MISO_T_PRTDSI__OUT_SEL0       (* (reg8 *) MISO_T__PRTDSI__OUT_SEL0) 
#define MISO_T_PRTDSI__OUT_SEL1       (* (reg8 *) MISO_T__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MISO_T_PRTDSI__SYNC_OUT       (* (reg8 *) MISO_T__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MISO_T__SIO_CFG)
    #define MISO_T_SIO_HYST_EN        (* (reg8 *) MISO_T__SIO_HYST_EN)
    #define MISO_T_SIO_REG_HIFREQ     (* (reg8 *) MISO_T__SIO_REG_HIFREQ)
    #define MISO_T_SIO_CFG            (* (reg8 *) MISO_T__SIO_CFG)
    #define MISO_T_SIO_DIFF           (* (reg8 *) MISO_T__SIO_DIFF)
#endif /* (MISO_T__SIO_CFG) */

/* Interrupt Registers */
#if defined(MISO_T__INTSTAT)
    #define MISO_T_INTSTAT            (* (reg8 *) MISO_T__INTSTAT)
    #define MISO_T_SNAP               (* (reg8 *) MISO_T__SNAP)
    
	#define MISO_T_0_INTTYPE_REG 		(* (reg8 *) MISO_T__0__INTTYPE)
#endif /* (MISO_T__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MISO_T_H */


/* [] END OF FILE */
