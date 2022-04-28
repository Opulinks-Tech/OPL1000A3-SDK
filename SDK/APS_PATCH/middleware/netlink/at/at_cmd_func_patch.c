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
#include "at_cmd_func_patch.h"
#include "at_cmd_sys_patch.h"
#include "at_cmd_tcpip_patch.h"
#include "at_cmd_wifi_patch.h"

void at_func_init_patch(void)
{
    at_cmd_sys_func_init_patch();
    at_cmd_tcpip_api_preinit_patch();
    at_cmd_wifi_func_init_patch();
}
