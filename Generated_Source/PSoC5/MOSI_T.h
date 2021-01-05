/*******************************************************************************
* File Name: MOSI_T.h  
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

#if !defined(CY_PINS_MOSI_T_H) /* Pins MOSI_T_H */
#define CY_PINS_MOSI_T_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MOSI_T_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MOSI_T__PORT == 15 && ((MOSI_T__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MOSI_T_Write(uint8 value);
void    MOSI_T_SetDriveMode(uint8 mode);
uint8   MOSI_T_ReadDataReg(void);
uint8   MOSI_T_Read(void);
void    MOSI_T_SetInterruptMode(uint16 position, uint16 mode);
uint8   MOSI_T_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MOSI_T_SetDriveMode() function.
     *  @{
     */
        #define MOSI_T_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MOSI_T_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MOSI_T_DM_RES_UP          PIN_DM_RES_UP
        #define MOSI_T_DM_RES_DWN         PIN_DM_RES_DWN
        #define MOSI_T_DM_OD_LO           PIN_DM_OD_LO
        #define MOSI_T_DM_OD_HI           PIN_DM_OD_HI
        #define MOSI_T_DM_STRONG          PIN_DM_STRONG
        #define MOSI_T_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MOSI_T_MASK               MOSI_T__MASK
#define MOSI_T_SHIFT              MOSI_T__SHIFT
#define MOSI_T_WIDTH              1u

/* Interrupt constants */
#if defined(MOSI_T__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MOSI_T_SetInterruptMode() function.
     *  @{
     */
        #define MOSI_T_INTR_NONE      (uint16)(0x0000u)
        #define MOSI_T_INTR_RISING    (uint16)(0x0001u)
        #define MOSI_T_INTR_FALLING   (uint16)(0x0002u)
        #define MOSI_T_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MOSI_T_INTR_MASK      (0x01u) 
#endif /* (MOSI_T__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MOSI_T_PS                     (* (reg8 *) MOSI_T__PS)
/* Data Register */
#define MOSI_T_DR                     (* (reg8 *) MOSI_T__DR)
/* Port Number */
#define MOSI_T_PRT_NUM                (* (reg8 *) MOSI_T__PRT) 
/* Connect to Analog Globals */                                                  
#define MOSI_T_AG                     (* (reg8 *) MOSI_T__AG)                       
/* Analog MUX bux enable */
#define MOSI_T_AMUX                   (* (reg8 *) MOSI_T__AMUX) 
/* Bidirectional Enable */                                                        
#define MOSI_T_BIE                    (* (reg8 *) MOSI_T__BIE)
/* Bit-mask for Aliased Register Access */
#define MOSI_T_BIT_MASK               (* (reg8 *) MOSI_T__BIT_MASK)
/* Bypass Enable */
#define MOSI_T_BYP                    (* (reg8 *) MOSI_T__BYP)
/* Port wide control signals */                                                   
#define MOSI_T_CTL                    (* (reg8 *) MOSI_T__CTL)
/* Drive Modes */
#define MOSI_T_DM0                    (* (reg8 *) MOSI_T__DM0) 
#define MOSI_T_DM1                    (* (reg8 *) MOSI_T__DM1)
#define MOSI_T_DM2                    (* (reg8 *) MOSI_T__DM2) 
/* Input Buffer Disable Override */
#define MOSI_T_INP_DIS                (* (reg8 *) MOSI_T__INP_DIS)
/* LCD Common or Segment Drive */
#define MOSI_T_LCD_COM_SEG            (* (reg8 *) MOSI_T__LCD_COM_SEG)
/* Enable Segment LCD */
#define MOSI_T_LCD_EN                 (* (reg8 *) MOSI_T__LCD_EN)
/* Slew Rate Control */
#define MOSI_T_SLW                    (* (reg8 *) MOSI_T__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MOSI_T_PRTDSI__CAPS_SEL       (* (reg8 *) MOSI_T__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MOSI_T_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MOSI_T__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MOSI_T_PRTDSI__OE_SEL0        (* (reg8 *) MOSI_T__PRTDSI__OE_SEL0) 
#define MOSI_T_PRTDSI__OE_SEL1        (* (reg8 *) MOSI_T__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MOSI_T_PRTDSI__OUT_SEL0       (* (reg8 *) MOSI_T__PRTDSI__OUT_SEL0) 
#define MOSI_T_PRTDSI__OUT_SEL1       (* (reg8 *) MOSI_T__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MOSI_T_PRTDSI__SYNC_OUT       (* (reg8 *) MOSI_T__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MOSI_T__SIO_CFG)
    #define MOSI_T_SIO_HYST_EN        (* (reg8 *) MOSI_T__SIO_HYST_EN)
    #define MOSI_T_SIO_REG_HIFREQ     (* (reg8 *) MOSI_T__SIO_REG_HIFREQ)
    #define MOSI_T_SIO_CFG            (* (reg8 *) MOSI_T__SIO_CFG)
    #define MOSI_T_SIO_DIFF           (* (reg8 *) MOSI_T__SIO_DIFF)
#endif /* (MOSI_T__SIO_CFG) */

/* Interrupt Registers */
#if defined(MOSI_T__INTSTAT)
    #define MOSI_T_INTSTAT            (* (reg8 *) MOSI_T__INTSTAT)
    #define MOSI_T_SNAP               (* (reg8 *) MOSI_T__SNAP)
    
	#define MOSI_T_0_INTTYPE_REG 		(* (reg8 *) MOSI_T__0__INTTYPE)
#endif /* (MOSI_T__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MOSI_T_H */


/* [] END OF FILE */
