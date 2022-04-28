/**
 * @file
 * Dynamic Host Configuration Protocol client
 *
 * @defgroup dhcp4 DHCPv4
 * @ingroup ip4
 * DHCP (IPv4) related functions
 * This is a DHCP client for the lwIP TCP/IP stack. It aims to conform
 * with RFC 2131 and RFC 2132.
 *
 * @todo:
 * - Support for interfaces other than Ethernet (SLIP, PPP, ...)
 *
 * Options:
 * @ref DHCP_COARSE_TIMER_SECS (recommended 60 which is a minute)
 * @ref DHCP_FINE_TIMER_MSECS (recommended 500 which equals TCP coarse timer)
 *
 * dhcp_start() starts a DHCP client instance which
 * configures the interface by obtaining an IP address lease and maintaining it.
 *
 * Use dhcp_release() to end the lease and use dhcp_stop()
 * to remove the DHCP client.
 *
 * @see netifapi_dhcp4
 */

/*
 * Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
 * Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
 *
 * Author: Leon Woestenberg <leon.woestenberg@gmx.net>
 *
 */

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/def.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/dns.h"
#include "lwip/etharp.h"
#include "lwip/prot/dhcp.h"

#include <string.h>

#if defined(LWIP_ROMBUILD)
#include "ipv4/dhcp_if.h"
#endif

#if OPL_WIFI_SLEEP_MODE
#include "controller_wifi_patch.h"

extern uint8_t opl_wifi_ps_mode_restore;
extern uint8_t opl_wifi_ps_mode_debug;
#endif

#if OPL_DISABLE_PHY_EQU
#include "controller_wifi_patch.h"
#endif

extern uint8_t g_dhcp_retry_mode;
extern uint32_t g_dhcp_retry_interval;
extern uint32_t g_restored_ip_addr;

/*
 * This function is workaround for OPL1000 products.
 * 1. Enter Wi-Fi Light PS Pool mode.
 * 2. Disable Wi-Fi PHY Equalizer.
 * 3. Restore MTU size.
 */
void opl_enter_operation(struct netif *netif)
{
    //1. WiFi PS pool
#if OPL_WIFI_SLEEP_MODE
    /* PS Light Pool mode */
    if ((opl_wifi_ps_mode_debug == false)) {
        if (opl_wifi_ps_mode_restore != STA_PS_AWAKE_MODE) {
            CtrlWifi_PsStateForceInternal(STA_MODE_LIGHT_PS, false);
        }
    }
#endif

    //2. PHY EQU
#if OPL_DISABLE_PHY_EQU
    ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_DISABLE_EQU_VAL);
#endif

    //3. MTU size
    netif->mtu = opl_lwip_mtu;
}

/*
 * This function is workaround for OPL1000 products.
 * 1. Restore Wi-Fi Deep PS Pool mode.
 * 2. Enable Wi-Fi PHY Equalizer.
 * 3. Update MTU size.
 */
void opl_leave_operation(struct netif *netif)
{
    //1. WiFi PS pool
#if OPL_WIFI_SLEEP_MODE
    /* Restore PS Pool mode */
    if ((opl_wifi_ps_mode_debug == false)) {
        CtrlWifi_PsStateForceInternal(opl_wifi_ps_mode_restore, false);
    }
#endif

    //2. PHY EQU
#if OPL_DISABLE_PHY_EQU
    ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_ENABLE_EQU_VAL);
#endif

    //3. MTU size
    netif->mtu = opl_lwip_mtu_update;
}

static bool
dhcp_ip_addr_restore_patch(struct netif *netif)
{
    struct dhcp *dhcp = netif_dhcp_data(netif);
    if (g_restored_ip_addr == IPADDR_ANY) {
        return false;
    }
    else {
        dhcp->offered_ip_addr.addr = g_restored_ip_addr;
        printf("restore ip_addr =%0x\n", g_restored_ip_addr);
        return true;
    }
}

void lwip_load_interface_dhcp_patch(void)
{
    g_dhcp_retry_mode     = 0;
    g_dhcp_retry_interval = 2000;
    
    dhcp_restore_mtu_adpt    =    opl_enter_operation;
    dhcp_update_mtu_adpt     =    opl_leave_operation;
    dhcp_ip_addr_restore_adpt =   dhcp_ip_addr_restore_patch;
}

#endif /* LWIP_IPV4 && LWIP_DHCP */
