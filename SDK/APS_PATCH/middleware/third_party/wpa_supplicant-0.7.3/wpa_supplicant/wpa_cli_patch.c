/*
 * WPA Supplicant - command line interface for wpa_supplicant daemon
 * Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmsis_os.h"
#include "controller_wifi.h"
#include "controller_wifi_com.h"
#include "wpa_cli.h"
#include "driver_netlink.h"
#include "msg.h"

#include "wpa_cli_patch.h"
#include "controller_wifi_patch.h"

extern int wpa_cli_dbg_impl(int argc, char *argv[]);


/*
 * For debug used.
 * wpa_dbg rf #WiFi_PS_POLL_MODE
 *     #WiFi_PS_POLL_MODE
 *         0: Deep mode
 *         1: No PS POLL, (RF always wake-up)
 *         3: Light mode
 */
static int wpa_cli_dbg_patch(int argc, char *argv[])
{
    if (argc > 1) {
        if (!strcmp(argv[1], "rf")) {
            if (argc > 2) {
                int ps_mode = atoi(argv[2]);
                CtrlWifi_PsStateForceInternal(ps_mode, true);
                tracer_cli(LOG_HIGH_LEVEL, "ok, mode=%d\r\n", ps_mode);
                return TRUE;
            }
        }
    }

    wpa_cli_dbg_impl(argc, argv);
    return TRUE;
}

void wpa_cli_func_init_patch(void)
{
    wpa_cli_dbg                     = wpa_cli_dbg_patch;
}
