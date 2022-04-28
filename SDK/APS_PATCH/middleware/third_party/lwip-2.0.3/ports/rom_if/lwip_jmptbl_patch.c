/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2021
******************************************************************************/

#include "lwip_jmptbl_patch.h"

extern void lwip_load_interface_etharp_patch(void);
extern void lwip_load_interface_ethernet_patch(void);
extern void lwip_load_interface_dhcp_patch(void);

void lwip_module_interface_init_patch(void)
{
    lwip_load_interface_etharp_patch();
    lwip_load_interface_ethernet_patch();
    lwip_load_interface_dhcp_patch();
}
