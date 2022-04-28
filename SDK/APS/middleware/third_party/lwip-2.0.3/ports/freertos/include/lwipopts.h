/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
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
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

#include "ports/opl_option/opl_lwipopts.h"

#define LWIP_IPV4                  1
#define LWIP_IPV6                  1

#define NO_SYS                     0
#define LWIP_SOCKET               (NO_SYS==0)
#define LWIP_NETCONN              (NO_SYS==0)

#define LWIP_IGMP                  LWIP_IPV4
#define LWIP_ICMP                  LWIP_IPV4

#define LWIP_SNMP                  0//LWIP_UDP
#define MIB2_STATS                 0//LWIP_SNMP

#define LWIP_DNS                   LWIP_UDP
#define LWIP_MDNS_RESPONDER        LWIP_UDP

#define LWIP_NUM_NETIF_CLIENT_DATA (LWIP_MDNS_RESPONDER)

#define LWIP_HAVE_LOOPIF           1
#define LWIP_NETIF_LOOPBACK        1
#define LWIP_LOOPBACK_MAX_PBUFS    10

#define TCP_LISTEN_BACKLOG         1

#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 1

//#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 0
/**
 * LWIP_NETIF_TX_SINGLE_PBUF: if this is set to 1, lwIP tries to put all data
 * to be sent into one single pbuf. This is for compatibility with DMA-enabled
 * MACs that do not support scatter-gather.
 * Beware that this might involve CPU-memcpy before transmitting that would not
 * be needed without this flag! Use this only if you need to!
 *
 * @todo: TCP and IP-frag do not work with this, yet:
 */
#define LWIP_NETIF_TX_SINGLE_PBUF             1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
#define LWIP_COMPAT_SOCKETS        1
#define LWIP_SO_RCVTIMEO           1
#define LWIP_SO_RCVBUF             0
#define LWIP_SO_SNDTIMEO           1
#define LWIP_SO_LINGER             1

#define LWIP_TCP_KEEPALIVE         1
#define SO_REUSE                   1

#define LWIP_TCPIP_CORE_LOCKING    1

#define LWIP_NETIF_LINK_CALLBACK   1
#define LWIP_NETIF_STATUS_CALLBACK 1
#define LWIP_NETIF_IP_CHANGE_CALLBACK 1

#define LWIP_DYNAMIC_DEBUG_ENABLE
#if defined(LWIP_DYNAMIC_DEBUG_ENABLE)
//#define LWIP_DBG_MIN_LEVEL LWIP_DBG_LEVEL_WARNING
//#define MEMP_OVERFLOW_CHECK         1
//#define MEMP_SANITY_CHECK           1
#define LWIP_DEBUG LWIP_DBG_ON
#define LWIP_DEBUG_IDX(idx) ((idx) >> 8)

#define ETHARP_DEBUG                    0x0000U
#define NETIF_DEBUG                     0x0100U
#define PBUF_DEBUG                      0x0200U
#define API_LIB_DEBUG                   0x0300U
#define API_MSG_DEBUG                   0x0400U
#define SOCKETS_DEBUG                   0x0500U
#define ICMP_DEBUG                      0x0600U
#define IGMP_DEBUG                      0x0700U
#define INET_DEBUG                      0x0800U
#define IP_DEBUG                        0x0900U
#define IP_REASS_DEBUG                  0x0a00U
#define RAW_DEBUG                       0x0b00U
#define MEM_DEBUG                       0x0c00U
#define MEMP_DEBUG                      0x0d00U
#define SYS_DEBUG                       0x0e00U
#define TIMERS_DEBUG                    0x0f00U
#define TCP_DEBUG                       0x1000U
#define TCP_INPUT_DEBUG                 0x1100U
#define TCP_FR_DEBUG                    0x1200U
#define TCP_RTO_DEBUG                   0x1300U
#define TCP_CWND_DEBUG                  0x1400U
#define TCP_WND_DEBUG                   0x1500U
#define TCP_OUTPUT_DEBUG                0x1600U
#define TCP_RST_DEBUG                   0x1700U
#define TCP_QLEN_DEBUG                  0x1800U
#define UDP_DEBUG                       0x1900U
#define TCPIP_DEBUG                     0x1a00U
#define PPP_DEBUG                       0x1b00U
#define SLIP_DEBUG                      0x1c00U
#define DHCP_DEBUG                      0x1d00U
#define AUTOIP_DEBUG                    0x1e00U
#define SNMP_MSG_DEBUG                  0x1f00U
#define SNMP_MIB_DEBUG                  0x2000U
#define DNS_DEBUG                       0x2100U

