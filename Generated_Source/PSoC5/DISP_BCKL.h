/*******************************************************************************
* File Name: DISP_BCKL.h  
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

#if !defined(CY_PINS_DISP_BCKL_H) /* Pins DISP_BCKL_H */
#define CY_PINS_DISP_BCKL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DISP_BCKL_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DISP_BCKL__PORT == 15 && ((DISP_BCKL__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DISP_BCKL_Write(uint8 value);
void    DISP_BCKL_SetDriveMode(uint8 mode);
uint8   DISP_BCKL_ReadDataReg(void);
uint8   DISP_BCKL_Read(void);
void    DISP_BCKL_SetInterruptMode(uint16 position, uint16 mode);
uint8   DISP_BCKL_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DISP_BCKL_SetDriveMode() function.
     *  @{
     */
        #define DISP_BCKL_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DISP_BCKL_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DISP_BCKL_DM_RES_UP          PIN_DM_RES_UP
        #define DISP_BCKL_DM_RES_DWN         PIN_DM_RES_DWN
        #define DISP_BCKL_DM_OD_LO           PIN_DM_OD_LO
        #define DISP_BCKL_DM_OD_HI           PIN_DM_OD_HI
        #define DISP_BCKL_DM_STRONG          PIN_DM_STRONG
        #define DISP_BCKL_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DISP_BCKL_MASK               DISP_BCKL__MASK
#define DISP_BCKL_SHIFT              DISP_BCKL__SHIFT
#define DISP_BCKL_WIDTH              1u

/* Interrupt constants */
#if defined(DISP_BCKL__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DISP_BCKL_SetInterruptMode() function.
     *  @{
     */
        #define DISP_BCKL_INTR_NONE      (uint16)(0x0000u)
        #define DISP_BCKL_INTR_RISING    (uint16)(0x0001u)
        #define DISP_BCKL_INTR_FALLING   (uint16)(0x0002u)
        #define DISP_BCKL_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DISP_BCKL_INTR_MASK      (0x01u) 
#endif /* (DISP_BCKL__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DISP_BCKL_PS                     (* (reg8 *) DISP_BCKL__PS)
/* Data Register */
#define DISP_BCKL_DR                     (* (reg8 *) DISP_BCKL__DR)
/* Port Number */
#define DISP_BCKL_PRT_NUM                (* (reg8 *) DISP_BCKL__PRT) 
/* Connect to Analog Globals */                                                  
#define DISP_BCKL_AG                     (* (reg8 *) DISP_BCKL__AG)                       
/* Analog MUX bux enable */
#define DISP_BCKL_AMUX                   (* (reg8 *) DISP_BCKL__AMUX) 
/* Bidirectional Enable */                                                        
#define DISP_BCKL_BIE                    (* (reg8 *) DISP_BCKL__BIE)
/* Bit-mask for Aliased Register Access */
#define DISP_BCKL_BIT_MASK               (* (reg8 *) DISP_BCKL__BIT_MASK)
/* Bypass Enable */
#define DISP_BCKL_BYP                    (* (reg8 *) DISP_BCKL__BYP)
/* Port wide control signals */                                                   
#define DISP_BCKL_CTL                    (* (reg8 *) DISP_BCKL__CTL)
/* Drive Modes */
#define DISP_BCKL_DM0                    (* (reg8 *) DISP_BCKL__DM0) 
#define DISP_BCKL_DM1                    (* (reg8 *) DISP_BCKL__DM1)
#define DISP_BCKL_DM2                    (* (reg8 *) DISP_BCKL__DM2) 
/* Input Buffer Disable Override */
#define DISP_BCKL_INP_DIS                (* (reg8 *) DISP_BCKL__INP_DIS)
/* LCD Common or Segment Drive */
#define DISP_BCKL_LCD_COM_SEG            (* (reg8 *) DISP_BCKL__LCD_COM_SEG)
/* Enable Segment LCD */
#define DISP_BCKL_LCD_EN                 (* (reg8 *) DISP_BCKL__LCD_EN)
/* Slew Rate Control */
#define DISP_BCKL_SLW                    (* (reg8 *) DISP_BCKL__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DISP_BCKL_PRTDSI__CAPS_SEL       (* (reg8 *) DISP_BCKL__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DISP_BCKL_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DISP_BCKL__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DISP_BCKL_PRTDSI__OE_SEL0        (* (reg8 *) DISP_BCKL__PRTDSI__OE_SEL0) 
#define DISP_BCKL_PRTDSI__OE_SEL1        (* (reg8 *) DISP_BCKL__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DISP_BCKL_PRTDSI__OUT_SEL0       (* (reg8 *) DISP_BCKL__PRTDSI__OUT_SEL0) 
#define DISP_BCKL_PRTDSI__OUT_SEL1       (* (reg8 *) DISP_BCKL__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DISP_BCKL_PRTDSI__SYNC_OUT       (* (reg8 *) DISP_BCKL__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DISP_BCKL__SIO_CFG)
    #define DISP_BCKL_SIO_HYST_EN        (* (reg8 *) DISP_BCKL__SIO_HYST_EN)
    #define DISP_BCKL_SIO_REG_HIFREQ     (* (reg8 *) DISP_BCKL__SIO_REG_HIFREQ)
    #define DISP_BCKL_SIO_CFG            (* (reg8 *) DISP_BCKL__SIO_CFG)
    #define DISP_BCKL_SIO_DIFF           (* (reg8 *) DISP_BCKL__SIO_DIFF)
#endif /* (DISP_BCKL__SIO_CFG) */

/* Interrupt Registers */
#if defined(DISP_BCKL__INTSTAT)
    #define DISP_BCKL_INTSTAT            (* (reg8 *) DISP_BCKL__INTSTAT)
    #define DISP_BCKL_SNAP               (* (reg8 *) DISP_BCKL__SNAP)
    
	#define DISP_BCKL_0_INTTYPE_REG 		(* (reg8 *) DISP_BCKL__0__INTTYPE)
#endif /* (DISP_BCKL__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DISP_BCKL_H */


/* [] END OF FILE */
