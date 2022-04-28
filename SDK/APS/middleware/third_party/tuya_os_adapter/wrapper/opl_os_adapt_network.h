/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

#ifndef __OPL_OS_ADAPT_NETWORK_H__
#define __OPL_OS_ADAPT_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_network.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef UNW_ERRNO_T (*t_opl_os_adapt_net_get_errno_fp)(void);
typedef UNW_IP_ADDR_T (*t_opl_os_adapt_net_addr_fp)(const signed char *ip);
typedef UNW_IP_ADDR_T (*t_opl_os_adapt_net_str2addr_fp)(signed char *ip);
typedef int (*t_opl_os_adapt_net_fd_set_fp)(const int fd, UNW_FD_SET_T* fds);
typedef int (*t_opl_os_adapt_net_fd_clear_fp)(const int fd, UNW_FD_SET_T* fds);
typedef int (*t_opl_os_adapt_net_fd_isset_fp)(const int fd, UNW_FD_SET_T* fds);
typedef int (*t_opl_os_adapt_net_fd_zero_fp)(UNW_FD_SET_T* fds);
typedef int (*t_opl_os_adapt_net_select_fp)(const int maxfd, UNW_FD_SET_T *readfds, UNW_FD_SET_T *writefds,\
                        UNW_FD_SET_T *errorfds, const unsigned int ms_timeout);
typedef int (*t_opl_os_adapt_net_close_fp)(const int fd);
typedef int (*t_opl_os_adapt_net_shutdown_fp)(const int fd, const int how);
typedef int (*t_opl_os_adapt_net_socket_create_fp)(const UNW_PROTOCOL_TYPE type);
typedef int (*t_opl_os_adapt_net_connect_fp)(const int fd, const UNW_IP_ADDR_T addr, const unsigned short port);
typedef int (*t_opl_os_adapt_net_connect_raw_fp)(const int fd, void *p_socket_addr, const int len);
typedef int (*t_opl_os_adapt_net_bind_fp)(const int fd, const UNW_IP_ADDR_T addr, const unsigned short port);
typedef int (*t_opl_os_adapt_net_socket_bind_fp)(const int fd, const char *ip);
typedef int (*t_opl_os_adapt_net_listen_fp)(const int fd, const int backlog);
typedef int (*t_opl_os_adapt_net_accept_fp)(const int fd, UNW_IP_ADDR_T *addr, unsigned short *port);
typedef int (*t_opl_os_adapt_net_send_fp)(const int fd, const void *buf, const unsigned int nbytes);
typedef int (*t_opl_os_adapt_net_send_to_fp)(const int fd, const void *buf, const unsigned int nbytes,\
                         const UNW_IP_ADDR_T addr,const unsigned short port);
typedef int (*t_opl_os_adapt_net_recv_fp)(const int fd, void *buf, const unsigned int nbytes);
typedef int (*t_opl_os_adapt_net_recv_nd_size_fp)(const int fd, \
                              void *buf, \
                              const unsigned int buf_size, \
                              const unsigned int nd_size);
typedef int (*t_opl_os_adapt_net_recvfrom_fp)(const int fd, \
                          void *buf, \
                          const unsigned int nbytes,\
                          UNW_IP_ADDR_T *addr, \
                          unsigned short *port);
typedef int (*t_opl_os_adapt_net_set_block_fp)(const int fd, const bool block);
typedef int (*t_opl_os_adapt_net_get_nonblock_fp)(const int fd);
typedef int (*t_opl_os_adapt_net_set_timeout_fp)(const int fd, \
                             const int ms_timeout,\
                             const UNW_TRANS_TYPE_E type);
typedef int (*t_opl_os_adapt_net_set_bufsize_fp)(const int fd, \
                             const int buf_size,\
                             const UNW_TRANS_TYPE_E type);
typedef int (*t_opl_os_adapt_net_set_reuse_fp)(const int fd);
typedef int (*t_opl_os_adapt_net_disable_nagle_fp)(const int fd);
typedef int (*t_opl_os_adapt_net_set_boardcast_fp)(const int fd);
typedef int (*t_opl_os_adapt_net_set_keepalive_fp)(const int fd, \
                               const bool alive,\
                               const unsigned int idle, \
                               const unsigned int intr,\
                               const unsigned int cnt);
typedef int (*t_opl_os_adapt_net_gethostbyname_fp)(const char *domain, UNW_IP_ADDR_T *addr);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_net_get_errno_fp opl_os_adapt_net_get_errno;
extern t_opl_os_adapt_net_addr_fp opl_os_adapt_net_addr;
extern t_opl_os_adapt_net_str2addr_fp opl_os_adapt_net_str2addr;
extern t_opl_os_adapt_net_fd_set_fp opl_os_adapt_net_fd_set;
extern t_opl_os_adapt_net_fd_clear_fp opl_os_adapt_net_fd_clear;
extern t_opl_os_adapt_net_fd_isset_fp opl_os_adapt_net_fd_isset;
extern t_opl_os_adapt_net_fd_zero_fp opl_os_adapt_net_fd_zero;
extern t_opl_os_adapt_net_select_fp opl_os_adapt_net_select;
extern t_opl_os_adapt_net_close_fp opl_os_adapt_net_close;
extern t_opl_os_adapt_net_shutdown_fp opl_os_adapt_net_shutdown;
extern t_opl_os_adapt_net_socket_create_fp opl_os_adapt_net_socket_create;
extern t_opl_os_adapt_net_connect_fp opl_os_adapt_net_connect;
extern t_opl_os_adapt_net_connect_raw_fp opl_os_adapt_net_connect_raw;
extern t_opl_os_adapt_net_bind_fp opl_os_adapt_net_bind;
extern t_opl_os_adapt_net_socket_bind_fp opl_os_adapt_net_socket_bind;
extern t_opl_os_adapt_net_listen_fp opl_os_adapt_net_listen;
extern t_opl_os_adapt_net_accept_fp opl_os_adapt_net_accept;
extern t_opl_os_adapt_net_send_fp opl_os_adapt_net_send;
extern t_opl_os_adapt_net_send_to_fp opl_os_adapt_net_send_to;
extern t_opl_os_adapt_net_recv_fp opl_os_adapt_net_recv;
extern t_opl_os_adapt_net_recv_nd_size_fp opl_os_adapt_net_recv_nd_size;
extern t_opl_os_adapt_net_recvfrom_fp opl_os_adapt_net_recvfrom;
extern t_opl_os_adapt_net_set_block_fp opl_os_adapt_net_set_block;
extern t_opl_os_adapt_net_get_nonblock_fp opl_os_adapt_net_get_nonblock;
extern t_opl_os_adapt_net_set_timeout_fp opl_os_adapt_net_set_timeout;
extern t_opl_os_adapt_net_set_bufsize_fp opl_os_adapt_net_set_bufsize;
extern t_opl_os_adapt_net_set_reuse_fp opl_os_adapt_net_set_reuse;
extern t_opl_os_adapt_net_disable_nagle_fp opl_os_adapt_net_disable_nagle;
extern t_opl_os_adapt_net_set_boardcast_fp opl_os_adapt_net_set_boardcast;
extern t_opl_os_adapt_net_set_keepalive_fp opl_os_adapt_net_set_keepalive;
extern t_opl_os_adapt_net_gethostbyname_fp opl_os_adapt_net_gethostbyname;

// Sec 5: declaration of global function prototype
void opl_os_adapt_network_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_NETWORK_H__