#else

//#define LWIP_DEBUG
#ifdef LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK         1
#define MEMP_SANITY_CHECK           1

#define LWIP_DBG_MIN_LEVEL         0
#define PPP_DEBUG                  LWIP_DBG_OFF
#define MEM_DEBUG                  LWIP_DBG_OFF
#define MEMP_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                 LWIP_DBG_OFF
#define API_LIB_DEBUG              LWIP_DBG_OFF
#define API_MSG_DEBUG              LWIP_DBG_OFF
#define TCPIP_DEBUG                LWIP_DBG_ON
#define NETIF_DEBUG                LWIP_DBG_ON
#define SOCKETS_DEBUG              LWIP_DBG_ON
#define DNS_DEBUG                  LWIP_DBG_OFF
#define AUTOIP_DEBUG               LWIP_DBG_OFF
#define DHCP_DEBUG                 LWIP_DBG_ON
#define ETHARP_DEBUG               LWIP_DBG_OFF
#define IP_DEBUG                   LWIP_DBG_ON
#define IP_REASS_DEBUG             LWIP_DBG_OFF
#define ICMP_DEBUG                 LWIP_DBG_ON
#define IGMP_DEBUG                 LWIP_DBG_OFF
#define UDP_DEBUG                  LWIP_DBG_OFF
#define TCP_DEBUG                  LWIP_DBG_ON
#define TCP_INPUT_DEBUG            LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG           LWIP_DBG_ON
#define TCP_RTO_DEBUG              LWIP_DBG_ON
#define TCP_CWND_DEBUG             LWIP_DBG_ON
#define TCP_WND_DEBUG              LWIP_DBG_ON
#define TCP_FR_DEBUG               LWIP_DBG_OFF
#define TCP_QLEN_DEBUG             LWIP_DBG_OFF
#define TCP_RST_DEBUG              LWIP_DBG_OFF
#endif

#endif

#define LWIP_DBG_TYPES_ON         (LWIP_DBG_ON|LWIP_DBG_TRACE|LWIP_DBG_STATE|LWIP_DBG_FRESH|LWIP_DBG_HALT)

/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/
/**
 * TCPIP_THREAD_NAME: The name assigned to the main tcpip thread.
 */
#define TCPIP_THREAD_NAME              "opl_tcpip"

/**
 * TCPIP_THREAD_STACKSIZE: The stack size used by the main tcpip thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//stack size value is in words(4bytes), see the definition of freeRTOS "portSTACK_TYPE"
#ifdef LWIP_DEBUG
#define TCPIP_THREAD_STACKSIZE          tcpip_thread_stacksize //512 // (512*4)bytes
#else
#define TCPIP_THREAD_STACKSIZE          tcpip_thread_stacksize //256 change to 512 for wifi security
#endif

/**
 * TCPIP_THREAD_PRIO: The priority assigned to the main tcpip thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#define TCPIP_THREAD_PRIO               tcpip_thread_prio //(tskIDLE_PRIORITY + 1)

/**
 * DEFAULT_THREAD_NAME: The name assigned to any other lwIP thread.
 */
#define DEFAULT_THREAD_NAME            "lwIP"

/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#define DEFAULT_THREAD_STACKSIZE        256 // (256*4) bytes

/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#define DEFAULT_THREAD_PRIO             (tskIDLE_PRIORITY + 1)

