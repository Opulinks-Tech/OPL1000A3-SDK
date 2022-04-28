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
 *******************************************************************************/
/**
 * @file at_cmd_sys.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File supports the System module AT Commands.
 *
 */
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "svn_rev.h"

/*
 * @brief Command at+gmr
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_gmr_patch(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        msg_print_uart1("\r\n");

        msg_print_uart1("AT version info: %s \r\n", AT_VER);
        msg_print_uart1("SDK version info: %d \r\n", SVN_REVISION);
        msg_print_uart1("Compile time: %s \r\n", SVN_TIME_NOW);

        msg_print_uart1("\r\nOK\r\n");
    }

    return true;
}

/**
  * @brief AT Command Table for System Module
  *
  */
extern at_command_t gAtCmdTbl_Sys[32];

/*
 * @brief AT Command Interface Initialization for System modules
 *
 */
void at_cmd_sys_func_init_patch(void)
{
    gAtCmdTbl_Sys[1].cmd_handle = at_cmd_sys_gmr_patch;
}
