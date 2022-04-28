/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file ps_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _PS_PATCH_H_
#define _PS_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "ps.h"
#include "cmsis_os.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
// public function for other module
void ps_patch_init(void);

// private function
void ps_lock_gpio_apply_patch(void);
uint32_t ps_synchronize_timers_patch(void);
TickType_t ps_adjust_tick_patch(TickType_t tick);

// public function for user

// ps task
void ps_task_create_patch(void);


#ifdef __cplusplus
}
#endif
#endif  /* _PS_PATCH_H_ */