/**
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */
#define TCPIP_MBOX_SIZE                 opl_lwip_tcpip_mbox_size
#define DEFAULT_TCP_RECVMBOX_SIZE       opl_lwip_tcp_recv_mbox_size
#define DEFAULT_UDP_RECVMBOX_SIZE       opl_lwip_udp_recv_mbox_size
#define DEFAULT_RAW_RECVMBOX_SIZE       opl_lwip_raw_recv_mbox_size
#define DEFAULT_ACCEPTMBOX_SIZE         opl_lwip_accept_mbox_size

/**
 * TCP_TMR_INTERVAL: TCP timer interval
 */
#define TCP_TMR_INTERVAL                250
#define TCP_TMR_INTERVAL_VALUE          opl_lwip_tcp_tmr_interval

/* ---------- Memory options ---------- */
/**
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc provided by your C-library
 * instead of the lwip internal allocator. Can save code size if you
 * already use it.
 */
#define MEM_LIBC_MALLOC         1

/**
* MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the lwip pool allocator.
* Especially useful with MEM_LIBC_MALLOC but handle with care regarding execution
* speed and usage from interrupts!
*/
#define MEMP_MEM_MALLOC         0

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                (14 * 1024)


/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/
 /* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           16
/* MEMP_NUM_RAW_PCB: the number of UDP protocol control blocks. One
   per active RAW "connection". */
#define MEMP_NUM_RAW_PCB        8
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        8
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        8
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 8
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        16
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
//#define MEMP_NUM_SYS_TIMEOUT    15

/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
#define MEMP_NUM_NETBUF         2
/* MEMP_NUM_NETCONN: the number of struct netconns. */
#define MEMP_NUM_NETCONN        10
/* MEMP_NUM_TCPIP_MSG_*: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
#define MEMP_NUM_TCPIP_MSG_API   16
#define MEMP_NUM_TCPIP_MSG_INPKT 16


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
//#define PBUF_POOL_SIZE          160

/* We used the Opulinks option, set the TCP_MSS_DEFAULT is 1460 bytes
   for PBUF_POOL_BUFSIZE use. (Lwip receive buffer size)*/
#define PBUF_POOL_TCP_MSS_DEFAULT  1460
/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE      LWIP_MEM_ALIGN_SIZE(PBUF_POOL_TCP_MSS_DEFAULT+40+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)

/**
 * PBUF_LINK_HLEN: the number of bytes that should be allocated for a
 * link level header. The default is 14, the standard value for
 * Ethernet.
 */
#define PBUF_LINK_HLEN          16

/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    (NO_SYS==0)


/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* We used the Opulinks option, so disable the check setting in preprocessor. */
#define LWIP_DISABLE_TCP_SANITY_CHECKS 1

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

/* TCP Maximum segment size. */
#define TCP_MSS                 opl_lwip_tcp_mss

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             opl_lwip_tcp_snd_buf

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN        opl_lwip_tcp_snd_queue_len

/* TCP writable space (bytes). This must be less than or equal
   to TCP_SND_BUF. It is the amount of space which must be
   available in the tcp snd_buf for select to return writable */
#define TCP_SNDLOWAT            opl_lwip_tcp_snd_low_wat

/**
 * TCP_SNDQUEUELOWAT: TCP writable bufs (pbuf count). This must be less
 * than TCP_SND_QUEUELEN. If the number of pbufs queued on a pcb drops below
 * this number, select returns writable (combined with TCP_SNDLOWAT).
 */
 #define TCP_SNDQUEUELOWAT      opl_lwip_tcp_snd_queue_low_wat

/* TCP receive window. */
#define TCP_WND                 opl_lwip_tcp_receive_wnd

/**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */
#define TCP_WND_UPDATE_THRESHOLD opl_lwip_tcp_wnd_update_threshold

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              opl_lwip_tcp_max_retry_of_data

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           opl_lwip_tcp_max_retry_of_sync

/**
 * TCP_OVERSIZE: The maximum number of bytes that tcp_write may
 * allocate ahead of time in an attempt to create shorter pbuf chains
 * for transmission. The meaningful range is 0 to TCP_MSS. Some
 * suggested values are:
 *
 * 0:         Disable oversized allocation. Each tcp_write() allocates a new
              pbuf (old behaviour).
 * 1:         Allocate size-aligned pbufs with minimal excess. Use this if your
 *            scatter-gather DMA requires aligned fragments.
 * 128:       Limit the pbuf/memory overhead to 20%.
 * TCP_MSS:   Try to create unfragmented TCP packets.
 * TCP_MSS/4: Try to create 4 fragments or less per TCP packet.
 */
#define TCP_OVERSIZE            1
#define TCP_OVERSIZE_VALUE      opl_lwip_tcp_over_size
/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
#define LWIP_ARP                1
#define ARP_TABLE_SIZE          10
#define ARP_QUEUEING            1


/* ---------- IP options ---------- */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD              0

/* IP reassembly and segmentation.These are orthogonal even
 * if they both deal with IP fragments */
#define IP_REASSEMBLY           1
#define IP_REASS_MAX_PBUFS      10
#define MEMP_NUM_REASSDATA      10
#define IP_FRAG                 1
//#define LWIP_IPV6_FRAG          1


/* ---------- ICMP options ---------- */
#define ICMP_TTL                255


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. */
#define LWIP_DHCP               1

#define LWIP_NETIF_HOSTNAME     1

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
/* ARP before DHCP causes multi-second delay  - turn it off ? */
#define DHCP_DOES_ARP_CHECK    (LWIP_DHCP)


/* ---------- AUTOIP options ------- */
#define LWIP_AUTOIP            (LWIP_DHCP)
#define LWIP_DHCP_AUTOIP_COOP  (LWIP_DHCP && LWIP_AUTOIP)


/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define LWIP_UDPLITE            LWIP_UDP
#define UDP_TTL                 255


/* ---------- RAW options ---------- */
#define LWIP_RAW                1


/* ---------- Statistics options ---------- */

#define LWIP_STATS              1
#define LWIP_STATS_DISPLAY      1

#if LWIP_STATS
#define LINK_STATS              1
#define IP_STATS                1
#define ICMP_STATS              1
#define IGMP_STATS              1
#define IPFRAG_STATS            1
#define UDP_STATS               1
#define TCP_STATS               1
#define MEM_STATS               1
#define MEMP_STATS              1
#define PBUF_STATS              1
#define SYS_STATS               1
#endif /* LWIP_STATS */


/* ---------- PPP options ---------- */

#define PPP_SUPPORT             0      /* Set > 0 for PPP */

#if PPP_SUPPORT

#define NUM_PPP                 1      /* Max PPP sessions. */


/* Select modules to enable.  Ideally these would be set in the makefile but
 * we're limited by the command line length so you need to modify the settings
 * in this file.
 */
#define PPPOE_SUPPORT           1
#define PPPOS_SUPPORT           1

#define PAP_SUPPORT             1      /* Set > 0 for PAP. */
#define CHAP_SUPPORT            1      /* Set > 0 for CHAP. */
#define MSCHAP_SUPPORT          0      /* Set > 0 for MSCHAP */
#define CBCP_SUPPORT            0      /* Set > 0 for CBCP (NOT FUNCTIONAL!) */
#define CCP_SUPPORT             0      /* Set > 0 for CCP */
#define VJ_SUPPORT              1      /* Set > 0 for VJ header compression. */
#define MD5_SUPPORT             1      /* Set > 0 for MD5 (see also CHAP) */

#endif /* PPP_SUPPORT */


#define LWIP_DHCP_MAX_NTP_SERVERS       3

/* Opulinks options */
#define OPL_LWIP                        1
#define OPL_DHCP                        1
#define OPL_DHCP_RESTORE_LAST_IP        1
#define OPL_RANDOM_TCP_PORT             1
#define OPL_TCP_RTO                     1
#define OPL_PS_SMART_SLEEP              1
#define OPL_ARP_DUPLICATE_ADDR_DETECT   1
#define OPL_DYN_UPDATE_MTU              1
#define OPL_WIFI_SLEEP_MODE             1
#define OPL_DISABLE_PHY_EQU             1

#endif /* LWIP_LWIPOPTS_H */
